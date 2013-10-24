/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include <QEvent>
#include <QMouseEvent>
#include <QTimer>
#include <qglshaderprogram.h>
#include <qgltexture2d.h>
#include <qglscenenode.h>
#include <qglrendersequencer.h>
#include <qglrenderordercomparator.h>
#include <btBulletDynamicsCommon.h>
#include "gameview.h"
#include "ball.h"
#include "blackhole.h"
#include "level.h"
#include "platform.h"
#include "pausebutton.h"
#include "explosionparticle.h"
#include "lightparticle.h"
#include "audiomanager.h"
#include "menumanager.h"
#include "scoremodel.h"
#include "particlesystem.h"
#include "blocksshader.h"
#include "blackholeshadereffect.h"



/*!
  \class GameRenderOrderComparator
  \brief Custom implementation to provide render comparator for the
         QGLRenderSequencer to optimize the drawing.
*/
class GameRenderOrderComparator : public QGLRenderOrderComparator
{
public:
    GameRenderOrderComparator() {}
    virtual ~GameRenderOrderComparator() {}

    // Returns true if the lhs render order is equal to the rhs; otherwise
    // returns false.
    virtual bool isEqualTo(const QGLRenderOrder &lhs,
                           const QGLRenderOrder &rhs)
    {
        Q_UNUSED(lhs);
        Q_UNUSED(rhs);

        return true;
    }

    // Returns true if the lhs render order is less than the rhs; otherwise
    // returns false.
    virtual bool isLessThan(const QGLRenderOrder &lhs,
                            const QGLRenderOrder &rhs)
    {
        Q_UNUSED(lhs);
        Q_UNUSED(rhs);

        return true;
    }
};


/*!
  \class GameView
  \brief The director object of the application. All game objects, particles,
         audio manager are created here. Derived from Qt3D QGLView.
*/


// Z-position of platforms in the space.
const qreal GameView::PLATFORM_Z_POS = 70.0f;


/*!
  Constructor, instead of here, most of the initialization code is made in
  initializeGL, when the Opengl context is avalable.
*/
GameView::GameView(QWidget *parent)
    : QGLView(parent)
{
    m_Level = 0;
    m_MenuManager = 0;
    m_ExplosionParticles = 0;
    m_LightParticles = 0;
    m_FPSCounter = 0;
    m_BlokFlashPower  = 0.0f;
    m_BlackHoleShaderEffect = 0;

    setAttribute(Qt::WA_AcceptTouchEvents);
}


/*!
  Destructor, deletes all game objects, effects, Bullet world and other Bullet
  objects.
*/
GameView::~GameView()
{
    delete m_RootNode;

    delete m_BlokShaderEffect;
    delete m_BlackHoleShaderEffect;

    delete m_DynamicsWorld;
    delete m_Solver;
    delete m_CollisionConfiguration;
    delete m_Dispatcher;
    delete m_Broadphase;
}


/*!
  Initializes the Bullet engine. Default settings are used, the gravity is set
  to zero, as we simulate the black hole manually in the updateGL for the balls.
*/
void GameView::initializeBulletEngine()
{
    m_Broadphase = new btDbvtBroadphase();

    m_CollisionConfiguration = new btDefaultCollisionConfiguration();
    m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration);
    m_Solver = new btSequentialImpulseConstraintSolver;

    // Create world
    m_DynamicsWorld = new btDiscreteDynamicsWorld(m_Dispatcher,
                                                  m_Broadphase,
                                                  m_Solver,
                                                  m_CollisionConfiguration);

    m_DynamicsWorld->setInternalTickCallback(simulationCallback, this);

    // Zero gravity, we simulate the gravity manually later on the code.
    m_DynamicsWorld->setGravity(btVector3(0, 0, 0));
}


