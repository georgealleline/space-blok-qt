/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */


// Per-pixel lighting - fragment shader side.

uniform lowp sampler2D qt_Texture0;
uniform lowp sampler2D qt_Texture1;

varying mediump vec3 qNormal;
uniform mediump vec4 ambient;
uniform mediump vec4 diffuse;
varying mediump vec4 vertex;
varying mediump vec3 toLight;
varying highp vec2 qt_TexCoord0;
uniform mediump vec4 specular;
uniform mediump float shininess;


void main(void)
{
    mediump vec4 color = texture2D(qt_Texture0, qt_TexCoord0);
    mediump vec4 reflectionColor = texture2D(qt_Texture1, vertex.xy * 0.2 +
                                             qNormal.xy * 0.5);
    mediump float lmul = dot(normalize(qNormal), normalize(toLight));
    mediump float clmul = clamp(abs(lmul) * lmul, 0.0, 1.0);
    clmul = clmul * clmul / (0.9 + dot(toLight, toLight) * 0.0005);
    lmul = clamp(-lmul, 0.0, 1.0);

    gl_FragColor = vec4(color.xyz * (diffuse.xyz * vec3(clmul, clmul, clmul) +
                                     ambient.xyz * vec3(lmul,lmul,lmul)) +
                        reflectionColor.xyz * shininess +
                        specular.xyz * color.w,
                        clamp(5.0 + vertex.z * 0.05, 0.0, 1.0)
                        );
}
