#include "gstreamerplayerwork.h"

GstreamerPlayerWork::GstreamerPlayerWork(GstreamerPlayerSession *session,
                                         GstreamerPlayerControl *control,
                                         QObject *parent)
    : QObject(parent),
      m_session(session),
      m_control(control)
{

}

void GstreamerPlayerWork::start()
{
    connect(m_session, SIGNAL(sessionPropertyChanged(QString,QVariant)), this, SLOT(_q_sessionPropertyChanged(QString,QVariant)));
    connect(m_session, SIGNAL(handlePause()), this, SLOT(_q_handlePause()));
    connect(m_session, SIGNAL(handlePlay()), this, SLOT(_q_handlePlay()));
    connect(m_session, SIGNAL(handleStop()), this, SLOT(_q_handleStop()));

    m_control->stop();
    m_control->setMedia(m_session->media());
    m_control->setPosition(m_session->position());
    m_control->setVolume(m_session->volume());
    m_control->setMuted(m_session->isMuted());
    m_control->setPlaybackRate(m_session->playbackRate());

    connect(m_control, SIGNAL(audioAvailableChanged(bool)), this, SLOT(_q_audioAvailableChanged(bool)));
    connect(m_control, SIGNAL(bufferStatusChanged(int)), this, SLOT(_q_bufferStatusChanged(int)));
    connect(m_control, SIGNAL(durationChanged(qint64)), this, SLOT(_q_durationChanged(qint64)));
    connect(m_control, SIGNAL(error(int,QString)), this, SLOT(_q_error(int,QString)));
    connect(m_control, SIGNAL(mediaStatusChanged(MultimediaPlayer::MediaStatus)), this, SLOT(_q_mediaStatusChanged(MultimediaPlayer::MediaStatus)));
    connect(m_control, SIGNAL(mutedChanged(bool)), this, SLOT(_q_mutedChanged(bool)));
    connect(m_control, SIGNAL(playbackRateChanged(qreal)), this, SLOT(_q_playbackRateChanged(qreal)));
    connect(m_control, SIGNAL(positionChanged(qint64)), this, SLOT(_q_positionChanged(qint64)));
    connect(m_control, SIGNAL(seekableChanged(bool)), this, SLOT(_q_seekableChanged(bool)));
    connect(m_control, SIGNAL(stateChanged(MultimediaPlayer::State)), this, SLOT(_q_stateChanged(MultimediaPlayer::State)));
    connect(m_control, SIGNAL(videoAvailableChanged(bool)), this, SLOT(_q_videoAvailableChanged(bool)));
    connect(m_control, SIGNAL(volumeChanged(int)), this, SLOT(_q_volumeChanged(int)));
    connect(m_control, SIGNAL(tagsChanged()), this, SLOT(_q_tagsChanged()));
}

void GstreamerPlayerWork::stop()
{
    disconnect(m_session, SIGNAL(sessionPropertyChanged(QString,QVariant)), this, SLOT(_q_sessionPropertyChanged(QString,QVariant)));
    disconnect(m_session, SIGNAL(handlePause()), this, SLOT(_q_handlePause()));
    disconnect(m_session, SIGNAL(handlePlay()), this, SLOT(_q_handlePlay()));
    disconnect(m_session, SIGNAL(handleStop()), this, SLOT(_q_handleStop()));

    disconnect(m_control, SIGNAL(mediaChanged(MediaContent)), this, SLOT(_q_mediaChanged(MediaContent)));
    disconnect(m_control, SIGNAL(audioAvailableChanged(bool)), this, SLOT(_q_audioAvailableChanged(bool)));
    disconnect(m_control, SIGNAL(bufferStatusChanged(int)), this, SLOT(_q_bufferStatusChanged(int)));
    disconnect(m_control, SIGNAL(durationChanged(qint64)), this, SLOT(_q_durationChanged(qint64)));
    disconnect(m_control, SIGNAL(error(int,QString)), this, SLOT(_q_error(int,QString)));
    disconnect(m_control, SIGNAL(mediaStatusChanged(MultimediaPlayer::MediaStatus)), this, SLOT(_q_mediaStatusChanged(MultimediaPlayer::MediaStatus)));
    disconnect(m_control, SIGNAL(mutedChanged(bool)), this, SLOT(_q_mutedChanged(bool)));
    disconnect(m_control, SIGNAL(playbackRateChanged(qreal)), this, SLOT(_q_playbackRateChanged(qreal)));
    disconnect(m_control, SIGNAL(positionChanged(qint64)), this, SLOT(_q_positionChanged(qint64)));
    disconnect(m_control, SIGNAL(seekableChanged(bool)), this, SLOT(_q_seekableChanged(bool)));
    disconnect(m_control, SIGNAL(stateChanged(MultimediaPlayer::State)), this, SLOT(_q_stateChanged(MultimediaPlayer::State)));
    disconnect(m_control, SIGNAL(videoAvailableChanged(bool)), this, SLOT(_q_videoAvailableChanged(bool)));
    disconnect(m_control, SIGNAL(volumeChanged(int)), this, SLOT(_q_volumeChanged(int)));

    m_control->stop();

    emit finished();
}