/*!
  Initializes the Qt3D materials used in the application.
*/
void GameView::initializeMaterials()
{
    m_MaterialCollection = new QGLMaterialCollection(this);

    QGLTexture2D *lightTexture = new QGLTexture2D;
    lightTexture->setImage(QImage(":/reflectionmap.png"));

    QColor ambientColor = QColor(64, 64, 190);

    QGLMaterial *material = new QGLMaterial;
    material->setObjectName("PauseButtonMaterial");
    QGLTexture2D *texture = new QGLTexture2D;
    texture->setImage(QImage(":/pause_button.png"));
    material->setTexture(texture);
    m_MaterialCollection->addMaterial(material);

    // Black hole material
    material = new QGLMaterial;
    material->setObjectName("BlackHoleMaterial");
    texture = new QGLTexture2D;
    texture->setImage(QImage(":/BlackHole.jpg"));
    material->setTexture(texture);
    texture = new QGLTexture2D;
    texture->setImage(QImage(":/bh_sprial.png"));
    material->setTexture(texture, 1);
    material->setTextureCombineMode(QGLMaterial::Replace);
    m_MaterialCollection->addMaterial(material);


    // Black hole center material
    material = new QGLMaterial;
    material->setObjectName("BlackHoleFrontMaterial");
    texture = new QGLTexture2D;
    texture->setImage(QImage(":/bh_sprial.png"));
    material->setTexture(texture);
    material->setTexture(lightTexture, 1);
    m_MaterialCollection->addMaterial(material);

    // Platform material
    material = new QGLMaterial;
    material->setObjectName("PlatformMaterial");
    texture = new QGLTexture2D;
    texture->setImage(QImage(":/platformtex.png"));
    material->setTexture(texture);
    m_MaterialCollection->addMaterial(material);

    // Blok material
    material = new QGLMaterial;
    material->setObjectName("BlokMaterial");
    texture = new QGLTexture2D;
    texture->setImage(QImage(":/SimpleBlock.png"));
    material->setTexture(texture);
    material->setTexture(lightTexture, 1);
    material->setAmbientColor(ambientColor);
    material->setShininess(0.2f);
    m_MaterialCollection->addMaterial(material);

    // Ball materials
    material = new QGLMaterial;
    material->setObjectName("BallMaterial1");
    material->setAmbientColor(ambientColor);
    material->setDiffuseColor(QColor(255, 255, 64));
    material->setTexture(texture, 0);
    material->setTexture(lightTexture, 1);
    material->setShininess(1.0f);
    material->setSpecularColor(material->diffuseColor());
    m_MaterialCollection->addMaterial(material);

    material = new QGLMaterial;
    material->setObjectName("BallMaterial2");
    material->setAmbientColor(ambientColor);
    material->setDiffuseColor(QColor(255, 64, 64));
    material->setTexture(texture, 0);
    material->setTexture(lightTexture, 1);
    material->setShininess(1.0f);
    material->setSpecularColor(material->diffuseColor());
    m_MaterialCollection->addMaterial(material);

    material = new QGLMaterial;
    material->setObjectName("BallMaterial3");
    material->setDiffuseColor(QColor(64, 64, 255));
    material->setAmbientColor(ambientColor);
    material->setTexture(texture, 0);
    material->setTexture(lightTexture, 1);
    material->setShininess(1.0f);
    material->setSpecularColor(material->diffuseColor());
    m_MaterialCollection->addMaterial(material);

    material = new QGLMaterial;
    material->setObjectName("BallMaterial4");
    material->setDiffuseColor(QColor(64, 255, 64));
    material->setAmbientColor(ambientColor);
    material->setTexture(texture, 0);
    material->setTexture(lightTexture,1);
    material->setShininess(1.0f);
    material->setSpecularColor(material->diffuseColor());
    m_MaterialCollection->addMaterial(material);

    material = new QGLMaterial;
    material->setObjectName("LightFlareMaterial");
    texture = new QGLTexture2D;
    texture->setImage(QImage(":/flare.png"));
    material->setTexture(texture);
    m_MaterialCollection->addMaterial(material);

    material = new QGLMaterial;
    material->setObjectName("FontMaterial");
    texture = new QGLTexture2D;
    texture->setImage(QImage(":/scorefont.png"));
    material->setTexture(texture);
    m_MaterialCollection->addMaterial(material);
}


/*!
  Initializes the GE audio engine.
*/
void GameView::initializeAudioEngine()
{
    m_AudioManager = new AudioManager(this);
    m_AudioManager->applyWindEffect(0.1, 0.2);
}


/*!
  Initializes the menu manager.
*/
void GameView::initializeMenuManager()
{
    m_MenuManager = new MenuManager(camera(), this);

    connect(m_MenuManager, SIGNAL(newGame()), this, SLOT(loadLevel()));
}


