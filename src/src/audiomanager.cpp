/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */


#include "audiomanager.h"
#include "pullaudioout.h"
#include "pushaudioout.h"
#include "audiomixer.h"
#include "audiobuffer.h"
#include "audiobufferplayinstance.h"
#include "echoeffect.h"
#include "cutoffeffect.h"

using namespace GE;

/*!
  \class AudioManager
  \brief Handles the playing and real time manipulation of the audio effects.
*/


/*!
  Constuctor, initializes the GE audio objects and begins to play the wind
  effect.
*/
AudioManager::AudioManager(QObject *parent)
    : QObject(parent)
{
    m_AudioMixer = new AudioMixer(this);

#ifdef Q_OS_SYMBIAN
    m_AudioOut = new PullAudioOut(m_AudioMixer, this);
#else
    m_AudioOut = new PushAudioOut(m_AudioMixer, this);
#endif

    m_HitSound = AudioBuffer::loadWav(":/click.wav", this);
    m_Wind = AudioBuffer::loadWav(":/effect.wav", this);

    m_WindPlayInstance = m_Wind->playWithMixer(*m_AudioMixer);
    m_WindPlayInstance->setLoopCount(-1);
}


/*!
  Plays the ball-hit-to-a-block sound effect using the GE audio mixer.
*/
void AudioManager::playHitSound()
{
    m_HitSound->playWithMixer(*m_AudioMixer);
    m_HitSound->playWithMixer(*m_AudioMixer);
}


/*!
  Applies the real time manipulation to the wind effect.
*/
void AudioManager::applyWindEffect(float speed, float power)
{
    m_WindPlayInstance->setSpeed(0.1f + speed );
    m_WindPlayInstance->setLeftVolume(0.4f + power * 0.6f);
    m_WindPlayInstance->setRightVolume(0.4f + power * 0.6f);
}
