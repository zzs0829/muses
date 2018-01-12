TARGET = Multimedia

QT += core-private network

CONFIG += git_build

MODULE_PLUGIN_TYPES = \
    mediaservice \
    medialist \

PUBLIC_HEADERS += \
                    multimediaglobal.h \
                    multimediaservice.h \
                    multimediacontroller.h \
                    multimediaplayer.h \
                    multimediaenumdebug.h

PRIVATE_HEADERS += \
                    multimediaglobal_p.h \
                    multimediaservice_p.h \

SOURCES += \
    multimediaservice.cpp \
    multimediacontroller.cpp \
    multimediaplayer.cpp \
    multimediabindableinterface.cpp \
    multimediaobject.cpp \
    multimediaengine.cpp \
    multimediacontroller_p.cpp \
    multimediaresourcesetinterface.cpp \
    multimediaserviceproviderplugin.cpp \
    multimediaresourcepolicy.cpp \
    multimedia.cpp \
    multimediaobject_p.cpp \
    multimediaglobal.cpp \
    multimediapluginloader.cpp \
    multimediaserviceprovider.cpp \
    multimediasession.cpp \
    multimediasession_p.cpp \
    multimediasessionwork.cpp \
    multimediasessionwork_p.cpp \
    multimediametadata.cpp

include(media/media.pri)
include(controller/controller.pri)
include(session/session.pri)

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS \
    multimediabindableinterface.h \
    multimediaobject.h \
    multimediaengine.h \
    multimediacontroller_p.h \
    multimediaresourcesetinterface.h \
    multimediaserviceproviderplugin.h \
    multimediaresourcepolicy.h \
    multimedia.h \
    multimediaobject_p.h \
    multimediapluginloader.h \
    multimediaserviceprovider.h \
    multimediasession.h \
    multimediasession_p.h \
    multimediasessionwork.h \
    multimediasessionwork_p.h \
    multimediametadata.h

load(qt_module)