/*!
  Creates the shader effects, game objects, black hole, platforms and the
  particle systems.
*/
void GameView::createGameObjects()
{
    // Root node that will hold all of the Qt3D QGLSceneNode's
    m_RootNode = new QGLSceneNode;

    m_BlokShaderEffect = new BlocksShaderEffect();
    m_BlokShaderEffect->setMaximumLights(1);

    m_BlackHoleShaderEffect = new BlackHoleShaderEffect();
    m_BlackHoleShaderEffect->setMaximumLights(1);

    m_BlackHole = new BlackHole(m_DynamicsWorld, QVector3D(0, 0, 1), 0,
                                m_MaterialCollection,
                                m_MaterialCollection->indexOf("BlackHoleMaterial"),
                                m_BlackHoleShaderEffect, m_RootNode);
    m_RootNode->addNode(m_BlackHole);

    QQuaternion rotation;
    rotation = QQuaternion::fromAxisAndAngle(1, 0, 0, 90);

    QMatrix4x4 scaleMat;
    scaleMat.scale(3.5f);
    QGLSceneNode *scoreNode;

    Platform *platform = new Platform(
                m_DynamicsWorld,
                QVector3D(16, 8.5, PLATFORM_Z_POS),
                QQuaternion::fromAxisAndAngle(0, 0, 1, 0) * rotation,
                m_MaterialCollection,
                m_MaterialCollection->indexOf("PlatformMaterial"),
                m_BlokShaderEffect,
                this,
                m_RootNode);
    platform->setBallMaterial(m_MaterialCollection,
                              m_MaterialCollection->indexOf("BallMaterial1"),
                              m_BlokShaderEffect,
                              Platform::YELLOW_BALL);
    platform->setLocalTransform(platform->localTransform() * scaleMat);
    scoreNode = platform->setFontMaterial(m_MaterialCollection,
                              m_MaterialCollection->indexOf("FontMaterial"));
    m_RootNode->addNode(scoreNode);
    m_Platforms.push_back(platform);


    platform = new Platform(
                m_DynamicsWorld,
                QVector3D(-16, 8.5, PLATFORM_Z_POS),
                QQuaternion::fromAxisAndAngle(0, 0, 1, 90) * rotation,
                m_MaterialCollection,
                m_MaterialCollection->indexOf("PlatformMaterial"),
                m_BlokShaderEffect,
                this,
                m_RootNode);
    platform->setBallMaterial(m_MaterialCollection,
                              m_MaterialCollection->indexOf("BallMaterial2"),
                              m_BlokShaderEffect,
                              Platform::RED_BALL);
    platform->setLocalTransform(platform->localTransform() * scaleMat);
    scoreNode = platform->setFontMaterial(m_MaterialCollection,
                              m_MaterialCollection->indexOf("FontMaterial"));
    m_RootNode->addNode(scoreNode);
    m_Platforms.push_back(platform);


    platform = new Platform(
                m_DynamicsWorld,
                QVector3D(-16, -8.5, PLATFORM_Z_POS),
                QQuaternion::fromAxisAndAngle(0, 0, 1, 180) * rotation,
                m_MaterialCollection,
                m_MaterialCollection->indexOf("PlatformMaterial"),
                m_BlokShaderEffect,
                this,
                m_RootNode);
    platform->setBallMaterial(m_MaterialCollection,
                              m_MaterialCollection->indexOf("BallMaterial3"),
                              m_BlokShaderEffect,
                              Platform::BLUE_BALL);
    platform->setLocalTransform(platform->localTransform() * scaleMat);
    scoreNode = platform->setFontMaterial(m_MaterialCollection,
                              m_MaterialCollection->indexOf("FontMaterial"));
    m_RootNode->addNode(scoreNode);
    m_Platforms.push_back(platform);


    platform = new Platform(
                m_DynamicsWorld,
                QVector3D(16, -8.5, PLATFORM_Z_POS),
                QQuaternion::fromAxisAndAngle(0, 0, 1, 270) * rotation,
                m_MaterialCollection,
                m_MaterialCollection->indexOf("PlatformMaterial"),
                m_BlokShaderEffect,
                this,
                m_RootNode);
    platform->setBallMaterial(m_MaterialCollection,
                              m_MaterialCollection->indexOf("BallMaterial4"),
                              m_BlokShaderEffect,
                              Platform::GREEN_BALL);
    platform->setLocalTransform(platform->localTransform() * scaleMat);
    scoreNode = platform->setFontMaterial(m_MaterialCollection,
                              m_MaterialCollection->indexOf("FontMaterial"));
    m_RootNode->addNode(scoreNode);
    m_Platforms.push_back(platform);


    // Create pause button.
    m_PauseButton = new PauseButton(
                QVector3D(0.0f, -8.5f, PLATFORM_Z_POS),
                m_MaterialCollection,
                m_MaterialCollection->indexOf("PauseButtonMaterial"),
                this,
                m_RootNode);
    connect(m_PauseButton, SIGNAL(clicked()), this, SLOT(pauseGame()));


    // Create explosion particles and particle system.
    QList<IParticle*> *particleList = new QList<IParticle*>();
    for (int i=0; i<100; i++) {
        ExplosionParticle *particle =
                new ExplosionParticle(m_BlokShaderEffect, m_MaterialCollection);
        particleList->push_back(particle);
    }
    m_ExplosionParticles = new ParticleSystem(particleList, this);


    // Create light particles and particle system.
    QList<IParticle*> *lightParticleList = new QList<IParticle*>();
    for (int i=0; i<100; i++) {
        LightParticle *particle =
                new LightParticle(m_MaterialCollection,
                                  m_MaterialCollection->indexOf(
                                      "LightFlareMaterial"));
        lightParticleList->push_back(particle);
    }

    m_LightParticles = new ParticleSystem(lightParticleList, this);
}


