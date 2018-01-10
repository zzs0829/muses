#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <Multimedia/multimediaplayer.h>
#include <Multimedia/mediacontent.h>
#include "mediaplaylist.h"

HS_BEGIN_NAMESPACE

class MediaPlayerFactory;
class MediaPlayerPrivate;
class MULTIMEDIA_EXPORT MediaPlayer : public MultimediaPlayer
{
    Q_OBJECT
public:
    explicit MediaPlayer(const QString &key, QObject *parent = Q_NULLPTR, Flags flags = Flags());
    ~MediaPlayer();

    MediaPlaylist *playlist() const;

    State state() const;
    MediaStatus mediaStatus() const;

    MediaContent media() const;
    // When playing a resource file, don't return the backend's current media, which
    // can be a temporary file.
    MediaContent currentMedia() const;

    qint64 duration() const;
    qint64 position() const;

    int volume() const;
    bool isMuted() const;
    bool isAudioAvailable() const;
    bool isVideoAvailable() const;

    int bufferStatus() const;

    bool isSeekable() const;
    qreal playbackRate() const;

    Error error() const;
    QString errorString() const;

//    QMultimedia::AvailabilityStatus availability() const override;

public slots:
    void play();
    void pause();
    void stop();

    void setPosition(qint64 position);
    void setVolume(int v);
    void setMuted(bool muted);

    void setPlaybackRate(qreal rate);

    void setMedia(const MediaContent &media);
    void setPlaylist(MediaPlaylist *playlist);

signals:
    void mediaChanged(const MediaContent &media);
    // When playing a resource file, don't return the backend's current media, which
    // can be a temporary file.
    void currentMediaChanged(const MediaContent &media);

    void stateChanged(MultimediaPlayer::State newState);
    void mediaStatusChanged(MultimediaPlayer::MediaStatus status);

    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);

    void volumeChanged(int volume);
    void mutedChanged(bool muted);
    void audioAvailableChanged(bool available);
    void videoAvailableChanged(bool videoAvailable);

    void bufferStatusChanged(int percentFilled);

    void seekableChanged(bool seekable);
    void playbackRateChanged(qreal rate);

    void error(MultimediaPlayer::Error error);

protected:
    MediaPlayerPrivate *d_ptr;

private:
    friend class MediaPlayerFactory;

    Q_DECLARE_PRIVATE(MediaPlayer)
};

HS_END_NAMESPACE

#endif // MEDIAPLAYER_H
