#include "nativeplaylistengine.h"

NativePlaylistEngine::NativePlaylistEngine(const QString &path,
                                           const QStringList &nameFilters,
                                           QObject *parent) :
    MultimediaEngine(parent),
    m_findWorker(0),
    m_path(path),
    m_nameFilters(nameFilters),
    m_originalContent(new MediaContent(QUrl::fromLocalFile(path))),
    m_groupedContent(new MediaContent(QUrl::fromLocalFile(path)))
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
    connect(m_findWorker, SIGNAL(findWorkFinished(QFileInfoList)),
            SLOT(_onFindWorkFinished(QFileInfoList)));
    connect(m_findWorker, SIGNAL(finished()), m_findWorker, SLOT(deleteLater()));
    m_findWorker->start();
}

// private slots:
void NativePlaylistEngine::_onFindWorkFinished(const QFileInfoList &filelist)
{
    m_filelist = filelist;

    MediaContent original(QUrl::fromLocalFile(m_path));
    MediaContent grouped(QUrl::fromLocalFile(m_path));
    QMap<QString, MediaContent> group;
    group.insert(m_path, grouped);

    foreach (const QFileInfo &file, m_filelist) {
        QUrl fileUrl = QUrl::fromLocalFile(file.absoluteFilePath());
        original << MediaContent(fileUrl);


        QString parentPath = file.absoluteFilePath();
        do {
            MediaContent content(QUrl::fromLocalFile(parentPath));
            parentPath = QFileInfo(parentPath).absoluteDir().path();
            QUrl parentUrl = QUrl::fromLocalFile(parentPath);
            MediaContent parentGroup = group.value(parentPath, MediaContent(parentUrl));
            parentGroup.append(content);
        }while(parentPath != m_path);
    }

    original.sort();
    grouped.sort();

    m_originalContent.reset(&original);
    m_groupedContent.reset(&grouped);
}
