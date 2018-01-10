#include "gstreamerplayerservice.h"
#include "gstreamerplayersession_p.h"
#include <QUuid>

GstreamerPlayerService::GstreamerPlayerService(QObject *parent) :
    MultimediaService(parent),
    m_engine(0),
    m_control(0),
    m_work(0)
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

MultimediaSession* GstreamerPlayerService::requestSession(const QString &key)
{
    GstreamerPlayerSession *session = m_sessions.value(key, Q_NULLPTR);
    if(!session) {
        QString id = QUuid::createUuid().toString();
        session = new GstreamerPlayerSession(id, key, this);
        m_sessions.insert(key, session);
    }

    // Avaiable
    availableSession(session);

    return session;
}

void GstreamerPlayerService::releaseSession(MultimediaSession *session)
{
    GstreamerPlayerSession *playerSession = qobject_cast<GstreamerPlayerSession *>(session);
    if(!playerSession) {
        return;
    }

    if(playerSession->sessionState() == MultimediaSession::Available) {
        if(m_work) {
            m_work->session()->d_func()->setState(MultimediaSession::NotAvailable);
            m_work->stop();
            m_work = 0;
        }
    }

    m_sessions.remove(m_sessions.key(playerSession));
    playerSession->deleteLater();
}

void GstreamerPlayerService::availableSession(MultimediaSession *session)
{
    GstreamerPlayerSession *playerSession = qobject_cast<GstreamerPlayerSession *>(session);
    if(!playerSession) {
        return;
    }

    if(m_work) {
        m_work->session()->d_func()->setState(MultimediaSession::NotAvailable);
        m_work->stop();
    }

    m_work = new GstreamerPlayerWork(playerSession, m_control, this);
    connect(m_work, SIGNAL(finished()), SLOT(deleteLater()));
    playerSession->d_func()->setState(MultimediaSession::Available);
    m_work->start();
}
