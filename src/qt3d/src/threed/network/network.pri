INCLUDEPATH += $$PWD
VPATH += $$PWD
HEADERS += \
    qdownloadmanager.h \
    qabstractdownloadmanager.h \
    qthreadeddownloadmanager.h
SOURCES += \
    qdownloadmanager.cpp \
    qthreadeddownloadmanager.cpp \
    qabstractdownloadmanager.cpp
PRIVATE_HEADERS += \
    qthreadeddownloadmanager_p.h
