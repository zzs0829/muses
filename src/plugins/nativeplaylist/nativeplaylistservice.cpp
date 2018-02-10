#include "nativeplaylistservice.h"
#include "nativeplaylistcontrol.h"
#include <QUuid>

Q_DECLARE_LOGGING_CATEGORY(__list__)

NativePlaylistService::NativePlaylistService(QObject *parent) :
    MultimediaService(parent)
{
}

MultimediaController* NativePlaylistService::requestController(const char *uid)
{
    if (qstrcmp(uid, MediaPlaylistController_uid) == 0)
        return 0;
    return 0;
}

/*
Example environment

HS_PLAYLIST_USB_ROOT=/media/disk
HS_PLAYLIST_MUSIC_NAME_FILTERS=*.mp3,*.aac,*.wma,*.flac,*.mp2,*.m4a,*.wav,*.ac3,*.ogg,*.ape,*.amr
HS_PLAYLIST_VIDEO_NAME_FILTERS=*.avi,*.mp4,*.3gp,*.mov,*.flv,*.vob,*.mkv,*.asf,*.rmvb,*.rm,*.divx,*.wmv,*.mpg,*.mpeg,*.m4v,*.ts
HS_PLAYLIST_PICTURE_NAME_FILTERS=*.jpg,*.png
HS_PLAYLIST_TEXT_NAME_FILTERS=*.txt,*.log

*/
MultimediaSession* NativePlaylistService::requestSession(const QString &key)
{
    NativePlaylistSession *session = m_sessions.value(key, Q_NULLPTR);
    if(!session) {
        QString id = QUuid::createUuid().toString();


        QByteArray envUsbRoot = qgetenv("HS_PLAYLIST_USB_ROOT");
        QByteArray envFilters;
        Q_ASSERT_X(!envUsbRoot.isEmpty(), "create NativePlaylistServicePlugin",
                   "The environment [ HS_PLAYLIST_USB_ROOT ] is not seted.");
        if(envUsbRoot.isEmpty())
            qWarning("The environment [ HS_PLAYLIST_USB_ROOT ] is not seted.");

        if (key == QLatin1String("USB_MUSIC")) {
            envFilters = qgetenv("HS_PLAYLIST_MUSIC_NAME_FILTERS");
            Q_ASSERT_X(!envFilters.isEmpty(), "create NativePlaylistServicePlugin",
                       "The environment [ HS_PLAYLIST_MUSIC_NAME_FILTERS ] is not seted.");
            if(envFilters.isEmpty())
                qWarning("The environment [ HS_PLAYLIST_MUSIC_NAME_FILTERS ] is not seted.");
        } else if (key == QLatin1String("USB_VIDEO")) {
            envFilters = qgetenv("HS_PLAYLIST_VIDEO_NAME_FILTERS");
            Q_ASSERT_X(!envFilters.isEmpty(), "create NativePlaylistServicePlugin",
                       "The environment [ HS_PLAYLIST_VIDEO_NAME_FILTERS ] is not seted.");
            if(envFilters.isEmpty())
                qWarning("The environment [ HS_PLAYLIST_VIDEO_NAME_FILTERS ] is not seted.");
        }else if (key == QLatin1String("USB_PICTURE")) {
            envFilters = qgetenv("HS_PLAYLIST_PICTURE_NAME_FILTERS");
            Q_ASSERT_X(!envFilters.isEmpty(), "create NativePlaylistServicePlugin",
                       "The environment [ HS_PLAYLIST_PICTURE_NAME_FILTERS ] is not seted.");
            if(envFilters.isEmpty())
                qWarning("The environment [ HS_PLAYLIST_PICTURE_NAME_FILTERS ] is not seted.");
        }else if (key == QLatin1String("USB_TEXT")) {
            envFilters = qgetenv("HS_PLAYLIST_TEXT_NAME_FILTERS");
            Q_ASSERT_X(!envFilters.isEmpty(), "create NativePlaylistServicePlugin",
                       "The environment [ HS_PLAYLIST_TEXT_NAME_FILTERS ] is not seted.");
            if(envFilters.isEmpty())
                qWarning("The environment [ HS_PLAYLIST_TEXT_NAME_FILTERS ] is not seted.");
        }

        session = new NativePlaylistSession(id, key, envUsbRoot, envFilters, this);
        m_sessions.insert(key, session);

        qCInfo(__list__, "[ Session: %s ] created.(%s)",
               qPrintable(session->sessionKey()),
               qPrintable(session->sessionId()));
    }else{
        qCInfo(__list__, "[ Session: %s ] requested.(%s)",
               qPrintable(session->sessionKey()),
               qPrintable(session->sessionId()));
    }

    // Avaiable
    availableSession(session);

    return session;
}

void NativePlaylistService::releaseSession(MultimediaSession *session)
{
    NativePlaylistSession *listSession = qobject_cast<NativePlaylistSession *>(session);
    if(!listSession) {
        qCWarning(__list__, "[ Session ] is null to release.");
        return;
    }
    qCInfo(__list__, "[ Session: %s ] released.(%s)",
           qPrintable(session->sessionKey()),
           qPrintable(session->sessionId()));

    m_sessions.remove(m_sessions.key(listSession));
    listSession->deleteLater();
}

void NativePlaylistService::availableSession(MultimediaSession *session)
{
    NativePlaylistSession *listSession = qobject_cast<NativePlaylistSession *>(session);
    if(!listSession) {
        qCWarning(__list__, "[ Session ] is null to set available.");
        return;
    }
    qCInfo(__list__, "[ Session ] is available by default.");
}
