#ifndef GSTREAMERPLAYERSESSION_H
#define GSTREAMERPLAYERSESSION_H

#include <Multimedia/mediaplayersession.h>
#include <Multimedia/mediaplayercontroller.h>

class GstreamerPlayerService;
class GstreamerPlayerWork;
class GstreamerPlayerSessionPrivate;
class GstreamerPlayerSession : public MediaPlayerSession
{
    Q_OBJECT
    friend class GstreamerPlayerService;
    friend class GstreamerPlayerWork;
public:
    GstreamerPlayerSession(const QString &id, const QString &key, QObject *parent = 0);

private:
    Q_DISABLE_COPY(GstreamerPlayerSession)
    Q_DECLARE_PRIVATE(GstreamerPlayerSession)
};

#endif // GSTREAMERPLAYERSESSION_H
