/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "blackholeshadereffect.h"

/*!
  \class BlackHoleShaderEffect
  \brief Shader program to implement black hole whirl effect.

*/


/*!
  Constructor, sets the vertex and fragment shaders from files.
*/
BlackHoleShaderEffect::BlackHoleShaderEffect()
{
    setVertexShaderFromFile(":/shaders/blackhole.vsh");
    setFragmentShaderFromFile(":/shaders/blackhole.fsh");
}
