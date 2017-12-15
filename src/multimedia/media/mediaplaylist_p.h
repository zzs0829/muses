#ifndef MEDIAPLAYLIST_P_H
#define MEDIAPLAYLIST_P_H

#include "mediaplaylist.h"
#include "private/qobject_p.h"


HS_BEGIN_NAMESPACE

class MediaPlaylistControl;
class MediaPlaylistPrivate: public QObjectPrivate
{
    Q_DECLARE_PUBLIC(MediaPlaylist)
public:
    MediaPlaylistControl *control;
};

HS_END_NAMESPACE

#endif // MEDIAPLAYLIST_P_H
