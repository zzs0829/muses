#include "multimediaplayer.h"

HS_BEGIN_NAMESPACE

static void qRegisterMultimediaPlayerMetaTypes()
{
    qRegisterMetaType<MultimediaPlayer::State>("MultimediaPlayer::State");
    qRegisterMetaType<MultimediaPlayer::MediaStatus>("MultimediaPlayer::MediaStatus");
    qRegisterMetaType<MultimediaPlayer::Error>("MultimediaPlayer::Error");
}

Q_CONSTRUCTOR_FUNCTION(qRegisterMultimediaPlayerMetaTypes)


/*!
    构造一个 MultimediaPlayer 实例
    指定参数： \a parent 和 \a flags。
*/
MultimediaPlayer::MultimediaPlayer(QObject *parent, MultimediaPlayer::Flags flags) :
    QObject(parent)
{
    Q_UNUSED(flags);
}

/*!
    销毁 MultimediaPlayer 对象。
*/

MultimediaPlayer::~MultimediaPlayer()
{

}



// Enums
/*!
    \enum MultimediaPlayer::State

    定义当前多媒体播放器的状态

    \value StoppedState 多媒体播放器处于停止状态。
    \value PlayingState 多媒体播放器处于正播放状态。
    \value PausedState 多媒体播放器处于暂停状态，唤醒后会继续播放。
*/

/*!
    \enum MultimediaPlayer::MediaStatus

    定义当前多媒体播放器中的媒体状态

    \value UnknownMediaStatus 未知媒体状态。
    \value NoMedia 当前多媒体播放器无媒体文件，播放器处于StoppedState。
    \value LoadingMedia 当前媒体文件正在被加载，播放器可以处于任何状态。
    \value LoadedMedia 当前媒体文件已经被加载完成，播放器处于StoppedState。
    \value StalledMedia 当前多媒体播放器由于缓冲不足或者其他临时中断，播放挂起，播放器可以处于PlayingState或PausedState。
    \value BufferingMedia 当前多媒体播放器正在缓冲数据，播放器可以处于PlayingState或PausedState。
    \value BufferedMedia 当前多媒体播放器完全缓冲数据，播放器可以处于PlayingState或PausedState。
    \value EndOfMedia 当前多媒体播放器播放至媒体的结束位置，播放器处于StoppedState。
    \value InvalidMedia 当前媒体无法被播放，播放器处于StoppedState。
*/

/*!
    \enum MultimediaPlayer::Flag

    \value LowLatency 简单的音频格式，低延时，可用于发出哔哔声，铃声等。
    \value StreamPlayback 播放基于IODevice的文件流。
    \value VideoSurface 播放视频。
*/

/*!
    \enum MultimediaPlayer::Error

    定义当前多媒体播放器的错误异常状况

    \value NoError 没有发生任何错误。
    \value ResourceError 媒体资源发生错误。
    \value FormatError 媒体资源格式不支持发生错误。
    \value NetworkError 发生网络连接的错误。
    \value AccessDeniedError 播放媒体资源没有适当的权限发生错误。
    \value ServiceMissingError 多媒体播放器服务丢失，发生播放错误。
*/

HS_END_NAMESPACE
