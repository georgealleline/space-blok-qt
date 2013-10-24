/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef EXPLOSIONPARTICLE_H
#define EXPLOSIONPARTICLE_H

#include <qglscenenode.h>
#include "qglshaderprogrameffect.h"
#include "particlesystem.h"


class ExplosionParticle : public IParticle, public QGLSceneNode
{
public:
    ExplosionParticle(QGLShaderProgramEffect *effect,
                      QGLMaterialCollection *materialCollection);

    virtual void particleUpdated();
    virtual void targetReached();

    virtual float getInitialLifeTime() const;
    virtual float getFriction() const;
    virtual float getRotationMax() const;

    virtual void setParticleSpecificAttribute(int att);

    void resetParticle();

    virtual void draw(QGLPainter *painter);

protected:
    int m_DiffuseLoc;
};

#endif // EXPLOSIONPARTICLE_H
