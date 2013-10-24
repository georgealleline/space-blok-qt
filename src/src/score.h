#ifndef SCORE_H
#define SCORE_H

#include <qglscenenode.h>
#include <QVector3D>
#include <QQuaternion>

class QGLPainter;
class QGLMaterialCollection;
class ScoreDigit;


class Score : public QGLSceneNode
{
    Q_OBJECT
public:

    explicit Score(QGLMaterialCollection *materialCollection,
                   int materialIndex,
                   const QVector3D &pos,
                   const QQuaternion &quaternion,
                   QObject *parent = 0);

    void draw(QGLPainter *painter);

    int score() const;
    void setScore(int score);

protected:

    QList<ScoreDigit*> m_Digits;
};

#endif // SCORE_H
