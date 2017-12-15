#ifndef MULTIMEDIAENGINE_H
#define MULTIMEDIAENGINE_H

#include <Multimedia/multimediaglobal.h>
#include <QtCore/qobject.h>


HS_BEGIN_NAMESPACE

class MULTIMEDIA_EXPORT MultimediaEngine : public QObject
{
    Q_OBJECT
public:
    explicit MultimediaEngine(QObject *parent = 0);

signals:

public slots:
};

HS_END_NAMESPACE

#endif // MULTIMEDIAENGINE_H
