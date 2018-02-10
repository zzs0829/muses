#ifndef MEDIAPLAYLISTPROVIDER_H
#define MEDIAPLAYLISTPROVIDER_H

#include <QObject>
#include <Multimedia/multimediaglobal.h>
#include <Multimedia/mediacontent.h>
#include <Multimedia/mediaplaylist.h>


HS_BEGIN_NAMESPACE

class MediaPlaylistProviderPrivate
{
public:
    MediaPlaylistProviderPrivate()
    {}
    virtual ~MediaPlaylistProviderPrivate()
    {}
};

class MULTIMEDIA_EXPORT MediaPlaylistProvider : public QObject
{
Q_OBJECT
public:
    MediaPlaylistProvider(QObject *parent=0);
    virtual ~MediaPlaylistProvider();

    virtual bool load(const QNetworkRequest &request, const char *format = 0);
    virtual bool load(QIODevice * device, const char *format = 0);
    virtual bool save(const QUrl &location, const char *format = 0);
    virtual bool save(QIODevice * device, const char *format);

    // for PlaylistModel
    virtual int mediaCount() const = 0;
    // for PlaylistModel
    virtual MediaContent media(int index) const = 0;

    virtual bool isReadOnly() const;

    virtual bool addMedia(const MediaContent &content);
    virtual bool addMedia(const QList<MediaContent> &contentList);
    virtual bool insertMedia(int index, const MediaContent &content);
    virtual bool insertMedia(int index, const QList<MediaContent> &content);
    virtual bool moveMedia(int from, int to);
    virtual bool removeMedia(int pos);
    virtual bool removeMedia(int startIndex, int endIndex);
    virtual bool clear();

public Q_SLOTS:
    virtual void shuffle();

Q_SIGNALS:
    // for PlaylistModel
    void mediaAboutToBeInserted(int startIndex, int endIndex);
    void mediaInserted(int startIndex, int endIndex);

    // for PlaylistModel
    void mediaAboutToBeRemoved(int startIndex, int endIndex);
    void mediaRemoved(int startIndex, int endIndex);

    // for PlaylistModel
    void mediaChanged(int startIndex, int endIndex);

    // for PlaylistModel
    void loaded();
    void loadFailed(MediaPlaylist::Error, const QString& errorMessage);

protected:
    MediaPlaylistProviderPrivate *d_ptr;
    MediaPlaylistProvider(MediaPlaylistProviderPrivate &dd, QObject *parent);

private:
    Q_DECLARE_PRIVATE(MediaPlaylistProvider)
};

HS_END_NAMESPACE

#endif // MEDIAPLAYLISTPROVIDER_H