/*!
  Loads the level. If level was already loaded the existing level is destroyed.
*/
void GameView::loadLevel()
{
    delete m_Level;
    m_Level = 0;

    m_Level = new Level(m_DynamicsWorld,
                        QVector3D(0, 0, PLATFORM_Z_POS),
                        QQuaternion(),
                        m_MaterialCollection,
                        m_MaterialCollection->indexOf("BlokMaterial"),
                        m_BlokShaderEffect,
                        m_RootNode);

    // Delete all existing balls
    qDeleteAll(m_Balls);
    m_Balls.clear();

    // Reset the scores on all platforms.
    foreach (Platform *platform, m_Platforms) {
        platform->resetScore();
        platform->resetBallCreationTime();
        platform->removeBall();
    }

    // Reset the explosion particles, they might carry points from
    // the previous game to the platforms / players.
    QList<IParticle*> *particles = m_ExplosionParticles->particles();
    foreach(IParticle *particle, *particles) {
        ExplosionParticle *explosionParticle =
                static_cast<ExplosionParticle*>(particle);

        explosionParticle->resetParticle();
    }
}


/*!
  Pauses the game and shows the in-game-menu.
*/
void GameView::pauseGame()
{
    m_AudioManager->applyWindEffect(0.0f, 0.0f);
    m_MenuManager->toInGameMenu();
}


/*!
  Removes the given game object from the member QList container.
  Does not delete the given object.
*/
void GameView::removeBall(GameObject *gameObject)
{
    QList<Ball*>::iterator itBall;
    for (itBall = m_Balls.begin(); itBall != m_Balls.end(); itBall++) {
        if (*itBall == gameObject) {
            m_Balls.erase(itBall);
            break;
        }
    }
}


/*!
  Converts the a point from widget coordinate system eg. 640 x 360 to range
  0..1 x 1..0 eg. 120 x 120 equals 0.25 x 0.66667. The y-axis is inverted. Used
  to convert x, y coordinates to appropriate when passing mouse events to QML
  menu system via QGraphicsEmbedScene.
*/
QPointF GameView::convertPointToGLPos(const QPointF &pos)
{
    return QPointF(pos.x() / width(), 1 - pos.y() / height());
}


/*!
  Maps given point in x,y coordinates to x,y,z coordinates on the 3D coordinate
  on the given z-level. Used to map the mouse press on the view to the 3D
  point.
*/
QVector3D GameView::map2DPointToZLevelPoint(const QPoint &point, qreal zPos)
{
    QVector4D viewPortPoint = mapPoint(point);
    QVector3D nearPlaneWorldCoordinate =
            (viewPortPoint * camera()->modelViewMatrix() +
             camera()->eye()).toVector3D();

    QVector3D rayCastDirection = nearPlaneWorldCoordinate - camera()->eye();
    rayCastDirection.normalize();

    float distanceToTarget =
            (nearPlaneWorldCoordinate.z() - zPos) / -rayCastDirection.z();

    QVector3D dir = rayCastDirection * distanceToTarget;

    return nearPlaneWorldCoordinate + dir;
}


