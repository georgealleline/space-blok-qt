/**
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */


#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <QObject>
#include <QtOpenGL/qgl.h>
#include <qvector2darray.h>
#include <qvector3darray.h>
#include "scoremodel.h"

class QDeclarativeEngine;
class QDeclarativeComponent;
class QGraphicsEmbedScene;
class QGraphicsObject;
class QGLCamera;
class QGLMaterialCollection;
class QGLMaterial;
class QGLPainter;
class QGLSceneNode;
class QPixmap;

class MenuManager : public QObject
{
    Q_OBJECT
public:
    explicit MenuManager(QGLCamera *camera, QObject *parent = 0);

    void deliverEvent(QEvent *event, const QPointF &texCoord);
    void draw(QGLPainter *painter);

    bool isMenuShown() const;

protected:
    void setMenuShown(bool isShown);
    void toggleSwipe(bool onOff);


    QGLMaterialCollection *m_MaterialCollection;
    QGLMaterial *m_MenuMaterial;
    GLuint m_GraphicsSceneTextureId;
    QGLSceneNode *m_MenuNode;

    QDeclarativeEngine *m_DeclarativeEngine;
    QDeclarativeComponent *m_DeclarativeComponent;
    QGraphicsObject *m_MainQML;

    QGraphicsEmbedScene *m_GraphicsScene;

    bool m_MenuShown;

    ScoreModel *m_ScoreModel;

signals:
    void newGame();

public slots:
    void toMainMenu();
    void toInGameMenu();
    void toWinningScreen(const ScoreModel::typeScoreList &scores);
    void renderToTexture();

protected slots:

    void newGameStarted();
    void resumeGame();
};

#endif // MENUMANAGER_H
