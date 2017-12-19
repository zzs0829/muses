#ifndef MULTIMEDIAPLAYER_H
#define MULTIMEDIAPLAYER_H

#include <Multimedia/multimediaobject.h>
#include <Multimedia/multimediaglobal.h>
#include <Multimedia/multimediacontroller.h>

HS_BEGIN_NAMESPACE

class MULTIMEDIA_EXPORT MultimediaPlayer : public MultimediaObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_ENUMS(MediaStatus)
    Q_ENUMS(Error)
public:
    enum State
    {
        StoppedState,
        PlayingState,
        PausedState
    };

    enum MediaStatus
    {
        UnknownMediaStatus,
        NoMedia,
        LoadingMedia,
        LoadedMedia,
        StalledMedia,
        BufferingMedia,
        BufferedMedia,
        EndOfMedia,
        InvalidMedia
    };

    enum Flag
    {
        LowLatency = 0x01,      // 简单的音频格式，低延时，可用于发出哔哔声，铃声等。
        StreamPlayback = 0x02,  // 播放基于IODevice的文件流。
        VideoSurface = 0x04     // 播放视频。
    };
    Q_DECLARE_FLAGS(Flags, Flag) //  Flags类型用于存储一个位或“|”的组合组合值。

    enum Error
    {
        NoError,
        ResourceError,
        FormatError,
        NetworkError,
        AccessDeniedError,
        ServiceMissingError
    };

public:
    explicit MultimediaPlayer(QObject *parent = Q_NULLPTR, Flags flags = Flags());
    ~MultimediaPlayer();

signals:

public slots:
};

HS_END_NAMESPACE

Q_DECLARE_METATYPE(MultimediaPlayer::State)
Q_DECLARE_METATYPE(MultimediaPlayer::MediaStatus)
Q_DECLARE_METATYPE(MultimediaPlayer::Error)

MULTIMEDIA_ENUM_DEBUG(MultimediaPlayer, State)
MULTIMEDIA_ENUM_DEBUG(MultimediaPlayer, MediaStatus)
MULTIMEDIA_ENUM_DEBUG(MultimediaPlayer, Error)

#endif // MULTIMEDIAPLAYER_H
