#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include <qglscenenode.h>
#include <QVector3D>

class QGLPainter;
class QGLMaterial;
class QGLAbstractScene;

class SceneNode : public QGLSceneNode
{
public:
    SceneNode(QGLSceneNode *meshObject, QObject *parent = 0);
    SceneNode(QGLAbstractScene *abstractScene, QObject *parent = 0);

    void initialize(QGLView *view, QGLPainter *painter);
    void draw(QGLPainter *painter);

    void setMaterial(QGLMaterial *material);
};

#endif // MESHOBJECT_H
