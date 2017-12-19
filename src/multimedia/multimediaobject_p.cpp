#include "multimediaobject_p.h"

MultimediaObjectPrivate::MultimediaObjectPrivate(MultimediaObject *parent):
    q_ptr(parent),
    notifyTimer(0)
{
    notifyTimer = new QTimer(q_ptr);
    notifyTimer->setInterval(1000);
    q_ptr->connect(notifyTimer, SIGNAL(timeout()), SLOT(_q_notify()));
}

void MultimediaObjectPrivate::_q_notify()
{
    Q_Q(MultimediaObject);

    const QMetaObject* m = q->metaObject();

    for (int pi : qAsConst(notifyProperties)) {
        QMetaProperty p = m->property(pi);
        p.notifySignal().invoke(
            q, QGenericArgument(QMetaType::typeName(p.userType()), p.read(q).data()));
    }
}
