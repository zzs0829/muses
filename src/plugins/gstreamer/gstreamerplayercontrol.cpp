#include "gstreamerplayercontrol.h"
#include <Multimedia/multimediaresourcepolicy.h>
#include <QTime>


HS_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(__gst__)

GstreamerPlayerControl::GstreamerPlayerControl(GstreamerPlayerEngine *engine, QObject *parent)
    : MediaPlayerController(parent)
    , m_engine(engine)
    , m_userRequestedState(MultimediaPlayer::StoppedState)
    , m_currentState(MultimediaPlayer::StoppedState)
    , m_mediaStatus(MultimediaPlayer::NoMedia)
    , m_bufferProgress(-1)
    , m_pendingSeekPosition(-1)
    , m_setMediaPending(false)
    , m_stream(0)
    , m_resources(0)
    , notifyTimer(0)
{
    m_resources = MultimediaResourcePolicy::createResourceSet<MultimediaResourceSetInterface>();
    Q_ASSERT(m_resources);

    connect(m_engine, SIGNAL(positionChanged(qint64)),
            this, SIGNAL(positionChanged(qint64)));
    connect(m_engine, SIGNAL(durationChanged(qint64)),
            this, SIGNAL(durationChanged(qint64)));
    connect(m_engine, SIGNAL(mutedStateChanged(bool)),
            this, SIGNAL(mutedChanged(bool)));
    connect(m_engine, SIGNAL(volumeChanged(int)),
            this, SIGNAL(volumeChanged(int)));
    connect(m_engine, SIGNAL(stateChanged(MultimediaPlayer::State)),
            this, SLOT(updateSessionState(MultimediaPlayer::State)));
    connect(m_engine,SIGNAL(bufferingProgressChanged(int)),
            this, SLOT(setBufferProgress(int)));
    connect(m_engine, SIGNAL(playbackFinished()),
            this, SLOT(processEOS()));
    connect(m_engine, SIGNAL(audioAvailableChanged(bool)),
            this, SIGNAL(audioAvailableChanged(bool)));
    connect(m_engine, SIGNAL(videoAvailableChanged(bool)),
            this, SIGNAL(videoAvailableChanged(bool)));
    connect(m_engine, SIGNAL(seekableChanged(bool)),
            this, SIGNAL(seekableChanged(bool)));
    connect(m_engine, SIGNAL(error(int,QString)),
            this, SIGNAL(error(int,QString)));
    connect(m_engine, SIGNAL(invalidMedia()),
            this, SLOT(handleInvalidMedia()));
    connect(m_engine, SIGNAL(playbackRateChanged(qreal)),
            this, SIGNAL(playbackRateChanged(qreal)));

    connect(m_resources, SIGNAL(resourcesGranted()), SLOT(handleResourcesGranted()));
    //denied signal should be queued to have correct state update process,
    //since in playOrPause, when acquire is call on resource set, it may trigger a resourcesDenied signal immediately,
    //so handleResourcesDenied should be processed later, otherwise it will be overwritten by state update later in playOrPause.
    connect(m_resources, SIGNAL(resourcesDenied()), this, SLOT(handleResourcesDenied()), Qt::QueuedConnection);
    connect(m_resources, SIGNAL(resourcesLost()), SLOT(handleResourcesLost()));

    notifyTimer = new QTimer(this);
    notifyTimer->setInterval(1000);
    notifyTimer->setTimerType(Qt::PreciseTimer);
    connect(notifyTimer, SIGNAL(timeout()), SLOT(_q_notify()));
}

GstreamerPlayerControl::~GstreamerPlayerControl()
{
    MultimediaResourcePolicy::destroyResourceSet(m_resources);
}

MultimediaResourceSetInterface* GstreamerPlayerControl::resources() const
{
    return m_resources;
}

qint64 GstreamerPlayerControl::position() const
{
    if (m_mediaStatus == MultimediaPlayer::EndOfMedia)
        return m_engine->duration();

    return m_pendingSeekPosition != -1 ? m_pendingSeekPosition : m_engine->position();
}

qint64 GstreamerPlayerControl::duration() const
{
    return m_engine->duration();
}

MultimediaPlayer::State GstreamerPlayerControl::state() const
{
    return m_currentState;
}

MultimediaPlayer::MediaStatus GstreamerPlayerControl::mediaStatus() const
{
    return m_mediaStatus;
}

int GstreamerPlayerControl::bufferStatus() const
{
    if (m_bufferProgress == -1) {
        return m_engine->state() == MultimediaPlayer::StoppedState ? 0 : 100;
    } else
        return m_bufferProgress;
}

int GstreamerPlayerControl::volume() const
{
    return m_engine->volume();
}

bool GstreamerPlayerControl::isMuted() const
{
    return m_engine->isMuted();
}

