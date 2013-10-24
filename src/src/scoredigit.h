#ifndef SCOREDIGIT_H
#define SCOREDIGIT_H

#include <qglscenenode.h>

class QGLMaterialCollection;
class QGLPainter;
class QVector3D;
class QQuaternion;

class ScoreDigit : public QGLSceneNode
{
    Q_OBJECT

public:
    explicit ScoreDigit(QGLMaterialCollection *materialCollection,
                       int materialIndex,
                       const QVector3D &pos,
                       QGLSceneNode *parent = 0);
    virtual ~ScoreDigit();

    void draw(QGLPainter *painter);

    int value() const;
    void setValue(int value);

protected:
    int m_Value;
};

#endif // SCOREDIGIT_H
