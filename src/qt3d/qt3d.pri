
#INCLUDEPATH += $$PWD
#INCLUDEPATH += $$PWD/src/threed

#INCLUDEPATH += src/plugins/sceneformats/assimp
#INCLUDEPATH += 3rdparty/assimp/include

include(3rdparty/assimp/assimp.pri)

include(src/threed/global/global.pri)
include(src/threed/painting/painting.pri)
include(src/threed/arrays/arrays.pri)
include(src/threed/effects/effects.pri)
include(src/threed/geometry/geometry.pri)
include(src/threed/viewing/viewing.pri)
include(src/threed/materials/materials.pri)
include(src/threed/math3d/math3d.pri)
include(src/threed/scene/scene.pri)
include(src/threed/network/network.pri)
include(src/private/private.pri)
include(src/threed/graphicsview/graphicsview.pri)
include(src/threed/textures/textures.pri)
include(src/threed/surfaces/surfaces.pri)
include(src/threed/api/api.pri)


HEADERS += \
    $$PWD/src/plugins/sceneformats/assimp/qailoader.h \
    $$PWD/src/plugins/sceneformats/assimp/qaiscene.h \
    $$PWD/src/plugins/sceneformats/assimp/qaiscenehandler.h \
    $$PWD/src/plugins/sceneformats/assimp/qaimesh.h \
    $$PWD/src/plugins/sceneformats/assimp/ailoaderiostream.h \
    $$PWD/src/plugins/sceneformats/assimp/ailoaderiosystem.h

SOURCES += \
    $$PWD/src/plugins/sceneformats/assimp/qailoader.cpp \
    $$PWD/src/plugins/sceneformats/assimp/qaiscene.cpp \
    $$PWD/src/plugins/sceneformats/assimp/qaiscenehandler.cpp \
    $$PWD/src/plugins/sceneformats/assimp/qaimesh.cpp \
    $$PWD/src/plugins/sceneformats/assimp/ailoaderiostream.cpp \
    $$PWD/src/plugins/sceneformats/assimp/ailoaderiosystem.cpp \
    $$PWD/src/plugins/sceneformats/assimp/pluginmain.cpp

HEADERS += $$PRIVATE_HEADERS
