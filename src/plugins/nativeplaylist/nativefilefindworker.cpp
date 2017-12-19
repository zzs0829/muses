#include "nativefilefindworker.h"
#include <QDirIterator>
#include <QDebug>
#ifdef TIME_DEBUG
#include <QTime>
#endif

NativeFileFindWorker::NativeFileFindWorker(QObject *parent)
    : QThread(parent),
      abort(false),
      sortFlags(QDir::Name),
      needUpdate(false),
      folderUpdate(false),
      sortUpdate(false),
      showFiles(true),
      showDirs(false),
      showDirsFirst(false),
      showDotAndDotDot(false),
      showHidden(false),
      showOnlyReadable(false),
      caseSensitive(false)
{

}

NativeFileFindWorker::~NativeFileFindWorker()
{
    QMutexLocker locker(&mutex);
    abort = true;
    condition.wakeOne();
    locker.unlock();
    wait();
}

void NativeFileFindWorker::clear()
{
    QMutexLocker locker(&mutex);
}

void NativeFileFindWorker::removePath(const QString &path)
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(path);
    currentPath.clear();
}

void NativeFileFindWorker::setPath(const QString &path)
{
    Q_ASSERT(!path.isEmpty());

    QMutexLocker locker(&mutex);
    currentPath = path;
    needUpdate = true;
    if(this->isRunning()) {
        condition.wakeAll();
    }
}

void NativeFileFindWorker::setRootPath(const QString &path)
{
    Q_ASSERT(!path.isEmpty());

    QMutexLocker locker(&mutex);
    rootPath = path;
}

void NativeFileFindWorker::setSortFlags(QDir::SortFlags flags)
{
    QMutexLocker locker(&mutex);
    sortFlags = flags;
    if(this->isRunning()) {
        sortUpdate = true;
        condition.wakeAll();
    }
}

void NativeFileFindWorker::setNameFilters(const QStringList & filters)
{
    QMutexLocker locker(&mutex);
    nameFilters = filters;
    if(this->isRunning()) {
        folderUpdate = true;
        condition.wakeAll();
    }
}

void NativeFileFindWorker::setShowFiles(bool show)
{
    QMutexLocker locker(&mutex);
    showFiles = show;
    if(this->isRunning()) {
        folderUpdate = true;
        condition.wakeAll();
    }
}

void NativeFileFindWorker::setShowDirs(bool showFolders)
{
    QMutexLocker locker(&mutex);
    showDirs = showFolders;
    if(this->isRunning()) {
        folderUpdate = true;
        condition.wakeAll();
    }
}

void NativeFileFindWorker::setShowDirsFirst(bool show)
{
    QMutexLocker locker(&mutex);
    showDirsFirst = show;
    if(this->isRunning()) {
        folderUpdate = true;
        condition.wakeAll();
    }
}

void NativeFileFindWorker::setShowDotAndDotDot(bool on)
{
    QMutexLocker locker(&mutex);
    showDotAndDotDot = on;
    if(this->isRunning()) {
        folderUpdate = true;
        needUpdate = true;
        condition.wakeAll();
    }
}

void NativeFileFindWorker::setShowHidden(bool on)
{
    QMutexLocker locker(&mutex);
    showHidden = on;
    if(this->isRunning()) {
        folderUpdate = true;
        needUpdate = true;
        condition.wakeAll();
    }
}

void NativeFileFindWorker::setShowOnlyReadable(bool on)
{
    QMutexLocker locker(&mutex);
    showOnlyReadable = on;
    if(this->isRunning()) {
        folderUpdate = true;
        condition.wakeAll();
    }
}

void NativeFileFindWorker::setCaseSensitive(bool on)
{
    QMutexLocker locker(&mutex);
    caseSensitive = on;
    if(this->isRunning()) {
        folderUpdate = true;
        condition.wakeAll();
    }
}

void NativeFileFindWorker::run()
{
    forever {
        bool updateFiles = false;
        QMutexLocker locker(&mutex);
        if (abort) {
            return;
        }
        if (currentPath.isEmpty() || !needUpdate)
            condition.wait(&mutex);

        if (abort) {
            return;
        }

        if (!currentPath.isEmpty()) {
            updateFiles = true;
        }
        if (updateFiles) {
            findFileInfos(currentPath);
        }
        locker.unlock();
    }
}


void NativeFileFindWorker::findFileInfos(const QString &path)
{
#ifdef TIME_DEBUG
            QTime time;
            time.start();
#endif

    QDir::Filters filter;
    if (caseSensitive)
        filter = QDir::CaseSensitive;
    if (showFiles)
        filter = filter | QDir::Files;
    if (showDirs)
        filter = filter | QDir::AllDirs | QDir::Drives;
    if (!showDotAndDotDot)
        filter = filter | QDir::NoDot | QDir::NoDotDot;
    else if (path == rootPath)
        filter = filter | QDir::NoDotDot;
    if (showHidden)
        filter = filter | QDir::Hidden;
    if (showOnlyReadable)
        filter = filter | QDir::Readable;
    if (showDirsFirst)
        sortFlags = sortFlags | QDir::DirsFirst;

    QList<QFileInfo> filelist;
    QDirIterator it(path, nameFilters, filter, QDirIterator::Subdirectories);

    while (it.hasNext()) {
          filelist << QFileInfo(it.next());
          qDebug() << filelist.last().filePath();
    }

#ifdef TIME_DEBUG
    int times = time.elapsed();
    if(times > 1000)
        qDebug() << "Find files times: " << times/1000.0 << "s" << "( PATH:" << path << " )";
    else
        qDebug() << "Find files times: " << times << "ms" << "( PATH:" << path << " )";
#endif

    emit directoryChanged(path, filelist);
    needUpdate = false;
}
