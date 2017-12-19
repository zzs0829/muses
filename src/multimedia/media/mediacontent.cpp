#include "mediacontent.h"
#include "mediaplaylist.h"
#include <QtCore/QPointer>

HS_BEGIN_NAMESPACE

static void qRegisterMediaContentMetaTypes()
{
    qRegisterMetaType<MediaContent>();
}

Q_CONSTRUCTOR_FUNCTION(qRegisterMediaContentMetaTypes)


class MediaContentPrivate : public QSharedData
{
public:
    MediaContentPrivate():
        isPlaylistOwned(false)
    {}

    MediaContentPrivate(const MediaContentPrivate &other):
        QSharedData(other),
        request(other.request),
        playlist(other.playlist),
        isPlaylistOwned(false)
    {}

    MediaContentPrivate(MediaPlaylist *pls, bool isOwn):
        playlist(pls),
        isPlaylistOwned(isOwn)
    {
    }

    ~MediaContentPrivate()
    {
        if (isPlaylistOwned && !playlist.isNull())
            playlist.data()->deleteLater();
    }

    bool operator ==(const MediaContentPrivate &other) const
    {
        return request == other.request && playlist == other.playlist;
    }

    QNetworkRequest request;

    QPointer<MediaPlaylist> playlist;
    bool isPlaylistOwned;
private:
    MediaContentPrivate& operator=(const MediaContentPrivate &other);
};

/*!
    Constructs a null MediaContent.
*/
MediaContent::MediaContent()
{

}
MediaContent::MediaContent(const QUrl &url):
    d(new MediaContentPrivate)
{
    d->request = QNetworkRequest(url);
}

/*!
    Constructs a media content with \a url providing a reference to the content.
*/
MediaContent::MediaContent(const QNetworkRequest &request):
    d(new MediaContentPrivate)
{
    d->request = request;
}

/*!
    Constructs a copy of the media content \a other.
*/

MediaContent::MediaContent(const MediaContent &other):
    d(other.d)
{
}

/*!
    Constructs a media content with \a playlist.

    \a contentUrl of a playlist is an optional parameter and can be empty.

    Set \a takeOwnership to true if you want MediaContent to take ownership of the playlist.
    \a takeOwnership is set to false by default.
*/

MediaContent::MediaContent(MediaPlaylist *playlist, bool takeOwnership):
    d(new MediaContentPrivate(playlist, takeOwnership))
{
}

/*!
    Destroys the media content object.
*/

MediaContent::~MediaContent()
{
}

/*!
    Assigns the value of \a other to this media content.
*/

MediaContent& MediaContent::operator=(const MediaContent &other)
{
    d = other.d;
    return *this;
}

/*!
    Returns true if \a other is equivalent to this media content; false otherwise.
*/

bool MediaContent::operator==(const MediaContent &other) const
{
    return (d.constData() == 0 && other.d.constData() == 0) ||
            (d.constData() != 0 && other.d.constData() != 0 &&
             *d.constData() == *other.d.constData());
}

/*!
    Returns true if \a other is not equivalent to this media content; false otherwise.
*/

bool MediaContent::operator!=(const MediaContent &other) const
{
    return !(*this == other);
}

/*!
    Returns true if this media content is null (uninitialized); false otherwise.
*/

bool MediaContent::isNull() const
{
    return d.constData() == 0;
}

/*!
    Returns a QUrl that represents that resource for this media content.
*/

QUrl MediaContent::url() const
{
    return request().url();
}

/*!
    Returns a QNetworkRequest that represents that canonical resource for this media content.
*/

QNetworkRequest MediaContent::request() const
{
    return d->request;
}

/*!
    Returns a playlist for this media content or 0 if this MediaContent is not a playlist.
*/

MediaPlaylist *MediaContent::playlist() const
{
    return d.constData() != 0
            ? d->playlist.data()
            : 0;
}



HS_END_NAMESPACE
