#ifndef MEDIAPLAYLISTSESSION_H
#define MEDIAPLAYLISTSESSION_H


#include <Multimedia/multimediasession.h>
#include <Multimedia/mediaplaylistprovider.h>
#include <Multimedia/mediaplaylist.h>
#include <Multimedia/mediacontent.h>

HS_BEGIN_NAMESPACE

class MediaPlaylistSessionPrivate;
class MULTIMEDIA_EXPORT MediaPlaylistSession : public MultimediaSession
{
    Q_OBJECT
protected:
    explicit MediaPlaylistSession(const QString &id, const QString &key, QObject *parent = Q_NULLPTR);
    MediaPlaylistSession(MediaPlaylistSessionPrivate &dd, QObject *parent);

public:
    // for PlaylistModel
    virtual int mediaCount() const = 0;
    // for PlaylistModel
    virtual MediaContent media(int index) const = 0;

    virtual int currentIndex() const = 0;
    virtual void setCurrentIndex(int position) = 0;
    virtual int nextIndex(int steps) const = 0;
    virtual int previousIndex(int steps) const = 0;

    virtual void next() = 0;
    virtual void previous() = 0;

    virtual MediaPlaylist::PlaybackMode playbackMode() const = 0;
    virtual void setPlaybackMode(MediaPlaylist::PlaybackMode mode) = 0;

signals:
    void playlistProviderChanged();
    void currentIndexChanged(int position);
    void currentMediaChanged(const MediaContent&);
    void playbackModeChanged(MediaPlaylist::PlaybackMode mode);

    // for PlaylistModel
    void mediaChanged(int startIndex, int endIndex);

private:
    Q_DISABLE_COPY(MediaPlaylistSession)
    Q_DECLARE_PRIVATE(MediaPlaylistSession)
};

HS_END_NAMESPACE

#endif // MEDIAPLAYLISTSESSION_H
