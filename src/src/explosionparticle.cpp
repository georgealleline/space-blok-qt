/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include <QMatrix4x4>
#include <qglbuilder.h>
#include <qglsphere.h>
#include <qglcube.h>
#include <qglshaderprogram.h>
#include "explosionparticle.h"
#include "platform.h"

/*!
  \class ExplosionParticle
  \brief Particle the represents explosion of the blok object. Contains Qt3D
         cubes with given shader effect and material.
*/


/*!
  Constructor, creates Qt3D Cube object as a particle, with lighting normals
  separated for each face for a faceted appearance.
*/
ExplosionParticle::ExplosionParticle(QGLShaderProgramEffect *effect,
                                     QGLMaterialCollection *materialCollection)
    : IParticle()
{
    m_DiffuseLoc = -1;

    QGLBuilder builder;
    builder.newSection(QGL::Faceted);
    builder << QGLCube(0.6);
    addNode(builder.finalizedSceneNode());

    if (effect)
        setUserEffect(effect);

    setPalette(materialCollection);
}


/*!
  Virtual method indicating the the particle has reached it target. The player
  score will be updated here. The fadeout animation is started.
*/
void ExplosionParticle::targetReached()
{
    Platform *platform = static_cast<Platform*>(m_UserData);
    if (platform) {
        platform->addScore(1);
        m_UserData = 0;
    }

    if (m_LifeTime > 2.0f) {
        // Start fadeout animation
        m_LifeTime = 1.0f;
    }
}


/*!
  Virtual methos defining the initial life time of the particle.
  Explosion particle will never die.
*/
float ExplosionParticle::getInitialLifeTime() const
{
    // This type will never die until said so
    return 10000.0f;
}


/*!
  Virtual method defining the friction of the explosion particle.
*/
float ExplosionParticle::getFriction() const
{
    return 0.55f;
}


/*!
  Virtual method defining the rotation max of the explosion particle.
*/
float ExplosionParticle::getRotationMax() const
{
    return 2.0f;
}


/*!
  Virtual method setting the material index of the explosion particle.
*/
void ExplosionParticle::setParticleSpecificAttribute(int att)
{
    // Mesh index
    setMaterialIndex(att);
}


/*!
  Virtual method to update the particle position and transform. Also
  the fade out animation is implemented here.
  If the particle is not active its position is set to origo, although
  the inactive particles should not be drawn at all.
*/
void ExplosionParticle::particleUpdated()
{
    if (isActive() == true) {
        float psize = getLifeTime() * 2.0f;
        if (psize > 1.0f)
            psize = 1.0f;

        QMatrix4x4 mat;
        mat.rotate(m_Rotation[0], 1,0,0);
        mat.rotate(m_Rotation[1], 0,1,0);
        mat.rotate(m_Rotation[2], 0,0,1);
        mat.scale(psize);

        setLocalTransform( mat );
        setPosition(m_ParticlePos);
    }
    else {
        setPosition(QVector3D());
    }
}


/*!
  Resets the particle, makes it inactive and removes the possible pointer
  to the platform which is used to make particle carry points to the
  platform.
*/
void ExplosionParticle::resetParticle()
{
    m_LifeTime = 0.0f;
    m_UserData = 0;
}


/*!
  Draw the particle, if effect was given in the constructor, we set the
  diffuse value for the shader in order to color the particle as same
  as the ball that hit the particle.
*/
void ExplosionParticle::draw(QGLPainter *painter)
{
    if (userEffect()) {
        QGLShaderProgram *program =
                static_cast<QGLShaderProgramEffect*>(userEffect())->program();

        if (m_DiffuseLoc == -1) {
            m_DiffuseLoc = program->uniformLocation("diffuse");
        }

        program->setUniformValue(m_DiffuseLoc, material()->diffuseColor());

        QGLSceneNode::draw(painter);

        program->setUniformValue(m_DiffuseLoc,
                                 QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
    }
    else {
        QGLSceneNode::draw(painter);
    }
}
