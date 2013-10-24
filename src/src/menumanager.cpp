/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include <QApplication>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>
#include <QGraphicsObject>
#include <qglpainter.h>
#include <qgraphicsembedscene.h>
#include <qglmaterialcollection.h>
#include <qglbuilder.h>
#include <qglcamera.h>
#include <qglframebufferobject.h>
#include "menumanager.h"

#ifdef MEEGO_EDITION_HARMATTAN
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif

/*!
  \class MenuManager
  \brief Handles the creation, managing and showing of the QML menu system.
         The main idea of the implementation is to render QML scene to a
         texture and show the texture in the face of the 3D object that fills
         the camera view.
         Class also handles all interaction between the 3D and QML such as
         transferring of mouse events to the QML.
*/


/*!
  Constructor, creates QGraphicsEmbedScene and loads the main.qml. Qt3D plane
  is created and positioned on the cameras near plane filling the whole view
  by the camera. The menu material is assigned for the QGLSceneNode. The
  texture of the material will be upadted later on renderToTexture calls.
*/
MenuManager::MenuManager(QGLCamera *camera, QObject *parent)
    : QObject(parent)
{
    setMenuShown(false);

    qmlRegisterType<ScoreModel>("DataElements", 1, 0, "ScoreModel");

    m_GraphicsScene = new QGraphicsEmbedScene(0, 0, 640, 360, this);
    m_GraphicsScene->setItemIndexMethod(QGraphicsScene::NoIndex);

    m_DeclarativeEngine = new QDeclarativeEngine(this);
    m_DeclarativeComponent =
            new QDeclarativeComponent(m_DeclarativeEngine,
                                      QUrl("qrc:/main.qml"), this);
    m_MainQML =
            qobject_cast<QGraphicsObject*>(m_DeclarativeComponent->create());
    m_GraphicsScene->addItem(m_MainQML);

    m_ScoreModel = m_MainQML->findChild<ScoreModel *>("scoreModel");

    m_MaterialCollection = new QGLMaterialCollection(this);
    m_MenuMaterial = new QGLMaterial(this);
    m_MenuMaterial->setObjectName("MenuMaterial");

    QGLTexture2D *texture = new QGLTexture2D(this);

    m_MenuMaterial->setTexture(texture);
    int materialIndex = m_MaterialCollection->addMaterial(m_MenuMaterial);

    // Create plane to the near plane and fill the view with it,
    // the size should be calculated some how to fill the view exactly, but this
    // will do for now.
    QGLBuilder builder;
    builder.addPane(QSizeF(3.546, 1.994625));
    m_MenuNode = builder.finalizedSceneNode();

    m_MenuNode->setPalette(m_MaterialCollection);
    m_MenuNode->setMaterialIndex(materialIndex);
    m_MenuNode->setEffect(QGL::FlatReplaceTexture2D);
    m_MenuNode->setPosition(QVector3D(0,
                                      0,
                                      camera->eye().z() - camera->nearPlane()));

    // QML signals and slots
    connect(m_MainQML, SIGNAL(newGame()), this, SLOT(newGameStarted()));
    connect(m_MainQML, SIGNAL(resume()), this, SLOT(resumeGame()));
    connect(m_MainQML, SIGNAL(exit()), qApp, SLOT(quit()));
}


/*!
  Returns true if the menus are shown (will be drawn).
*/
bool MenuManager::isMenuShown() const
{
    return m_MenuShown;
}


/*!
*/
void MenuManager::setMenuShown(bool isShown)
{
    m_MenuShown = isShown;
    toggleSwipe(m_MenuShown);
}


/*!
  Shows the main menu. Will set the menus to be drawn.
*/
void MenuManager::toMainMenu()
{
    QMetaObject::invokeMethod(m_MainQML, "toMainMenu", Qt::AutoConnection);
    setMenuShown(true);
}


/*!
  Shows the in-game-menu. Will set the menus to be drawn.
  The in-game-menu is not possible to be drawn with aplha blending
  currently, so the back of the in-game-menu will be black.
*/
void MenuManager::toInGameMenu()
{
    QMetaObject::invokeMethod(m_MainQML, "toInGameMenu", Qt::AutoConnection);
    setMenuShown(true);
}


