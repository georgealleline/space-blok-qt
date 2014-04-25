# Copyright (c) 2011-2014 Microsoft Mobile.

# Comment the following line out for better performance. Using the definition
# enables debug logging which is convenient for locating problems in the code
# but is also very costly in terms of performance.
#DEFINES += GE_DEBUG

INCLUDEPATH += $${GE_PATH}/src

HEADERS  += \
    $${GE_PATH}/src/trace.h \
    $${GE_PATH}/src/geglobal.h \
    $${GE_PATH}/src/audiobuffer.h \
    $${GE_PATH}/src/audiobufferplayinstance.h \
    $${GE_PATH}/src/audiomixer.h \
    $${GE_PATH}/src/audioout.h \
    $${GE_PATH}/src/pushaudioout.h \
    $${GE_PATH}/src/pullaudioout.h \
    $${GE_PATH}/src/audiosourceif.h \
    $${GE_PATH}/src/audioeffect.h \
    $${GE_PATH}/src/echoeffect.h \
    $${GE_PATH}/src/cutoffeffect.h

SOURCES += \
    $${GE_PATH}/src/audiobuffer.cpp \
    $${GE_PATH}/src/audiobufferplayinstance.cpp \
    $${GE_PATH}/src/audiomixer.cpp \
    $${GE_PATH}/src/pushaudioout.cpp \
    $${GE_PATH}/src/pullaudioout.cpp \
    $${GE_PATH}/src/audiosourceif.cpp \
    $${GE_PATH}/src/audioeffect.cpp \
    $${GE_PATH}/src/echoeffect.cpp \
    $${GE_PATH}/src/cutoffeffect.cpp

QT += core gui

symbian {
    message(Symbian build)

    CONFIG += mobility
    MOBILITY += multimedia systeminfo

    # For volume keys
    LIBS += -lremconcoreapi -lremconinterfacebase

    # Uncomment the following define to enable a very ugly hack to set the
    # volume level on Symbian devices higher. By default, on Symbian, the volume
    # level is very low when audio is played using QAudioOutput class. For now,
    # this ugly hack is the only way to set the volume louder.
    #
    # WARNING: The volume hack (see the GEAudioOut.cpp file) is VERY DANGEROUS
    # because the data to access the volume interface is retrieved manually with
    # pointer manipulation. Should the library, in which the interface is
    # implemented, be modified even a tiny bit, the application using this hack
    # might crash.
    #
    #DEFINES += QTGAMEENABLER_USE_VOLUME_HACK

    contains(DEFINES, QTGAMEENABLER_USE_VOLUME_HACK) {
        # Include paths and libraries required for the volume hack.
        message(Symbian volume hack enabled)
        INCLUDEPATH += /epoc32/include/mmf/common
        INCLUDEPATH += /epoc32/include/mmf/server
        LIBS += -lmmfdevsound
    }

    #DEFINES += GE_PULLMODEAUDIO

    INCLUDEPATH += /epoc32/include/mw
    HEADERS += $${GE_PATH}/src/volumekeys.h
    SOURCES += $${GE_PATH}/src/volumekeys.cpp
}

unix:!symbian {
    maemo5 {
        message(Maemo 5 build)

        QT += multimedia
        CONFIG += mobility
        MOBILITY += systeminfo
    } else {
        contains(MEEGO_EDITION, harmattan) {
            message(Harmattan build)

            DEFINES += MEEGO_EDITION_HARMATTAN
            CONFIG += mobility
            MOBILITY += multimedia systeminfo
        }
        else {
            message(Unix based desktop build)

            CONFIG += mobility
            MOBILITY += multimedia systeminfo
            #DEFINES += GE_PULLMODEAUDIO
        }
    }
}

windows {
    message(Windows desktop build)

    QT += multimedia
    DEFINES += GE_NOMOBILITY
}

# End of file.
