#include "mediaplayer.h"
#include "mediaplayer_p.h"

MediaPlayer::MediaPlayer(QObject *parent, Flags flags) :
    MultimediaPlayer(parent),
    d_ptr(new MediaPlayerPrivate(this))
{
    Q_UNUSED(flags)

}

MediaPlayer::~MediaPlayer()
{

}

MediaContent MediaPlayer::media() const
{
    Q_D(const MediaPlayer);

    return d->curMedia;
}

MediaContent MediaPlayer::currentMedia() const
{
    Q_D(const MediaPlayer);

    // When playing a resource file, don't return the backend's current media, which
    // can be a temporary file.
    if (!d->qrcMedia.isNull())
        return d->qrcMedia;

    if (d->control)
        return d->control->media();

    return MediaContent();
}

void MediaPlayer::play()
{
    Q_D(MediaPlayer);

    if (d->control == 0) {
        QMetaObject::invokeMethod(d, "_q_error", Qt::QueuedConnection,
                                  Q_ARG(int, MultimediaPlayer::AccessDeniedError),
                                  Q_ARG(QString, tr("The MediaPlayer object does not have a valid control")));
        return;
    }

    //if playlist control is available, the service should advance itself
//    if (d->playlist && !d->playlist->isEmpty()) {
//        // switch to playing state
//        if (d->state != MultimediaPlayer::PlayingState)
//            d->_q_stateChanged(MultimediaPlayer::PlayingState);

//        if (d->playlist->currentIndex() == -1) {
//            d->playlist->setCurrentIndex(0);
//        }
//    }

    // Reset error conditions
    d->error = NoError;
    d->errorString = QString();

    d->control->play();
}

void MediaPlayer::pause()
{
    Q_D(MediaPlayer);

    if (d->control != 0)
        d->control->pause();
}

void MediaPlayer::stop()
{
    Q_D(MediaPlayer);

    if (d->control != 0)
        d->control->stop();

    if (d->playlist && d->state != MultimediaPlayer::StoppedState) {
        d->state = MultimediaPlayer::StoppedState;
        removePropertyWatch("position");
        emit stateChanged(MultimediaPlayer::StoppedState);
    }
}

void MediaPlayer::setPosition(qint64 position)
{
    Q_D(MediaPlayer);

    if (d->control == 0)
        return;

    d->position = position;

    d->control->setPosition(qMax(position, 0ll));
}

void MediaPlayer::setVolume(int v)
{
    Q_D(MediaPlayer);

    if (d->control == 0)
        return;

    int clamped = qBound(0, v, 100);
    if (clamped == volume())
        return;

    d->volume = clamped;

    d->control->setVolume(clamped);
}

void MediaPlayer::setMuted(bool muted)
{
    Q_D(MediaPlayer);

    if (d->control == 0 || muted == isMuted())
        return;

    d->muted = muted;

    d->control->setMuted(muted);
}

MediaPlaylist *MediaPlayer::playlist() const
{
    Q_D(const MediaPlayer);
    return d->playlist;
}

void MediaPlayer::setMedia(const MediaContent &media)
{
    Q_D(MediaPlayer);
    if(d->curMedia == media)
        return;

    stop();

    d->curMedia = media;
    emit mediaChanged(d->curMedia);

    d->setMedia(d->curMedia);
}

void MediaPlayer::setPlaylist(MediaPlaylist *playlist)
{
    Q_D(MediaPlayer);
    d->playlist = playlist;
    // TODO: setPlaylist
}

MultimediaPlayer::State MediaPlayer::state() const
{
    Q_D(const MediaPlayer);
    return d->state;
}

MultimediaPlayer::MediaStatus MediaPlayer::mediaStatus() const
{
    Q_D(const MediaPlayer);
    return d->status;
}

qint64 MediaPlayer::duration() const
{
    Q_D(const MediaPlayer);
    return d->duration;
}

qint64 MediaPlayer::position() const
{
    Q_D(const MediaPlayer);
    return d->position;
}

int MediaPlayer::volume() const
{
    Q_D(const MediaPlayer);
    return d->volume;
}

bool MediaPlayer::isMuted() const
{
    Q_D(const MediaPlayer);
    return d->muted;
}

bool MediaPlayer::isAudioAvailable() const
{
    Q_D(const MediaPlayer);
    return d->audioAvailable;
}

bool MediaPlayer::isVideoAvailable() const
{
    Q_D(const MediaPlayer);
    return d->videoAvailable;
}

int MediaPlayer::bufferStatus() const
{
    Q_D(const MediaPlayer);
    return d->bufStatus;
}

bool MediaPlayer::isSeekable() const
{
    Q_D(const MediaPlayer);
    return d->seekable;
}

qreal MediaPlayer::playbackRate() const
{
    Q_D(const MediaPlayer);
    return d->rate;
}

void MediaPlayer::setPlaybackRate(qreal rate)
{
    Q_D(MediaPlayer);
    d->rate = rate;

    if (d->control != 0)
        d->control->setPlaybackRate(rate);
}

MultimediaPlayer::Error MediaPlayer::error() const
{
    Q_D(const MediaPlayer);
    return d->error;
}

QString MediaPlayer::errorString() const
{
    Q_D(const MediaPlayer);
    return d->errorString;
}
