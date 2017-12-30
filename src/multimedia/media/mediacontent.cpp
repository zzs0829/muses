#include "mediacontent.h"
#include "mediaplaylist.h"
#include <QtCore/QPointer>

HS_BEGIN_NAMESPACE

// USING LOG
Q_DECLARE_LOGGING_CATEGORY(__media__)

static void qRegisterMediaContentMetaTypes()
{
    qRegisterMetaType<MediaContent>();
}

Q_CONSTRUCTOR_FUNCTION(qRegisterMediaContentMetaTypes)


class MediaContentPrivate : public QSharedData
{
public:
    MediaContentPrivate()
    {}

    MediaContentPrivate(const MediaContentPrivate &other):
        QSharedData(other),
        request(other.request)
    {}

    ~MediaContentPrivate()
    {
        members.clear();
    }

    bool operator ==(const MediaContentPrivate &other) const
    {
        return request == other.request &&
                members == other.members;
    }

    QNetworkRequest request;
    QList<MediaContent> members;
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
    Returns true if this media content is a group; false otherwise.
*/
bool MediaContent::isGroup() const
{
    if(isNull())
        return false;

    return !(d->members.isEmpty());
}

/*!
    Returns true if this media content is empty (url or group); false otherwise.
*/
bool MediaContent::isEmpty() const
{
    if(isNull())
        return true;

    if(isGroup()) {
        return d->members.isEmpty();
    }else{
        return d->request.url().isEmpty();
    }
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
    Removes all members from the group or Resets the content of the request.
*/
void MediaContent::clear()
{
    if(isNull()) {
        return;
    }

    removeAll();
    d->request = QNetworkRequest();
}

/*!
    Returns the number of members in the group.
*/
int MediaContent::count() const
{
    if(isNull()) {
        return 0;
    }

    return d->members.count();
}

/*!
    Returns true if the group contains an occurrence of member; otherwise returns false.
*/
bool MediaContent::contains(const MediaContent &member) const
{
    if(isNull()) {
        qCWarning(__media__) << "The media content is null to contains member.";
        return false;
    }

    return d->members.contains(member);
}

/*!
    Inserts member at the end of the media content group.
*/
void MediaContent::append(const MediaContent &member)
{
    if(isNull()) {
        qCWarning(__media__) << "The media content is null to append member.";
        return;
    }

    if(!(d->members.contains(member))) {
        d->members.append(member);
    }
}

/*!
    Removes all members from the group.
*/
int MediaContent::removeAll()
{
    if(isNull()) {
        qCWarning(__media__) << "The media content is null to remove all members.";
        return 0;
    }

    int count = d->members.count();
    d->members.clear();
    return count;
}

/*!
    Removes the first occurrence of member in the group and returns true on success; otherwise returns false.
*/
bool MediaContent::remove(const MediaContent &member)
{
    if(isNull()) {
        qCWarning(__media__) << "The media content is null to remove the first occurrence of member.";
        return false;
    }

    return d->members.removeOne(member);
}

void MediaContent::sort(SortFlags flags)
{
    if(isNull()) {
        qCWarning(__media__) << "The media content is null to sort all of members.";
        return;
    }
    std::sort(d->members.begin(), d->members.end(), [&flags](
              const MediaContent &member1, const MediaContent &member2 ){
        bool isGroupFirst = flags.testFlag(MediaContent::GroupFirst);
        bool isGroupLast = flags.testFlag(MediaContent::GroupLast);
        bool isUrlString = flags.testFlag(MediaContent::UrlString);
        bool isReversed = flags.testFlag(MediaContent::Reversed);
        QString url1 = member1.url().toString();
        QString url2 = member2.url().toString();
        if(isGroupFirst) {
            if(member1.isGroup() && !member2.isGroup())
                return !isReversed;
            else if(!member1.isGroup() && member2.isGroup())
                return isReversed;
            else if(url1 > url2)
                return !isReversed;
            else
                return isReversed;
        }else if(isGroupLast) {
            if(member1.isGroup() && !member2.isGroup())
                return isReversed;
            else if(!member1.isGroup() && member2.isGroup())
                return !isReversed;
            else if(url1 > url2)
                return !isReversed;
            else
                return isReversed;
        }else if(isUrlString) {
            if(url1 > url2)
                return !isReversed;
            else
                return isReversed;
        }else{
            return false;
        }
    });

    foreach (MediaContent content, d->members) {
        if(content.isGroup()) {
            content.sort(flags);
        }
    }
}



HS_END_NAMESPACE
