#include <qglview.h>
#include <qglpainter.h>
#include <qglmaterial.h>
#include <qglabstractscene.h>
#include "scenenode.h"

SceneNode::SceneNode(QGLSceneNode *meshObject, QObject *parent)
    : QGLSceneNode(parent)
{
}


SceneNode::SceneNode(QGLAbstractScene *abstractScene, QObject *parent)
    : QObject(parent)
{
    m_Position = QVector3D(0, 0, 0);
    m_Scale = 1.0f;
}


void SceneNode::setMaterial(QGLMaterial *material)
{
    m_Material = material;
    m_MeshObject->setMaterial(material);

    // The material is set recursively to all children.
    QList<QGLSceneNode*> children = m_MeshObject->allChildren();
    foreach (QGLSceneNode *node, children) {
        node->setMaterial(material);
        node->setEffect(QGL::LitModulateTexture2D);
    }
}


void SceneNode::initialize(QGLView *view, QGLPainter *painter)
{
}


void SceneNode::draw(QGLPainter *painter)
{
    /*
    // Position the model at its designated position, scale, and orientation.
    painter->modelViewMatrix().push();
    painter->modelViewMatrix().translate(m_Position);

    if (m_Scale != 1.0f) {
        painter->modelViewMatrix().scale(m_Scale);
    }

    if (m_RotationAngle != 0.0f) {
        painter->modelViewMatrix().rotate(m_RotationAngle, m_RotationVector);
    }

    m_MeshObject->draw(painter);

    // Restore the modelview matrix.
    painter->modelViewMatrix().pop();
    */
}
