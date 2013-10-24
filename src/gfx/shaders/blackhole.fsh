/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */


// Per-pixel lighting - fragment shader side.

uniform lowp sampler2D qt_Texture0; // blackhole
uniform lowp sampler2D qt_Texture1; // blackhole spiral
varying highp vec2 qt_TexCoord0;
varying highp vec2 qt_TexCoord1;
varying highp vec2 qt_TexCoord2;

void main(void)
{
     lowp vec4 t0 = texture2D(qt_Texture0, qt_TexCoord0);
     lowp vec4 t1 = texture2D(qt_Texture1, qt_TexCoord1);
     lowp vec4 t2 = texture2D(qt_Texture1, qt_TexCoord2);

     gl_FragColor = mix(mix(t0, t1, t1.w), t2, t2.w);
}
