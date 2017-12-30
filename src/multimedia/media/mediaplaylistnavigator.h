#ifndef MEDIAPLAYLISTNAVIGATOR_H
#define MEDIAPLAYLISTNAVIGATOR_H

#include <QObject>
#include <Multimedia/multimediaglobal.h>
#include <Multimedia/mediacontent.h>
#include <Multimedia/mediaplaylist.h>
#include <Multimedia/mediaplaylistprovider.h>

HS_BEGIN_NAMESPACE

class MediaPlaylistNavigatorPrivate;
class MULTIMEDIA_EXPORT MediaPlaylistNavigator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MediaPlaylist::PlaybackMode playbackMode READ playbackMode WRITE setPlaybackMode NOTIFY playbackModeChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE jump NOTIFY currentIndexChanged)
    Q_PROPERTY(MediaContent currentItem READ currentItem)

public:
    MediaPlaylistNavigator(MediaPlaylistProvider *playlist, QObject *parent = 0);
    virtual ~MediaPlaylistNavigator();

    MediaPlaylistProvider *playlist() const;
    void setPlaylist(MediaPlaylistProvider *playlist);

    MediaPlaylist::PlaybackMode playbackMode() const;

    MediaContent currentItem() const;
    MediaContent nextItem(int steps = 1) const;
    MediaContent previousItem(int steps = 1) const;

    virtual MediaContent itemAt(int position) const;
    virtual int mediaCount() const;

    int currentIndex() const;
    virtual int nextIndex(int steps = 1) const;
    virtual int previousIndex(int steps = 1) const;

public Q_SLOTS:
    void next();
    void previous();

    void jump(int);

    void setPlaybackMode(MediaPlaylist::PlaybackMode mode);

Q_SIGNALS:
    void activated(const MediaContent &content);
    void currentIndexChanged(int);
    void playbackModeChanged(MediaPlaylist::PlaybackMode mode);

    void surroundingItemsChanged();

protected:
    MediaPlaylistNavigatorPrivate *d_ptr;

private:
    Q_DISABLE_COPY(MediaPlaylistNavigator)
    Q_DECLARE_PRIVATE(MediaPlaylistNavigator)

    Q_PRIVATE_SLOT(d_func(), void _q_mediaInserted(int start, int end))
    Q_PRIVATE_SLOT(d_func(), void _q_mediaRemoved(int start, int end))
    Q_PRIVATE_SLOT(d_func(), void _q_mediaChanged(int start, int end))
};

HS_END_NAMESPACE

#endif // MEDIAPLAYLISTNAVIGATOR_H
