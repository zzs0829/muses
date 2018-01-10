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
        Media
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
    void handlePlay();
    void handlePause();
    void handleStop();

protected:
    explicit MediaPlayerSession(const QString &id, const QString &key, QObject *parent = 0);
    MediaPlayerSession(MediaPlayerSessionPrivate &dd, QObject *parent);

private:
    Q_DISABLE_COPY(MediaPlayerSession)
    Q_DECLARE_PRIVATE(MediaPlayerSession)

    static QMap<PropertyType, QString> propertyMap;
    static void initPropertyMap();
};

HS_END_NAMESPACE

#endif // MEDIAPLAYERSESSION_H
