#ifndef NATIVEFILEFINDWORKER_H
#define NATIVEFILEFINDWORKER_H

#include <QThread>
#include <QDir>
#include <QMutex>
#include <QWaitCondition>
#include <QFileInfo>

class NativeFileFindWorker : public QThread
{
    Q_OBJECT
public:
    explicit NativeFileFindWorker(QObject *parent = 0);
    ~NativeFileFindWorker();

    void clear();
    void removePath(const QString &path);
    void setPath(const QString &path);
    void setRootPath(const QString &path);
    void setSortFlags(QDir::SortFlags flags);
    void setNameFilters(const QStringList & nameFilters);
    void setShowFiles(bool show);
    void setShowDirs(bool showFolders);
    void setShowDirsFirst(bool show);
    void setShowDotAndDotDot(bool on);
    void setShowHidden(bool on);
    void setShowOnlyReadable(bool on);
    void setCaseSensitive(bool on);

signals:
    void directoryChanged(const QString &directory, const QList<QFileInfo> &list) const;

public slots:

protected:
    void run() Q_DECL_OVERRIDE;
    void findFileInfos(const QString &path);

private:
    QMutex mutex;
    QWaitCondition condition;
    volatile bool abort;
    QList<QFileInfo> currentFileList;
    QDir::SortFlags sortFlags;
    QString currentPath;
    QString rootPath;
    QStringList nameFilters;
    bool needUpdate;
    bool folderUpdate;
    bool sortUpdate;
    bool showFiles;
    bool showDirs;
    bool showDirsFirst;
    bool showDotAndDotDot;
    bool showHidden;
    bool showOnlyReadable;
    bool caseSensitive;
};

#endif // NATIVEFILEFINDWORKER_H
