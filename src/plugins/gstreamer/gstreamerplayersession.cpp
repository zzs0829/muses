#include "gstreamerplayersession.h"
#include "gstreamerplayersession_p.h"


GstreamerPlayerSession::GstreamerPlayerSession(const QString &id, const QString &key, QObject *parent) :
    MediaPlayerSession(* new GstreamerPlayerSessionPrivate(id, key), parent)
{
}
