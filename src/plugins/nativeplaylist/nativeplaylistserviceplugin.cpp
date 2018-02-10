#include "nativeplaylistserviceplugin.h"
#include "nativeplaylistservice.h"



MultimediaService* NativePlaylistServicePlugin::create(const QString &key)
{
    if (key == QLatin1String(MEDIASERVICE_PLAYLIST))
        return new NativePlaylistService;

    qWarning() << "NativePlaylist service plugin: unsupported key:" << key;
    return 0;
}

void NativePlaylistServicePlugin::release(MultimediaService *service)
{
    delete service;
}
