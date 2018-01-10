#ifndef MULTIMEDIASERVICE_P_H
#define MULTIMEDIASERVICE_P_H

#include "multimediaservice.h"
#include <Multimedia/multimediasessionwork.h>

HS_BEGIN_NAMESPACE

class MultimediaServicePrivate
{
    Q_DECLARE_PUBLIC(MultimediaService)
public:
    MultimediaServicePrivate():
        q_ptr(0),
        sessionWork(0)
    {}
    virtual ~MultimediaServicePrivate() {}

    MultimediaService *q_ptr;
    MultimediaSessionWork *sessionWork;
};

HS_END_NAMESPACE

#endif // MULTIMEDIASERVICE_P_H
