#include "mediaplaylistnavigator.h"

#include <QtCore/qdebug.h>


class MediaPlaylistNullProvider : public MediaPlaylistProvider
{
public:
    MediaPlaylistNullProvider() :MediaPlaylistProvider() {}
    ~MediaPlaylistNullProvider() {}
    int mediaCount() const override {return 0;}
    MediaContent media(int) const override { return MediaContent(); }
};

Q_GLOBAL_STATIC(MediaPlaylistNullProvider, _q_nullMediaPlaylist)

class MediaPlaylistNavigatorPrivate
{
    Q_DECLARE_PUBLIC(MediaPlaylistNavigator)
public:
    MediaPlaylistNavigatorPrivate()
        :playlist(0),
        currentPos(-1),
        lastValidPos(-1),
        playbackMode(MediaPlaylist::Sequential),
        randomPositionsOffset(-1)
    {
    }

    MediaPlaylistProvider *playlist;
    int currentPos;
    int lastValidPos; //to be used with CurrentItemOnce playback mode
    MediaPlaylist::PlaybackMode playbackMode;
    MediaContent currentItem;

    mutable QList<int> randomModePositions;
    mutable int randomPositionsOffset;

    int nextItemPos(int steps = 1) const;
    int previousItemPos(int steps = 1) const;

    void _q_mediaInserted(int start, int end);
    void _q_mediaRemoved(int start, int end);
    void _q_mediaChanged(int start, int end);

    MediaPlaylistNavigator *q_ptr;
};


int MediaPlaylistNavigatorPrivate::nextItemPos(int steps) const
{
    if (playlist->mediaCount() == 0)
        return -1;

    if (steps == 0)
        return currentPos;

    switch (playbackMode) {
        case MediaPlaylist::CurrentItemOnce:
            return /*currentPos == -1 ? lastValidPos :*/ -1;
        case MediaPlaylist::CurrentItemInLoop:
            return currentPos;
        case MediaPlaylist::Sequential:
            {
                int nextPos = currentPos+steps;
                return nextPos < playlist->mediaCount() ? nextPos : -1;
            }
        case MediaPlaylist::Loop:
            return (currentPos+steps) % playlist->mediaCount();
        case MediaPlaylist::Random:
            {
                //TODO: limit the history size

                if (randomPositionsOffset == -1) {
                    randomModePositions.clear();
                    randomModePositions.append(currentPos);
                    randomPositionsOffset = 0;
                }

                while (randomModePositions.size() < randomPositionsOffset+steps+1)
                    randomModePositions.append(-1);
                int res = randomModePositions[randomPositionsOffset+steps];
                if (res<0 || res >= playlist->mediaCount()) {
                    res = qrand() % playlist->mediaCount();
                    randomModePositions[randomPositionsOffset+steps] = res;
                }

                return res;
            }
    }

    return -1;
}

int MediaPlaylistNavigatorPrivate::previousItemPos(int steps) const
{
    if (playlist->mediaCount() == 0)
        return -1;

    if (steps == 0)
        return currentPos;

    switch (playbackMode) {
        case MediaPlaylist::CurrentItemOnce:
            return /*currentPos == -1 ? lastValidPos :*/ -1;
        case MediaPlaylist::CurrentItemInLoop:
            return currentPos;
        case MediaPlaylist::Sequential:
            {
                int prevPos = currentPos == -1 ? playlist->mediaCount() - steps : currentPos - steps;
                return prevPos>=0 ? prevPos : -1;
            }
        case MediaPlaylist::Loop:
            {
                int prevPos = currentPos - steps;
                while (prevPos<0)
                    prevPos += playlist->mediaCount();
                return prevPos;
            }
        case MediaPlaylist::Random:
            {
                //TODO: limit the history size

                if (randomPositionsOffset == -1) {
                    randomModePositions.clear();
                    randomModePositions.append(currentPos);
                    randomPositionsOffset = 0;
                }

                while (randomPositionsOffset-steps < 0) {
                    randomModePositions.prepend(-1);
                    randomPositionsOffset++;
                }

                int res = randomModePositions[randomPositionsOffset-steps];
                if (res<0 || res >= playlist->mediaCount()) {
                    res = qrand() % playlist->mediaCount();
                    randomModePositions[randomPositionsOffset-steps] = res;
                }

                return res;
            }
    }

    return -1;
}

MediaPlaylistNavigator::MediaPlaylistNavigator(MediaPlaylistProvider *playlist, QObject *parent)
    : QObject(parent)
    , d_ptr(new MediaPlaylistNavigatorPrivate)
{
    d_ptr->q_ptr = this;

    setPlaylist(playlist ? playlist : _q_nullMediaPlaylist());
}

MediaPlaylistNavigator::~MediaPlaylistNavigator()
{
    delete d_ptr;
}

MediaPlaylist::PlaybackMode MediaPlaylistNavigator::playbackMode() const
{
    return d_func()->playbackMode;
}

