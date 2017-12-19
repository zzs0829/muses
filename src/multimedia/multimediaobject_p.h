#ifndef MULTIMEDIAOBJECT_P_H
#define MULTIMEDIAOBJECT_P_H

#include "multimediaobject.h"

#include <QtCore/qbytearray.h>
#include <QtCore/qset.h>
#include <QtCore/qtimer.h>

class MultimediaObjectPrivate
{
    Q_DECLARE_PUBLIC(MultimediaObject)
public:
    MultimediaObjectPrivate(MultimediaObject *parent);

    void _q_notify();

public:
    MultimediaObject *q_ptr;
    QTimer* notifyTimer;
    QSet<int> notifyProperties;
};

#endif // MULTIMEDIAOBJECT_P_H
