/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include <QTime>
#include <QEvent>
#include <QMouseEvent>
#include <qglabstractscene.h>
#include <qglshaderprogram.h>
#include <btBulletDynamicsCommon.h>
#include "ball.h"
#include "gameview.h"
#include "platform.h"
#include "score.h"
#include "math.h"


/*!
  \class Platform
  \brief Represents a platform that generates balls on top of them. Handles
         the player swipes and holds the score of the each player.
*/


/*!
  Constructor, creates platform to given position. Imports platform.obj 3D model
  and creates and instance of it. The platform does not have Bullet collision
  shape / body at all and the object is only visible object.
*/
Platform::Platform(btDiscreteDynamicsWorld *world, const QVector3D &pos,
                   const QQuaternion &quaternion,
                   QGLMaterialCollection *materialCollection,
                   int materialIndex,
                   QGLShaderProgramEffect *effect,
                   GameView *view,
                   QObject *parent)
    : GameObject(world, pos, quaternion, parent),
      m_GameView(view),
      m_MillSecondsToCreateNewBall(2000)
{
    m_BallId = UNKNOWN;
    m_ScoreFont = 0;

    m_SpecularLoc = -1;
    m_ShininessLoc = -1;

    m_ShaderEffect = effect;
    m_GameObjectType = PLATFORM;
    m_Ball = 0;
    m_BallInitialPos = pos;
    m_BallInitialPos.setZ(m_BallInitialPos.z() + 1.0f);
    m_LastTime = 10000000;
    m_Pressed = false;

    QGLAbstractScene *levelModel =
            QGLAbstractScene::loadScene(":/platform.obj");
    QGLSceneNode *node = levelModel->mainNode()->clone();
    addNode(node);

    delete levelModel;
    levelModel = 0;

    if (effect) {
        setUserEffect(effect);
    }
    else {
        setEffect(QGL::FlatReplaceTexture2D);
    }

    foreach(QGLSceneNode *node, node->allChildren()) {
        node->setPalette(materialCollection);
        node->setMaterialIndex(materialIndex);
    }
}


/*!
  Creates new Ball object on top of the platform. Sets this plaform as the
  creator of the ball.
*/
Ball* Platform::createBall()
{
    // Parent is set as 0. The GameView is responsible of destroying all of
    // the GameObjects.
    m_Ball = new Ball(m_World,
                      m_BallInitialPos,
                      0.6f,
                      m_BallMaterialCollection,
                      m_BallMaterialIndex,
                      true,
                      m_BallShaderEffect,
                      0);

    m_Ball->setPlatform(this);

    return m_Ball;
}


/*!
  Checks if enough time has passed and if it is required to create new ball
  on top of the platform.
  Returns the newly created ball or 0 if the ball already exists on top of
  the platform or not enough time has been passed when the last ball was
  swiped.
*/
Ball* Platform::addBallIfRequred(int relTime)
{
    if (m_Ball) {
        return 0;
    }

    m_LastTime += relTime;
    if (m_LastTime > m_MillSecondsToCreateNewBall) {
        m_LastTime = 0;

        return createBall();
    }

    return 0;
}


/*!
  Removes the ball laying on top of the platform. Used in a new game, when
  previous game's balls are destroyed.
*/
void Platform::removeBall()
{
    m_Ball = 0;
}


/*!
  Resets the time when last ball was created making the ball regenerate
  on next addBallIfRequired call.
*/
void Platform::resetBallCreationTime()
{
    m_LastTime = m_MillSecondsToCreateNewBall + 1;
}


/*!
  Tests if the given position is close enough the platform position.
*/
bool Platform::hitTest(const QVector3D &pos)
{
    qreal length = (m_BallInitialPos - pos).length();

    if (length < 7) {
        return true;
    }

    return false;
}


/*!
  Handles MouseButtonPress, MouseButtonRelease and touch events.
  Other events will not be processed. Used to detect swipes that throw the
  ball from platform.
*/
bool Platform::handleEvent(QEvent *event)
{
    if (!m_Ball) {
        return false;
    }

    switch (event->type()) {
    case QEvent::MouseButtonPress: {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QVector3D pos =
                m_GameView->map2DPointToZLevelPoint(
                    mouseEvent->pos(), m_BallInitialPos.z());
        return handlePressInput(pos, -1);
    }
    case QEvent::MouseButtonRelease: {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QVector3D pos =
                m_GameView->map2DPointToZLevelPoint(
                    mouseEvent->pos(), m_BallInitialPos.z());
        return handleReleaseInput(pos, -1);
    }
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd: {
        QList<QTouchEvent::TouchPoint> touchPoints =
                static_cast<QTouchEvent*>(event)->touchPoints();

        foreach (const QTouchEvent::TouchPoint &touchPoint, touchPoints) {
            switch (touchPoint.state()) {
            case Qt::TouchPointPressed: {
                QVector3D pos =
                        m_GameView->map2DPointToZLevelPoint(
                            touchPoint.pos().toPoint(),
                            m_BallInitialPos.z());
                handlePressInput(pos, touchPoint.id());
                break;
            }
            case Qt::TouchPointReleased: {
                QVector3D pos =
                        m_GameView->map2DPointToZLevelPoint(
                            touchPoint.pos().toPoint(),
                            m_BallInitialPos.z());
                handleReleaseInput(pos, touchPoint.id());
                break;
            }
            default: {
                break;
            }
            }
        }
        break;
    }
    default:
        return false;
    }

    return true;
}