/*!
  Checks if the game ends. If all level bloks are destroyed and all explosion
  particles have reached the platforms (bringing all scores to players) it
  is time to end the game.
*/
void GameView::checkEndOfGame()
{
    if (m_Level == 0) {
        return;
    }

    if (m_Level->isAllBloksDestroyed() == false) {
        return;
    }

    QList<IParticle*> *particles = m_ExplosionParticles->particles();

    foreach (IParticle *particle, *particles) {
        if (particle->isActive()) {
            return;
        }
    }

    // All the bloks are destroyed and all explosion particles has reached
    // their target platform carrying their points to the player. It is
    // time to end the game and show the scores in winning screen.

    ScoreModel::typeScoreList scores;

    foreach (Platform *platform, m_Platforms) {
        scores << ScoreModel::typeScoreData(platform->score(),
                                            platform->ballId());
    }

    m_AudioManager->applyWindEffect(0.0f, 0.0f);
    m_MenuManager->toWinningScreen(scores);

    // Delete the game level so that the game won't end all the time.
    delete m_Level;
    m_Level = 0;
}


/*!
  Handles the mouse press and touch events. For example the player interaction
  with game objects are handled here, as well as interaction with the menu
  system.
  Touch event are received because the Qt::WA_AcceptTouchEvents attribute was
  set for the view in the constructor. The events are delivered for
  corresponding object depending on the case. If menus are visible MenuManager
  will handle the event, otherwise events are delivered to pause button and
  Platform objects to be handled.
*/
bool GameView::event(QEvent *event)
{
    // In the start of the application m_MenuManager might not be
    // available yet.
    if (m_MenuManager == 0) {
        return QGLView::event(event);
    }

    // If the menu is shown direct the mouse events to the Menu manager.
    if (m_MenuManager->isMenuShown()) {
        switch (event->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseMove:
        case QEvent::KeyPress:
        case QEvent::Wheel:
        case QEvent::MouseButtonDblClick: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            m_MenuManager->deliverEvent(event,
                                        convertPointToGLPos(mouseEvent->pos()));
            return true;
        }
        default:
            return QGLView::event(event);
        };
    }
    else {
        switch (event->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease: {
            // Deliver press events to the pause button
            if (m_PauseButton->handleEvent(event)) {
                return true;
            }
        }
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        case QEvent::TouchEnd: {
            // Deliver mouse press / release and touch events for the
            // platforms.
            foreach (Platform *platform, m_Platforms) {
                platform->handleEvent(event);
            }
            return true;
        }
        case QEvent::KeyPress:
        case QEvent::Wheel:
        case QEvent::MouseButtonDblClick: {
            // Overridden to prevent mouse orbiting / zooming rotating
            // of Qt3D QGLView.
            return true;
        }
        default:
            // Other event such as paint event will be handled by the
            // base class.
            return QGLView::event(event);
        };
    }

    return QGLView::event(event);
}


/*!
  Static method to receive substep simulation call back from the
  Bullet's stepSimulation.
*/
void GameView::simulationCallback(btDynamicsWorld *world, btScalar time)
{
    GameView *view = static_cast<GameView*>(world->getWorldUserInfo());
    view->simulateSubStep(time);
}


