/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */


#include <qglbuilder.h>
#include <qglsphere.h>
#include <qglshaderprogram.h>
#include <qglshaderprogrameffect.h>
#include <btBulletDynamicsCommon.h>
#include "ball.h"
#include "gameview.h"
#include "platform.h"


/*!
  \class Ball
  \brief Represents a Ball in the 3D space.
*/


/*!
  Constuctor, creates a Sphere shaped 3D object to a Qt3D scene with given
  parameters. Also creates a Bullet sphere shape to add sphere-like collision
  parameters to the object.
*/
Ball::Ball(btDiscreteDynamicsWorld *world,
           const QVector3D &pos, qreal radius,
           QGLMaterialCollection *materialCollection,
           int materialIndex,
           bool still,
           QGLShaderProgramEffect *effect,
           QObject *parent)
    : GameObject(world, pos, QQuaternion(), parent)
{
    m_GameObjectType = BALL;

    m_Platform = 0;
    m_AmbientLoc = -1;
    m_DiffuseLoc = -1;
    m_SpecularLoc = -1;
    m_ShininessLoc = -1;

    QGLBuilder builder;
    builder << QGLSphere(radius * 2.0f, 3);
    addNode(builder.finalizedSceneNode());
    if (effect)
        setUserEffect(effect);
    else
        setEffect(QGL::FlatReplaceTexture2D);

    if (materialCollection && materialIndex != -1) {
        setPalette(materialCollection);
        setMaterialIndex(materialIndex);
    }

    btCollisionShape *shape = new btSphereShape(radius);

    btScalar mass = 5;
    btVector3 inertia(0, 0, 0);
    shape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo bodyCI(mass, this, shape, inertia);
    bodyCI.m_angularSleepingThreshold = 0.0f;
    bodyCI.m_linearSleepingThreshold = 0.0f;
    bodyCI.m_restitution = 0.9f;

    if (still) {
        bodyCI.m_linearDamping = 1.0f;
    }

    m_Body = new btRigidBody(bodyCI);
    m_Body->setUserPointer(this);

    world->addRigidBody(m_Body, COL_BALL, COL_BALL | COL_BLACK_HOLE | COL_BLOK | COL_PLATFORM);
}


/*!
  Returns the platform the ball belongs to.
*/
Platform* Ball::platform() const
{
    return m_Platform;
}


/*!
  Sets the platform the ball belongs to.
*/
void Ball::setPlatform(Platform *platform)
{
    m_Platform = platform;
}


/*!
  Applies black hole gravity effect. The ball will be pulled towars the§
  origo of the 3D space, that is where the black hole center exists in
  this simulation.
*/
void Ball::applyGravity()
{
    btVector3 pos = body()->getWorldTransform().getOrigin();
    body()->applyForce((btVector3(0, 0, 0) - pos), btVector3(0, 0, 0));
}


/*!
  Draws the ball, if effects were applied on the constructor, the shaders
  uniforms are updated make the grid show on the ball.
*/
void Ball::draw(QGLPainter *painter)
{
    if (userEffect()) {
        QGLShaderProgram *program =
                static_cast<QGLShaderProgramEffect*>(userEffect())->program();

        if (m_AmbientLoc == -1) {
            m_AmbientLoc = program->uniformLocation("ambient");
        }

        if (m_DiffuseLoc == -1) {
            m_DiffuseLoc = program->uniformLocation("diffuse");
        }

        if (m_SpecularLoc == -1) {
            m_SpecularLoc = program->uniformLocation("specular");
        }

        if (m_ShininessLoc == -1) {
            m_ShininessLoc = program->uniformLocation("shininess");
        }

        program->setUniformValue(m_AmbientLoc, material()->ambientColor());
        program->setUniformValue(m_DiffuseLoc, material()->diffuseColor());
        program->setUniformValue(m_SpecularLoc,
                                 QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
        program->setUniformValue(m_ShininessLoc, (GLfloat)material()->shininess());

        GameObject::draw(painter);

        program->setUniformValue(m_DiffuseLoc,
                                 QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
    }
    else {
        GameObject::draw(painter);
    }
}
