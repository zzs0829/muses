#ifndef NATIVEPLAYLISTSERVICE_H
#define NATIVEPLAYLISTSERVICE_H

#include <Multimedia/multimediaservice.h>


class NativePlaylistService : public MultimediaService
{
    Q_OBJECT
public:
    NativePlaylistService(const QString &path, const QString &filters,
                          QObject *parent = 0);

    virtual MultimediaController* requestController(const char *uid) Q_DECL_OVERRIDE;

private:
    MultimediaController *m_control;
};

#endif // NATIVEPLAYLISTSERVICE_H
