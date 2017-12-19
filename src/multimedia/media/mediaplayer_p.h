#ifndef MEDIAPLAYER_P_H
#define MEDIAPLAYER_P_H

#include "mediaplayer.h"

#include <QtCore/qfileinfo.h>

class MediaPlayerPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(MediaPlayer)
public:
    explicit MediaPlayerPrivate(MediaPlayer *parent = 0);

    void setMedia(const MediaContent &media);

signals:
    void requestSetMedia(const MediaContent &media);

public slots:
    void _q_stateChanged(MultimediaPlayer::State state);
    void _q_mediaStatusChanged(MultimediaPlayer::MediaStatus status);
    void _q_error(int error, const QString &errorString);

public:
    MediaPlayer *q_ptr;

    MediaPlaylist *playlist;
    MediaContent qrcMedia;
    QScopedPointer<QFile> qrcFile;

    MultimediaPlayer::State state;
    MultimediaPlayer::MediaStatus status;
    MultimediaPlayer::Error error;
    QString errorString;
    int ignoreNextStatusChange;
};

#endif // MEDIAPLAYER_P_H
