#ifndef MEDIAPLAYLISTPROVIDER_H
#define MEDIAPLAYLISTPROVIDER_H

#include <QObject>
#include <Multimedia/multimediaglobal.h>
#include <Multimedia/mediacontent.h>
#include <Multimedia/mediaplaylist.h>


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
    virtual bool removeMedia(int start, int end);
    virtual bool clear();

public Q_SLOTS:
    virtual void shuffle();

Q_SIGNALS:
    // for PlaylistModel
    void mediaAboutToBeInserted(int start, int end);
    void mediaInserted(int start, int end);

    // for PlaylistModel
    void mediaAboutToBeRemoved(int start, int end);
    void mediaRemoved(int start, int end);

    // for PlaylistModel
    void mediaChanged(int start, int end);

    // for PlaylistModel
    void loaded();
    void loadFailed(MediaPlaylist::Error, const QString& errorMessage);

protected:
    MediaPlaylistProviderPrivate *d_ptr;
    MediaPlaylistProvider(MediaPlaylistProviderPrivate &dd, QObject *parent);

private:
    Q_DECLARE_PRIVATE(MediaPlaylistProvider)
};

#endif // MEDIAPLAYLISTPROVIDER_H
