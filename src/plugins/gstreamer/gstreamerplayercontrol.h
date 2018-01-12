#ifndef GSTREAMERPLAYERCONTROL_H
#define GSTREAMERPLAYERCONTROL_H

#include <Multimedia/mediaplayercontroller.h>
#include <Multimedia/multimediaresourcesetinterface.h>
#include "gstreamerplayerengine.h"
#include <QtCore/qstack.h>
#include <QtCore/qtimer.h>

HS_BEGIN_NAMESPACE

class GstreamerPlayerControl : public MediaPlayerController
{
    Q_OBJECT
public:
    GstreamerPlayerControl(GstreamerPlayerEngine *engine, QObject *parent = 0);
    ~GstreamerPlayerControl();

    MultimediaPlayer::State state() const Q_DECL_OVERRIDE;
    MultimediaPlayer::MediaStatus mediaStatus() const Q_DECL_OVERRIDE;

    qint64 position() const Q_DECL_OVERRIDE;
    qint64 duration() const Q_DECL_OVERRIDE;

    int bufferStatus() const Q_DECL_OVERRIDE;

    int volume() const Q_DECL_OVERRIDE;
    bool isMuted() const Q_DECL_OVERRIDE;

    bool isAudioAvailable() const Q_DECL_OVERRIDE;
    bool isVideoAvailable() const Q_DECL_OVERRIDE;
    void setVideoOutput(QObject *output);

    bool isSeekable() const Q_DECL_OVERRIDE;
//    MultimediaTimeRange availablePlaybackRanges() const Q_DECL_OVERRIDE;

    qreal playbackRate() const Q_DECL_OVERRIDE;
    void setPlaybackRate(qreal rate) Q_DECL_OVERRIDE;

    MediaContent media() const Q_DECL_OVERRIDE;
    const QIODevice *mediaStream() const Q_DECL_OVERRIDE;
    void setMedia(const MediaContent&) Q_DECL_OVERRIDE;

    MultimediaResourceSetInterface* resources() const;

    QVariantMap tags() const;

public Q_SLOTS:
    void setPosition(qint64 pos) Q_DECL_OVERRIDE;

    void play() Q_DECL_OVERRIDE;
    void pause() Q_DECL_OVERRIDE;
    void stop() Q_DECL_OVERRIDE;

    void setVolume(int volume) Q_DECL_OVERRIDE;
    void setMuted(bool muted) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void updateSessionState(MultimediaPlayer::State state);
    void updateMediaStatus();
    void processEOS();
    void setBufferProgress(int progress);

    void handleInvalidMedia();

    void handleResourcesGranted();
    void handleResourcesLost();
    void handleResourcesDenied();

    void _q_notify();

private:
    void playOrPause(MultimediaPlayer::State state);

    void pushState();
    void popAndNotifyState();

    GstreamerPlayerEngine *m_engine;
    MultimediaPlayer::State m_userRequestedState;
    MultimediaPlayer::State m_currentState;
    MultimediaPlayer::MediaStatus m_mediaStatus;
    QStack<MultimediaPlayer::State> m_stateStack;
    QStack<MultimediaPlayer::MediaStatus> m_mediaStatusStack;

    int m_bufferProgress;
    qint64 m_pendingSeekPosition;
    bool m_setMediaPending;
    MediaContent m_currentResource;
    QIODevice *m_stream;

    MultimediaResourceSetInterface *m_resources;

    QTimer* notifyTimer;
};

HS_END_NAMESPACE

#endif // GSTREAMERPLAYERCONTROL_H
