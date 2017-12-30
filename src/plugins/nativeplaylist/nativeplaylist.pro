TARGET = nativeplaylist

QT += core-private network multimedia-private


INCLUDEPATH += $$PWD

HEADERS += \
    nativeplaylistserviceplugin.h \
    nativeplaylistservice.h \
    nativeplaylistengine.h \
    nativefilefindworker.h \
    nativeplaylistcontrol.h \
    nativeplaylistprovider.h

SOURCES += \
    nativeplaylistserviceplugin.cpp \
    nativeplaylistservice.cpp \
    nativeplaylistengine.cpp \
    nativefilefindworker.cpp \
    nativeplaylistcontrol.cpp \
    nativeplaylistprovider.cpp

PLUGIN_TYPE = mediaservice
PLUGIN_CLASS_NAME = NativePlaylistServicePlugin
load(qt_plugin)

DISTFILES += \
    playlist.json
