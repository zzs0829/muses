#ifndef MEDIAPLAYLIST_H
#define MEDIAPLAYLIST_H

#include <QtCore/qobject.h>

#include <Multimedia/multimediaglobal.h>

HS_BEGIN_NAMESPACE

class MediaPlaylistPrivate;
class MULTIMEDIA_EXPORT MediaPlaylist : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(MediaPlaylist::PlaybackMode playbackMode READ playbackMode WRITE setPlaybackMode NOTIFY playbackModeChanged)
//    Q_PROPERTY(MediaContent currentMedia READ currentMedia NOTIFY currentMediaChanged)
//    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_ENUMS(PlaybackMode Error)

public:
    enum PlaybackMode { CurrentItemOnce, CurrentItemInLoop, Sequential, Loop, Random };
    enum Error { NoError, FormatError, FormatNotSupportedError, NetworkError, AccessDeniedError };
public:
    explicit MediaPlaylist(QObject *parent = Q_NULLPTR);
    virtual ~MediaPlaylist();

#if 0
    PlaybackMode playbackMode() const;
    void setPlaybackMode(PlaybackMode mode);

    int currentIndex() const;
    MediaContent currentMedia() const;

    int nextIndex(int steps = 1) const;
    int previousIndex(int steps = 1) const;

    MediaContent media(int index) const;

    int mediaCount() const;
    bool isEmpty() const;
    bool isReadOnly() const;

signals:
    void currentIndexChanged(int index);
    void playbackModeChanged(MediaPlaylist::PlaybackMode mode);
    void currentMediaChanged(const MediaContent&);

public slots:
#endif
private:
    Q_DECLARE_PRIVATE(MediaPlaylist)
};

HS_END_NAMESPACE

Q_DECLARE_METATYPE(HS_PREPEND_NAMESPACE(MediaPlaylist)::PlaybackMode)
Q_DECLARE_METATYPE(HS_PREPEND_NAMESPACE(MediaPlaylist)::Error)

MULTIMEDIA_ENUM_DEBUG(HS_PREPEND_NAMESPACE(MediaPlaylist), PlaybackMode)
MULTIMEDIA_ENUM_DEBUG(HS_PREPEND_NAMESPACE(MediaPlaylist), Error)

#endif // MEDIAPLAYLIST_H
