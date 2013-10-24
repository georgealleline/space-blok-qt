#include <QMouseEvent>
#include <qglbuilder.h>
#include <qglpainter.h>
#include <qglmaterialcollection.h>
#include "pausebutton.h"
#include "gameview.h"

/*!
  \class PauseButton
  \brief Represents pause button.
*/


/*!
  Constructor, creates pause button.
*/
PauseButton::PauseButton(const QVector3D &pos,
                         QGLMaterialCollection *materialCollection,
                         int materialIndex,
                         GameView *view,
                         QObject *parent)
    : QGLSceneNode(parent),
      m_GameView(view),
      m_Pressed(false),
      m_MaterialCollection(materialCollection),
      m_MaterialIndex(materialIndex)

{
    QGLBuilder builder;
    builder.addPane(QSizeF(3.0f, 3.0f));

    QGLSceneNode *node = builder.finalizedSceneNode();
    addNode(node);
    node->setParent(this);

    setPosition(pos);

    setEffect(QGL::FlatReplaceTexture2D);

    setPalette(materialCollection);
    setMaterialIndex(materialIndex);

    m_ResetScale = localTransform();
}


/*!
  Destructor.
*/
PauseButton::~PauseButton()
{
}


/*!
  Handles mouse button press and mouse button release events and emits
  clicked signal accordingly.
*/
bool PauseButton::handleEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::MouseButtonPress: {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QVector3D pos = m_GameView->map2DPointToZLevelPoint(
                    mouseEvent->pos(), position().z());

        if (hitTest(pos)) {
            m_Pressed = true;
            QMatrix4x4 mat = localTransform();
            mat.scale(0.95f);
            setLocalTransform(mat);
            return true;
        }
        break;
    }
    case QEvent::MouseButtonRelease: {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QVector3D pos = m_GameView->map2DPointToZLevelPoint(
                    mouseEvent->pos(), position().z());

        if (hitTest(pos) && m_Pressed) {
            emit clicked();
        }

        m_Pressed = false;
        setLocalTransform(m_ResetScale);
        break;
    }
    default:
        return false;
    }

    return false;
}


/*!
  Tests if the given point is near the button.
*/
bool PauseButton::hitTest(const QVector3D &pos)
{
    qreal length = (pos - position()).length();

    if (length < 2) {
        return true;
    }

    return false;
}


/*!
  Draws the pause button with the given painter.
*/
void PauseButton::draw(QGLPainter *painter)
{
    // Draw the pause button on top of everything else
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    QGLSceneNode::draw(painter);

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}
