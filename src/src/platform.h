/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */


#ifndef PLATFORM_H
#define PLATFORM_H

#include <qglshaderprogrameffect.h>
#include <QTime>
#include "gameobject.h"

class QGLMaterialCollection;
class Ball;
class GameView;
class Score;

class Platform : public GameObject
{
public:
    enum BALL_ID {
        UNKNOWN = 0,
        RED_BALL,
        YELLOW_BALL,
        GREEN_BALL,
        BLUE_BALL
    };


    Platform(btDiscreteDynamicsWorld *world,
             const QVector3D &pos,
             const QQuaternion &quaternion,
             QGLMaterialCollection *materialCollection,
             int materialIndex,
             QGLShaderProgramEffect *effect,
             GameView *view,
             QObject *parent);

    void addScore(int score);
    int score() const;
    void resetScore();

    QColor ballColor() const;
    BALL_ID ballId () const;

    Ball* addBallIfRequred(int relTime);

    void removeBall();

    void resetBallCreationTime();

    bool handlePressInput(const QVector3D &pos, int touchId);
    bool handleReleaseInput(const QVector3D &pos, int touchId);
    bool handleEvent(QEvent *event);

    bool hitTest(const QVector3D &pos);

    virtual void draw(QGLPainter *painter);

    void setBallMaterial(QGLMaterialCollection *materialCollection,
                         int materialIndex,
                         QGLShaderProgramEffect *ballEffect,
                         BALL_ID ballId = UNKNOWN);
    QGLSceneNode* setFontMaterial(QGLMaterialCollection *materialCollection,
                                  int materialIndex);

protected:
    Ball* createBall();

protected:
    GameView *m_GameView;

    QTime m_SwipePressTime;
    QVector3D m_SwipePressPos;
    bool m_Pressed;
    int m_TouchId;

    const int m_MillSecondsToCreateNewBall;
    int m_LastTime;

    QVector3D m_BallInitialPos;
    Ball *m_Ball;

    QGLMaterialCollection *m_BallMaterialCollection;
    int m_BallMaterialIndex;
    QGLShaderProgramEffect *m_BallShaderEffect;

    QGLShaderProgramEffect *m_ShaderEffect;

    Score *m_ScoreFont;

    BALL_ID m_BallId;

    int m_SpecularLoc;
    int m_ShininessLoc;
};

#endif // PLATFORM_H
