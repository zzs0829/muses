#include "multimedia.h"
#include <QtQml/qqmlextensionplugin.h>

Multimedia::Multimedia()
{

}



class MultimediaQuickModule : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    MultimediaQuickModule(QObject *parent = 0) : QQmlExtensionPlugin(parent) { }
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("MUS.Multimedia"));

        // 1.0 types
//        qmlRegisterType<QDeclarativeVideoOutput>(uri, 1, 0, "VideoOutput");

    }

    void initializeEngine(QQmlEngine *engine, const char *uri) override
    {
        Q_UNUSED(uri);
        Q_UNUSED(engine);
    }
};
