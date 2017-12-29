#ifndef NATIVEPLAYLISTENGINE_H
#define NATIVEPLAYLISTENGINE_H

#include <Multimedia/multimediaengine.h>
#include <Multimedia/mediacontent.h>
#include "nativefilefindworker.h"

class NativePlaylistEngine : public MultimediaEngine
{
    Q_OBJECT
public:
    explicit NativePlaylistEngine(const QString &path,
                                  const QStringList &nameFilters,
                                  QObject *parent = 0);

public slots:
    void startFindWork();

signals:

private slots:
    void _onFindWorkFinished(const QFileInfoList &filelist);

private:
    NativeFileFindWorker *m_findWorker;
    QString m_path;
    QStringList m_nameFilters;
    QFileInfoList m_filelist;
    QScopedPointer<MediaContent> m_originalContent;
    QScopedPointer<MediaContent> m_groupedContent;
};

#endif // NATIVEPLAYLISTENGINE_H
