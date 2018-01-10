#ifndef MULTIMEDIASERVICE_H
#define MULTIMEDIASERVICE_H

#include <QtCore/qobject.h>
#include <Multimedia/multimediaglobal.h>
#include <Multimedia/multimediacontroller.h>
#include <Multimedia/multimediasession.h>


HS_BEGIN_NAMESPACE

class MultimediaServicePrivate;
class MULTIMEDIA_EXPORT MultimediaService : public QObject
{
    Q_OBJECT

public:
    virtual ~MultimediaService();

    virtual MultimediaController* requestController(const char *uid) = 0;

    template <typename T> inline T requestController() {
        if (MultimediaController *controller =
                requestController(multimediacontroller_uid<T>())) {
            if (T typedController = qobject_cast<T>(controller))
                return typedController;
//            releaseController(controller);
        }
        return 0;
    }

    virtual MultimediaSession* requestSession(const QString &key) = 0;

    virtual void releaseSession(MultimediaSession *session) = 0;

    virtual void availableSession(MultimediaSession *session) = 0;

protected:
    explicit MultimediaService(QObject *parent);
    MultimediaService(MultimediaServicePrivate &dd, QObject *parent);
    MultimediaServicePrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(MultimediaService)
};

HS_END_NAMESPACE

#endif // MULTIMEDIASERVICE_H