/*!
  Shows the winning screen with scores. The two lists provides the scores
  and ball colors to be shown. The information is set to the custom
  list model which pointer was retrieved in the consturctor of this class
  from the QML. When the model is updated the change will be shown in the
  WinningScreen in the QML.
*/
void MenuManager::toWinningScreen(const ScoreModel::typeScoreList &scores)
{
    m_ScoreModel->setScores(scores);

    QMetaObject::invokeMethod(m_MainQML, "toWinningScreen",
                              Qt::AutoConnection);

    setMenuShown(true);
}


/*!
  Handles the "New game" menu item click on the Main menu of the QML.
*/
void MenuManager::newGameStarted()
{
    setMenuShown(false);

    emit newGame();
}


/*!
  Handles the "Resume" menu item click on the in-game-menu of the QML.
*/
void MenuManager::resumeGame()
{
    setMenuShown(false);
}


/*!
  Delivers a event to the QML code. The texCoord will be in scale 0,0..1,1
  which the QGraphicsEmbedScene will map to the QML scene point.
*/
void MenuManager::deliverEvent(QEvent *event, const QPointF &texCoord)
{
    m_GraphicsScene->deliverEvent(event, texCoord);
}


/*!
    For Harmattan. Disables/enables the Swipe.
    Swipe is disabled while the game is ongoing, and when a menu is shown
    the swipe is enabled.
*/
void MenuManager::toggleSwipe(bool onOff)
{
#ifdef MEEGO_EDITION_HARMATTAN
    QWidget *activeWindow = QApplication::activeWindow();
    Display *dpy = QX11Info::display();
    Atom atom;

    if (!activeWindow) {
        qWarning("QApplication::activeWindow returned NULL!");
        return;
    }

    atom = XInternAtom(dpy, "_MEEGOTOUCH_CUSTOM_REGION", False);

    if (onOff) {
        if (XDeleteProperty(dpy, activeWindow->effectiveWinId(), atom) < 0)
            qWarning("XDeleteProperty for _MEEGOTOUCH_CUSTOM_REGION failed!");
    } else {
        unsigned int customRegion[] =
        {
            activeWindow->x(),
            activeWindow->y(),
            activeWindow->width(),
            activeWindow->height()
        };

        XChangeProperty(dpy, activeWindow->winId(), atom,
            XA_CARDINAL, 32, PropModeReplace,
            reinterpret_cast<unsigned char*>(&customRegion[0]), 4);
    }
#else
    Q_UNUSED(onOff);
#endif
}


/*!
  Renders the QML scene to the texture which will update the QGLSceneNode's
  material -> making the QML scene show in the face of the 3D object.
  Code was copied from the QGraphicsEmbedScene::renderToTexture and
  rendering was made manually due of the bugs in the QGraphicsEmbedScene
  class.
*/
void MenuManager::renderToTexture()
{
    // Determine the fbo size we will need.
    QSize size = m_GraphicsScene->sceneRect().size().toSize();
    QSize fboSize = size; //QGL::nextPowerOfTwo(size);
    if (fboSize.isEmpty())
        fboSize = QSize(16, 16);

    static QGLFramebufferObject *fbo = 0;

    if (fbo == 0) {
        fbo = new QGLFramebufferObject(fboSize);
        if (!fbo->isValid()) {
            qDebug() << "Failed to create fbo";
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindTexture(GL_TEXTURE_2D, fbo->texture());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    QPainter painter(fbo);
    m_GraphicsScene->render(&painter);
    painter.end();

    GLuint textureId = fbo->texture();

    QGLTexture2D *texture = m_MenuMaterial->texture();
    if (texture == 0 || texture->textureId() != textureId) {
        texture = QGLTexture2D::fromTextureId(textureId, QSize(640, 360));
        m_MenuMaterial->setTexture(texture);
    }

    /*
    // Working, but too slow code on device.
    QGLTexture2D *texture = m_MenuMaterial->texture();

    QPainter painter(&m_RenderImage);
    m_GraphicsScene->render(&painter);
    texture->setPixmap(m_RenderImage);
    */
}


/*!
  Draws the menus, the QML scene is first rendered to texture and the
  QGLSceneNode is drawn to the screen.
*/
void MenuManager::draw(QGLPainter *painter)
{
    renderToTexture();
    m_MenuNode->draw(painter);
}
