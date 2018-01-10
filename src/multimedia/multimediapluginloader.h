#ifndef MULTIMEDIAPLUGINLOADER_H
#define MULTIMEDIAPLUGINLOADER_H

#include <Multimedia/multimediaglobal.h>
#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qmap.h>
#include <QtCore/qjsonobject.h>

HS_BEGIN_NAMESPACE

class QFactoryLoader;
class MULTIMEDIA_EXPORT MultimediaPluginLoader
{
public:
    MultimediaPluginLoader(const char *iid,
                   const QString &suffix = QString(),
                   Qt::CaseSensitivity = Qt::CaseSensitive);
    virtual ~MultimediaPluginLoader();

    QStringList keys() const;
    QObject* instance(QString const &key);
    QList<QObject*> instances(QString const &key);

private:
    void loadMetadata();

    QByteArray  m_iid;
    QString     m_location;
    QMap<QString, QList<QJsonObject> > m_metadata;

    QFactoryLoader *m_factoryLoader;
};

HS_END_NAMESPACE

#endif // MULTIMEDIAPLUGINLOADER_H
