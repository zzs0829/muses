#include "gstreamerplayerwork.h"

GstreamerPlayerWork::GstreamerPlayerWork(GstreamerPlayerSession *session,
                                         GstreamerPlayerControl *control,
                                         QObject *parent)
    : QObject(parent),
      m_session(session),
      m_control(control)
{

}

void GstreamerPlayerWork::start()
{
    // TODO: start

}

void GstreamerPlayerWork::stop()
{
    // TODO: stop
    this->deleteLater();
}


void GstreamerPlayerWork::_q_sessionPropertyChanged(const QString &key,
                                                    const QVariant &value)
{
    Q_UNUSED(key);
    Q_UNUSED(value);
}
