#include "mediaplayersession.h"
#include "mediaplayersession_p.h"


HS_BEGIN_NAMESPACE

QMap<MediaPlayerSession::PropertyType, QString> MediaPlayerSession::propertyMap;

MediaPlayerSession::MediaPlayerSession(QObject *parent) :
    MultimediaSession(* new MediaPlayerSessionPrivate, parent)
{
}

MediaPlayerSession::MediaPlayerSession(MediaPlayerSessionPrivate &dd,
                                     QObject *parent)  :
    MultimediaSession(dd, parent)
{
}

void MediaPlayerSession::initPropertyMap()
{
    static bool inited = false;
    if(!inited) {
        propertyMap.insert(PlaybackState, QStringLiteral("PlaybackState"));
        propertyMap.insert(MediaStatus, QStringLiteral("MediaStatus"));
        propertyMap.insert(Duration, QStringLiteral("Duration"));
        propertyMap.insert(Position, QStringLiteral("Position"));
        propertyMap.insert(Volume, QStringLiteral("Volume"));
        propertyMap.insert(Muted, QStringLiteral("Muted"));
        propertyMap.insert(BufferStatus, QStringLiteral("BufferStatus"));
        propertyMap.insert(AudioAvailable, QStringLiteral("AudioAvailable"));
        propertyMap.insert(VideoAvailable, QStringLiteral("VideoAvailable"));
        propertyMap.insert(Seekable, QStringLiteral("Seekable"));
        propertyMap.insert(PlaybackRate, QStringLiteral("PlaybackRate"));
        propertyMap.insert(MediaContent, QStringLiteral("MediaContent"));
        inited = true;
    }
}

MediaPlayerSession::PropertyType MediaPlayerSession::propertyType(const QString &key)
{
    initPropertyMap();
    return propertyMap.key(key);
}

QString MediaPlayerSession::propertyKey(MediaPlayerSession::PropertyType type)
{
    initPropertyMap();
    return propertyMap.value(type);
}

HS_END_NAMESPACE
