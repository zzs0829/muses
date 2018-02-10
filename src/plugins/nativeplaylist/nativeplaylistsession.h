#ifndef NATIVEPLAYLISTSESSION_H
#define NATIVEPLAYLISTSESSION_H

#include <Multimedia/mediaplaylistsession.h>
#include <Multimedia/mediaplayercontroller.h>

class NativePlaylistService;
class NativePlaylistSessionPrivate;
class NativePlaylistSession : public MediaPlaylistSession
{
    Q_OBJECT
    friend class NativePlaylistService;
public:
    NativePlaylistSession(const QString &id, const QString &key,
                          const QString &path,
                          const QString &filters,
                          QObject *parent = 0);

    // for PlaylistModel
    virtual int mediaCount() const Q_DECL_OVERRIDE;
    // for PlaylistModel
    virtual MediaContent media(int index) const Q_DECL_OVERRIDE;

    virtual int currentIndex() const Q_DECL_OVERRIDE;
    virtual void setCurrentIndex(int position) Q_DECL_OVERRIDE;
    virtual int nextIndex(int steps) const Q_DECL_OVERRIDE;
    virtual int previousIndex(int steps) const Q_DECL_OVERRIDE;

    virtual void next() Q_DECL_OVERRIDE;
    virtual void previous() Q_DECL_OVERRIDE;

    virtual MediaPlaylist::PlaybackMode playbackMode() const Q_DECL_OVERRIDE;
    virtual void setPlaybackMode(MediaPlaylist::PlaybackMode mode) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(NativePlaylistSession)
    Q_DECLARE_PRIVATE(NativePlaylistSession)
};

#endif // NATIVEPLAYLISTSESSION_H
