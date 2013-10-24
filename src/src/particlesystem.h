/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef __CPARTICLESYSTEM__
#define __CPARTICLESYSTEM__

#include <QList>
#include <QVector3D>


class IParticle : public QObject
{
    Q_OBJECT

public:
    IParticle();
    virtual ~IParticle();

    /*!
      This method is called when particle is changed. from non-active to
      active / position is updated etc.
      The user should validate his renderable's information according particle's
      information here.
      The actual position if the particle is at member particlePos.
    */
    virtual void particleUpdated() = 0;

    virtual void targetReached();

    virtual float getFriction() const;
    virtual float getMass() const;
    virtual float getInitialLifeTime() const;
    virtual float getRotationMax() const;

    bool isActive() const;

    void update(const float frameDelta);

    inline void die();
    inline void setPos(const QVector3D &p);
    inline void setDir(const QVector3D &d);

    void randomRotation(float maxSpeed);
    void setAttributes(float inLifeTime, float inMass, float inFriction);

    float getLifeTime() const;

    void setTarget(const QVector3D &target, float power);

    virtual void setParticleSpecificAttribute(int att);
	
    void* userData() const;
    void setUserData(void *userData);

protected:

    // Yaw, Pitch and Roll
    float m_Rotation[3];
    // And their randomized increments through time
    float m_Rotation_inc[3];

    float m_Mass;
    float m_Friction;
    float m_LifeTime;
    QVector3D m_ParticlePos;
    QVector3D m_ParticleDir;

    // Target to a position
    float m_AimPower;
    QVector3D m_AimTo;

    void *m_UserData;
};


class ParticleSystem : public QObject
{
    Q_OBJECT

public:
    ParticleSystem(QList <IParticle*> *pool, QObject *parent);
    virtual ~ParticleSystem();

    void spray(int count, const QVector3D &pos, const QVector3D &dir,
               float posRandomR, float dirRandomR,
               const QVector3D &aimto = QVector3D(0,0,0),
               float aimpower = 0, int particleSpecific = 0,
               void *userData = 0);

    void update(const float frameDelta);

    QList<IParticle*>* particles() const;

protected:
    IParticle *freeParticleFromPool();
    QList<IParticle*> *m_ParticlePool;
};


#endif


