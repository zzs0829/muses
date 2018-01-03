#include "mediaplayer_p.h"
#include <QtCore/qtemporaryfile.h>

MediaPlayerPrivate::MediaPlayerPrivate(MediaPlayer *parent) :
    QObject(parent),
    q_ptr(parent),
    control(0),
    playlist(0),
    state(MultimediaPlayer::StoppedState),
    status(MultimediaPlayer::UnknownMediaStatus),
    duration(-1),
    position(0),
    volume(100),
    muted(false),
    bufStatus(-1),
    audioAvailable(false),
    videoAvailable(false),
    seekable(false),
    rate(1.0),
    error(MultimediaPlayer::NoError),
    ignoreNextStatusChange(-1)
{

}

void MediaPlayerPrivate::setMedia(const MediaContent &media)
{
    Q_Q(MediaPlayer);

    if (!control)
        return;

    QScopedPointer<QFile> file;
    // Backends can't play qrc files directly.
    // If the backend supports StreamPlayback, we pass a QFile for that resource.
    // If it doesn't, we copy the data to a temporary file and pass its path.
    if (!media.isNull() && media.url().scheme() == QLatin1String("qrc")) {
        qrcMedia = media;
        file.reset(new QFile(QLatin1Char(':') + media.url().path()));
        if (!file->open(QFile::ReadOnly)) {
            QMetaObject::invokeMethod(this, "_q_error", Qt::QueuedConnection,
                                      Q_ARG(int, MultimediaPlayer::ResourceError),
                                      Q_ARG(QString, QStringLiteral("Attempting to play invalid Qt resource")));
            QMetaObject::invokeMethod(this, "_q_mediaStatusChanged", Qt::QueuedConnection,
                                      Q_ARG(MultimediaPlayer::MediaStatus, MultimediaPlayer::InvalidMedia));
            file.reset();
            // Ignore the next NoMedia status change, we just want to clear the current media
            // on the backend side since we can't load the new one and we want to be in the
            // InvalidMedia status.
            ignoreNextStatusChange = MediaPlayer::NoMedia;
            control->setMedia(MediaContent());

        } else {
#if QT_CONFIG(temporaryfile)
            QTemporaryFile *tempFile = new QTemporaryFile;

            // Preserve original file extension, some backends might not load the file if it doesn't
            // have an extension.
            const QString suffix = QFileInfo(*file).suffix();
            if (!suffix.isEmpty())
                tempFile->setFileTemplate(tempFile->fileTemplate() + QLatin1Char('.') + suffix);

            // Copy the qrc data into the temporary file
            tempFile->open();
            char buffer[4096];
            while (true) {
                qint64 len = file->read(buffer, sizeof(buffer));
                if (len < 1)
                    break;
                tempFile->write(buffer, len);
            }
            tempFile->close();

            file.reset(tempFile);
            control->setMedia(MediaContent(QUrl::fromLocalFile(file->fileName())));
#else
            qWarning("Qt was built with -no-feature-temporaryfile: playback from resource file is not supported!");
#endif
        }
    } else {
        qrcMedia = MediaContent();
        control->setMedia(media);
    }

    qrcFile.swap(file); // Cleans up any previous file
}

void MediaPlayerPrivate::bindControl(MediaPlayerController *ctl)
{
    Q_Q(MediaPlayer);
    if(!ctl) {
        qWarning("Media player can not bind the null controller.");
        return;
    }
    if(control) {
        qWarning("Media player has bound the controller.");
        return;
    }

    // Initlinze
    ctl->setVolume(volume);
    ctl->setMuted(muted);
    ctl->setPlaybackRate(rate);
    ctl->setPosition(position);

    control = ctl;

    // Init media
    setMedia(curMedia);

    connect(control, SIGNAL(mediaChanged(MediaContent)), SLOT(_q_handleMediaChanged(MediaContent)));
    connect(control, SIGNAL(stateChanged(MultimediaPlayer::State)), SLOT(_q_stateChanged(MultimediaPlayer::State)));
    connect(control, SIGNAL(mediaStatusChanged(MultimediaPlayer::MediaStatus)),
            SLOT(_q_mediaStatusChanged(MultimediaPlayer::MediaStatus)));
    connect(control, SIGNAL(error(int,QString)), SLOT(_q_error(int,QString)));

    connect(control, SIGNAL(durationChanged(qint64)), q, SIGNAL(durationChanged(qint64)));
    connect(control, SIGNAL(positionChanged(qint64)), q, SIGNAL(positionChanged(qint64)));
    connect(control, SIGNAL(audioAvailableChanged(bool)), q, SIGNAL(audioAvailableChanged(bool)));
    connect(control, SIGNAL(videoAvailableChanged(bool)), q, SIGNAL(videoAvailableChanged(bool)));
    connect(control, SIGNAL(volumeChanged(int)), q, SIGNAL(volumeChanged(int)));
    connect(control, SIGNAL(mutedChanged(bool)), q, SIGNAL(mutedChanged(bool)));
    connect(control, SIGNAL(seekableChanged(bool)), q, SIGNAL(seekableChanged(bool)));
    connect(control, SIGNAL(playbackRateChanged(qreal)), q, SIGNAL(playbackRateChanged(qreal)));
    connect(control, SIGNAL(bufferStatusChanged(int)), q, SIGNAL(bufferStatusChanged(int)));

    _q_stateChanged(control->state());
    _q_mediaStatusChanged(control->mediaStatus());

}

