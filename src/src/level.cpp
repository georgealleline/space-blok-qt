/**
 * Copyright (c) 2011-2014 Microsoft Mobile and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */


#include <btBulletDynamicsCommon.h>
#include <qglabstractscene.h>
#include <qglbuilder.h>
#include <qglshaderprogram.h>
#include <qglcube.h>
#include <QDebug>
#include "qgeometrydata.h"
#include "ball.h"
#include "level.h"
#include "blocksshader.h"

#define BLOK_MASS 1.0f

/*!
  \class Level
  \brief Represents the game level (the combined body of several cubes).
         Imports .obj file defining the 3D object and generates Bullet
         rigid body as well as collision shapes to mimic the cubes in 3D
         model. Handles the ball hits, counts the hit counts per cube and
         manages the destruction of each cube.
*/


/*!
  Constuctor, loads the 3D model of the level from .obj file. Scans the
  vertexes of the model and generates collision shapes for the each cube. The
  each collision shape are compound to a single body, making the level rotate
  as one. The linear damping of the body is set to really large to prevent
  the level object to move. Only the revolution of the object is allowed.

  Each Qt3D QGLSceneNode representing a single cube will have user pointer set
  to a BlokData object, containing the information how many hits the
  corresponding blok object has received. The Bullet body object will have user
  pointer to the this object to allow retrieving of the level object in the
  collision handling.
*/
Level::Level(btDiscreteDynamicsWorld *world,
             const QVector3D &pos,
             const QQuaternion &quaternion,
             QGLMaterialCollection *materialCollection,
             int blokMaterialIndex,
             QGLShaderProgramEffect *effect,
             QObject *parent)
    : GameObject(world, pos, quaternion, parent)
{
    m_GameObjectType = BLOK;
    btScalar mass = 0;

    m_SpecularLoc = -1;
    m_LightPositionLoc = -1;
    m_ShininessLoc = -1;

    QGLAbstractScene *levelModel = QGLAbstractScene::loadScene(":/level.obj");
    addNode(levelModel->mainNode()->clone(this));
    delete levelModel;

    if (effect)
        setUserEffect(effect);
    else
        setEffect(QGL::FlatReplaceTexture2D);

    // Scale the level to better size
    foreach (QGLSceneNode *node, allChildren()) {
        QGeometryData gmData = node->geometry();
        if (gmData.vertices().isEmpty()) {
            continue;
        }

        for (int i = 0; i<gmData.vertices().size(); i++) {
            gmData.vertex(i) *= 1.3f;
        }

        break;
    }

    btCompoundShape *compoundShape = new btCompoundShape;
    compoundShape->setUserPointer(this);

    QList<QGLSceneNode*> children = allChildren();
    QList<QGLSceneNode*>::iterator it;

    for (it = children.begin(); it != children.end(); it++) {
        QGLSceneNode *node = *it;

        QGeometryData gmData = node->geometry();
        if (gmData.vertices().isEmpty()) {
            continue;
        }

        // 100 is max
        QVector3D *vlist[100];
        int foundCount = 0;
        for (int i=node->start(); i<node->start() + node->count(); i++) {
            QVector3D &v =
                    node->geometry().vertex(node->geometry().indices().at(i));

            bool alreadyExists = false;
            for (int check=0; check<foundCount; check++) {
                if ((*vlist[check]) == v) {
                    alreadyExists = true;
                    break;
                }
            }

            if (alreadyExists == false) {
                vlist[foundCount] = &v;
                foundCount++;
            }
        }

        QVector3D avg;
        for (int i=0; i<foundCount; i++) {
            avg += (*vlist[i]);
        }
        avg /= foundCount;

        bool centerBlok = false;
        if((QVector3D(0, 0, 0) - avg).length() < 4.0f) {
            centerBlok = true;
        }

        QVector3D xv = (*vlist[1]) - (*vlist[0]);
        float scannedSize = xv.length();
        xv.normalize();
        QVector3D yv = (*vlist[3]) - (*vlist[0]);
        yv.normalize();
        QVector3D zv = QVector3D::crossProduct( yv, xv );
        zv.normalize();

        btMatrix3x3 bmatrix = btMatrix3x3(xv.x(), xv.y(), xv.z(),
                                          yv.x(), yv.y(), yv.z(),
                                          zv.x(), zv.y(), zv.z());

        btTransform trans = btTransform(bmatrix,
                                        btVector3(avg.x(), avg.y(), avg.z()));

        btBoxShape *boxShape = new btBoxShape(
                    btVector3(scannedSize/2.0f,
                              scannedSize/2.0f,
                              scannedSize/2.0f));
        compoundShape->addChildShape(trans, boxShape);

        if (materialCollection && blokMaterialIndex != -1) {
            node->setPalette(materialCollection);
            node->setMaterialIndex(blokMaterialIndex);
        }

        if (centerBlok) {
            // The center bloks will be harder to destroy.
            node->setUserData(1, new BlokData(3));
        }
        else {
            // Outer bloks will be destroyed on first hit.
            node->setUserData(1, new BlokData(2));

        }
        boxShape->setUserPointer(node);

        mass += BLOK_MASS;
    }

    btVector3 inertia(0, 0, 0);
    compoundShape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo bodyCI(mass,
                                                    this,
                                                    compoundShape,
                                                    inertia);
    bodyCI.m_angularSleepingThreshold = 0.0f;
    bodyCI.m_linearSleepingThreshold = 0.0f;
    bodyCI.m_linearDamping = 1000.0f;
    bodyCI.m_angularDamping = 0.05f;
    bodyCI.m_restitution = 0.5f;

    m_Body = new btRigidBody(bodyCI);
    m_Body->setUserPointer(this);

    world->addRigidBody(m_Body, COL_BLOK, COL_BLACK_HOLE | COL_BALL);
}


