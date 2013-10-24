/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */


#ifndef BALL_H
#define BALL_H

#include "gameobject.h"

class QGLShaderProgramEffect;
class QGLMaterialCollection;
class Platform;

class Ball : public GameObject
{
    Q_OBJECT
public:
    Ball(btDiscreteDynamicsWorld *world, const QVector3D &pos, qreal radius,
         QGLMaterialCollection *materialCollection, int materialIndex = -1,
         bool still = false,
         QGLShaderProgramEffect *effect = 0,
         QObject *parent = 0);

    Platform* platform() const;
    void setPlatform(Platform *platform);

    void applyGravity();

    virtual void draw(QGLPainter *painter);

protected:
    // Platform that the ball origins.
    Platform *m_Platform;

    int m_AmbientLoc;
    int m_DiffuseLoc;
    int m_SpecularLoc;
    int m_ShininessLoc;
};

#endif // BALL_H