void MediaPlayerPrivate::unbindControl(MediaPlayerController *ctl)
{
    Q_Q(MediaPlayer);
    if(!ctl) {
        qWarning("Media player can not unbind the null controller.");
    }
    if(!control) {
        qWarning("Media player has unbound the controller.");
        return;
    }

    disconnect(control, SIGNAL(mediaChanged(MediaContent)), this, SLOT(_q_handleMediaChanged(MediaContent)));
    disconnect(control, SIGNAL(stateChanged(MultimediaPlayer::State)), this, SLOT(_q_stateChanged(MultimediaPlayer::State)));
    disconnect(control, SIGNAL(mediaStatusChanged(MultimediaPlayer::MediaStatus)),
               this, SLOT(_q_mediaStatusChanged(MultimediaPlayer::MediaStatus)));
    disconnect(control, SIGNAL(error(int,QString)), this, SLOT(_q_error(int,QString)));

    disconnect(control, SIGNAL(durationChanged(qint64)), q, SIGNAL(durationChanged(qint64)));
    disconnect(control, SIGNAL(positionChanged(qint64)), q, SIGNAL(positionChanged(qint64)));
    disconnect(control, SIGNAL(audioAvailableChanged(bool)), q, SIGNAL(audioAvailableChanged(bool)));
    disconnect(control, SIGNAL(videoAvailableChanged(bool)), q, SIGNAL(videoAvailableChanged(bool)));
    disconnect(control, SIGNAL(volumeChanged(int)), q, SIGNAL(volumeChanged(int)));
    disconnect(control, SIGNAL(mutedChanged(bool)), q, SIGNAL(mutedChanged(bool)));
    disconnect(control, SIGNAL(seekableChanged(bool)), q, SIGNAL(seekableChanged(bool)));
    disconnect(control, SIGNAL(playbackRateChanged(qreal)), q, SIGNAL(playbackRateChanged(qreal)));
    disconnect(control, SIGNAL(bufferStatusChanged(int)), q, SIGNAL(bufferStatusChanged(int)));

    control->stop();
    _q_stateChanged(MultimediaPlayer::StoppedState);
    _q_mediaStatusChanged(MultimediaPlayer::UnknownMediaStatus);

    control = Q_NULLPTR;
}

void MediaPlayerPrivate::_q_stateChanged(MultimediaPlayer::State ps)
{
    Q_Q(MediaPlayer);

    // Backend switches into stopped state every time new media is about to be loaded.
    // If media player has a playlist loaded make sure player doesn' stop.
    /*
    if (playlist && playlist->currentIndex() != -1 && ps != state && ps == MultimediaPlayer::StoppedState) {
        if (control->mediaStatus() == MultimediaPlayer::EndOfMedia ||
                control->mediaStatus() == MultimediaPlayer::InvalidMedia) {
            // if media player is not stopped, and
            // we have finished playback for the current media,
            // advance to the next item in the playlist
            Q_ASSERT(state != MultimediaPlayer::StoppedState);
            playlist->next();
            return;
        } else if (control->mediaStatus() == MultimediaPlayer::LoadingMedia) {
            return;
        }
    }
    */

    if (ps != state) {
        state = ps;

        if (ps == MediaPlayer::PlayingState)
            q->addPropertyWatch("position");
        else
            q->removePropertyWatch("position");

        emit q->stateChanged(ps);
    }
}

void MediaPlayerPrivate::_q_mediaStatusChanged(MultimediaPlayer::MediaStatus s)
{
    Q_Q(MediaPlayer);

    if (int(s) == ignoreNextStatusChange) {
        ignoreNextStatusChange = -1;
        return;
    }

    if (s != status) {
        status = s;

        switch (s) {
        case MultimediaPlayer::StalledMedia:
        case MultimediaPlayer::BufferingMedia:
            q->addPropertyWatch("bufferStatus");
            break;
        default:
            q->removePropertyWatch("bufferStatus");
            break;
        }

        emit q->mediaStatusChanged(s);
    }
}

void MediaPlayerPrivate::_q_error(int error, const QString &errorString)
{
    Q_Q(MediaPlayer);

    this->error = MultimediaPlayer::Error(error);
    this->errorString = errorString;
    emit q->error(this->error);

    // TODO:
//    if (playlist)
//        playlist->next();
}

void MediaPlayerPrivate::_q_handleMediaChanged(const MediaContent &media)
{
    Q_Q(MediaPlayer);

    emit q->currentMediaChanged(qrcMedia.isNull() ? media : qrcMedia);
}