void GstreamerPlayerWork::_q_sessionPropertyChanged(const QString &key,
                                                    const QVariant &value)
{
    MediaPlayerSession::PropertyType type = MediaPlayerSession::propertyType(key);
    Q_UNUSED(value);
    switch (type) {
    case MediaPlayerSession::Position:
        m_control->setPosition(m_session->position());
        break;
    case MediaPlayerSession::Volume:
        m_control->setVolume(m_session->volume());
        break;
    case MediaPlayerSession::Muted:
        m_control->setMuted(m_session->isMuted());
        break;
    case MediaPlayerSession::PlaybackRate:
        m_control->setPlaybackRate(m_session->playbackRate());
        break;
    case MediaPlayerSession::Media:
        m_control->setMedia(m_session->media());
        break;
    default:
        break;
    }
}

void GstreamerPlayerWork::_q_handlePause()
{
    m_control->pause();
}

void GstreamerPlayerWork::_q_handlePlay()
{
    m_control->play();
}

void GstreamerPlayerWork::_q_handleStop()
{
    m_control->stop();
}

void GstreamerPlayerWork::_q_mediaChanged(const MediaContent &content)
{
    m_session->_updateMedia(content);
}

void GstreamerPlayerWork::_q_durationChanged(qint64 duration)
{
    m_session->_updateDuration(duration);
}

void GstreamerPlayerWork::_q_positionChanged(qint64 position)
{
    m_session->_updatePosition(position);
}

void GstreamerPlayerWork::_q_stateChanged(MultimediaPlayer::State newState)
{
    m_session->_updatePlaybackState(newState);
}

void GstreamerPlayerWork::_q_mediaStatusChanged(MultimediaPlayer::MediaStatus status)
{
    m_session->_updateMediaStatus(status);
}

void GstreamerPlayerWork::_q_volumeChanged(int volume)
{
    m_session->_updateVolume(volume);
}

void GstreamerPlayerWork::_q_mutedChanged(bool muted)
{
    m_session->_updateMuted(muted);
}

void GstreamerPlayerWork::_q_audioAvailableChanged(bool audioAvailable)
{
    m_session->_updateAudioAvailable(audioAvailable);
}

void GstreamerPlayerWork::_q_videoAvailableChanged(bool videoAvailable)
{
    m_session->_updateVideoAvailable(videoAvailable);
}

void GstreamerPlayerWork::_q_bufferStatusChanged(int percentFilled)
{
    m_session->_updateBufferStatus(percentFilled);
}

void GstreamerPlayerWork::_q_seekableChanged(bool seekable)
{
    m_session->_updateSeekable(seekable);
}

void GstreamerPlayerWork::_q_playbackRateChanged(qreal rate)
{
    m_session->_updatePlaybackRate(rate);
}

void GstreamerPlayerWork::_q_error(int error, const QString &errorString)
{
    m_session->_updateError(error, errorString);
}

void GstreamerPlayerWork::_q_tagsChanged()
{
    m_session->_updateTags(m_control->tags());
}
