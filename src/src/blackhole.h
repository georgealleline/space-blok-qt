/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */


#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include "gameobject.h"

class QGLMaterialCollection;
class QGLShaderProgramEffect;

class BlackHole : public GameObject
{
    Q_OBJECT
public:
    BlackHole(btDiscreteDynamicsWorld *world,
              const QVector3D &planeVector,
              qreal planeConstant,
              QGLMaterialCollection *materialCollection,
              int materialIndex,
              QGLShaderProgramEffect *effect = 0,
              QObject *parent = 0);

    void rotateBlackHole(int relTime);

    virtual void draw(QGLPainter *painter);

protected:

    float m_Spin;
    int m_RotMatLoc;
};

#endif // BLACKHOLE_H
