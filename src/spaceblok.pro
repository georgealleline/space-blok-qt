#
# Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
#
# For the applicable distribution terms see the license text file included in
# the distribution.

QT += opengl declarative

TARGET = spaceblok
TEMPLATE = app

VERSION = 1.2

CONFIG += qt3d

# Include static Qt3D, the Qt3D source will be compiled into the
# application binary. Be sure to comment the CONFIG += qt3d, if
# you decide to use the static build.
#include(qt3d/qt3d.pri)


# Include bullet sources
include(bullet/bullet.pri)


# Include Game Enabler audio sources
GE_PATH = $$PWD/geaudio
include(geaudio/qtgameenableraudio.pri)

SOURCES += \
    src/main.cpp \
    src/gameview.cpp \
    src/gameobject.cpp \
    src/platform.cpp \
    src/ball.cpp \
    src/blackhole.cpp \
    src/level.cpp \
    src/particlesystem.cpp \
    src/explosionparticle.cpp \
    src/blocksshader.cpp \
    src/lightparticle.cpp \
    src/audiomanager.cpp \
    src/menumanager.cpp \
    src/pausebutton.cpp \
    src/scoremodel.cpp \
    src/scoredigit.cpp \
    src/score.cpp \
    src/blackholeshadereffect.cpp


HEADERS += \
    src/gameview.h \
    src/gameobject.h \
    src/platform.h \
    src/ball.h \
    src/blackhole.h \
    src/level.h \
    src/particlesystem.h \
    src/explosionparticle.h \
    src/blocksshader.h \
    src/lightparticle.h \
    src/audiomanager.h \
    src/menumanager.h \
    src/pausebutton.h \
    src/scoremodel.h \
    src/scoredigit.h \
    src/score.h \
    src/blackholeshadereffect.h


RESOURCES += \
    gfx/resources.qrc \
    audio/audio.qrc \
    qml/qml.qrc


OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qml/*.qml \
    gfx/shaders/bloks.vsh \
    gfx/shaders/bloks.fsh \
    gfx/shaders/blackhole.vsh \
    gfx/shaders/blackhole.fsh


symbian {
    # Publish the app version to source code.
    DEFINES += APP_VERSION=\"$$VERSION\"

    TARGET = SpaceBlok

    TARGET.UID3 = 0xE4B75D6C

    TARGET.EPOCHEAPSIZE = 0x100000 0x2000000
    TARGET.EPOCSTACKSIZE = 0x14000

    contains(SYMBIAN_VERSION, Symbian3) {
        # Uses volume hack in GE audio engine to set the volume higher.
        #DEFINES += QTGAMEENABLER_USE_VOLUME_HACK

        # Define the preprocessor macro to get the version in our app.
        DEFINES += APP_VERSION=\"$$VERSION\"
    }
    else {
        message(Symbian^1)
        DEFINES += Q_OS_SYMBIAN_1
    }

    # To lock the application to landscape orientation.
    LIBS += -lcone -leikcore -lavkon

    ICON = icons/spaceblok.svg

    # For enabling hardware floats.
    MMP_RULES += "OPTION gcce -march=armv6"
    MMP_RULES += "OPTION gcce -mfpu=vfp"
    MMP_RULES += "OPTION gcce -mfloat-abi=softfp"
    MMP_RULES += "OPTION gcce -marm"
}


contains(DEFINES, MEEGO_EDITION_HARMATTAN) {
    # Publish the app version to source code.
    DEFINES += APP_VERSION=\\\"$$VERSION\\\"

    target.path = /opt/usr/bin

    desktopfile.files = qtc_packaging/debian_harmattan/spaceblok.desktop
    desktopfile.path = /usr/share/applications

    icon.files = icons/spaceblok.png
    icon.path = /usr/share/icons/hicolor/64x64/apps

    gameclassify.files += qtc_packaging/debian_harmattan/spaceblok.conf
    gameclassify.path = /usr/share/policy/etc/syspart.conf.d

    INSTALLS += \
        target \
        desktopfile \
        gameclassify \
        icon
}
