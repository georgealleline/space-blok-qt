/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include <qglpainter.h>
#include <qglscenenode.h>
#include <btBulletDynamicsCommon.h>
#include "gameobject.h"

/*!
  \class GameObject
  \brief The base object of all game objects in the application. Compines the
         Qt3D visible object and Bullet physics object so that the visible 3D
         model is sync with the Bullet rigid body. GameObject is multiple
         inherited from QGLSceneNode and Bullet btMotionState. The
         btMotionState is used to inform how the Bullet engine moves /
         translates the physic object.
         The inherited objects should add QGLSceneNode as child as this object
         and create Bullet rigid body to the member variable m_Body representing
         the physics model of the object.
*/


/*!
  Constructor, set the Bullet discrete world, position, and rotation
  (quaternion) for the object.
*/
GameObject::GameObject(btDiscreteDynamicsWorld *world,
                       const QVector3D &pos,
                       const QQuaternion &quaternion,
                       QObject *parent)
    : QGLSceneNode(parent),
      m_World(world),
      m_Body(0),
      m_GameObjectType(UNKNOWN),
      m_Pos(btTransform(btQuaternion(quaternion.x(),
                                     quaternion.y(),
                                     quaternion.z(),
                                     quaternion.scalar()),
                        btVector3(pos.x(), pos.y(), pos.z())))

{
    // If the game object does not have Bullet body (which would move
    // QGLSceneNode) we set the position and orientation of QGLSceneNode
    // initially here.
    setPosition(pos);

    QMatrix4x4 mat = localTransform();
    mat.rotate(quaternion);
    setLocalTransform(mat);
}


/*!
  Destructor, destroys the m_SceneNode and m_Body if they are not already
  destroyed. Also destroys the Bullet collision shape (if not already
  destroyed), removes the Bullet body from the Bullet discrete world and
  destroyes the Bullet body (if not already destroyed).
*/
GameObject::~GameObject()
{
    if (m_Body) {
        m_World->removeRigidBody(m_Body);
        delete m_Body->getCollisionShape();
        delete m_Body;
        m_Body = 0;
    }
}


/*!
  Returns the Bullet rigid body.
*/
btRigidBody* GameObject::body() const
{
    return m_Body;
}


/*!
  Returns the game object type of the game object. The all game object types
  are defined on the GameObject enum.
*/
GameObject::enGameObjectType GameObject::gameObjectType() const
{
    return m_GameObjectType;
}


/*!
  Sets the initial position of the game object in Bullet rigid world. Derived
  method from the btMotionState object.
*/
void GameObject::getWorldTransform(btTransform& worldTrans) const
{
    worldTrans = m_Pos;
}


/*!
  Reports the changed position / rotation / transform of the Bullet rigid body.
  Updates the position / rotation / transform of the Qt3D QGLSceneNode of the
  game object.
*/
void GameObject::setWorldTransform(const btTransform& worldTrans)
{
    m_Pos = worldTrans;

    float ftemp[16];
    worldTrans.getOpenGLMatrix(ftemp);

    QMatrix4x4 ntrans( ftemp[0], ftemp[4], ftemp[8], 0.0f,
                       ftemp[1], ftemp[5], ftemp[9], 0.0f,
                       ftemp[2], ftemp[6], ftemp[10], 0.0f,
                       0.0f, 0.0f, 0.0f, 1.0f );

    setLocalTransform(ntrans);
    setPosition(QVector3D(ftemp[12], ftemp[13], ftemp[14]));
}


/*!
  Draws the Qt3D QGLSceneNode with the given painter.
*/
void GameObject::draw(QGLPainter *painter)
{
    QGLSceneNode::draw(painter);
}
