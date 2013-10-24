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
varying highp vec2 qt_TexCoord0;

uniform vec3 lightPosition;

varying mediump vec3 qNormal;
varying mediump vec4 vertex;
varying mediump vec3 toLight;

void main(void)
{
    qt_TexCoord0 = qt_MultiTexCoord0.st;
    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;
    vertex = qt_ModelViewMatrix * qt_Vertex;
    toLight = lightPosition - vertex.xyz;
    qNormal = normalize(qt_NormalMatrix * qt_Normal);
}
