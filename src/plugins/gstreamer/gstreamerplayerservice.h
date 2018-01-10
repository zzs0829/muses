#ifndef GSTREAMERPLAYERSERVICE_H
#define GSTREAMERPLAYERSERVICE_H

#include <Multimedia/multimediaservice.h>
#include "gstreamerplayersession.h"
#include "gstreamerplayerengine.h"
#include "gstreamerplayercontrol.h"
#include "gstreamerplayerwork.h"

class GstreamerPlayerService : public MultimediaService
{
    Q_OBJECT
public:
    explicit GstreamerPlayerService(QObject *parent = Q_NULLPTR);

    virtual MultimediaController* requestController(const char *uid) Q_DECL_OVERRIDE;

    virtual MultimediaSession* requestSession(const QString &key) Q_DECL_OVERRIDE;

    virtual void releaseSession(MultimediaSession *session) Q_DECL_OVERRIDE;

    virtual void availableSession(MultimediaSession *session) Q_DECL_OVERRIDE;

signals:

public slots:

private:
    QMap<QString, GstreamerPlayerSession *> m_sessions;
    GstreamerPlayerEngine *m_engine;
    GstreamerPlayerControl *m_control;
    GstreamerPlayerWork *m_work;
};

#endif // GSTREAMERPLAYERSERVICE_H
