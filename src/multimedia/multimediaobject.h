#ifndef MULTIMEDIAOBJECT_H
#define MULTIMEDIAOBJECT_H

#include <QtCore/qobject.h>

#include <Multimedia/multimediaglobal.h>

HS_BEGIN_NAMESPACE

class MultimediaObjectPrivate;
class MULTIMEDIA_EXPORT MultimediaObject : public QObject
{
    Q_OBJECT
public:
    virtual ~MultimediaObject();

signals:

public slots:

protected:
    explicit MultimediaObject(QObject *parent = 0);

    void addPropertyWatch(QByteArray const &name);
    void removePropertyWatch(QByteArray const &name);

    MultimediaObjectPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(MultimediaObject)
    Q_PRIVATE_SLOT(d_func(), void _q_notify())
};

HS_END_NAMESPACE


#endif // MULTIMEDIAOBJECT_H
