/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */


#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class QGLPainter;
class btDiscreteDynamicsWorld;
class btRigidBody;

#include <QObject>
#include <qglscenenode.h>
#include <QVector3D>
#include <QQuaternion>
#include <LinearMath/btMotionState.h>

#define BIT(x) (1<<(x))

class GameObject : public QGLSceneNode, public btMotionState
{
    Q_OBJECT
public:

    enum enGameObjectType {
        UNKNOWN,
        BLOK,
        BALL,
        PLATFORM,
        BLACK_HOLE
    };

    enum enCollisionTypes {
        COL_NOTHING = 0,         // Collide with nothing
        COL_BLOK = BIT(0),       // Collide with Bloks
        COL_BALL = BIT(1),       // Collide with Balls
        COL_PLATFORM = BIT(2),   // Collide with Platforms
        COL_BLACK_HOLE = BIT(3)  // Collide with black hole
    };

    explicit GameObject(btDiscreteDynamicsWorld *world,
                        const QVector3D &pos = QVector3D(),
                        const QQuaternion &quaternion = QQuaternion(),
                        QObject *parent = 0);
    virtual ~GameObject();

    // btMotionState derived methods
    virtual void getWorldTransform(btTransform& worldTrans) const;
    virtual void setWorldTransform(const btTransform& worldTrans);

    // QGLSceneNode derived method
    virtual void draw(QGLPainter *painter);

    btRigidBody* body() const;
    enGameObjectType gameObjectType() const;

protected:

    btDiscreteDynamicsWorld *m_World;
    btRigidBody *m_Body;
    enGameObjectType m_GameObjectType;
    btTransform m_Pos;
};

#endif // GAMEOBJECT_H
