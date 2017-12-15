#ifndef GSTREAMERPLAYERCONTROL_H
#define GSTREAMERPLAYERCONTROL_H

#include <Multimedia/mediaplayercontroller.h>
#include <Multimedia/multimediaresourcesetinterface.h>
#include "gstreamerplayerengine.h"
#include <QStack>

HS_BEGIN_NAMESPACE

class GstreamerPlayerControl : public MediaPlayerController
{
    Q_OBJECT
public:
    GstreamerPlayerControl(GstreamerPlayerEngine *engine, QObject *parent = 0);
    ~GstreamerPlayerControl();

    MultimediaPlayer::State state() const override;
    MultimediaPlayer::MediaStatus mediaStatus() const override;

    qint64 position() const override;
    qint64 duration() const override;

    int bufferStatus() const override;

    int volume() const override;
    bool isMuted() const override;

    bool isAudioAvailable() const override;
    bool isVideoAvailable() const override;
    void setVideoOutput(QObject *output);

    bool isSeekable() const override;
//    MultimediaTimeRange availablePlaybackRanges() const override;

    qreal playbackRate() const override;
    void setPlaybackRate(qreal rate) override;

    MediaContent media() const override;
    const QIODevice *mediaStream() const override;
    void setMedia(const MediaContent&) override;

    MultimediaResourceSetInterface* resources() const;

public Q_SLOTS:
    void setPosition(qint64 pos) override;

    void play() override;
    void pause() override;
    void stop() override;

    void setVolume(int volume) override;
    void setMuted(bool muted) override;

private Q_SLOTS:
    void updateSessionState(MultimediaPlayer::State state);
    void updateMediaStatus();
    void processEOS();
    void setBufferProgress(int progress);

    void handleInvalidMedia();

    void handleResourcesGranted();
    void handleResourcesLost();
    void handleResourcesDenied();

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
};

HS_END_NAMESPACE

#endif // GSTREAMERPLAYERCONTROL_H
