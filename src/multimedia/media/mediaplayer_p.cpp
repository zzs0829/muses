#include "mediaplayer_p.h"
#include <QtCore/qtemporaryfile.h>

MediaPlayerPrivate::MediaPlayerPrivate(MediaPlayer *parent) :
    QObject(parent),
    q_ptr(parent),
    playlist(0),
    state(MultimediaPlayer::StoppedState),
    status(MultimediaPlayer::UnknownMediaStatus),
    error(MultimediaPlayer::NoError),
    ignoreNextStatusChange(-1)
{

}

void MediaPlayerPrivate::setMedia(const MediaContent &media)
{
    Q_Q(MediaPlayer);

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
            emit requestSetMedia(MediaContent());

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
            emit requestSetMedia(MediaContent(QUrl::fromLocalFile(file->fileName())));
#else
            qWarning("Qt was built with -no-feature-temporaryfile: playback from resource file is not supported!");
#endif
        }
    } else {
        qrcMedia = MediaContent();
        emit requestSetMedia(media);
    }

    qrcFile.swap(file); // Cleans up any previous file
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
