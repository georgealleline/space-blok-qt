/**
 * Copyright (c) 2011 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef DECLARATIVEAUDIOEFFECT_H
#define DECLARATIVEAUDIOEFFECT_H

#include <QDeclarativeItem>
#include "audioeffect.h"
#include "declarativeaudiobuffer.h"

namespace GE {

class DeclarativeAudioEffect : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(GE::DeclarativeAudioBuffer *buffer READ buffer WRITE setBuffer)

public:
    explicit DeclarativeAudioEffect(QDeclarativeItem *parent = 0);
    virtual ~DeclarativeAudioEffect();

    void setBuffer(DeclarativeAudioBuffer *buffer);
    DeclarativeAudioBuffer *buffer();

protected:
    AudioEffect *m_effect;
    DeclarativeAudioBuffer *m_buffer;

    friend class DeclarativeAudioBuffer;
    friend class DeclarativeAudioMixer;
};

} // namespace GE

QML_DECLARE_TYPE(GE::DeclarativeAudioEffect)

#endif // DECLARATIVEAUDIOEFFECT_H

