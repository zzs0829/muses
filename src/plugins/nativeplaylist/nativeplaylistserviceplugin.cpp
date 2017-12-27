#include "nativeplaylistserviceplugin.h"
#include "nativeplaylistservice.h"

/*

HS_PLAYLIST_USB_ROOT=/media/disk
HS_PLAYLIST_MUSIC_NAME_FILTERS=

*/


MultimediaService* NativePlaylistServicePlugin::create(const QString &key)
{
    QByteArray envUsbRoot = qgetenv("HS_PLAYLIST_USB_ROOT");
    Q_ASSERT_X(!envUsbRoot.isEmpty(), "create NativePlaylistServicePlugin",
               "The environment [ HS_PLAYLIST_USB_ROOT ] is not seted.");

    if (key == QLatin1String("USB_MUSIC")) {
        QByteArray envFilters = qgetenv("HS_PLAYLIST_MUSIC_NAME_FILTERS");
        Q_ASSERT_X(!envFilters.isEmpty(), "create NativePlaylistServicePlugin",
                   "The environment [ HS_PLAYLIST_MUSIC_NAME_FILTERS ] is not seted.");

        return new NativePlaylistService(envUsbRoot, envFilters, this);
    } else if (key == QLatin1String("USB_VIDEO")) {
        QByteArray envFilters = qgetenv("HS_PLAYLIST_VIDEO_NAME_FILTERS");
        Q_ASSERT_X(!envFilters.isEmpty(), "create NativePlaylistServicePlugin",
                   "The environment [ HS_PLAYLIST_VIDEO_NAME_FILTERS ] is not seted.");

        return new NativePlaylistService(envUsbRoot, envFilters, this);

    }else if (key == QLatin1String("USB_PICTURE")) {
        QByteArray envFilters = qgetenv("HS_PLAYLIST_PICTURE_NAME_FILTERS");
        Q_ASSERT_X(!envFilters.isEmpty(), "create NativePlaylistServicePlugin",
                   "The environment [ HS_PLAYLIST_PICTURE_NAME_FILTERS ] is not seted.");

        return new NativePlaylistService(envUsbRoot, envFilters, this);

    }else if (key == QLatin1String("USB_TEXT")) {
        QByteArray envFilters = qgetenv("HS_PLAYLIST_TEXT_NAME_FILTERS");
        Q_ASSERT_X(!envFilters.isEmpty(), "create NativePlaylistServicePlugin",
                   "The environment [ HS_PLAYLIST_TEXT_NAME_FILTERS ] is not seted.");

        return new NativePlaylistService(envUsbRoot, envFilters, this);
    }

    Q_ASSERT_X(false, "create NativePlaylistServicePlugin", "The key is not a supported.");
    return 0;
}

void NativePlaylistServicePlugin::release(MultimediaService *service)
{
    delete service;
}
