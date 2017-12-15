#ifndef MULTIMEDIACONTROLLER_P_H
#define MULTIMEDIACONTROLLER_P_H

#include "multimediacontroller.h"

HS_BEGIN_NAMESPACE

class MultimediaControllerPrivate
{
public:
    virtual ~MultimediaControllerPrivate();

    MultimediaController *q_ptr;
};

HS_END_NAMESPACE

#endif // MULTIMEDIACONTROLLER_P_H
