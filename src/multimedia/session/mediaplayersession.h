#ifndef MEDIAPLAYERSESSION_H
#define MEDIAPLAYERSESSION_H


#include <Multimedia/multimediasession.h>
#include <Multimedia/multimediaplayer.h>
#include <Multimedia/mediacontent.h>


HS_BEGIN_NAMESPACE

class MediaPlayerSessionPrivate;
class MULTIMEDIA_EXPORT MediaPlayerSession : public MultimediaSession
{
    Q_OBJECT
public:
    enum PropertyType {
        PlaybackState,
        MediaStatus,
        Duration,
        Position,
        Volume,
        Muted,
        BufferStatus,
        AudioAvailable,
        VideoAvailable,
        Seekable,
        PlaybackRate,
        Media,
        Error,
        ErrorString
    };

    static PropertyType propertyType(const QString &key);
    static QString propertyKey(MediaPlayerSession::PropertyType type);

    virtual MultimediaPlayer::State state() const;
    virtual MultimediaPlayer::MediaStatus mediaStatus() const;
    virtual qint64 duration() const;
    virtual qint64 position() const;
    virtual void setPosition(qint64 position);
    virtual int volume() const;
    virtual void setVolume(int volume);
    virtual bool isMuted() const;
    virtual void setMuted(bool muted);
    virtual int bufferStatus() const;
    virtual bool isAudioAvailable() const;
    virtual bool isVideoAvailable() const;
    virtual bool isSeekable() const;
    virtual qreal playbackRate() const;
    virtual void setPlaybackRate(qreal rate);
    virtual MediaContent media() const;
    virtual void setMedia(const MediaContent &media);

    virtual void play();
    virtual void pause();
    virtual void stop();

signals:
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

    void handlePlay();
    void handlePause();
    void handleStop();

protected:
    explicit MediaPlayerSession(const QString &id, const QString &key, QObject *parent = 0);
    MediaPlayerSession(MediaPlayerSessionPrivate &dd, QObject *parent);

    void _updateMedia(const MediaContent& content);
    void _updatePlaybackState(MultimediaPlayer::State newState);
    void _updateMediaStatus(MultimediaPlayer::MediaStatus status);
    void _updateDuration(qint64 duration);
    void _updatePosition(qint64 position);
    void _updateVolume(int volume);
    void _updateMuted(bool muted);
    void _updateBufferStatus(int percentFilled);
    void _updateAudioAvailable(bool audioAvailable);
    void _updateVideoAvailable(bool videoAvailable);
    void _updateSeekable(bool seekable);
    void _updatePlaybackRate(qreal rate);
    void _updateError(int err, const QString &errorString);

private:
    static QMap<PropertyType, QString> propertyMap;
    static void initPropertyMap();
    void initProperty();

private:
    Q_DISABLE_COPY(MediaPlayerSession)
    Q_DECLARE_PRIVATE(MediaPlayerSession)
};

HS_END_NAMESPACE

#endif // MEDIAPLAYERSESSION_H
