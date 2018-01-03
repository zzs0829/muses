#include "multimediapluginloader.h"
#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>
#include <QtCore/qjsonarray.h>
#include <private/qfactoryloader_p.h>


HS_BEGIN_NAMESPACE

MultimediaPluginLoader::MultimediaPluginLoader(const char *iid,
                                               const QString &location,
                                               Qt::CaseSensitivity caseSensitivity):
    m_iid(iid)
{
    m_location = QString::fromLatin1("/%1").arg(location);
    m_factoryLoader = new QFactoryLoader(m_iid, m_location, caseSensitivity);
    loadMetadata();
}

MultimediaPluginLoader::~MultimediaPluginLoader()
{
    delete m_factoryLoader;
}

QStringList MultimediaPluginLoader::keys() const
{
    return m_metadata.keys();
}

QObject* MultimediaPluginLoader::instance(QString const &key)
{
    if (!m_metadata.contains(key))
        return 0;

    int idx = m_metadata.value(key).first().value(QStringLiteral("index")).toDouble();
    if (idx < 0)
        return 0;

    return m_factoryLoader->instance(idx);
}

QList<QObject*> MultimediaPluginLoader::instances(QString const &key)
{
    if (!m_metadata.contains(key))
        return QList<QObject*>();

    QList<QObject *> objects;
    const auto list = m_metadata.value(key);
    for (const QJsonObject &jsonobj : list) {
        int idx = jsonobj.value(QStringLiteral("index")).toDouble();
        if (idx < 0)
            continue;

        QObject *object = m_factoryLoader->instance(idx);
        if (!objects.contains(object)) {
            objects.append(object);
        }
    }

    return objects;
}

void MultimediaPluginLoader::loadMetadata()
{
#if !defined QT_NO_DEBUG
    const bool showDebug = qgetenv("QT_DEBUG_PLUGINS").toInt() > 0;
#endif

#if !defined QT_NO_DEBUG
        if (showDebug)
            qDebug() << "MultimediaPluginLoader: loading metadata for iid " << m_iid << " at location " << m_location;
#endif

    if (!m_metadata.isEmpty()) {
#if !defined QT_NO_DEBUG
        if (showDebug)
            qDebug() << "MultimediaPluginLoader: already loaded metadata, returning";
#endif
        return;
    }

    QList<QJsonObject> meta = m_factoryLoader->metaData();
    for (int i = 0; i < meta.size(); i++) {
        QJsonObject jsonobj = meta.at(i).value(QStringLiteral("MetaData")).toObject();
        jsonobj.insert(QStringLiteral("index"), i);
#if !defined QT_NO_DEBUG
        if (showDebug)
            qDebug() << "MultimediaPluginLoader: Inserted index " << i << " into metadata: " << jsonobj;
#endif

        QJsonArray arr = jsonobj.value(QStringLiteral("Services")).toArray();
        // Preserve compatibility with older plugins (made before 5.1) in which
        // services were declared in the 'Keys' property
        if (arr.isEmpty())
            arr = jsonobj.value(QStringLiteral("Keys")).toArray();

        for (const QJsonValue &value : qAsConst(arr)) {
            QString key = value.toString();

            if (!m_metadata.contains(key)) {
#if !defined QT_NO_DEBUG
                if (showDebug)
                    qDebug() << "MultimediaPluginLoader: Inserting new list for key: " << key;
#endif
                m_metadata.insert(key, QList<QJsonObject>());
            }

            m_metadata[key].append(jsonobj);
        }
    }
}

HS_END_NAMESPACE
