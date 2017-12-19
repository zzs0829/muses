#include "nativeplaylistservice.h"

NativePlaylistService::NativePlaylistService(QObject *parent) :
    MultimediaService(parent)
{

}

MultimediaController* NativePlaylistService::requestController(const char *uid)
{
    if (qstrcmp(uid, "MediaPlayerController_uid") == 0)
        return m_control;
    return 0;
}
