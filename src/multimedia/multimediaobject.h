#ifndef MULTIMEDIAOBJECT_H
#define MULTIMEDIAOBJECT_H

#include <QtCore/qobject.h>

#include <Multimedia/multimediaglobal.h>

HS_BEGIN_NAMESPACE

class MULTIMEDIA_EXPORT MultimediaObject : public QObject
{
    Q_OBJECT
public:
    explicit MultimediaObject(QObject *parent = 0);

signals:

public slots:
};

HS_END_NAMESPACE


#endif // MULTIMEDIAOBJECT_H
