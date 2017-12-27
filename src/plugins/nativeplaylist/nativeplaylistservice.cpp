#include "nativeplaylistservice.h"

NativePlaylistService::NativePlaylistService(const QString &path,
                                             const QString &nameFilters,
                                             QObject *parent) :
    MultimediaService(parent),
    m_rootPath(path)
{
    formatNameFilters(nameFilters.split(',', QString::SkipEmptyParts));
}

MultimediaController* NativePlaylistService::requestController(const char *uid)
{
    if (qstrcmp(uid, "MediaPlayerController_uid") == 0)
        return m_control;
    return 0;
}

void NativePlaylistService::formatNameFilters(const QStringList &filters)
{
    m_nameFilters.clear();
    foreach (const QString &name, filters) {
        if(name.startsWith(QLatin1String("*."))) {
            m_nameFilters.append(name);
        }else if(name.startsWith(".")) {
            m_nameFilters.append("*" + name);
        }else if(!name.contains(QRegExp("\\W+"))){
            m_nameFilters.append("*." + name);
        }else{
            m_nameFilters.append(name);
        }
    }
}
