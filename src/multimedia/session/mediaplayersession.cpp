#include "mediaplayersession.h"
#include "mediaplayersession_p.h"


HS_BEGIN_NAMESPACE

QMap<MediaPlayerSession::PropertyType, QString> MediaPlayerSession::propertyMap;

MediaPlayerSession::MediaPlayerSession(const QString &id, const QString &key, QObject *parent) :
    MultimediaSession(* new MediaPlayerSessionPrivate(id, key), parent)
{
    initProperty();
}

MediaPlayerSession::MediaPlayerSession(MediaPlayerSessionPrivate &dd,
                                     QObject *parent)  :
    MultimediaSession(dd, parent)
{
    initProperty();
}

void MediaPlayerSession::initProperty()
{
    _updateMedia(MediaContent());
    _updatePlaybackState(MultimediaPlayer::StoppedState);
    _updateMediaStatus(MultimediaPlayer::NoMedia);
    _updateDuration(-1);
    _updatePosition(0);
    _updateVolume(100);
    _updateMuted(false);
    _updateBufferStatus(-1);
    _updateAudioAvailable(false);
    _updateVideoAvailable(false);
    _updateSeekable(false);
    _updatePlaybackRate(1.0);
    _updateError(0, QString());
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
        propertyMap.insert(Error, QStringLiteral("Error"));
        propertyMap.insert(ErrorString, QStringLiteral("ErrorString"));
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
    QString key = MediaPlayerSession::propertyKey(Position);
    setSessionProperty(key, position);
}

int MediaPlayerSession::volume() const
{
    QString key = MediaPlayerSession::propertyKey(Volume);

    return sessionProperty(key).toInt();
}

void MediaPlayerSession::setVolume(int volume)
{
    QString key = MediaPlayerSession::propertyKey(Volume);
    setSessionProperty(key, volume);
}

bool MediaPlayerSession::isMuted() const
{
    QString key = MediaPlayerSession::propertyKey(Muted);

    return sessionProperty(key).toBool();
}

void MediaPlayerSession::setMuted(bool muted)
{
    QString key = MediaPlayerSession::propertyKey(Muted);
    setSessionProperty(key, muted);
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
    QString key = MediaPlayerSession::propertyKey(PlaybackRate);
    setSessionProperty(key, rate);
}

MediaContent MediaPlayerSession::media() const
{
    QString key = MediaPlayerSession::propertyKey(Media);

    return sessionProperty(key).value<MediaContent>();
}

