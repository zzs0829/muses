#include "multimediaobject.h"
#include "multimediaobject_p.h"

HS_BEGIN_NAMESPACE

MultimediaObject::MultimediaObject(QObject *parent) :
    QObject(parent),
    d_ptr(new MultimediaObjectPrivate(this))
{

}

MultimediaObject::~MultimediaObject()
{
    delete d_ptr;
}

void MultimediaObject::addPropertyWatch(QByteArray const &name)
{
    Q_D(MultimediaObject);

    const QMetaObject* m = metaObject();

    int index = m->indexOfProperty(name.constData());

    if (index != -1 && m->property(index).hasNotifySignal()) {
        d->notifyProperties.insert(index);

        if (!d->notifyTimer->isActive())
            d->notifyTimer->start();
    }
}

void MultimediaObject::removePropertyWatch(QByteArray const &name)
{
    Q_D(MultimediaObject);

    int index = metaObject()->indexOfProperty(name.constData());

    if (index != -1) {
        d->notifyProperties.remove(index);

        if (d->notifyProperties.isEmpty())
            d->notifyTimer->stop();
    }
}

#include "moc_multimediaobject.cpp"
HS_END_NAMESPACE
