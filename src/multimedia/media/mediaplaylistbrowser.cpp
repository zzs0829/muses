#include "mediaplaylistbrowser.h"

HS_BEGIN_NAMESPACE

MediaPlaylistBrowser::MediaPlaylistBrowser(QObject *parent)
    :QObject(parent), d_ptr(new MediaPlaylistBrowserPrivate)
{
}

MediaPlaylistBrowser::MediaPlaylistBrowser(MediaPlaylistBrowserPrivate &dd, QObject *parent)
    :QObject(parent), d_ptr(&dd)
{
}

MediaPlaylistBrowser::~MediaPlaylistBrowser()
{
}


HS_END_NAMESPACE
