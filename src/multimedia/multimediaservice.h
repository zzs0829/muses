#ifndef MULTIMEDIASERVICE_H
#define MULTIMEDIASERVICE_H

#include <Multimedia/multimediaglobal.h>
#include <QtCore/qobject.h>

#include "Multimedia/multimediacontroller.h"


HS_BEGIN_NAMESPACE

class MultimediaServicePrivate;
class MULTIMEDIA_EXPORT MultimediaService : public QObject
{
    Q_OBJECT
protected:
    explicit MultimediaService(QObject *parent);
    MultimediaService(MultimediaServicePrivate &dd, QObject *parent);

public:
    ~MultimediaService();

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

signals:

public slots:

private:
    Q_DECLARE_PRIVATE(MultimediaService)
};

HS_END_NAMESPACE

#endif // MULTIMEDIASERVICE_H
