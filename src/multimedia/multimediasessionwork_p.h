#ifndef MULTIMEDIASESSIONWORK_P_H
#define MULTIMEDIASESSIONWORK_P_H

#include <Multimedia/private/multimediaglobal_p.h>
#include <Multimedia/multimediasessionwork.h>

HS_BEGIN_NAMESPACE

class MULTIMEDIA_PRIVATE_EXPORT MultimediaSessionWorkPrivate
{
    Q_DECLARE_PUBLIC(MultimediaSessionWork)
public:
    MultimediaSessionWorkPrivate(MultimediaSession *s, MultimediaController *c);
    virtual ~MultimediaSessionWorkPrivate();

    MultimediaSessionWork *q_ptr;
    MultimediaSession *session;
    MultimediaController *control;
};

HS_END_NAMESPACE

#endif // MULTIMEDIASESSIONWORK_P_H
