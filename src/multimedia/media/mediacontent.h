#ifndef MEDIACONTENT_H
#define MEDIACONTENT_H

#include <Multimedia/multimediaglobal.h>
#include <QtCore/qurl.h>
#include <QtCore/qshareddata.h>
#include <QtNetwork/qnetworkrequest.h>


HS_BEGIN_NAMESPACE

class MediaContentPrivate;
class MULTIMEDIA_EXPORT MediaContent
{
public:    
    enum SortFlag
    {
        NoSort          = 0x00,
        GroupFirst      = 0x01,
        GroupLast       = 0x02,
        UrlString       = 0x04,
        Reversed        = 0x80
    };
    Q_DECLARE_FLAGS(SortFlags, SortFlag) //  Flags类型用于存储一个位或“|”的组合组合值。

    MediaContent();
    MediaContent(const QUrl &url);
    MediaContent(const QNetworkRequest &request);
    MediaContent(const MediaContent &other);
    ~MediaContent();

    MediaContent& operator=(const MediaContent &other);

    bool operator==(const MediaContent &other) const;
    bool operator!=(const MediaContent &other) const;

    bool isGroup() const;
    bool isNull() const;
    bool isEmpty() const;

    QUrl url() const;
    QNetworkRequest request() const;
    void clear();

    int count() const;
    bool contains(const MediaContent &member) const;
    void append(const MediaContent &member);
    int removeAll();
    bool remove(const MediaContent &member);
    void sort(SortFlags flags = GroupFirst);

    inline MediaContent &operator<< (const MediaContent &member)
    { append(member); return *this; }

private:
    QSharedDataPointer<MediaContentPrivate> d;
};

HS_END_NAMESPACE

Q_DECLARE_METATYPE(HS_PREPEND_NAMESPACE(MediaContent))

#endif // MEDIACONTENT_H
