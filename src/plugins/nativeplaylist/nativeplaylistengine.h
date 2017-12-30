#ifndef NATIVEPLAYLISTENGINE_H
#define NATIVEPLAYLISTENGINE_H

#include <Multimedia/multimediaengine.h>
#include <Multimedia/mediacontent.h>
#include "nativefilefindworker.h"

class NativePlaylistEngine : public MultimediaEngine
{
    Q_OBJECT
public:
    NativePlaylistEngine(const QString &path,
                         const QStringList &nameFilters,
                         QObject *parent = 0);

public slots:
    void startFindWork();

signals:
    void mediaContentFound(const QList<MediaContent> &medias);
    void finished();

private slots:
    void _onFindWorkFinished(const QFileInfoList &filelist);

private:
    NativeFileFindWorker *m_findWorker;
    QString m_path;
    QStringList m_nameFilters;
    QFileInfoList m_filelist;
};

#endif // NATIVEPLAYLISTENGINE_H
