#ifndef NATIVEPLAYLISTSERVICE_H
#define NATIVEPLAYLISTSERVICE_H

#include <Multimedia/multimediaservice.h>


class NativePlaylistService : public MultimediaService
{
    Q_OBJECT
public:
    NativePlaylistService(const QString &path, const QString &nameFilters,
                          QObject *parent = 0);

    virtual MultimediaController* requestController(const char *uid) Q_DECL_OVERRIDE;

private:
    void formatNameFilters(const QStringList &filters);

    MultimediaController *m_control;
    QString m_rootPath;
    QString m_nameFilters;
};

#endif // NATIVEPLAYLISTSERVICE_H
