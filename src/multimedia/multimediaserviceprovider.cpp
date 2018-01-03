#include "multimediaserviceprovider.h"
#include "multimediaserviceproviderplugin.h"
#include "multimediapluginloader.h"

Q_GLOBAL_STATIC_WITH_ARGS(MultimediaPluginLoader, loader,
        (MultimediaServiceProviderInterface_iid, QLatin1String("mediaservice"), Qt::CaseInsensitive))

class PluginServiceProvider : public MultimediaServiceProvider
{
    struct MediaServiceData {
        QByteArray type;
        MultimediaServiceProviderPlugin *plugin;
        MediaServiceData() : plugin(0) { }
    };

    QMap<const MultimediaService*, MediaServiceData> mediaServiceData;
public:
    MultimediaService* requestService(const QByteArray &type) override
    {
        QString key(QLatin1String(type.constData()));

        QList<MultimediaServiceProviderPlugin *>plugins;
        const auto instances = loader()->instances(key);
        for (QObject *obj : instances) {
            MultimediaServiceProviderPlugin *plugin =
                qobject_cast<MultimediaServiceProviderPlugin*>(obj);
            if (plugin)
                plugins << plugin;
        }

        if (!plugins.isEmpty()) {
            MultimediaServiceProviderPlugin *plugin = 0;

            if (plugin != 0) {
                MultimediaService *service = plugin->create(key);
                if (service != 0) {
                    MediaServiceData d;
                    d.type = type;
                    d.plugin = plugin;
                    mediaServiceData.insert(service, d);
                }

                return service;
            }
        }

        qWarning() << "defaultServiceProvider::requestService(): no service found for -" << key;
        return 0;
    }

    void releaseService(MultimediaService *service) override
    {
        if (service != 0) {
            MediaServiceData d = mediaServiceData.take(service);

            if (d.plugin != 0)
                d.plugin->release(service);
        }
    }

};


Q_GLOBAL_STATIC(PluginServiceProvider, pluginProvider)

static MultimediaServiceProvider *qt_defaultMediaServiceProvider = 0;

void MultimediaServiceProvider::setDefaultServiceProvider(MultimediaServiceProvider *provider)
{
    qt_defaultMediaServiceProvider = provider;
}

MultimediaServiceProvider *MultimediaServiceProvider::defaultServiceProvider()
{
    return qt_defaultMediaServiceProvider != 0
            ? qt_defaultMediaServiceProvider
            : static_cast<MultimediaServiceProvider *>(pluginProvider());
}
