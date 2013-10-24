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
#include "lightparticle.h"

/*!
  \class LightParticle
  \brief Implements the look and behavior of light partice.
*/


/*!
  Constuctor, creates 2D pane by using Qt3D QGLBuilder to which there is
  a light material set.
*/
LightParticle::LightParticle(QGLMaterialCollection *materialCollection,
                             int materialIndex)
{
    QGLBuilder builder;
    builder.newSection(QGL::Faceted);
    builder.addPane(QSizeF(4.0f, 4.0f));
    addNode(builder.finalizedSceneNode());

    setEffect(QGL::FlatReplaceTexture2D);
    setPalette(materialCollection);
    setMaterialIndex(materialIndex);
}


/*!
  Define the initial life time for the light particle. Some randomization
  is used to make particles different.
*/
float LightParticle::getInitialLifeTime() const
{
    return 0.5f + (float)(rand() & 255) / 512.0f;
}


/*!
  Zero friction for the light partice.
*/
float LightParticle::getFriction() const
{
    return 0.0f;
}


/*!
  Define the maximum value for the rotation of the light particle in rad/s.
  The particle system will randomly rotate the particles.
*/
float LightParticle::getRotationMax() const
{
    return 2.0f;
}


/*!
  Informs that the particle position has been updated by the particle system.
  The QGLSceneNode is updated to move the visual representation of the particle.
*/
void LightParticle::particleUpdated()
{
    if (isActive() == true) {
        float psize = getLifeTime() * 2.0f;

        if (psize > 1.0f)
            psize = 1.0f;

        QMatrix4x4 mat;
        mat.rotate(m_Rotation[2], 0,0,1);
        mat.scale(psize);

        setLocalTransform(mat);
        setPosition(m_ParticlePos);
    }
    else {
        setPosition(QVector3D());
    }
}
