#include "multimediacontroller.h"
#include "multimediacontroller_p.h"

HS_BEGIN_NAMESPACE

MultimediaController::MultimediaController(QObject *parent)
    : QObject(parent)
    , d_ptr(new MultimediaControllerPrivate)
{
    d_ptr->q_ptr = this;
}

MultimediaController::MultimediaController(MultimediaControllerPrivate &dd, QObject *parent)
    : QObject(parent)
    , d_ptr(&dd)

{
    d_ptr->q_ptr = this;
}

MultimediaController::~MultimediaController()
{
    delete d_ptr;
}


HS_END_NAMESPACE
