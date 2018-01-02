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

MediaPlaylist *MediaPlayer::playlist() const
{
    Q_D(const MediaPlayer);
    return d->playlist;
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
    // TODO: setPlaybackRate
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
