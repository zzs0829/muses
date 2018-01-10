#include "mediaplayersession.h"
#include "mediaplayersession_p.h"


HS_BEGIN_NAMESPACE

QMap<MediaPlayerSession::PropertyType, QString> MediaPlayerSession::propertyMap;

MediaPlayerSession::MediaPlayerSession(const QString &id, const QString &key, QObject *parent) :
    MultimediaSession(* new MediaPlayerSessionPrivate(id, key), parent)
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
        propertyMap.insert(Media, QStringLiteral("Media"));
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

MultimediaPlayer::State MediaPlayerSession::state() const
{
    QString key = MediaPlayerSession::propertyKey(PlaybackState);

    return MultimediaPlayer::State(sessionProperty(key).toInt());
}

MultimediaPlayer::MediaStatus MediaPlayerSession::mediaStatus() const
{
    QString key = MediaPlayerSession::propertyKey(MediaStatus);

    return MultimediaPlayer::MediaStatus(sessionProperty(key).toInt());
}

qint64 MediaPlayerSession::duration() const
{
    QString key = MediaPlayerSession::propertyKey(Duration);

    return sessionProperty(key).toLongLong();
}

qint64 MediaPlayerSession::position() const
{
    QString key = MediaPlayerSession::propertyKey(Position);

    return sessionProperty(key).toLongLong();
}

void MediaPlayerSession::setPosition(qint64 position)
{
    if(this->position() == position)
        return;

    QString key = MediaPlayerSession::propertyKey(Position);
    setSessionProperty(key, position);
    emit sessionPropertyChanged(key, QVariant::fromValue(position));
}

int MediaPlayerSession::volume() const
{
    QString key = MediaPlayerSession::propertyKey(Volume);

    return sessionProperty(key).toInt();
}

void MediaPlayerSession::setVolume(int volume)
{
    if(this->volume() == volume)
        return;

    QString key = MediaPlayerSession::propertyKey(Volume);
    setSessionProperty(key, volume);
    emit sessionPropertyChanged(key, QVariant::fromValue(volume));
}

bool MediaPlayerSession::isMuted() const
{
    QString key = MediaPlayerSession::propertyKey(Muted);

    return sessionProperty(key).toBool();
}

void MediaPlayerSession::setMuted(bool muted)
{
    if(this->isMuted() == muted)
        return;

    QString key = MediaPlayerSession::propertyKey(Muted);
    setSessionProperty(key, muted);
    emit sessionPropertyChanged(key, QVariant::fromValue(muted));
}

int MediaPlayerSession::bufferStatus() const
{
    QString key = MediaPlayerSession::propertyKey(BufferStatus);

    return sessionProperty(key).toInt();
}

bool MediaPlayerSession::isAudioAvailable() const
{
    QString key = MediaPlayerSession::propertyKey(AudioAvailable);

    return sessionProperty(key).toBool();
}

bool MediaPlayerSession::isVideoAvailable() const
{
    QString key = MediaPlayerSession::propertyKey(VideoAvailable);

    return sessionProperty(key).toBool();
}

bool MediaPlayerSession::isSeekable() const
{
    QString key = MediaPlayerSession::propertyKey(Seekable);

    return sessionProperty(key).toBool();
}

qreal MediaPlayerSession::playbackRate() const
{
    QString key = MediaPlayerSession::propertyKey(PlaybackRate);

    return sessionProperty(key).toReal();
}

void MediaPlayerSession::setPlaybackRate(qreal rate)
{
    if(this->playbackRate() == rate)
        return;

    QString key = MediaPlayerSession::propertyKey(PlaybackRate);
    setSessionProperty(key, rate);
    emit sessionPropertyChanged(key, QVariant::fromValue(rate));
}

MediaContent MediaPlayerSession::media() const
{
    QString key = MediaPlayerSession::propertyKey(Media);

    return sessionProperty(key).value<MediaContent>();
}

void MediaPlayerSession::setMedia(const MediaContent &media)
{
    if(this->media() == media)
        return;

    QString key = MediaPlayerSession::propertyKey(Media);
    setSessionProperty(key, QVariant::fromValue(media));
    emit sessionPropertyChanged(key, QVariant::fromValue(media));
}

void MediaPlayerSession::play()
{
    emit handlePlay();
}

void MediaPlayerSession::pause()
{
    emit handlePause();
}

void MediaPlayerSession::stop()
{
    emit handleStop();
}

HS_END_NAMESPACE
