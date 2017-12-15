#include "mediaplaylist.h"
#include "mediaplaylist_p.h"

HS_BEGIN_NAMESPACE

static void qRegisterMediaPlaylistMetaTypes()
{
    qRegisterMetaType<MediaPlaylist::Error>();
    qRegisterMetaType<MediaPlaylist::PlaybackMode>();
}

Q_CONSTRUCTOR_FUNCTION(qRegisterMediaPlaylistMetaTypes)

/*!
  Create a new playlist object with the given \a parent.
*/
MediaPlaylist::MediaPlaylist(QObject *parent)  :
    QObject(*new MediaPlaylistPrivate, parent)
{

}

/*!
  Destroys the playlist.
  */

MediaPlaylist::~MediaPlaylist()
{
//    Q_D(MediaPlaylist);

//    if (d->mediaObject)
//        d->mediaObject->unbind(this);

//    delete d_ptr;
}

#if 0
/*!
  \property MediaPlaylist::playbackMode

  This property defines the order that items in the playlist are played.

  \sa MediaPlaylist::PlaybackMode
*/

MediaPlaylist::PlaybackMode MediaPlaylist::playbackMode() const
{
    return d_func()->control->playbackMode();
}

void MediaPlaylist::setPlaybackMode(MediaPlaylist::PlaybackMode mode)
{
    Q_D(MediaPlaylist);
    d->control->setPlaybackMode(mode);
}

/*!
  Returns position of the current media content in the playlist.
*/
int MediaPlaylist::currentIndex() const
{
    return d_func()->control->currentIndex();
}

/*!
  Returns the current media content.
*/

MediaContent MediaPlaylist::currentMedia() const
{
    return d_func()->playlist()->media(currentIndex());
}

/*!
  Returns the index of the item, which would be current after calling next()
  \a steps times.

  Returned value depends on the size of playlist, current position
  and playback mode.

  \sa MediaPlaylist::playbackMode(), previousIndex()
*/
int MediaPlaylist::nextIndex(int steps) const
{
    return d_func()->control->nextIndex(steps);
}

/*!
  Returns the index of the item, which would be current after calling previous()
  \a steps times.

  \sa MediaPlaylist::playbackMode(), nextIndex()
*/

int MediaPlaylist::previousIndex(int steps) const
{
    return d_func()->control->previousIndex(steps);
}


/*!
  Returns the number of items in the playlist.

  \sa isEmpty()
  */
int MediaPlaylist::mediaCount() const
{
    return d_func()->playlist()->mediaCount();
}

/*!
  Returns true if the playlist contains no items, otherwise returns false.

  \sa mediaCount()
  */
bool MediaPlaylist::isEmpty() const
{
    return mediaCount() == 0;
}

/*!
  Returns true if the playlist can be modified, otherwise returns false.

  \sa mediaCount()
  */
bool MediaPlaylist::isReadOnly() const
{
    return d_func()->playlist()->isReadOnly();
}

/*!
  Returns the media content at \a index in the playlist.
*/

MediaContent MediaPlaylist::media(int index) const
{
    return d_func()->playlist()->media(index);
}
#endif


HS_END_NAMESPACE
