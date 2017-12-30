#ifndef MEDIAPLAYLISTCONTROLLER_H
#define MEDIAPLAYLISTCONTROLLER_H

#include <QtCore/qobject.h>

#include <Multimedia/multimediaglobal.h>
#include <Multimedia/multimediacontroller.h>
#include <Multimedia/mediacontent.h>
#include <Multimedia/mediaplaylist.h>
#include <Multimedia/mediaplaylistprovider.h>

HS_BEGIN_NAMESPACE

class MULTIMEDIA_EXPORT MediaPlaylistController : public MultimediaController
{
    Q_OBJECT
protected:
    explicit MediaPlaylistController(QObject *parent = Q_NULLPTR);

public:

    virtual MediaPlaylistProvider* playlistProvider() const = 0;
    virtual bool setPlaylistProvider(MediaPlaylistProvider *playlist) = 0;

    virtual int currentIndex() const = 0;
    virtual void setCurrentIndex(int position) = 0;
    virtual int nextIndex(int steps) const = 0;
    virtual int previousIndex(int steps) const = 0;

    virtual void next() = 0;
    virtual void previous() = 0;

    virtual MediaPlaylist::PlaybackMode playbackMode() const = 0;
    virtual void setPlaybackMode(MediaPlaylist::PlaybackMode mode) = 0;


signals:
    void playlistProviderChanged();
    void currentIndexChanged(int position);
    void currentMediaChanged(const MediaContent&);
    void playbackModeChanged(MediaPlaylist::PlaybackMode mode);

};

#define MediaPlaylistController_uid "module.muses.mediaplaylistcontroller/1.0"
MULTIMEDIA_DECLARE_CONTROLLER(MediaPlaylistController, MediaPlaylistController_uid)

HS_END_NAMESPACE

#endif // MEDIAPLAYLISTCONTROLLER_H