/*!
  Handles the hit of a ball to the level. Finds the child collision object of
  the compound shape which is closest to the given worldHitPos. The hit count
  of the associated BlokData is decreased there has been enough time betweeen
  sequntial hits. If the hit count of the object decreases to 0, the
  corresponding child collision shape will be removed and the corresponding
  child QGLSceneNode will removed from the QGLSceneNode tree.
  The returned Level::HitInfo will report to the caller was the blok hit or
  destroyed or neither. The last may happen if the ball hits to the same BlokData
  say several times in 1 ms, and we want to prevent that from happening.
*/
Level::HitInfo Level::handleBallHit(const btVector3 &worldHitPos,
                                    Ball *ball,
                                    const QTime &hitTime)
{
    Q_UNUSED(ball);

    btCompoundShape *compoundShape =
            static_cast<btCompoundShape*>(m_Body->getCollisionShape());

    btCollisionShape *closestChildShape = 0;
    btScalar minDistance = 1000000.0f;

    btTransform bodyTransform = m_Body->getWorldTransform();

    // Retrieve the contact point in body coordinates
    btVector3 hitPoint = worldHitPos - bodyTransform.getOrigin();
    hitPoint = hitPoint * bodyTransform.getBasis();

    int childShapeCount = compoundShape->getNumChildShapes();
    if (childShapeCount == 0) {
        return HitInfo(false, false);
    }

    for (int i=0; i<childShapeCount; i++) {
        btCollisionShape *childShape = compoundShape->getChildShape(i);
        btTransform childTrans = compoundShape->getChildTransform(i);
        btScalar distance = hitPoint.distance2(childTrans.getOrigin());

        if (closestChildShape) {
            if (distance < minDistance) {
                closestChildShape = childShape;
                minDistance = distance;
            }
        }
        else {
            closestChildShape = childShape;
        }
    }


    // Remove the child collision shape and the QGLSceneNode
    QGLSceneNode *node =
            static_cast<QGLSceneNode*>(closestChildShape->getUserPointer());
    BlokData *blokData = static_cast<BlokData*>(node->userData(1));

    HitInfo hitInfo(false, false);

    if (blokData->m_HitTime.msecsTo(hitTime) > 300) {
        blokData->m_HitTime = hitTime;
        blokData->m_HitPoints--;
        hitInfo.m_BlokHit = true;
    }

    if (blokData->m_HitPoints <= 0) {
        compoundShape->removeChildShape(closestChildShape);
        static_cast<QGLSceneNode*>(node->parent())->removeNode(node);

        delete closestChildShape;
        delete node;

        btScalar invMass = m_Body->getInvMass();
        btScalar mass;

        hitInfo.m_BlokDestroyed = true;

        // Reduce the mass of the level object.
        if (invMass != 0.0f) {
            mass = 1 / invMass;
        }
        else {
            mass = 1 / 0.00001f;
        }

        mass -= BLOK_MASS;
        btVector3 inertia;
        compoundShape->calculateLocalInertia(mass, inertia);

        m_Body->setMassProps(mass, inertia);
    }

    return hitInfo;
}


/*!
  Returns true if all bloks in a level has been destroyed. In other words
  the m_Body has zero child collision shapes.
*/
bool Level::isAllBloksDestroyed() const
{
    btCompoundShape *compoundShape =
            static_cast<btCompoundShape*>(m_Body->getCollisionShape());

    if (compoundShape->getNumChildShapes() == 0) {
        return true;
    }

    return false;
}


/*!
  Sets the glowing level of "grid" on bloks.
*/
void Level::setGlowEffectValue(float glowValue)
{
    m_GlowValue = QVector4D(glowValue,
                            glowValue * 0.78f,
                            glowValue * 0.125f,
                            glowValue);
}


/*!
  Sets the light position in level.
*/
void Level::setLightPosition(const QVector3D &position)
{
    m_LightPosition = position;
}


/*!
    Draws the level using and applies some uniforms to the shader in order
    to apply effects.
*/
void Level::draw(QGLPainter *painter)
{
    if (userEffect()) {
        QGLShaderProgramEffect *effect =
                static_cast<QGLShaderProgramEffect*>(userEffect());

        QGLShaderProgram *program = effect->program();

        if (m_SpecularLoc == -1) {
            m_SpecularLoc = program->uniformLocation("specular");
        }

        if (m_LightPositionLoc == -1) {
            m_LightPositionLoc = program->uniformLocation("lightPosition");
        }

        if (m_ShininessLoc == -1) {
            m_ShininessLoc = program->uniformLocation("shininess");
        }

        effect->setActive(painter, true);
        program->setUniformValue(m_SpecularLoc, m_GlowValue);
        program->setUniformValue(m_LightPositionLoc, m_LightPosition);
        program->setUniformValue(m_ShininessLoc, 0.2f);

        GameObject::draw(painter);
    }
    else {
        GameObject::draw(painter);
    }
}
