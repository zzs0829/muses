#ifndef PLAYBACKCONTROLLER_H
#define PLAYBACKCONTROLLER_H

#include <Multimedia/multimediaglobal.h>
#include <QtCore/qobject.h>

#include <Multimedia/multimediacontroller.h>

HS_BEGIN_NAMESPACE

class MULTIMEDIA_EXPORT PlaybackController : public MultimediaController
{
    Q_OBJECT
protected:
    explicit PlaybackController(QObject *parent = Q_NULLPTR);

public:

    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;

signals:

public slots:
};

#define PlaybackController_uid "module.muses.playbackcontroller/1.0"
MULTIMEDIA_DECLARE_CONTROLLER(PlaybackController, PlaybackController_uid)

HS_END_NAMESPACE

#endif // PLAYBACKCONTROLLER_H