bool GstreamerPlayerControl::isSeekable() const
{
    return m_engine->isSeekable();
}

//QMediaTimeRange GstreamerPlayerControl::availablePlaybackRanges() const
//{
//    return m_engine->availablePlaybackRanges();
//}

qreal GstreamerPlayerControl::playbackRate() const
{
    return m_engine->playbackRate();
}

void GstreamerPlayerControl::setPlaybackRate(qreal rate)
{
    m_engine->setPlaybackRate(rate);
}

void GstreamerPlayerControl::setPosition(qint64 pos)
{
    qCInfo(__gst__, "[ SetPosition ] = %lld ms", pos);

    pushState();

    if (m_mediaStatus == MultimediaPlayer::EndOfMedia) {
        m_mediaStatus = MultimediaPlayer::LoadedMedia;
    }

    if (m_currentState == MultimediaPlayer::StoppedState) {
        m_pendingSeekPosition = pos;
        emit positionChanged(m_pendingSeekPosition);
    } else if (m_engine->isSeekable()) {
        m_engine->showPrerollFrames(true);
        m_engine->seek(pos);
        m_pendingSeekPosition = -1;
    } else if (m_engine->state() == MultimediaPlayer::StoppedState) {
        m_pendingSeekPosition = pos;
        emit positionChanged(m_pendingSeekPosition);
    } else if (m_pendingSeekPosition != -1) {
        m_pendingSeekPosition = -1;
        emit positionChanged(m_pendingSeekPosition);
    }

    popAndNotifyState();
}

void GstreamerPlayerControl::play()
{
    //m_userRequestedState is needed to know that we need to resume playback when resource-policy
    //regranted the resources after lost, since m_currentState will become paused when resources are
    //lost.
    m_userRequestedState = MultimediaPlayer::PlayingState;
    playOrPause(MultimediaPlayer::PlayingState);
}

void GstreamerPlayerControl::pause()
{
    m_userRequestedState = MultimediaPlayer::PausedState;
    playOrPause(MultimediaPlayer::PausedState);
}

void GstreamerPlayerControl::playOrPause(MultimediaPlayer::State newState)
{
    qCInfo(__gst__) << "[ PlayOrPause ] to" << newState
                    << "when" << m_currentState
                    << "url:" << m_currentResource.url().toString()
                    << "status:" << m_mediaStatus;

    if (m_mediaStatus == MultimediaPlayer::NoMedia)
        return;

    pushState();

    if (m_setMediaPending) {
        m_mediaStatus = MultimediaPlayer::LoadingMedia;
        setMedia(m_currentResource);
    }

    if (m_mediaStatus == MultimediaPlayer::EndOfMedia && m_pendingSeekPosition == -1) {
        m_pendingSeekPosition = 0;
    }

    if (!m_resources->isGranted())
        m_resources->acquire();

    if (m_resources->isGranted()) {
        // show prerolled frame if switching from stopped state
        if (m_pendingSeekPosition == -1) {
            m_engine->showPrerollFrames(true);
        } else if (m_engine->state() == MultimediaPlayer::StoppedState) {
            // Don't evaluate the next two conditions.
        } else if (m_engine->isSeekable()) {
            m_engine->pause();
            m_engine->showPrerollFrames(true);
            m_engine->seek(m_pendingSeekPosition);
            m_pendingSeekPosition = -1;
        } else {
            m_pendingSeekPosition = -1;
        }

        bool ok = false;

        //To prevent displaying the first video frame when playback is resumed
        //the pipeline is paused instead of playing, seeked to requested position,
        //and after seeking is finished (position updated) playback is restarted
        //with show-preroll-frame enabled.
        if (newState == MultimediaPlayer::PlayingState && m_pendingSeekPosition == -1)
            ok = m_engine->play();
        else
            ok = m_engine->pause();

        if (!ok)
            newState = MultimediaPlayer::StoppedState;
    }

    if (m_mediaStatus == MultimediaPlayer::InvalidMedia)
        m_mediaStatus = MultimediaPlayer::LoadingMedia;

    m_currentState = newState;

    if (m_mediaStatus == MultimediaPlayer::EndOfMedia || m_mediaStatus == MultimediaPlayer::LoadedMedia) {
        if (m_bufferProgress == -1 || m_bufferProgress == 100)
            m_mediaStatus = MultimediaPlayer::BufferedMedia;
        else
            m_mediaStatus = MultimediaPlayer::BufferingMedia;
    }

    popAndNotifyState();

    emit positionChanged(position());
}