void MediaPlayerSession::setMedia(const MediaContent &media)
{
    QString key = MediaPlayerSession::propertyKey(Media);
    setSessionProperty(key, QVariant::fromValue(media));
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

void MediaPlayerSession::_updateMedia(const MediaContent& content)
{
    Q_D(MediaPlayerSession);
    QString key = MediaPlayerSession::propertyKey(Media);
    if(sessionProperty(key).value<MediaContent>() == content)
        return;

    d->setProperty(key, QVariant::fromValue(content), false);
    emit mediaChanged(content);
}

void MediaPlayerSession::_updatePlaybackState(MultimediaPlayer::State newState)
{
    Q_D(MediaPlayerSession);
    QString key = MediaPlayerSession::propertyKey(PlaybackState);
    if(sessionProperty(key) == newState)
        return;

    d->setProperty(key, newState, false);
    emit stateChanged(newState);
}

void MediaPlayerSession::_updateMediaStatus(MultimediaPlayer::MediaStatus status)
{
    Q_D(MediaPlayerSession);
    QString key = MediaPlayerSession::propertyKey(MediaStatus);
    if(sessionProperty(key) == status)
        return;

    d->setProperty(key, status, false);
    emit mediaStatusChanged(status);
}

void MediaPlayerSession::_updateDuration(qint64 duration)
{
    Q_D(MediaPlayerSession);
    QString key = MediaPlayerSession::propertyKey(Duration);
    if(sessionProperty(key) == duration)
        return;

    d->setProperty(key, duration, false);
    emit durationChanged(duration);
}

void MediaPlayerSession::_updatePosition(qint64 position)
{
    Q_D(MediaPlayerSession);
    QString key = MediaPlayerSession::propertyKey(Position);
    if(sessionProperty(key) == position)
        return;

    d->setProperty(key, position, false);
    emit positionChanged(position);
}

void MediaPlayerSession::_updateVolume(int volume)
{
    Q_D(MediaPlayerSession);
    QString key = MediaPlayerSession::propertyKey(Volume);
    if(sessionProperty(key) == volume)
        return;

    d->setProperty(key, volume, false);
    emit volumeChanged(volume);
}

void MediaPlayerSession::_updateMuted(bool muted)
{
    Q_D(MediaPlayerSession);
    QString key = MediaPlayerSession::propertyKey(Muted);
    if(sessionProperty(key) == muted)
        return;

    d->setProperty(key, muted, false);
    emit mutedChanged(muted);
}

void MediaPlayerSession::_updateBufferStatus(int percentFilled)
{
    Q_D(MediaPlayerSession);
    QString key = MediaPlayerSession::propertyKey(BufferStatus);
    if(sessionProperty(key) == percentFilled)
        return;

    d->setProperty(key, percentFilled, false);
    emit bufferStatusChanged(percentFilled);
}

void MediaPlayerSession::_updateAudioAvailable(bool audioAvailable)
{
    Q_D(MediaPlayerSession);
    QString key = MediaPlayerSession::propertyKey(AudioAvailable);
    if(sessionProperty(key) == audioAvailable)
        return;

    d->setProperty(key, audioAvailable, false);
    emit audioAvailableChanged(audioAvailable);
}

void MediaPlayerSession::_updateVideoAvailable(bool videoAvailable)
{
    Q_D(MediaPlayerSession);
    QString key = MediaPlayerSession::propertyKey(VideoAvailable);
    if(sessionProperty(key) == videoAvailable)
        return;

    d->setProperty(key, videoAvailable, false);
    emit videoAvailableChanged(videoAvailable);
}

void MediaPlayerSession::_updateSeekable(bool seekable)
{
    Q_D(MediaPlayerSession);
    QString key = MediaPlayerSession::propertyKey(Seekable);
    if(sessionProperty(key) == seekable)
        return;

    d->setProperty(key, seekable, false);
    emit seekableChanged(seekable);
}

void MediaPlayerSession::_updatePlaybackRate(qreal rate)
{
    Q_D(MediaPlayerSession);
    QString key = MediaPlayerSession::propertyKey(PlaybackRate);
    if(sessionProperty(key) == rate)
        return;

    d->setProperty(key, rate, false);
    emit playbackRateChanged(rate);
}

void MediaPlayerSession::_updateError(int err, const QString &errorString)
{
    Q_D(MediaPlayerSession);
    QString key = MediaPlayerSession::propertyKey(Error);
    QString key1 = MediaPlayerSession::propertyKey(ErrorString);
    if(sessionProperty(key) == err)
        return;

    d->setProperty(key, err, false);
    d->setProperty(key1, errorString, false);
    emit error(err, errorString);
}

void MediaPlayerSession::_updateTags(const QVariantMap &tags)
{
    Q_D(MediaPlayerSession);
    QVariantMap oldTags = d->m_tags;
    d->m_tags.clear();
    bool changed = false;

    QMapIterator<QString ,QVariant> i(tags);
    while (i.hasNext()) {
         i.next();
         //use gstreamer native keys for elements not in our key map
         QString key = i.key();
         d->m_tags.insert(key, i.value());
         if (i.value() != oldTags.value(key)) {
             changed = true;
             emit metaDataChanged(key, i.value());
         }
    }

    if (oldTags.isEmpty() != d->m_tags.isEmpty()) {
//        emit metaDataAvailableChanged(isMetaDataAvailable());
        changed = true;
    }

    if (changed)
        emit metaDataChanged();
}

HS_END_NAMESPACE
