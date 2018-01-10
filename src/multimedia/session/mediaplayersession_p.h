#ifndef MEDIAPLAYERSESSION_P_H
#define MEDIAPLAYERSESSION_P_H

#include <Multimedia/private/multimediaglobal_p.h>
#include <Multimedia/private/multimediasession_p.h>

HS_BEGIN_NAMESPACE

class MULTIMEDIA_PRIVATE_EXPORT MediaPlayerSessionPrivate :
        public MultimediaSessionPrivate
{
public:
    MediaPlayerSessionPrivate(const QString &id, const QString &key);
};

HS_END_NAMESPACE

#endif // MEDIAPLAYERSESSION_P_H
