#ifndef MEDIAPLAYLISTSESSION_P_H
#define MEDIAPLAYLISTSESSION_P_H

#include <Multimedia/private/multimediaglobal_p.h>
#include <Multimedia/private/multimediasession_p.h>

HS_BEGIN_NAMESPACE


class MULTIMEDIA_PRIVATE_EXPORT MediaPlaylistSessionPrivate:
        public MultimediaSessionPrivate
{
public:
    MediaPlaylistSessionPrivate(const QString &id, const QString &key);
};

HS_END_NAMESPACE

#endif // MEDIAPLAYLISTSESSION_P_H
