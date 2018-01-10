#ifndef GSTREAMERPLAYERSESSION_P_H
#define GSTREAMERPLAYERSESSION_P_H

#include "gstreamerplayersession.h"
#include <Multimedia/private/mediaplayersession_p.h>

class GstreamerPlayerSessionPrivate : public MediaPlayerSessionPrivate
{
public:
    GstreamerPlayerSessionPrivate(const QString &id, const QString &key);
};

#endif // GSTREAMERPLAYERSESSION_P_H
