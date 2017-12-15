#include "gstreamerplayerservice.h"

GstreamerPlayerService::GstreamerPlayerService(QObject *parent) :
    MultimediaService(parent),
    m_engine(0),
    m_control(0)
{
    m_engine = new GstreamerPlayerEngine(this);
    m_control = new GstreamerPlayerControl(m_engine, this);
}

MultimediaController* GstreamerPlayerService::requestController(const char *uid)
{
    if (qstrcmp(uid, MediaPlayerController_uid) == 0)
        return m_control;
    return 0;
}
