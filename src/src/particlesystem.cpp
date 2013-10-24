/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include <QDebug>
#include "particlesystem.h"

#define TARGET_REACH_SQRDISTANCE_EPSILON (16.0f)


/*!
  \class IParticle
  \brief Interface for representing a single particle.
*/




/*!
  Constructor. Particle is set as non-active.
*/
IParticle::IParticle()
    : m_LifeTime(0.0f),
      m_UserData(0)
{
}


IParticle::~IParticle()
{
}


/*!
  The set target has been reached.
*/
void IParticle::targetReached()
{
}


/*!
  How much this particle slow's down through time. 0 = does not slow down at all
  1 = stops instantly. Reasonable values are little above zero.
*/
float IParticle::getFriction() const
{
    return 0.0f;
}


/*!
  Default mass of this particle.
*/
float IParticle::getMass() const
{
    return 1.0f;
}


/*!
  Used by the particlesystem when randomizing the particle
  Assigns a lifetime for a newly activated particle. Lifetime is in seconds.
  Example 1-3 seconds "return 1.0f + random(2.0f)"
*/
float IParticle::getInitialLifeTime() const
{
    return 1.0f;
}


/*!
  How fast particle will rotate maximumly.
*/
float IParticle::getRotationMax() const
{
    return 1.0f;
}


/*!
  Returns true when lifetime is above zero, false otherwise.
*/
bool IParticle::isActive() const
{
    if (m_LifeTime > 0)
        return true;
    else
        return false;
}


/*!
  Updates this particle's position and directions according passed time.
*/
void IParticle::update(const float frameDelta)
{
    // Lifetime
    m_LifeTime -= frameDelta;

    // Movement
    QVector3D temp = m_ParticleDir * frameDelta;
    m_ParticlePos += temp;

    // Friction / air resistance
    temp *= m_Friction;
    m_ParticleDir -= temp;

    // Rotation
    m_Rotation[0] += m_Rotation_inc[0] * frameDelta;
    m_Rotation[1] += m_Rotation_inc[1] * frameDelta;
    m_Rotation[2] += m_Rotation_inc[2] * frameDelta;

    if (m_AimPower > 0.0f) {
        temp = m_AimTo - m_ParticlePos;

        float sqrdistance = QVector3D::dotProduct(temp, temp);
        if (sqrdistance < TARGET_REACH_SQRDISTANCE_EPSILON) {
            // destination reached. Signal it.
            targetReached();
        }

        float power = (m_AimPower * 2.0f) / (sqrdistance * 0.01f + 1.0f);
        temp *= (power * frameDelta);
        m_ParticleDir += temp;
    }
}


/*!
  Marks the particle as non-active.
*/
inline void IParticle::die()
{
    m_LifeTime = 0.0f;
}


/*!
  Sets the position of the particle to given p.
*/
inline void IParticle::setPos(const QVector3D &p)
{
    m_ParticlePos = p;
}


/*!
  Sets the direction where the particle is goind to d.
*/
inline void IParticle::setDir(const QVector3D &d)
{
    m_ParticleDir = d;
}


/*!
  Randomizes the rotation of the particle. The maxSpeed will clamp the maximum
  value for the rotation.
*/
void IParticle::randomRotation(float maxSpeed)
{
    m_Rotation[0] = (float)(rand() & 255)/128.0f * 3.14159f;
    m_Rotation[1] = (float)(rand() & 255)/128.0f * 3.14159f;
    m_Rotation[2] = (float)(rand() & 255)/128.0f * 3.14159f;

    m_Rotation_inc[0] = maxSpeed * (float)((rand() & 255)-128)*maxSpeed;
    m_Rotation_inc[1] = maxSpeed * (float)((rand() & 255)-128)*maxSpeed;
    m_Rotation_inc[2] = maxSpeed * (float)((rand() & 255)-128)*maxSpeed;
}


