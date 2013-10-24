/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "blocksshader.h"

/*!
  \class BlocksShaderEffect
  \brief Shader program for the game. Implements fake diffuse environment lightning
         model with normal mapping.
*/


/*!
  Constructor, sets the vertex and fragment shaders from files.
*/
BlocksShaderEffect::BlocksShaderEffect()
{
    setVertexShaderFromFile(":/shaders/bloks.vsh");
    setFragmentShaderFromFile(":/shaders/bloks.fsh");
}
