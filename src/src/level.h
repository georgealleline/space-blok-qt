/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef LEVEL_H
#define LEVEL_H

#include "qglshaderprogrameffect.h"
#include <qglscenenode.h>
#include <LinearMath/btVector3.h>
#include "gameobject.h"
#include "QTime"

class QGLMaterialCollection;
class Ball;


class BlokData : public QObjectUserData {
public:
    int m_HitPoints;
    QTime m_HitTime;

    BlokData(int hitPoints)
        : m_HitPoints(hitPoints)
    {
        m_HitTime = QTime();
    }
};


class Level : public GameObject
{
    Q_OBJECT
public:
    struct HitInfo {
        bool m_BlokHit;
        bool m_BlokDestroyed;

        HitInfo(bool hit = false, bool destroyed = false)
            : m_BlokHit(hit),
              m_BlokDestroyed(destroyed)
        {
        }
    };

    Level(btDiscreteDynamicsWorld *world,
          const QVector3D &pos,
          const QQuaternion &quaternion,
          QGLMaterialCollection *materialCollection,
          int blokMaterialIndex,
          //Shader effect to be assigned for level objects.
          //0 = use FlatReplaceTexture2D
          QGLShaderProgramEffect *effect,
          QObject *parent = 0);

    HitInfo handleBallHit(const btVector3 &worldHitPos,
                          Ball *ball,
                          const QTime &hitTime);

    bool isAllBloksDestroyed() const;

    void setGlowEffectValue(float glowValue);
    void setLightPosition(const QVector3D &position);

    virtual void draw(QGLPainter *painter);

protected:

    QVector4D m_GlowValue;
    QVector3D m_LightPosition;

    int m_SpecularLoc;
    int m_LightPositionLoc;
    int m_ShininessLoc;
};

#endif // LEVEL_H
