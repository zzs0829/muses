#include "nativeplaylistsession.h"
#include "nativeplaylistcontrol.h"
#include <Multimedia/private/mediaplaylistsession_p.h>

class NativePlaylistSessionPrivate : public MediaPlaylistSessionPrivate
{
public:
    NativePlaylistSessionPrivate(const QString &id, const QString &key):
        MediaPlaylistSessionPrivate(id, key)
    { }

    NativePlaylistControl *control;
};

NativePlaylistSession::NativePlaylistSession(const QString &id, const QString &key,
                                             const QString &path,
                                             const QString &filters,
                                             QObject *parent) :
    MediaPlaylistSession(* new MediaPlaylistSessionPrivate(id, key), parent)
{
    Q_D(NativePlaylistSession);
    d->control = new NativePlaylistControl(path, filters, this);

    if(MediaPlaylistProvider* provider = d->control->playlistProvider()) {
        connect(provider, SIGNAL(mediaChanged(int,int)), SIGNAL(mediaChanged(int,int)));
    }
}

int NativePlaylistSession::mediaCount() const
{
    Q_D(const NativePlaylistSession);
    if(MediaPlaylistProvider* provider = d->control->playlistProvider()) {
        return provider->mediaCount();
    }else{
        return 0;
    }
}

MediaContent NativePlaylistSession::media(int index) const
{
    Q_D(const NativePlaylistSession);
    if(MediaPlaylistProvider* provider = d->control->playlistProvider()) {
        return provider->media(index);
    }else{
        return MediaContent();
    }
}

int NativePlaylistSession::currentIndex() const
{
    Q_D(const NativePlaylistSession);
    return d->control->currentIndex();
}

void NativePlaylistSession::setCurrentIndex(int position)
{
    Q_D(NativePlaylistSession);
    d->control->setCurrentIndex(position);
}

int NativePlaylistSession::nextIndex(int steps) const
{
    Q_D(const NativePlaylistSession);
    return d->control->nextIndex(steps);
}

int NativePlaylistSession::previousIndex(int steps) const
{
    Q_D(const NativePlaylistSession);
    return d->control->previousIndex(steps);
}

void NativePlaylistSession::next()
{
    Q_D(NativePlaylistSession);
    d->control->next();
}

void NativePlaylistSession::previous()
{
    Q_D(NativePlaylistSession);
    d->control->previous();
}

MediaPlaylist::PlaybackMode NativePlaylistSession::playbackMode() const
{
    Q_D(const NativePlaylistSession);
    return d->control->playbackMode();
}

void NativePlaylistSession::setPlaybackMode(MediaPlaylist::PlaybackMode mode)
{
    Q_D(NativePlaylistSession);
    d->control->setPlaybackMode(mode);
}
