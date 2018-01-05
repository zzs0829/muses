#ifndef GSTREAMERPLAYERWORK_H
#define GSTREAMERPLAYERWORK_H

#include <QObject>
#include "gstreamerplayersession.h"
#include "gstreamerplayercontrol.h"

class GstreamerPlayerWork : public QObject
{
    Q_OBJECT
public:
    explicit GstreamerPlayerWork(GstreamerPlayerSession *session,
                                 GstreamerPlayerControl *control,
                                 QObject *parent = Q_NULLPTR);

    GstreamerPlayerSession *session() { return m_session; }
    GstreamerPlayerControl *control() { return m_control; }

signals:

public slots:
    void start();
    void stop();

private slots:
    void _q_sessionPropertyChanged(const QString &key, const QVariant &value);

private:
    GstreamerPlayerSession *m_session;
    GstreamerPlayerControl *m_control;
};

#endif // GSTREAMERPLAYERWORK_H
