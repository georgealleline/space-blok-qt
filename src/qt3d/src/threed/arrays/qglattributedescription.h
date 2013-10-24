/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtQuick3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGLATTRIBUTEDESCRIPTION_H
#define QGLATTRIBUTEDESCRIPTION_H

#include <QtOpenGL/qgl.h>
#include "qt3dglobal.h"
#include "qglnamespace.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Qt3D)

class Q_QT3D_EXPORT QGLAttributeDescription
{
public:
    QGLAttributeDescription();
    QGLAttributeDescription(QGL::VertexAttribute attribute,
                            int tupleSize, GLenum type, int stride);

    bool isNull() const;

    QGL::VertexAttribute attribute() const;
    void setAttribute(QGL::VertexAttribute attribute);

    GLenum type() const;
    void setType(GLenum type);

    int sizeOfType() const;

    int tupleSize() const;
    void setTupleSize(int tupleSize);

    int stride() const;
    void setStride(int stride);

private:
    QGL::VertexAttribute m_attribute;
    GLenum m_type;
    int m_tupleSize;
    int m_stride;
};

inline QGLAttributeDescription::QGLAttributeDescription()
    : m_attribute(QGL::Position), m_type(GL_FLOAT),
      m_tupleSize(0), m_stride(0)
{
}

inline QGLAttributeDescription::QGLAttributeDescription
        (QGL::VertexAttribute attribute, int tupleSize, GLenum type, int stride)
    : m_attribute(attribute), m_type(type),
      m_tupleSize(tupleSize), m_stride(stride)
{
    Q_ASSERT(tupleSize >= 1 && tupleSize <= 4);
}

inline bool QGLAttributeDescription::isNull() const
{
    return m_tupleSize == 0;
}

inline QGL::VertexAttribute QGLAttributeDescription::attribute() const
{
    return m_attribute;
}

inline void QGLAttributeDescription::setAttribute(QGL::VertexAttribute attribute)
{
    m_attribute = attribute;
}

inline GLenum QGLAttributeDescription::type() const
{
    return m_type;
}

inline void QGLAttributeDescription::setType(GLenum type)
{
    m_type = type;
}

inline int QGLAttributeDescription::tupleSize() const
{
    return m_tupleSize;
}

inline void QGLAttributeDescription::setTupleSize(int tupleSize)
{
    Q_ASSERT(tupleSize >= 1 && tupleSize <= 4);
    m_tupleSize = tupleSize;
}

inline int QGLAttributeDescription::stride() const
{
    return m_stride;
}

inline void QGLAttributeDescription::setStride(int stride)
{
    m_stride = stride;
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
