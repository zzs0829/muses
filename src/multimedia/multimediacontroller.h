#ifndef MULTIMEDIACONTROLLER_H
#define MULTIMEDIACONTROLLER_H

#include <Multimedia/multimediaglobal.h>
#include <QtCore/qobject.h>

HS_BEGIN_NAMESPACE

class MultimediaControllerPrivate;
class MULTIMEDIA_EXPORT MultimediaController : public QObject
{
    Q_OBJECT

public:
    virtual ~MultimediaController();

protected:
    explicit MultimediaController(QObject *parent = 0);
    explicit MultimediaController(MultimediaControllerPrivate &dd, QObject *parent = Q_NULLPTR);

    MultimediaControllerPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(MultimediaController)
};

template <typename T> const char *multimediacontroller_uid() { return 0; }

#define MULTIMEDIA_DECLARE_CONTROLLER(Class, UId) \
    template <> inline const char *multimediacontroller_uid<Class *>() { return UId; }

HS_END_NAMESPACE

#endif // MULTIMEDIACONTROLLER_H
