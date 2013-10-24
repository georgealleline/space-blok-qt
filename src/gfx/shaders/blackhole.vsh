/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */


// Per-pixel lighting - vertex shader side.

attribute highp vec4 qt_Vertex;
attribute highp vec3 qt_Normal;
attribute highp vec4 qt_MultiTexCoord0;
uniform mediump mat4 qt_ModelViewMatrix;
uniform mediump mat4 qt_ModelViewProjectionMatrix;
uniform mediump mat3 qt_NormalMatrix;
uniform mediump mat2 rotMat;
varying highp vec2 qt_TexCoord0;
varying highp vec2 qt_TexCoord1;
varying highp vec2 qt_TexCoord2;

varying mediump vec4 vertex;

void main(void)
{
    const highp vec2 offset = vec2(0.5, 0.5);
    qt_TexCoord0 = qt_MultiTexCoord0.st;
    qt_TexCoord1 = ((qt_TexCoord0 - offset) * rotMat) + offset;
    qt_TexCoord2 = ((qt_TexCoord1 - offset) * rotMat) + offset;
    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;
}