void GstreamerPlayerControl::stop()
{
    m_userRequestedState = MultimediaPlayer::StoppedState;

    pushState();

    if (m_currentState != MultimediaPlayer::StoppedState) {
        m_currentState = MultimediaPlayer::StoppedState;
        m_engine->showPrerollFrames(false); // stop showing prerolled frames in stop state
        if (m_resources->isGranted())
            m_engine->pause();

        if (m_mediaStatus != MultimediaPlayer::EndOfMedia) {
            m_pendingSeekPosition = 0;
            emit positionChanged(position());
        }
    }

    popAndNotifyState();
}

void GstreamerPlayerControl::setVolume(int volume)
{
    m_engine->setVolume(volume);
}

void GstreamerPlayerControl::setMuted(bool muted)
{
    m_engine->setMuted(muted);
}

MediaContent GstreamerPlayerControl::media() const
{
    return m_currentResource;
}

const QIODevice *GstreamerPlayerControl::mediaStream() const
{
    return m_stream;
}

void GstreamerPlayerControl::setMedia(const MediaContent &content)
{
    QIODevice *stream = 0;
    if(content.isNull())
        qCInfo(__gst__) << "[ SetMedia ] = NULL ";
    else
        qCInfo(__gst__) << "[ SetMedia ] =" << content.url().toString();

    pushState();

    m_currentState = MultimediaPlayer::StoppedState;
    MediaContent oldMedia = m_currentResource;
    m_pendingSeekPosition = 0;
    m_engine->showPrerollFrames(false); // do not show prerolled frames until pause() or play() explicitly called
    m_setMediaPending = false;

    if (!content.isNull() || stream) {
        if (!m_resources->isGranted())
            m_resources->acquire();
    } else {
        m_resources->release();
    }

    m_engine->stop();

    bool userStreamValid = false;
    Q_UNUSED(userStreamValid)

    if (m_bufferProgress != -1) {
        m_bufferProgress = -1;
        emit bufferStatusChanged(0);
    }

    m_currentResource = content;
    m_stream = stream;

    QNetworkRequest request;

    if (m_stream) {
        userStreamValid = stream->isOpen() && m_stream->isReadable();
        request = content.request();
    } else if (!content.isNull()) {
        request = content.request();
    }

    m_engine->loadFromUri(request);

    if (!request.url().isEmpty()) {
        m_mediaStatus = MultimediaPlayer::LoadingMedia;
        m_engine->pause();
    } else {
        m_mediaStatus = MultimediaPlayer::NoMedia;
        setBufferProgress(0);
    }

    if (m_currentResource != oldMedia)
        emit mediaChanged(m_currentResource);

    emit positionChanged(position());

    if (content.isNull() && !stream)
        m_resources->release();

    popAndNotifyState();
}

void GstreamerPlayerControl::setVideoOutput(QObject *output)
{
    m_engine->setVideoRenderer(output);
}

bool GstreamerPlayerControl::isAudioAvailable() const
{
    return m_engine->isAudioAvailable();
}

bool GstreamerPlayerControl::isVideoAvailable() const
{
    return m_engine->isVideoAvailable();
}

void GstreamerPlayerControl::updateSessionState(MultimediaPlayer::State state)
{
    pushState();

    if (state == MultimediaPlayer::StoppedState) {
        m_engine->showPrerollFrames(false);
        m_currentState = MultimediaPlayer::StoppedState;
    }

    if (state == MultimediaPlayer::PausedState && m_currentState != MultimediaPlayer::StoppedState) {
        if (m_pendingSeekPosition != -1 && m_engine->isSeekable()) {
            m_engine->showPrerollFrames(true);
            m_engine->seek(m_pendingSeekPosition);
        }
        m_pendingSeekPosition = -1;

        if (m_currentState == MultimediaPlayer::PlayingState)
            m_engine->play();
    }

    updateMediaStatus();

    popAndNotifyState();
}

void GstreamerPlayerControl::updateMediaStatus()
{
    pushState();
    MultimediaPlayer::MediaStatus oldStatus = m_mediaStatus;

    switch (m_engine->state()) {
    case MultimediaPlayer::StoppedState:
        if (m_currentResource.isNull())
            m_mediaStatus = MultimediaPlayer::NoMedia;
        else if (oldStatus != MultimediaPlayer::InvalidMedia)
            m_mediaStatus = MultimediaPlayer::LoadingMedia;
        break;

    case MultimediaPlayer::PlayingState:
    case MultimediaPlayer::PausedState:
        if (m_currentState == MultimediaPlayer::StoppedState) {
            m_mediaStatus = MultimediaPlayer::LoadedMedia;
        } else {
            if (m_bufferProgress == -1 || m_bufferProgress == 100)
                m_mediaStatus = MultimediaPlayer::BufferedMedia;
            else
                m_mediaStatus = MultimediaPlayer::StalledMedia;
        }
        break;
    }

    if (m_currentState == MultimediaPlayer::PlayingState && !m_resources->isGranted())
        m_mediaStatus = MultimediaPlayer::StalledMedia;

    //EndOfMedia status should be kept, until reset by pause, play or setMedia
    if (oldStatus == MultimediaPlayer::EndOfMedia)
        m_mediaStatus = MultimediaPlayer::EndOfMedia;

    popAndNotifyState();
}

