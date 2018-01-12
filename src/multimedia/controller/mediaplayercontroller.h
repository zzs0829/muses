#ifndef MEDIAPLAYERCONTROLLER_H
#define MEDIAPLAYERCONTROLLER_H

#include <Multimedia/multimediacontroller.h>
#include <Multimedia/multimediaplayer.h>
#include <Multimedia/mediacontent.h>

HS_BEGIN_NAMESPACE

class MULTIMEDIA_EXPORT MediaPlayerController : public MultimediaController
{
    Q_OBJECT
public:
    virtual ~MediaPlayerController();

    virtual MultimediaPlayer::State state() const = 0;

    virtual MultimediaPlayer::MediaStatus mediaStatus() const = 0;

    virtual qint64 duration() const = 0;
    virtual qint64 position() const = 0;
    virtual void setPosition(qint64 position) = 0;

    virtual int volume() const = 0;
    virtual void setVolume(int volume) = 0;

    virtual bool isMuted() const = 0;
    virtual void setMuted(bool muted) = 0;

    virtual int bufferStatus() const = 0;

    virtual bool isAudioAvailable() const = 0;
    virtual bool isVideoAvailable() const = 0;

    virtual bool isSeekable() const = 0;

//    virtual MultimediaTimeRange availablePlaybackRanges() const = 0;

    virtual qreal playbackRate() const = 0;
    virtual void setPlaybackRate(qreal rate) = 0;

    virtual MediaContent media() const = 0;
    virtual const QIODevice *mediaStream() const = 0;
    virtual void setMedia(const MediaContent &media) = 0;

    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;

Q_SIGNALS:
    void mediaChanged(const MediaContent& content);
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    void stateChanged(MultimediaPlayer::State newState);
    void mediaStatusChanged(MultimediaPlayer::MediaStatus status);
    void volumeChanged(int volume);
    void mutedChanged(bool muted);
    void audioAvailableChanged(bool audioAvailable);
    void videoAvailableChanged(bool videoAvailable);
    void bufferStatusChanged(int percentFilled);
    void seekableChanged(bool);
//    void availablePlaybackRangesChanged(const MultimediaTimeRange&);
    void playbackRateChanged(qreal rate);
    void error(int error, const QString &errorString);
    void tagsChanged();

protected:
    explicit MediaPlayerController(QObject *parent = Q_NULLPTR);
};

#define MediaPlayerController_uid "module.muses.mediaplayercontroller/1.0"
MULTIMEDIA_DECLARE_CONTROLLER(MediaPlayerController, MediaPlayerController_uid)

HS_END_NAMESPACE

#endif // MEDIAPLAYERCONTROLLER_H
