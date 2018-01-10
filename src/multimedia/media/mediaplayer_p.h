#ifndef MEDIAPLAYER_P_H
#define MEDIAPLAYER_P_H

#include "mediaplayer.h"
#include <Multimedia/mediaplayersession.h>
#include <Multimedia/multimediaserviceprovider.h>
#include <Multimedia/multimediaserviceproviderplugin.h>

#include <QtCore/qfileinfo.h>

class MediaPlayerPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(MediaPlayer)
public:
    explicit MediaPlayerPrivate(MediaPlayer *parent = 0);

    void setMedia(const MediaContent &media);

public slots:
    void _q_stateChanged(MultimediaPlayer::State state);
    void _q_mediaStatusChanged(MultimediaPlayer::MediaStatus status);
    void _q_error(int error, const QString &errorString);
    void _q_handleMediaChanged(const MediaContent&);

public:
    MediaPlayer *q_ptr;

    MultimediaServiceProvider *provider;
    MultimediaService* service;
    MediaPlayerSession *session;
    MediaPlaylist *playlist;
    MediaContent curMedia;
    MediaContent qrcMedia;
    QScopedPointer<QFile> qrcFile;

    MultimediaPlayer::State state;
    MultimediaPlayer::MediaStatus status;
    qint64 duration;
    qint64 position;
    int volume;
    bool muted;
    int bufStatus;
    bool audioAvailable;
    bool videoAvailable;
    bool seekable;
    qreal rate;

    MultimediaPlayer::Error error;
    QString errorString;
    int ignoreNextStatusChange;
};

#endif // MEDIAPLAYER_P_H
