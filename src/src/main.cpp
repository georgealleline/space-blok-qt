/**
 * Copyright (c) 2011-2014 Microsoft Mobile and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include <QApplication>
#include <QDesktopWidget>
#include "gameview.h"

// Lock orientation in Symbian
#ifdef Q_OS_SYMBIAN
#include <eikenv.h>
#include <eikappui.h>
#include <aknenv.h>
#include <aknappui.h>
#endif

/*!
  The main function of the application.
*/
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GameView view;
#ifdef MEEGO_EDITION_HARMATTAN
    QSize windowSize;
    windowSize.setWidth(
                ((QApplication::desktop())->screenGeometry()).width());
    windowSize.setHeight(
                ((QApplication::desktop())->screenGeometry()).height());
    view.resize(windowSize);
    view.showFullScreen();
#elif defined(Q_OS_SYMBIAN)
    // Lock orientation on Symbian
    CAknAppUi* appUi =
            dynamic_cast<CAknAppUi*> (CEikonEnv::Static()->AppUi());
    TRAP_IGNORE(
        if (appUi) {
            appUi->SetOrientationL(CAknAppUi::EAppUiOrientationLandscape);
        }
    );

    view.setGeometry(QApplication::desktop()->screenGeometry());
    view.showFullScreen();
#else
    view.setGeometry(100, 100, 854, 480);
    view.show();
#endif

    return app.exec();
}
