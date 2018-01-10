#ifndef MULTIMEDIASESSIONWORK_H
#define MULTIMEDIASESSIONWORK_H

#include <QtCore/qobject.h>

#include <Multimedia/multimediaglobal.h>
#include <Multimedia/multimediasession.h>
#include <Multimedia/multimediacontroller.h>

HS_BEGIN_NAMESPACE

class MultimediaSessionWorkPrivate;
class MULTIMEDIA_EXPORT MultimediaSessionWork : public QObject
{
    Q_OBJECT
public:
    virtual ~MultimediaSessionWork();

    virtual void start() = 0;
    virtual void stop() = 0;

protected:
    MultimediaSessionWork(MultimediaSession *session, MultimediaController *control, QObject *parent = 0);
    MultimediaSessionWork(MultimediaSessionWorkPrivate &dd, QObject *parent);
    MultimediaSessionWorkPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(MultimediaSessionWork)
};

HS_END_NAMESPACE

#endif // MULTIMEDIASESSIONWORK_H