/*!
  Handles the press input on this platform.
*/
bool Platform::handlePressInput(const QVector3D &pos, int touchId)
{
    if (hitTest(pos) == false) {
        return false;
    }

    m_TouchId = touchId;
    m_Pressed = true;
    m_SwipePressTime = QTime::currentTime();
    m_SwipePressPos = pos;

    return true;
}


/*!
  Handles the release input on this platform.
*/
bool Platform::handleReleaseInput(const QVector3D &pos, int touchId)
{
    if (m_Pressed == false) {
        return false;
    }

    if (touchId != m_TouchId) {
        return false;
    }

    m_Pressed = false;

    QTime time = QTime::currentTime();
    int msecs = m_SwipePressTime.msecsTo(time);

    Q_UNUSED(msecs);
    // ToDo: implement swype velocity by using timestamps, at the moment
    // only the length of the swipe sets the impulse for the ball.

    m_Ball->body()->setDamping(0.0f, 0.0f);

    QVector3D direction = m_SwipePressPos - pos + QVector3D(0.0f, 0.0f, 3.0f);

    if (direction.length() > 25.0f) {
        direction.normalize();
        direction *= 25.0f;
    }

    btVector3 impulse = btVector3(-direction.x() * 7,
                                  -direction.y() * 7,
                                  direction.z() * 7);

    m_Ball->body()->applyImpulse(impulse, btVector3(0, 0, 0));
    m_Ball = 0;

    return true;
}


/*!
  Adds score to the platform.
*/
void Platform::addScore(int score)
{
    if (m_ScoreFont) {
        m_ScoreFont->setScore(m_ScoreFont->score() + score);
    }
}


/*!
  Returns the score of platform.
*/
int Platform::score() const
{
    if (m_ScoreFont) {
        return m_ScoreFont->score();
    }

    return -1;
}


/*!
  Sets the score to zero.
*/
void Platform::resetScore()
{
    if (m_ScoreFont) {
        m_ScoreFont->setScore(0);
    }
}


/*!
  Returns the ball color.
*/
QColor Platform::ballColor() const
{
    QGLMaterial *material =
            m_BallMaterialCollection->material(m_BallMaterialIndex);
    return material->diffuseColor();
}


/*!
  Returns the id (enum) of the ball describing the color of the ball.
*/
Platform::BALL_ID Platform::ballId () const
{
    return m_BallId;
}


/*!
  Draws the platform.
*/
void Platform::draw(QGLPainter *painter)
{
    if (userEffect()) {
        QGLShaderProgramEffect *effect =
                static_cast<QGLShaderProgramEffect*>(userEffect());

        effect->setActive(painter, true);

        QGLShaderProgram *program = effect->program();

        if (program) {
            if (m_SpecularLoc == -1) {
                m_SpecularLoc = program->uniformLocation("specular");
            }

            if (m_ShininessLoc == -1) {
                m_ShininessLoc = program->uniformLocation("shininess");
            }

            program->setUniformValue(m_SpecularLoc,
                                     QVector4D(0.3f, 0.3f, 0.3f, 0.3f));
            program->setUniformValue(m_ShininessLoc, 0.0f);
        }

        GameObject::draw(painter);
    }
    else {
        GameObject::draw(painter);
    }
}


/*!
  Sets the material and the effect to the Balls that this platform will
  generate.
*/
void Platform::setBallMaterial(QGLMaterialCollection *materialCollection,
                               int materialIndex,
                               QGLShaderProgramEffect *ballEffect,
                               BALL_ID ballId)
{
    m_BallMaterialCollection = materialCollection;
    m_BallMaterialIndex = materialIndex;
    m_BallShaderEffect = ballEffect;
    m_BallId = ballId;
}


/*!
  Creates a score font and sets the texture for it. The QGLSceneNode is returned
  to the caller, who should add the node to the drawing tree. By keeping the
  height of the tree short the Qt3D seems to perform better.
*/
QGLSceneNode* Platform::setFontMaterial(QGLMaterialCollection *materialCollection,
                                        int materialIndex)
{
    if (m_ScoreFont) {
        delete m_ScoreFont;
    }

    QVector3D platformPos = position();
    QVector3D scorePos;

    QQuaternion quaternion;
    if (platformPos.x() > 0 && platformPos.y() < 0) {
        quaternion = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), 45);
        scorePos = platformPos + QVector3D(-1.5f, 1.5f, 0);
    }
    else if (platformPos.x() > 0 && platformPos.y() > 0) {
        quaternion = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), 135);
        scorePos = platformPos + QVector3D(-1.5f, -1.5f, 0);
    }
    else if (platformPos.x() < 0 && platformPos.y() > 0) {
        quaternion = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), 225);
        scorePos = platformPos + QVector3D(1.5f, -1.5f, 0);
    }
    else if (platformPos.x() < 0 && platformPos.y() < 0) {
        quaternion = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), 315);
        scorePos = platformPos + QVector3D(1.5f, 1.5f, 0);
    }

    m_ScoreFont = new Score(materialCollection,
                            materialIndex,
                            scorePos,
                            quaternion,
                            0);

    return m_ScoreFont;
}
