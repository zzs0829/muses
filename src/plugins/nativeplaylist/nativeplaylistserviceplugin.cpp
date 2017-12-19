#include "nativeplaylistserviceplugin.h"

MultimediaService* NativePlaylistServicePlugin::create(const QString &key)
{
    if (key == QLatin1String(MEDIASERVICE_PLAYLIST))
        return 0;

    return 0;
}

void NativePlaylistServicePlugin::release(MultimediaService *service)
{
    delete service;
}