void GstreamerPlayerControl::processEOS()
{
    pushState();
    m_mediaStatus = MultimediaPlayer::EndOfMedia;
    emit positionChanged(position());
    m_engine->endOfMediaReset();

    if (m_currentState != MultimediaPlayer::StoppedState) {
        m_currentState = MultimediaPlayer::StoppedState;
        m_engine->showPrerollFrames(false); // stop showing prerolled frames in stop state
    }

    popAndNotifyState();
}

void GstreamerPlayerControl::setBufferProgress(int progress)
{
    if (m_bufferProgress == progress || m_mediaStatus == MultimediaPlayer::NoMedia)
        return;
    qCInfo(__gst__, "[ BufferProgress ] = %3d%%", progress);

    m_bufferProgress = progress;

    if (m_resources->isGranted()) {
        if (m_currentState == MultimediaPlayer::PlayingState &&
                m_bufferProgress == 100 &&
                m_engine->state() != MultimediaPlayer::PlayingState)
            m_engine->play();

        if (!m_engine->isLiveSource() && m_bufferProgress < 100 &&
                (m_engine->state() == MultimediaPlayer::PlayingState ||
                 m_engine->pendingState() == MultimediaPlayer::PlayingState))
            m_engine->pause();
    }

    updateMediaStatus();

    emit bufferStatusChanged(m_bufferProgress);
}

void GstreamerPlayerControl::handleInvalidMedia()
{
    pushState();
    m_mediaStatus = MultimediaPlayer::InvalidMedia;
    m_currentState = MultimediaPlayer::StoppedState;
    m_setMediaPending = true;
    popAndNotifyState();
}

void GstreamerPlayerControl::handleResourcesGranted()
{
    pushState();

    //This may be triggered when there is an auto resume
    //from resource-policy, we need to take action according to m_userRequestedState
    //rather than m_currentState
    m_currentState = m_userRequestedState;
    if (m_currentState != MultimediaPlayer::StoppedState)
        playOrPause(m_currentState);
    else
        updateMediaStatus();

    popAndNotifyState();
}

void GstreamerPlayerControl::handleResourcesLost()
{
    //on resource lost the pipeline should be paused
    //player status is changed to paused
    pushState();
    MultimediaPlayer::State oldState = m_currentState;

    m_engine->pause();

    if (oldState != MultimediaPlayer::StoppedState )
        m_currentState = MultimediaPlayer::PausedState;

    popAndNotifyState();
}

void GstreamerPlayerControl::handleResourcesDenied()
{
    //on resource denied the pipeline should stay paused
    //player status is changed to paused
    pushState();

    if (m_currentState != MultimediaPlayer::StoppedState )
        m_currentState = MultimediaPlayer::PausedState;

    popAndNotifyState();
}

void GstreamerPlayerControl::_q_notify()
{
    qCInfo(__gst__) <<  "[ NotifyPosition ]"
                     << QDateTime::fromSecsSinceEpoch(qRound(position()/1000.0), Qt::UTC).toString("hh:mm:ss") << "/"
                     << QDateTime::fromSecsSinceEpoch(qRound(duration()/1000.0), Qt::UTC).toString("hh:mm:ss");
    emit positionChanged(position());
    emit durationChanged(duration());
}

void GstreamerPlayerControl::pushState()
{
    m_stateStack.push(m_currentState);
    m_mediaStatusStack.push(m_mediaStatus);
}

void GstreamerPlayerControl::popAndNotifyState()
{
    Q_ASSERT(!m_stateStack.isEmpty());

    MultimediaPlayer::State oldState = m_stateStack.pop();
    MultimediaPlayer::MediaStatus oldMediaStatus = m_mediaStatusStack.pop();

    if (m_stateStack.isEmpty()) {
        if (m_mediaStatus != oldMediaStatus) {
            qCInfo(__gst__) << "[ NotifyStatus ] =" << m_mediaStatus;

            emit mediaStatusChanged(m_mediaStatus);
        }

        if (m_currentState != oldState) {
            qCInfo(__gst__) << "[ NotifyState ] =" << m_currentState;

            emit stateChanged(m_currentState);

            if(m_currentState == MultimediaPlayer::PlayingState) {
                if(!notifyTimer->isActive())
                    notifyTimer->start();
            }else{
                notifyTimer->stop();
            }
        }
    }
}


HS_END_NAMESPACE
