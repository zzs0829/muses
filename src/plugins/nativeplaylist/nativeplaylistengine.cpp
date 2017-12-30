#include "nativeplaylistengine.h"

NativePlaylistEngine::NativePlaylistEngine(const QString &path,
                                           const QStringList &nameFilters,
                                           QObject *parent) :
    MultimediaEngine(parent),
    m_findWorker(0),
    m_path(path),
    m_nameFilters(nameFilters)
{
}

// public slots:
void NativePlaylistEngine::startFindWork()
{
    if(m_findWorker) {
        m_findWorker->disconnect();
        m_findWorker->requestInterruption();
    }

    m_findWorker = new NativeFileFindWorker(this, m_path, m_nameFilters);
    qRegisterMetaType<QFileInfoList>("QFileInfoList");
    connect(m_findWorker, SIGNAL(findWorkFinished(QFileInfoList)),
            SLOT(_onFindWorkFinished(QFileInfoList)));
    connect(m_findWorker, SIGNAL(finished()), m_findWorker, SLOT(deleteLater()));
    m_findWorker->start();
}

// private slots:
void NativePlaylistEngine::_onFindWorkFinished(const QFileInfoList &filelist)
{
    m_filelist = filelist;
    QList<MediaContent> medias;
    foreach (const QFileInfo &file, m_filelist) {
        QUrl fileUrl = QUrl::fromLocalFile(file.absoluteFilePath());
        medias << MediaContent(fileUrl);
    }

    emit mediaContentFound(medias);
    emit finished();

#if 0
    QScopedPointer<MediaContent> m_originalContent;
    QScopedPointer<MediaContent> m_groupedContent;
    m_originalContent(new MediaContent(QUrl::fromLocalFile(path))),
    m_groupedContent(new MediaContent(QUrl::fromLocalFile(path)))


    MediaContent original(QUrl::fromLocalFile(m_path));
    MediaContent grouped(QUrl::fromLocalFile(m_path));
    QMap<QString, MediaContent> group;
    group.insert(m_path, grouped);

    foreach (const QFileInfo &file, m_filelist) {
        QUrl fileUrl = QUrl::fromLocalFile(file.absoluteFilePath());
        MediaContent content(fileUrl);

        original << content;

        QString parentPath = file.absoluteFilePath();
        do {
            parentPath = QFileInfo(parentPath).absoluteDir().path();
            QUrl parentUrl = QUrl::fromLocalFile(parentPath);
            if(!group.contains(parentPath)) {
                group.insert(parentPath, MediaContent(parentUrl));
            }
            MediaContent parentGroup = group.value(parentPath);
            parentGroup.append(content);
            content = parentGroup;
        }
        while(parentPath != m_path);
    }

//    original.sort();
    grouped.sort();

    m_originalContent.reset(&original);
    m_groupedContent.reset(&grouped);
#endif
}
