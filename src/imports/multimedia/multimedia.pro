CXX_MODULE = multimedia
TARGET  = MultimediaQuick
TARGETPATH = Multimedia

QT += qml quick network

HEADERS += \
    multimedia.h \

SOURCES += \
    multimedia.cpp \


load(qml_plugin)

DISTFILES += \
    qmldir
