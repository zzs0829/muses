#ifndef MEDIACONTENT_H
#define MEDIACONTENT_H

#include <Multimedia/multimediaglobal.h>
#include <QtCore/qurl.h>
#include <QtCore/qshareddata.h>
#include <QtNetwork/qnetworkrequest.h>

#include "mediaplaylist.h"


HS_BEGIN_NAMESPACE

class MediaPlaylist;

class MediaContentPrivate;
class MULTIMEDIA_EXPORT MediaContent
{
public:
    MediaContent();
    MediaContent(const QNetworkRequest &request);
    MediaContent(const MediaContent &other);
    MediaContent(MediaPlaylist *playlist, bool takeOwnership = false);
    ~MediaContent();

    MediaContent& operator=(const MediaContent &other);

    bool operator==(const MediaContent &other) const;
    bool operator!=(const MediaContent &other) const;

    bool isNull() const;

    QUrl resource() const;
    QNetworkRequest request() const;

    MediaPlaylist *playlist() const;

private:
    QSharedDataPointer<MediaContentPrivate> d;
};

HS_END_NAMESPACE

Q_DECLARE_METATYPE(HS_PREPEND_NAMESPACE(MediaContent))

#endif // MEDIACONTENT_H
