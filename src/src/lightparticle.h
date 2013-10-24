/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef __LIGHTPARTICLE__
#define __LIGHTPARTICLE__

#include <qglscenenode.h>
#include "particlesystem.h"

class LightParticle : public IParticle, public QGLSceneNode
{
public:
    LightParticle(QGLMaterialCollection *materialCollection, int materialIndex);

    virtual void particleUpdated();

    virtual float getInitialLifeTime() const;
    virtual float getFriction() const;
    virtual float getRotationMax() const;

protected:

};

#endif
