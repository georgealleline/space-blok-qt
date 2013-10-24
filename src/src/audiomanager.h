/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */


#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>

// GE Audio forward declarations
namespace GE {
    class AudioOut;
    class AudioMixer;
    class AudioBuffer;
    class AudioBufferPlayInstance;
    class CutOffEffect;
}


class AudioManager : public QObject
{
    Q_OBJECT
public:
    explicit AudioManager(QObject *parent = 0);

signals:

public slots:
    void playHitSound();
    void applyWindEffect(float speed, float power);

protected:
    GE::AudioOut *m_AudioOut;
    GE::AudioMixer *m_AudioMixer;

    GE::AudioBuffer *m_HitSound;
    GE::AudioBuffer *m_Wind;
    GE::AudioBufferPlayInstance *m_WindPlayInstance;

    GE::CutOffEffect *m_WindEffect;
};

#endif // AUDIOMANAGER_H