/*!
  Sets the attributes of the particle.
*/
void IParticle::setAttributes(float inLifeTime, float inMass, float inFriction)
{
    m_LifeTime = inLifeTime;
    m_Mass = inMass;
    m_Friction = inFriction;
}


/*!
  Gets the life time of the particle.
*/
float IParticle::getLifeTime() const
{
    return m_LifeTime;
}


/*!
  Sets the target where the particle will aim (move to) and power how fast
  the particle will move towards the target.
*/
void IParticle::setTarget(const QVector3D &target, float power)
{
    m_AimTo = target;
    m_AimPower = power;
}


/*!
  Sets the particle specific attribute that can control some custom behavior
  of the particle.
*/
void IParticle::setParticleSpecificAttribute(int att)
{
}


/*!
  Returns the user pointer associated to particle.
*/
void* IParticle::userData() const
{
    return m_UserData;
}


/*!
  Sets the user pointer for the particle.
*/
void IParticle::setUserData(void *userData)
{
    m_UserData = userData;
}



/*!
  \class ParticleSystem
  \brief Manages and updates the particles position / rotation of a same
         type (object).
*/


/*!
  Constructor, saves the particle pool, takes the ownership of the particles.
*/
ParticleSystem::ParticleSystem(QList <IParticle*> *pool, QObject *parent)
    : QObject(parent),
      m_ParticlePool(pool)
{
}


/*!
  Destructor, destroyed the particle pool.
*/
ParticleSystem::~ParticleSystem()
{
    foreach(IParticle *particle, *m_ParticlePool) {
        delete particle;
    }

    delete m_ParticlePool;
}


/*!
  Returns a particle from the pool that is inactive. If all particles are in use
  returns 0.
*/
IParticle* ParticleSystem::freeParticleFromPool()
{
    for (int i=0; i<m_ParticlePool->size(); i++) {
        if (m_ParticlePool->at(i)->isActive() == false) {
            return m_ParticlePool->at(i);
        }
    }

    // Failed to find a free particle from the pool
    return 0;
}


/*!
  Sprays given amount of particles to a given position, direction. Parameters
  posRandomR, dirRandomR will apply amount of randomizing to the position and
  direction. aimto, aimpower will define to which direction the particle will
  aim to and with which speed (power). particleSpecific paramter can be used
  to store particle system specific data / attribute. Also userData can be
  used to store used pointer for the particle.
*/
void ParticleSystem::spray(int count, const QVector3D &pos,
                           const QVector3D &dir, float posRandomR,
                           float dirRandomR, const QVector3D &aimto,
                           float aimpower, int particleSpecific,
                           void *userData)
{
    QVector3D temp;
    while (count > 0) {
        IParticle *p = freeParticleFromPool();
        if (!p) {
            qDebug() << "ParticleSystemWarning: Discarding emit since there "
                     << "are no free particles in the pool.";
            return;
        }

        // Randomize particle
        temp = QVector3D(-128 + (rand()&255), -128 + (rand()&255), -128 +
                         (rand()&255));
        temp.normalize();

        p->setPos(pos + temp*posRandomR);
        p->setDir(dir + temp*dirRandomR);
        p->setParticleSpecificAttribute(particleSpecific);
        p->setAttributes(p->getInitialLifeTime(), p->getMass(),
                         p->getFriction());
        p->randomRotation(p->getRotationMax());
        p->setTarget(aimto, aimpower);
        p->setUserData(userData);
        count--;
    }
}


/*!
  Updates the particles.
*/
void ParticleSystem::update(const float frameDelta)
{
    for (int i=0; i<m_ParticlePool->size(); i++) {
        IParticle *p = m_ParticlePool->at(i);
        if (p->isActive()) {
            p->update(frameDelta);
            p->particleUpdated();
        }
    }
}


/*!
  Returns the particles that are managed by the particle system.
*/
QList<IParticle*>* ParticleSystem::particles() const
{
    return m_ParticlePool;
}
