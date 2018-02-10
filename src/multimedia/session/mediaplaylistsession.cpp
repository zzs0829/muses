#include "mediaplaylistsession.h"
#include "mediaplaylistsession_p.h"

HS_BEGIN_NAMESPACE

MediaPlaylistSession::MediaPlaylistSession(const QString &id, const QString &key, QObject *parent):
    MultimediaSession(* new MediaPlaylistSessionPrivate(id, key), parent)
{

}

MediaPlaylistSession::MediaPlaylistSession(MediaPlaylistSessionPrivate &dd,
                                     QObject *parent)  :
    MultimediaSession(dd, parent)
{
}


HS_END_NAMESPACE