/*!
    Handles the collisions of the world.
*/
void GameView::simulateSubStep(btScalar time)
{
    Q_UNUSED(time);

    // Collision detection.
    int manifoldsCount = m_Dispatcher->getNumManifolds();

    // For destroying the game objects after the following for loop.
    QSet<Ball*> destroyGameObjectSet;

    for (int i=0; i<manifoldsCount; i++) {
        btPersistentManifold *contactManifold =
                m_Dispatcher->getManifoldByIndexInternal(i);

        int contactsCount = contactManifold->getNumContacts();

        for (int j=0; j<contactsCount; j++) {
            btManifoldPoint &pt = contactManifold->getContactPoint(j);

            btCollisionObject *objA =
                    static_cast<btCollisionObject*>(contactManifold->
                                                    getBody0());
            btCollisionObject *objB =
                    static_cast<btCollisionObject*>(contactManifold->
                                                    getBody1());

            GameObject *gameObjectA =
                    static_cast<GameObject*>(objA->getUserPointer());
            GameObject *gameObjectB =
                    static_cast<GameObject*>(objB->getUserPointer());

            if ((gameObjectA->gameObjectType() ==
                 GameObject::BALL &&
                 gameObjectB->gameObjectType() ==
                 GameObject::BLOK) ||
                    (gameObjectA->gameObjectType() ==
                     GameObject::BLOK && gameObjectB->gameObjectType() ==
                     GameObject::BALL))
            {
                ///////////////////////////////////////////////////////////////
                // Ball and Level object has collided
                Level *level;
                Ball *ball;

                if (gameObjectA->gameObjectType() == GameObject::BLOK) {
                    level = static_cast<Level*>(objA->getUserPointer());
                    ball = static_cast<Ball*>(objB->getUserPointer());
                }
                else {
                    level = static_cast<Level*>(objB->getUserPointer());
                    ball = static_cast<Ball*>(objA->getUserPointer());
                }

                btVector3 hitPoint = pt.getPositionWorldOnA();
                Level::HitInfo hitInfo = level->handleBallHit(hitPoint,
                                                              ball,
                                                              m_LastTime);

                if (hitInfo.m_BlokHit) {
                    Platform *platform = ball->platform();

                    QVector3D dir = QVector3D(pt.m_normalWorldOnB.x(),
                                              pt.m_normalWorldOnB.y(),
                                              pt.m_normalWorldOnB.z()) * 7.0f;

                    QVector3D pos = QVector3D(hitPoint.x(),
                                              hitPoint.y(),
                                              hitPoint.z());

                    m_LightTargetPosition = QVector3D(pos.x() * 3.0f,
                                                      pos.y() * 3.0f,
                                                      -45.0f);

                    // Bring the aimtarget little more to the centre
                    QVector3D aimTarget = platform->position();
                    aimTarget.setZ(aimTarget.z() - 10.0f);

                    // Flash the grid mildly
                    m_BlokFlashPower = 0.3;

                    m_ExplosionParticles->spray(1, pos, dir,
                                                0.1f, 7.0f,
                                                aimTarget,
                                                3.0f,
                                                ball->materialIndex(),
                                                platform);

                    m_AudioManager->playHitSound();

                    if (hitInfo.m_BlokDestroyed) {
                        m_LightParticles->spray(4, pos, QVector3D(0, 0, 16),
                                                0.1f, 16.0f,
                                                QVector3D(0,
                                                          0,
                                                          PLATFORM_Z_POS/2.0f),
                                                10.0f,
                                                ball->materialIndex());

                        // Flash the grid big time
                        m_BlokFlashPower = 1.0f;
                    }
                }
            }
            else if ((gameObjectA->gameObjectType() ==
                      GameObject::BALL &&
                      gameObjectB->gameObjectType() ==
                      GameObject::BLACK_HOLE) ||
                     (gameObjectA->gameObjectType() ==
                      GameObject::BLACK_HOLE &&
                      gameObjectB->gameObjectType() ==
                      GameObject::BALL))
            {
                ///////////////////////////////////////////////////////////////
                // Ball and Black hole has collided
                if (gameObjectA->gameObjectType() == GameObject::BALL) {
                    destroyGameObjectSet.insert(
                                static_cast<Ball*>(gameObjectA));
                }
                else if (gameObjectB->gameObjectType() == GameObject::BALL) {
                    destroyGameObjectSet.insert(
                                static_cast<Ball*>(gameObjectB));
                }
            }
        }
    }

    // Delayed destruction of game objects, because the game objects are
    // accessed two times in the collision loop.
    foreach (Ball *ball, destroyGameObjectSet) {
        removeBall(ball);
        delete ball;
    }
}


