#include "nativeplaylistengine.h"

NativePlaylistEngine::NativePlaylistEngine(QObject *parent) :
    MultimediaEngine(parent)
{

}

void NativePlaylistEngine::setRootPath(const QString &path)
{
    if(path == m_rootPath)
        return;

    m_rootPath = path;
}
