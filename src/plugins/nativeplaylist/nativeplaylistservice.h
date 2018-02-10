#ifndef NATIVEPLAYLISTSERVICE_H
#define NATIVEPLAYLISTSERVICE_H

#include <Multimedia/multimediaservice.h>
#include "nativeplaylistgobal.h"
#include "nativeplaylistsession.h"


class NativePlaylistService : public MultimediaService
{
    Q_OBJECT
public:
    NativePlaylistService(QObject *parent = 0);

    virtual MultimediaController* requestController(const char *uid) Q_DECL_OVERRIDE;

    virtual MultimediaSession* requestSession(const QString &key) Q_DECL_OVERRIDE;

    virtual void releaseSession(MultimediaSession *session) Q_DECL_OVERRIDE;

    virtual void availableSession(MultimediaSession *session) Q_DECL_OVERRIDE;

private:
    QMap<QString, NativePlaylistSession *> m_sessions;
};

#endif // NATIVEPLAYLISTSERVICE_H
