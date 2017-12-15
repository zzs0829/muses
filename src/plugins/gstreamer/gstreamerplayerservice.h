#ifndef GSTREAMERPLAYERSERVICE_H
#define GSTREAMERPLAYERSERVICE_H

#include <Multimedia/multimediaservice.h>
#include "gstreamerplayerengine.h"
#include "gstreamerplayercontrol.h"

class GstreamerPlayerService : public MultimediaService
{
    Q_OBJECT
public:
    explicit GstreamerPlayerService(QObject *parent = 0);

    virtual MultimediaController* requestController(const char *uid) Q_DECL_OVERRIDE;

signals:

public slots:

private:
    GstreamerPlayerEngine *m_engine;
    GstreamerPlayerControl *m_control;
};

#endif // GSTREAMERPLAYERSERVICE_H
