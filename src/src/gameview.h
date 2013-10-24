/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QTime>
#include <QVector3D>
#include <qglview.h>
#include <bullet/LinearMath/btScalar.h>

// Bullet forward declarations
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btDynamicsWorld;


// Qt3D forward declarations
class QGLMaterialCollection;


// Qt forward declarations
class QTimer;


// Application forward declarations
class GameObject;
class Level;
class PauseButton;
class BlackHole;
class Platform;
class Ball;
class AudioManager;
class MenuManager;
class ParticleSystem;
class BlocksShaderEffect;
class BlackHoleShaderEffect;

class GameView : public QGLView
{
    Q_OBJECT

public:
    GameView(QWidget *parent = 0);
    ~GameView();

    QVector3D map2DPointToZLevelPoint(const QPoint &point, qreal zPos);
    void checkEndOfGame();

protected slots:
    void updateGL();

    void loadLevel();
    void pauseGame();

protected:

    void initializeGL(QGLPainter *painter);

    void initializeBulletEngine();
    void initializeMaterials();
    void initializeAudioEngine();
    void initializeMenuManager();

    void createGameObjects();

    QPointF convertPointToGLPos(const QPointF &pos);
    void removeBall(GameObject *gameObject);

    void paintGL(QGLPainter *painter);
    bool event(QEvent *event);

    static void simulationCallback(btDynamicsWorld *world, btScalar time);
    void simulateSubStep(btScalar time);

protected:

    MenuManager *m_MenuManager;

    int m_FPSElapsedTime;
    int m_FPSCounter;
    float m_FPS;

    AudioManager *m_AudioManager;

    static const qreal PLATFORM_Z_POS;

    BlocksShaderEffect *m_BlokShaderEffect;
    BlackHoleShaderEffect *m_BlackHoleShaderEffect;

    QTimer *m_Timer;
    QTime m_LastTime;

    QGLMaterialCollection *m_MaterialCollection;

    QVector3D m_LightPosition;
    QVector3D m_LightTargetPosition;

    // for displaying emissive grid ontop of the bloks
    float m_BlokFlashPower;

    BlackHole *m_BlackHole;
    Level *m_Level;
    PauseButton *m_PauseButton;
    QList<Platform*> m_Platforms;
    QList<Ball*> m_Balls;
    QGLSceneNode *m_RootNode;

    ParticleSystem *m_ExplosionParticles;
    ParticleSystem *m_LightParticles;

    // Bullet objects
    btBroadphaseInterface* m_Broadphase;
    btDefaultCollisionConfiguration* m_CollisionConfiguration;
    btCollisionDispatcher* m_Dispatcher;
    btSequentialImpulseConstraintSolver* m_Solver;
    btDiscreteDynamicsWorld* m_DynamicsWorld;
};

#endif // GAMEVIEW_H
