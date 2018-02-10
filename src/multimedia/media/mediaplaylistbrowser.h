#ifndef MEDIAPLAYLISTBROWSER_H
#define MEDIAPLAYLISTBROWSER_H

#include <QObject>
#include <Multimedia/multimediaglobal.h>
#include <Multimedia/mediacontent.h>

HS_BEGIN_NAMESPACE

class MediaPlaylistBrowserPrivate
{
public:
    MediaPlaylistBrowserPrivate()
    {}
    virtual ~MediaPlaylistBrowserPrivate()
    {}
};

class MULTIMEDIA_EXPORT MediaPlaylistBrowser : public QObject
{
    Q_OBJECT
public:
    virtual ~MediaPlaylistBrowser();

    virtual int currentIndex() const = 0;
    virtual void setCurrentIndex(int position) = 0;

    // for PlaylistModel
    virtual int mediaCount() const = 0;
    // for PlaylistModel
    virtual MediaContent media(int index) const = 0;

signals:
    void currentIndexChanged(int position);


protected:
    MediaPlaylistBrowserPrivate *d_ptr;
    explicit MediaPlaylistBrowser(QObject *parent = 0);
    MediaPlaylistBrowser(MediaPlaylistBrowserPrivate &dd, QObject *parent);

private:
    Q_DECLARE_PRIVATE(MediaPlaylistBrowser)
};

HS_END_NAMESPACE

#endif // MEDIAPLAYLISTBROWSER_H
