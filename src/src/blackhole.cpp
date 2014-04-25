/**
 * Copyright (c) 2011-2014 Microsoft Mobile and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */


#include <qglbuilder.h>
#include <qglshaderprogrameffect.h>
#include <qglshaderprogram.h>
#include <btBulletDynamicsCommon.h>
#include "blackhole.h"

/*!
  \class BlackHole
  \brief Representes a black hole. Constructed from a Qt3D plane and Bullet
         plane shape. Static Bullet object colliding with Ball objects.
*/


/*!
  Constructor, creates Qt3D plane object and attaches static Bullet plane
  collision shape to it. Collides with Ball.
*/
BlackHole::BlackHole(btDiscreteDynamicsWorld *world,
                     const QVector3D &planeVector,
                     qreal planeConstant,
                     QGLMaterialCollection *materialCollection,
                     int materialIndex,
                     QGLShaderProgramEffect *effect,
                     QObject *parent)
    : GameObject(world, QVector3D(), QQuaternion(), parent)
{
    m_GameObjectType = BLACK_HOLE;
    m_Spin = 0.0f;
    m_RotMatLoc = -1;

    if (effect) {
        setUserEffect(effect);
    }
    else {
        setEffect(QGL::FlatReplaceTexture2D);
    }

    setPalette(materialCollection);
    setMaterialIndex(materialIndex);

    QGLBuilder builder;
    builder.addPane(QSizeF(85.0f, 80.0f));
    addNode(builder.finalizedSceneNode());
    setPosition(QVector3D(0, 0, planeConstant));

    btVector3 pv = btVector3(planeVector.x(), planeVector.y(), planeVector.z());
    btCollisionShape *planeShape = new btStaticPlaneShape(pv, planeConstant);
    btRigidBody::btRigidBodyConstructionInfo planeRigidBodyCI(
                0, this, planeShape, btVector3(0, 0, 0));

    m_Body = new btRigidBody(planeRigidBodyCI);
    m_Body->setUserPointer(this);

    world->addRigidBody(m_Body, COL_BLACK_HOLE, COL_BALL);
}


/*!
  Rotates the black hole.
*/
void BlackHole::rotateBlackHole(int relTime)
{
    m_Spin += relTime * 0.0005f;
}


/*!
  Draws the black hole.
*/
void BlackHole::draw(QGLPainter *painter)
{
    if (userEffect()) {
        QGLShaderProgramEffect *effect =
                static_cast<QGLShaderProgramEffect*>(userEffect());
        effect->setActive(painter, true);

        QMatrix2x2 rotMat;
        rotMat(0, 0) = cos(m_Spin);
        rotMat(1, 0) = -sin(m_Spin);
        rotMat(0, 1) = sin(m_Spin);
        rotMat(1, 1) = cos(m_Spin);

        if (m_RotMatLoc == -1) {
            m_RotMatLoc = effect->program()->uniformLocation("rotMat");
        }

        effect->program()->setUniformValue(m_RotMatLoc, rotMat);

        GameObject::draw(painter);
    }
    else {
        GameObject::draw(painter);
    }
}
