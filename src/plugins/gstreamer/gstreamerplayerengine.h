#ifndef GSTREAMERPLAYERENGINE_H
#define GSTREAMERPLAYERENGINE_H

#include <Multimedia/multimediaengine.h>
#include <Multimedia/multimediaplayer.h>

#include "gstreamerglobal.h"
#include "gstreamerbushelper.h"
#include "gstreamervideorendererinterface.h"

#include <QtNetwork/qnetworkrequest.h>
#include <gst/gst.h>


HS_BEGIN_NAMESPACE

class GstreamerPlayerEngine : public MultimediaEngine,
        public GstreamerBusMessageFilter
{
    Q_OBJECT
    Q_INTERFACES(GstreamerBusMessageFilter)
public:
    explicit GstreamerPlayerEngine(QObject *parent = 0);
    virtual ~GstreamerPlayerEngine();

    GstElement *playbin() const;
    GstreamerBusHelper *bus() const { return m_busHelper; }

    QNetworkRequest request() const;

    MultimediaPlayer::State state() const { return m_state; }
    MultimediaPlayer::State pendingState() const { return m_pendingState; }

    qint64 duration() const;
    qint64 position() const;

    int volume() const;
    bool isMuted() const;

    bool isAudioAvailable() const;
    bool isVideoAvailable() const;

    void setVideoRenderer(QObject *renderer);

    bool isSeekable() const;

    qreal playbackRate() const;
    void setPlaybackRate(qreal rate);

    QMap<QByteArray ,QVariant> tags() const { return m_tags; }

    bool isLiveSource() const;

    void endOfMediaReset();

    virtual bool processBusMessage(const GstreamerMessage &message) Q_DECL_OVERRIDE;

public slots:
    void loadFromUri(const QNetworkRequest &request);

    bool play();
    bool pause();
    void stop();

    bool seek(qint64 pos);

    void setVolume(int volume);
    void setMuted(bool muted);

    void showPrerollFrames(bool enabled);

signals:
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    void stateChanged(MultimediaPlayer::State state);
    void volumeChanged(int volume);
    void mutedStateChanged(bool muted);
    void audioAvailableChanged(bool audioAvailable);
    void videoAvailableChanged(bool videoAvailable);
    void bufferingProgressChanged(int percentFilled);
    void playbackFinished();
    void tagsChanged();
    void seekableChanged(bool);
    void error(int error, const QString &errorString);
    void invalidMedia();
    void playbackRateChanged(qreal);

private slots:
    void setSeekable(bool);
    void finishVideoOutputChange();
    void updateVideoRenderer();
    void updateVolume();
    void updateMuted();
    void updateDuration();

private:
    static void handleVolumeChange(GObject *o, GParamSpec *p, gpointer d);
    static void handleMutedChange(GObject *o, GParamSpec *p, gpointer d);

    void processInvalidMedia(MultimediaPlayer::Error errorCode, const QString& errorString);

    void removeVideoBufferProbe();
    void addVideoBufferProbe();
    void flushVideoProbes();
    void resumeVideoProbes();

private:
    QNetworkRequest m_request;
    MultimediaPlayer::State m_state;
    MultimediaPlayer::State m_pendingState;

    int m_volume;
    qreal m_playbackRate;
    bool m_muted;
    bool m_audioAvailable;
    bool m_videoAvailable;
    bool m_seekable;

    mutable qint64 m_lastPosition;
    qint64 m_duration;
    int m_durationQueries;

    bool m_displayPrerolledFrame;

    QMap<QByteArray, QVariant> m_tags;

    GstElement* m_playbin;
    GstElement* m_audioSink;
    GstElement* m_volumeElement;

    GstElement* m_videoSink;
    GstElement* m_videoOutputBin;
    GstElement* m_videoIdentity;
    GstElement* m_pendingVideoSink;
    GstElement* m_nullVideoSink;

    GstBus* m_bus;
    GstreamerBusHelper* m_busHelper;
    QObject *m_videoOutput;
    GstreamerVideoRendererInterface *m_renderer;

//    GstreamerVideoProbeControl *m_videoProbe;
//    GstreamerAudioProbeControl *m_audioProbe;


    enum SourceType
    {
        UnknownSrc,
        SoupHTTPSrc,
        UDPSrc,
        MMSSrc,
        RTSPSrc,
    };
    SourceType m_sourceType;
    bool m_everPlayed;
    bool m_isLiveSource;

    gulong pad_probe_id;
};

HS_END_NAMESPACE

#endif // GSTREAMERPLAYERENGINE_H
