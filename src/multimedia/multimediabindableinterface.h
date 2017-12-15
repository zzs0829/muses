#ifndef MULTIMEDIABINDABLEINTERFACE_H
#define MULTIMEDIABINDABLEINTERFACE_H

#include "multimediaobject.h"

HS_BEGIN_NAMESPACE

class MULTIMEDIA_EXPORT MultimediaBindableInterface
{
public:
    virtual ~MultimediaBindableInterface();

    virtual MultimediaObject *mediaObject() const = 0;

protected:
    friend class MultimediaObject;
    virtual bool setMediaObject(MultimediaObject *object) = 0;
};

HS_END_NAMESPACE

#define MultimediaBindableInterface_iid \
    "module.muses.multimediabindableinterface/1.0"
Q_DECLARE_INTERFACE(HS_PREPEND_NAMESPACE(MultimediaBindableInterface), MultimediaBindableInterface_iid)

#endif // MULTIMEDIABINDABLEINTERFACE_H
