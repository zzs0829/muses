#include "multimediaservice.h"
#include "multimediaservice_p.h"

HS_BEGIN_NAMESPACE

MultimediaService::MultimediaService(QObject *parent) :
    QObject(*new MultimediaServicePrivate, parent)
{
}

MultimediaService::MultimediaService(MultimediaServicePrivate &dd,
                                     QObject *parent) :
    QObject(dd, parent)
{
}

/*!
    Destroys a multimedia service.
*/

MultimediaService::~MultimediaService()
{
}

HS_END_NAMESPACE
