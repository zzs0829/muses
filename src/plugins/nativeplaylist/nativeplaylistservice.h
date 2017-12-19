#ifndef NATIVEPLAYLISTSERVICE_H
#define NATIVEPLAYLISTSERVICE_H

#include <Multimedia/multimediaservice.h>


class NativePlaylistService : public MultimediaService
{
    Q_OBJECT
public:
    explicit NativePlaylistService(QObject *parent = 0);

    virtual MultimediaController* requestController(const char *uid) Q_DECL_OVERRIDE;

private:
    MultimediaController *m_control;
};

#endif // NATIVEPLAYLISTSERVICE_H
