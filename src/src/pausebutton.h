#ifndef PAUSEBUTTON_H
#define PAUSEBUTTON_H

#include <qglscenenode.h>
#include <QVector3D>

class QGLMaterialCollection;
class QGLPainter;
class GameView;

class PauseButton : public QGLSceneNode
{
    Q_OBJECT

public:
    PauseButton(const QVector3D &pos,
                QGLMaterialCollection *materialCollection,
                int materialIndex,
                GameView *view,
                QObject *parent);
    virtual ~PauseButton();

    bool handleEvent(QEvent *event);

    virtual void draw(QGLPainter *painter);

signals:
    void clicked();

protected:
    bool hitTest(const QVector3D &pos);

protected:
    QMatrix4x4 m_ResetScale;

    GameView *m_GameView;
    bool m_Pressed;

    QGLMaterialCollection *m_MaterialCollection;
    int m_MaterialIndex;
};

#endif // PAUSEBUTTON_H
