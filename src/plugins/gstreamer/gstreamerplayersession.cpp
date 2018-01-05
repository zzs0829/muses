#include "gstreamerplayersession.h"
#include "gstreamerplayersession_p.h"


GstreamerPlayerSession::GstreamerPlayerSession(QObject *parent) :
    MediaPlayerSession(* new GstreamerPlayerSessionPrivate, parent)
{
}