/*!
  Updates the tick of game logic. The Bullet physics engine, particles, audio
  effects, etc are updated here. The collision detection of the ball and blok
  objects are run on detected and handled also in here.
*/
void GameView::updateGL()
{
    QTime time = QTime::currentTime();
    int relTime = m_LastTime.msecsTo(time);
    m_LastTime = time;

    // If the menus are visible, only freeze the world.
    if (m_MenuManager->isMenuShown()) {
        QGLView::updateGL();
        return;
    }

    // Measure framerate.
    if (m_FPSElapsedTime > 1000) {
        m_FPSElapsedTime = 0;
        m_FPS = m_FPSCounter;
        m_FPSCounter = 0;

        qDebug() << "FPS: " << m_FPS;
    }
    m_FPSCounter++;
    m_FPSElapsedTime += relTime;

    // Check if new balls are needed to be created on platforms.
    foreach (Platform *platform, m_Platforms) {
        Ball *ball = platform->addBallIfRequred(relTime);
        if (ball) {
            ball->setParent(m_RootNode);
            m_RootNode->addNode(ball);
            m_Balls.push_back(ball);
        }
    }

    // Move the light towards it's target position
    m_LightPosition += (m_LightTargetPosition - m_LightPosition) *
            (float)relTime * 0.002f;
    m_BlokFlashPower -= m_BlokFlashPower*(float)relTime * 0.004f;

    // Rotate the black hole
    if (m_BlackHole) {
        m_BlackHole->rotateBlackHole(relTime);
    }

    // Update the particles
    if (m_ExplosionParticles) {
        m_ExplosionParticles->update(relTime * 0.001);
    }

    if (m_LightParticles) {
        m_LightParticles->update(relTime * 0.001);
    }

    // Simulate the world, for more information see
    // http://bulletphysics.org/mediawiki-1.5.8/index.php/Stepping_the_World
    m_DynamicsWorld->stepSimulation(relTime * 0.001, 7);

    // Apply the custom "black hole" gravity to the balls.
    foreach(Ball *ball, m_Balls) {
        ball->applyGravity();
    }

    if (m_Level) {
        btVector3 zvec = m_Level->body()->getOrientation().getAxis();

        float speed =
                fabsf( m_Level->body()->getAngularVelocity().x() ) +
                fabsf( m_Level->body()->getAngularVelocity().y() ) +
                fabsf( m_Level->body()->getAngularVelocity().z() );


        zvec.normalize();
        float ax = fabs( zvec.x() );
        float ay = fabs( zvec.y() );
        float az = fabs( zvec.z() );

        // Value will be more high if one of the axis is "straight", low if not.
        float value =
                ax * (1.0f - ay) * (1.0f-az) +
                ay * (1.0f - ax) * (1.0f-az) +
                az * (1.0f - ay) * (1.0f-ax);

        m_AudioManager->applyWindEffect(speed/8.0f+value/8.0f, value);
    }
    else {
        m_AudioManager->applyWindEffect(0.0f, 0.0f);
    }

    // Check if end of game is about to happen.
    checkEndOfGame();

    // Will eventually call paintGL method to draw the world.
    QGLView::updateGL();
}


/*!
  Initializes the QtOpenGl. Mostly every object of the application are
  initialized because the OpenGl Context is available from this on. The QTimer
  is used to call the updateGL as fast as possible.
*/
void GameView::initializeGL(QGLPainter *painter)
{
    painter->renderSequencer()->setComparator(new GameRenderOrderComparator);

    camera()->setEye(QVector3D(0, 0, 120));

    initializeMenuManager();
    initializeBulletEngine();
    initializeMaterials();
    initializeAudioEngine();
    createGameObjects();

    m_LightPosition = QVector3D(20, 20, -25.0f);
    m_LightTargetPosition = m_LightPosition;

    // Create world timer
    m_Timer = new QTimer(this);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    m_Timer->start(0);

    // Show main menu on the start
    m_MenuManager->toMainMenu();
}


/*!
  Draws the world.
*/
void GameView::paintGL(QGLPainter *painter)
{
    /*
    // Uncomment to print the QGLSceneNode-tree to debug output
    static bool printed = false;
    if (printed == false) {
        qDumpScene(m_RootNode);
        printed = true;
    }
    */

    if (!m_MenuManager->isMenuShown()) {
        glFrontFace(GL_CCW);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        if (m_Level) {
            m_Level->setGlowEffectValue(m_BlokFlashPower);
            m_Level->setLightPosition(m_LightPosition);
        }

        // Render the QGLSceneNode tree
        m_RootNode->draw(painter);

        // Render explosion particles (cubes)
        foreach (IParticle *particle, *(m_ExplosionParticles->particles())) {
            ExplosionParticle *ep = static_cast<ExplosionParticle*>(particle);

            if (particle->isActive()) {
                ep->draw(painter);
            }
        }

        // Use blending and do not write to depthbuffer while
        // rendering these particles.
        painter->disableEffect();
        glEnable(GL_BLEND);

        // Render light particles
        foreach (IParticle *particle, *(m_LightParticles->particles())) {
            LightParticle *lp = static_cast<LightParticle*>(particle);

            if (particle->isActive()) {
                lp->draw(painter);
            }
        }
    }
    else {
        painter->disableEffect();
        m_MenuManager->draw(painter);
    }
}
