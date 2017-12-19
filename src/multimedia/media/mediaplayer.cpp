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

}

MultimediaPlayer::State MediaPlayer::state() const
{

}

MultimediaPlayer::MediaStatus MediaPlayer::mediaStatus() const
{

}

qint64 MediaPlayer::duration() const
{

}
qint64 MediaPlayer::position() const
{

}

int MediaPlayer::volume() const
{

}

bool MediaPlayer::isMuted() const
{

}

bool MediaPlayer::isAudioAvailable() const
{

}

bool MediaPlayer::isVideoAvailable() const
{

}

int MediaPlayer::bufferStatus() const
{

}

bool MediaPlayer::isSeekable() const
{

}
qreal MediaPlayer::playbackRate() const
{

}

MultimediaPlayer::Error MediaPlayer::error() const
{

}
QString MediaPlayer::errorString() const
{

}
