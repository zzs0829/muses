#include "nativeplaylistcontrol.h"
#include "nativeplaylistprovider.h"

NativePlaylistControl::NativePlaylistControl(const QString &path,
                                             const QString &filters,
                                             QObject *parent) :
    MediaPlaylistController(parent)
{
    QStringList nameFilters = formatNameFilters(filters);
    MediaPlaylistProvider *playlist = new NativePlaylistProvider(path, nameFilters, this);
    m_navigator = new MediaPlaylistNavigator(playlist, this);
    m_navigator->setPlaybackMode(MediaPlaylist::Sequential);

    connect(m_navigator, SIGNAL(currentIndexChanged(int)), SIGNAL(currentIndexChanged(int)));
    connect(m_navigator, SIGNAL(activated(MediaContent)), SIGNAL(currentMediaChanged(MediaContent)));
    connect(m_navigator, SIGNAL(playbackModeChanged(MediaPlaylist::PlaybackMode)), SIGNAL(playbackModeChanged(MediaPlaylist::PlaybackMode)));

    playlist->load(QNetworkRequest());
}

QStringList NativePlaylistControl::formatNameFilters(const QString &filters)
{

    QStringList nameFilters;
    foreach (const QString &name,
             filters.split(',', QString::SkipEmptyParts)) {
        if(name.startsWith(QLatin1String("*."))) {
            nameFilters.append(name);
        }else if(name.startsWith(".")) {
            nameFilters.append("*" + name);
        }else if(!name.contains(QRegExp("\\W+"))){
            nameFilters.append("*." + name);
        }else{
            nameFilters.append(name);
        }
    }
    return nameFilters;
}