void MediaPlaylistNavigator::setPlaybackMode(MediaPlaylist::PlaybackMode mode)
{
    Q_D(MediaPlaylistNavigator);
    if (d->playbackMode == mode)
        return;

    if (mode == MediaPlaylist::Random) {
        d->randomPositionsOffset = 0;
        d->randomModePositions.append(d->currentPos);
    } else if (d->playbackMode == MediaPlaylist::Random) {
        d->randomPositionsOffset = -1;
        d->randomModePositions.clear();
    }

    d->playbackMode = mode;

    emit playbackModeChanged(mode);
    emit surroundingItemsChanged();
}

MediaPlaylistProvider *MediaPlaylistNavigator::playlist() const
{
    return d_func()->playlist;
}

void MediaPlaylistNavigator::setPlaylist(MediaPlaylistProvider *playlist)
{
    Q_D(MediaPlaylistNavigator);

    if (d->playlist == playlist)
        return;

    if (d->playlist) {
        d->playlist->disconnect(this);
    }

    if (playlist) {
        d->playlist = playlist;
    } else {
        //assign to shared readonly null playlist
        d->playlist = _q_nullMediaPlaylist();
    }

    connect(d->playlist, SIGNAL(mediaInserted(int,int)), SLOT(_q_mediaInserted(int,int)));
    connect(d->playlist, SIGNAL(mediaRemoved(int,int)), SLOT(_q_mediaRemoved(int,int)));
    connect(d->playlist, SIGNAL(mediaChanged(int,int)), SLOT(_q_mediaChanged(int,int)));

    d->randomPositionsOffset = -1;
    d->randomModePositions.clear();

    if (d->currentPos != -1) {
        d->currentPos = -1;
        emit currentIndexChanged(-1);
    }

    if (!d->currentItem.isNull()) {
        d->currentItem = MediaContent();
        emit activated(d->currentItem); //stop playback
    }
}

MediaContent MediaPlaylistNavigator::currentItem() const
{
    return itemAt(d_func()->currentPos);
}

MediaContent MediaPlaylistNavigator::nextItem(int steps) const
{
    return itemAt(nextIndex(steps));
}

MediaContent MediaPlaylistNavigator::previousItem(int steps) const
{
    return itemAt(previousIndex(steps));
}

MediaContent MediaPlaylistNavigator::itemAt(int position) const
{
    return d_func()->playlist->media(position);
}

int MediaPlaylistNavigator::mediaCount() const
{
    return d_func()->playlist->mediaCount();
}

int MediaPlaylistNavigator::currentIndex() const
{
    return d_func()->currentPos;
}

int MediaPlaylistNavigator::nextIndex(int steps) const
{
    return d_func()->nextItemPos(steps);
}

int MediaPlaylistNavigator::previousIndex(int steps) const
{
    return d_func()->previousItemPos(steps);
}

void MediaPlaylistNavigator::next()
{
    Q_D(MediaPlaylistNavigator);

    int nextPos = nextIndex();

    if ( playbackMode() == MediaPlaylist::Random )
            d->randomPositionsOffset++;

    jump(nextPos);
}

void MediaPlaylistNavigator::previous()
{
    Q_D(MediaPlaylistNavigator);

    int prevPos = previousIndex();
    if ( playbackMode() == MediaPlaylist::Random )
        d->randomPositionsOffset--;

    jump(prevPos);
}

void MediaPlaylistNavigator::jump(int position)
{
    Q_D(MediaPlaylistNavigator);

    if (position < -1 || position >= mediaCount())
        position = -1;

    if (position != -1)
        d->lastValidPos = position;

    if (playbackMode() == MediaPlaylist::Random) {
        if (d->randomModePositions[d->randomPositionsOffset] != position) {
            d->randomModePositions.clear();
            d->randomModePositions.append(position);
            d->randomPositionsOffset = 0;
        }
    }

    if (position != -1)
        d->currentItem = d->playlist->media(position);
    else
        d->currentItem = MediaContent();

    if (position != d->currentPos) {
        d->currentPos = position;
        emit currentIndexChanged(d->currentPos);
        emit surroundingItemsChanged();
    }

    emit activated(d->currentItem);
}

void MediaPlaylistNavigatorPrivate::_q_mediaInserted(int start, int end)
{
    Q_Q(MediaPlaylistNavigator);

    if (currentPos >= start) {
        currentPos = end-start+1;
        q->jump(currentPos);
    }

    //TODO: check if they really changed
    emit q->surroundingItemsChanged();
}

void MediaPlaylistNavigatorPrivate::_q_mediaRemoved(int start, int end)
{
    Q_Q(MediaPlaylistNavigator);

    if (currentPos > end) {
        currentPos = currentPos - end-start+1;
        q->jump(currentPos);
    } else if (currentPos >= start) {
        //current item was removed
        currentPos = qMin(start, q->mediaCount()-1);
        q->jump(currentPos);
    }

    //TODO: check if they really changed
    emit q->surroundingItemsChanged();
}

void MediaPlaylistNavigatorPrivate::_q_mediaChanged(int start, int end)
{
    Q_Q(MediaPlaylistNavigator);

    if (currentPos >= start && currentPos<=end) {
        MediaContent src = playlist->media(currentPos);
        if (src != currentItem) {
            currentItem = src;
            emit q->activated(src);
        }
    }

    //TODO: check if they really changed
    emit q->surroundingItemsChanged();
}

#include "moc_mediaplaylistnavigator.cpp"
