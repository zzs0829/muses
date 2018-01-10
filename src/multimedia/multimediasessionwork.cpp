#include "multimediasessionwork.h"
#include "multimediasessionwork_p.h"

MultimediaSessionWork::MultimediaSessionWork(MultimediaSession *session,
                                             MultimediaController *control,
                                             QObject *parent) :
    QObject(parent)
  , d_ptr(new MultimediaSessionWorkPrivate(session, control))
{
    d_ptr->q_ptr = this;
}

MultimediaSessionWork::MultimediaSessionWork(MultimediaSessionWorkPrivate &dd,
                                             QObject *parent)  :
    QObject(parent)
  , d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

MultimediaSessionWork::~MultimediaSessionWork()
{
    delete d_ptr;
}
