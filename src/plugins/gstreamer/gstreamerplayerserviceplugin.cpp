#include "gstreamerplayerserviceplugin.h"
#include "gstreamerplayerservice.h"
#include "gstutils.h"

MultimediaService* GstreamerPlayerServicePlugin::create(const QString &key)
{
    GstUtils::initializeGst();

    if (key == QLatin1String(MEDIASERVICE_MEDIAPLAYER))
        return new GstreamerPlayerService;

    qWarning() << "Gstreamer service plugin: unsupported key:" << key;
    return 0;
}

void GstreamerPlayerServicePlugin::release(MultimediaService *service)
{
    delete service;
}
