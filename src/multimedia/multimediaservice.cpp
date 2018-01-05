#include "multimediaservice.h"
#include "multimediaservice_p.h"

HS_BEGIN_NAMESPACE

MultimediaService::MultimediaService(QObject *parent) :
    QObject(parent)
  , d_ptr(new MultimediaServicePrivate)
{
    d_ptr->q_ptr = this;
}

MultimediaService::MultimediaService(MultimediaServicePrivate &dd,
                                     QObject *parent) :
    QObject(parent)
  , d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

/*!
    Destroys a multimedia service.
*/

MultimediaService::~MultimediaService()
{
}

HS_END_NAMESPACE
