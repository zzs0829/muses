#include "mediaplayer.h"
#include "mediaplayer_p.h"

MediaPlayer::MediaPlayer(const QString &key, QObject *parent, Flags flags) :
    MultimediaPlayer(parent),
    d_ptr(new MediaPlayerPrivate(this))
{
    Q_UNUSED(flags)
    Q_D(MediaPlayer);
    d->provider = MultimediaServiceProvider::defaultServiceProvider();
    d->service = d->provider->requestService(MEDIASERVICE_MEDIAPLAYER);
    if(d->service == 0) {
        d->error = ServiceMissingError;
    }else{
        d->session = qobject_cast<MediaPlayerSession *>(d->service->requestSession(key));
         if (d->session != 0) {
             connect(d->session, SIGNAL(mediaChanged(MediaContent)), d, SLOT(_q_handleMediaChanged(MediaContent)));
             connect(d->session, SIGNAL(stateChanged(MultimediaPlayer::State)), d, SLOT(_q_stateChanged(MultimediaPlayer::State)));
             connect(d->session, SIGNAL(mediaStatusChanged(MultimediaPlayer::MediaStatus)),
                     d, SLOT(_q_mediaStatusChanged(MultimediaPlayer::MediaStatus)));
             connect(d->session, SIGNAL(error(int,QString)), d, SLOT(_q_error(int,QString)));

             connect(d->session, SIGNAL(durationChanged(qint64)), SIGNAL(durationChanged(qint64)));
             connect(d->session, SIGNAL(positionChanged(qint64)), SIGNAL(positionChanged(qint64)));
             connect(d->session, SIGNAL(audioAvailableChanged(bool)), SIGNAL(audioAvailableChanged(bool)));
             connect(d->session, SIGNAL(videoAvailableChanged(bool)), SIGNAL(videoAvailableChanged(bool)));
             connect(d->session, SIGNAL(volumeChanged(int)), SIGNAL(volumeChanged(int)));
             connect(d->session, SIGNAL(mutedChanged(bool)), SIGNAL(mutedChanged(bool)));
             connect(d->session, SIGNAL(seekableChanged(bool)), SIGNAL(seekableChanged(bool)));
             connect(d->session, SIGNAL(playbackRateChanged(qreal)), SIGNAL(playbackRateChanged(qreal)));
             connect(d->session, SIGNAL(bufferStatusChanged(int)), SIGNAL(bufferStatusChanged(int)));

             d->_q_stateChanged(d->session->state());
             d->_q_mediaStatusChanged(d->session->mediaStatus());

         }
    }

}

MediaPlayer::~MediaPlayer()
{
    Q_D(MediaPlayer);
    if (d->service) {
        if (d->session)
            d->service->releaseSession(d->session);
    }
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

    if (d->session)
        return d->session->media();

    return MediaContent();
}

void MediaPlayer::play()
{
    Q_D(MediaPlayer);

    if (d->session == 0) {
        QMetaObject::invokeMethod(d, "_q_error", Qt::QueuedConnection,
                                  Q_ARG(int, MultimediaPlayer::AccessDeniedError),
                                  Q_ARG(QString, tr("The MediaPlayer object does not have a valid control")));
        return;
    }

    //if playlist session is available, the service should advance itself
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

    d->session->play();
}

void MediaPlayer::pause()
{
    Q_D(MediaPlayer);

    if (d->session != 0)
        d->session->pause();
}

void MediaPlayer::stop()
{
    Q_D(MediaPlayer);

    if (d->session != 0)
        d->session->stop();

    if (d->playlist && d->state != MultimediaPlayer::StoppedState) {
        d->state = MultimediaPlayer::StoppedState;
        removePropertyWatch("position");
        emit stateChanged(MultimediaPlayer::StoppedState);
    }
}

void MediaPlayer::setPosition(qint64 position)
{
    Q_D(MediaPlayer);

    if (d->session == 0)
        return;

    d->session->setPosition(qMax(position, 0ll));
}

void MediaPlayer::setVolume(int v)
{
    Q_D(MediaPlayer);

    if (d->session == 0)
        return;

    int clamped = qBound(0, v, 100);
    if (clamped == volume())
        return;

    d->session->setVolume(clamped);
}

void MediaPlayer::setMuted(bool muted)
{
    Q_D(MediaPlayer);

    if (d->session == 0 || muted == isMuted())
        return;

    d->session->setMuted(muted);
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

    if (d->session != 0)
        return d->session->duration();

    return -1;
}

qint64 MediaPlayer::position() const
{
    Q_D(const MediaPlayer);

    if (d->session != 0)
        return d->session->position();

    return 0;
}

int MediaPlayer::volume() const
{
    Q_D(const MediaPlayer);

    if (d->session != 0)
        return d->session->volume();

    return 0;
}

bool MediaPlayer::isMuted() const
{
    Q_D(const MediaPlayer);

    if (d->session != 0)
        return d->session->isMuted();

    return false;
}

bool MediaPlayer::isAudioAvailable() const
{
    Q_D(const MediaPlayer);

    if (d->session != 0)
        return d->session->isAudioAvailable();

    return false;
}

bool MediaPlayer::isVideoAvailable() const
{
    Q_D(const MediaPlayer);

    if (d->session != 0)
        return d->session->isVideoAvailable();

    return false;
}

int MediaPlayer::bufferStatus() const
{
    Q_D(const MediaPlayer);

    if (d->session != 0)
        return d->session->bufferStatus();

    return 0;
}

bool MediaPlayer::isSeekable() const
{
    Q_D(const MediaPlayer);

    if (d->session != 0)
        return d->session->isSeekable();

    return false;
}

qreal MediaPlayer::playbackRate() const
{
    Q_D(const MediaPlayer);

    if (d->session != 0)
        return d->session->playbackRate();

    return 0.0;
}

void MediaPlayer::setPlaybackRate(qreal rate)
{
    Q_D(MediaPlayer);

    if (d->session != 0)
        d->session->setPlaybackRate(rate);
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
