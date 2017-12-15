TARGET = gstmediaplayer

QT += core-private network multimedia-private

CONFIG += link_pkgconfig
PKGCONFIG = gstreamer-1.0

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/gstutils.cpp \
    $$PWD/gstreamerbushelper.cpp \
    $$PWD/gstreamerbushelper_p.cpp \
    $$PWD/gstreamermessage.cpp \
    $$PWD/gstreamervideorendererinterface.cpp \
    $$PWD/gstreamerbufferprobe.cpp \
    $$PWD/gstreamervideoprobecontrol.cpp \
    $$PWD/gstreamerplayercontrol.cpp \
    $$PWD/gstreamerplayerserviceplugin.cpp \
    $$PWD/gstreamerplayerservice.cpp \
    $$PWD/gstreamerplayerengine.cpp

HEADERS += \
    $$PWD/gstutils.h \
    $$PWD/gstreamerbushelper.h \
    $$PWD/gstreamerbushelper_p.h \
    $$PWD/gstreamermessage.h \
    $$PWD/gstreamervideorendererinterface.h \
    $$PWD/gstreamerbufferprobe.h \
    $$PWD/gstreamervideoprobecontrol.h \
    $$PWD/gstreamerplayercontrol.h \
    $$PWD/gstreamerplayerserviceplugin.h \
    $$PWD/gstreamerplayerservice.h \
    $$PWD/gstreamerplayerengine.h


PLUGIN_TYPE = mediaservice
PLUGIN_CLASS_NAME = GstreamerPlayerServicePlugin
load(qt_plugin)

DISTFILES += \
    mediaplayer.json
