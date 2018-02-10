#ifndef NATIVEPLAYLISTCONTROL_H
#define NATIVEPLAYLISTCONTROL_H

#include <Multimedia/mediaplaylistcontroller.h>
#include <Multimedia/mediaplaylistnavigator.h>

class NativePlaylistControl : public MediaPlaylistController
{
    Q_OBJECT
public:
    explicit NativePlaylistControl(const QString &path,
                                   const QString &filters,
                                   QObject *parent = 0);

    ~NativePlaylistControl(){}

    MediaPlaylistProvider* playlistProvider() const Q_DECL_OVERRIDE
    { return m_navigator->playlist(); }

    bool setPlaylistProvider(MediaPlaylistProvider *mediaPlaylist) Q_DECL_OVERRIDE {
        m_navigator->setPlaylist(mediaPlaylist);
        emit playlistProviderChanged();
        return true;
    }

    int currentIndex() const Q_DECL_OVERRIDE { return m_navigator->currentIndex(); }
    void setCurrentIndex(int position) Q_DECL_OVERRIDE { m_navigator->jump(position); }
    int nextIndex(int steps) const Q_DECL_OVERRIDE { return m_navigator->nextIndex(steps); }
    int previousIndex(int steps) const Q_DECL_OVERRIDE { return m_navigator->previousIndex(steps); }

    void next() Q_DECL_OVERRIDE { m_navigator->next(); }
    void previous() Q_DECL_OVERRIDE { m_navigator->previous(); }

    MediaPlaylist::PlaybackMode playbackMode() const Q_DECL_OVERRIDE { return m_navigator->playbackMode(); }
    void setPlaybackMode(MediaPlaylist::PlaybackMode mode) Q_DECL_OVERRIDE { m_navigator->setPlaybackMode(mode); }

private:
    QStringList formatNameFilters(const QString &filters);
    MediaPlaylistNavigator *m_navigator;
};

#endif // NATIVEPLAYLISTCONTROL_H
