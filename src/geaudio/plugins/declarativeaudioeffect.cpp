/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "declarativeaudioeffect.h"
#include "trace.h"

using namespace GE;

DeclarativeAudioEffect::DeclarativeAudioEffect(QDeclarativeItem *parent)
    : QDeclarativeItem(parent),
      m_effect(NULL),
      m_buffer(NULL)
{
    DEBUG_INFO(this);
}

DeclarativeAudioEffect::~DeclarativeAudioEffect()
{
    DEBUG_POINT;
}

void DeclarativeAudioEffect::setBuffer(DeclarativeAudioBuffer *buffer)
{
    DEBUG_POINT;
    m_buffer = buffer;
    m_effect->setBuffer(buffer->buffer());
}

DeclarativeAudioBuffer *DeclarativeAudioEffect::buffer()
{
    DEBUG_POINT;
    return m_buffer;
}

