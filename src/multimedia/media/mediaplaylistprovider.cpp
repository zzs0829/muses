#include "mediaplaylistprovider.h"


MediaPlaylistProvider::MediaPlaylistProvider(QObject *parent)
    :QObject(parent), d_ptr(new MediaPlaylistProviderPrivate)
{
}

MediaPlaylistProvider::MediaPlaylistProvider(MediaPlaylistProviderPrivate &dd, QObject *parent)
    :QObject(parent), d_ptr(&dd)
{
}

MediaPlaylistProvider::~MediaPlaylistProvider()
{
    delete d_ptr;
}

bool MediaPlaylistProvider::load(const QNetworkRequest &request, const char *format)
{
    Q_UNUSED(request);
    Q_UNUSED(format);
    return false;
}

bool MediaPlaylistProvider::load(QIODevice * device, const char *format)
{
    Q_UNUSED(device);
    Q_UNUSED(format);
    return false;
}

bool MediaPlaylistProvider::save(const QUrl &location, const char *format)
{
    Q_UNUSED(location);
    Q_UNUSED(format);
    if(isReadOnly())
        qWarning() << "The playlist provider is readonly and cannot save location by format.";
    else
        qWarning() << "The playlist provider has not implement interface save(location,format).";
    return false;
}

bool MediaPlaylistProvider::save(QIODevice * device, const char *format)
{
    Q_UNUSED(device);
    Q_UNUSED(format);
    if(isReadOnly())
        qWarning() << "The playlist provider is readonly and cannot save device by format.";
    else
        qWarning() << "The playlist provider has not implement interface save(device,format).";
    return false;
}

bool MediaPlaylistProvider::isReadOnly() const
{
    return true;
}

bool MediaPlaylistProvider::addMedia(const MediaContent &media)
{
    Q_UNUSED(media);
    if(isReadOnly())
        qWarning() << "The playlist provider is readonly and cannot add media.";
    else
        qWarning() << "The playlist provider has not implement interface addMedia.";
    return false;
}

bool MediaPlaylistProvider::addMedia(const QList<MediaContent> &items)
{
    if(isReadOnly()) {
        qWarning() << "The playlist provider is readonly and cannot add medias.";
        return false;
    }else{
        for (const MediaContent &item : items) {
            if (!addMedia(item))
                return false;
        }
    }
    return true;
}

bool MediaPlaylistProvider::insertMedia(int position, const MediaContent &media)
{
    Q_UNUSED(position);
    Q_UNUSED(media);
    if(isReadOnly())
        qWarning() << "The playlist provider is readonly and cannot insert media.";
    else
        qWarning() << "The playlist provider has not implement interface insertMedia.";
    return false;
}

bool MediaPlaylistProvider::insertMedia(int position, const QList<MediaContent> &items)
{
    if(isReadOnly()) {
        qWarning() << "The playlist provider is readonly and cannot insert medias.";
        return false;
    }else{
        for (int i=0; i<items.count(); i++) {
            if (!insertMedia(position+i,items.at(i)))
                return false;
        }
    }

    return true;
}

bool MediaPlaylistProvider::moveMedia(int from, int to)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    if(isReadOnly())
        qWarning() << "The playlist provider is readonly and cannot move media.";
    else
        qWarning() << "The playlist provider has not implement interface moveMedia.";

    return false;
}

bool MediaPlaylistProvider::removeMedia(int position)
{
    Q_UNUSED(position);
    if(isReadOnly())
        qWarning() << "The playlist provider is readonly and cannot remove media.";
    else
        qWarning() << "The playlist provider has not implement interface removeMedia.";
    return false;
}

bool MediaPlaylistProvider::removeMedia(int start, int end)
{
    if(isReadOnly()) {
        qWarning() << "The playlist provider is readonly and cannot remove medias.";
        return false;
    }else{
        for (int pos=start; pos<=end; pos++) {
            if (!removeMedia(pos))
                return false;
        }
    }

    return true;
}

bool MediaPlaylistProvider::clear()
{
    return removeMedia(0, mediaCount()-1);
}

void MediaPlaylistProvider::shuffle()
{
}
