#ifndef MULTIMEDIAENGINE_H
#define MULTIMEDIAENGINE_H

#include <Multimedia/multimediaglobal.h>
#include <QtCore/qobject.h>


HS_BEGIN_NAMESPACE

class MULTIMEDIA_EXPORT MultimediaEngine : public QObject
{
    Q_OBJECT
public:
    virtual ~MultimediaEngine();
protected:
    explicit MultimediaEngine(QObject *parent = 0);
};

HS_END_NAMESPACE

#endif // MULTIMEDIAENGINE_H
