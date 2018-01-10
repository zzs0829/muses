#include "nativeplaylistservice.h"
#include "nativeplaylistcontrol.h"

NativePlaylistService::NativePlaylistService(const QString &path,
                                             const QString &filters,
                                             QObject *parent) :
    MultimediaService(parent),
    m_control(new NativePlaylistControl(path, filters, this))
{
}

MultimediaController* NativePlaylistService::requestController(const char *uid)
{
    if (qstrcmp(uid, MediaPlaylistController_uid) == 0)
        return m_control;
    return 0;
}

MultimediaSession* NativePlaylistService::requestSession(const QString &key)
{
    Q_UNUSED(key);
    return 0;
}

void NativePlaylistService::releaseSession(MultimediaSession *session)
{
    Q_UNUSED(session);
}

void NativePlaylistService::availableSession(MultimediaSession *session)
{
    Q_UNUSED(session);
}
