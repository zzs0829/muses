#include "nativefilefindworker.h"
#include <QDirIterator>
#include <QDebug>
#ifdef TIME_DEBUG
#include <QTime>
#endif

NativeFileFindWorker::NativeFileFindWorker(const QString &path,
                                           const QStringList &nameFilters,
                                           bool hidden,
                                           bool caseSensitive):
    NativeFileFindWorker(0, path, nameFilters, hidden, caseSensitive)
{}

NativeFileFindWorker::NativeFileFindWorker(QObject *parent,
                                           const QString &path,
                                           const QStringList &nameFilters,
                                           bool hidden,
                                           bool caseSensitive)
    : QThread(parent),
#if (QT_VERSION < QT_VERSION_CHECK(5, 2, 0))
      abort(false),
#endif
      m_path(path),
      m_nameFilters(nameFilters),
      m_filters(QDir::Files | QDir::NoDot | QDir::NoDotDot)
{
    if(hidden)
        m_filters |= QDir::Hidden;
    if(caseSensitive)
        m_filters |= QDir::CaseSensitive;
}

NativeFileFindWorker::~NativeFileFindWorker()
{
#if (QT_VERSION < QT_VERSION_CHECK(5, 2, 0))
    abort = true;
#else
    requestInterruption();
#endif

    wait();

#if DEBUG_FIND
    qDebug() << m_path << "[ DISTROY ]";
#endif
}

QString NativeFileFindWorker::path() const
{
    return m_path;
}

void NativeFileFindWorker::waitForFinished()
{
    QMutexLocker locker(&mutex);
    if(isFinished()) // already finished
        return;

    waitForFinishedCondition.wait(&mutex);
}


void NativeFileFindWorker::run()
{
    QMutexLocker locker(&mutex);
#ifdef TIME_DEBUG
    QTime time;
    time.start();
#endif
    QFileInfoList filelist;

    if(!m_nameFilters.isEmpty()) {
        QDirIterator it(m_path, m_nameFilters, m_filters,
                        QDirIterator::Subdirectories);
        while (it.hasNext()) {
#if (QT_VERSION < QT_VERSION_CHECK(5, 2, 0))
            if (abort) return;
#else
            if (isInterruptionRequested()) return;
#endif
            filelist << QFileInfo(it.next());
        }
    }

//    std::sort(filelist.begin(), filelist.end(), [](const QFileInfo &info1, const QFileInfo &info2) {
//        QStringList list1 = info1.absoluteFilePath().split(QDir::separator());
//        QStringList list2 = info2.absoluteFilePath().split(QDir::separator());
//        for(int i = 0; i < qMin(list1.size(), list2.size()); ++i) {
//            if(i == (list1.size() - 1) && i < (list2.size() - 1))
//                return true;
//            else if(i == (list2.size() - 1) && i < (list1.size() - 1))
//                return false;
//            if(list1.at(i) != list2.at(i))
//                return list1.at(i) > list2.at(i);
//        }
//        return false;
//    });

#ifdef TIME_DEBUG
    float times = time.elapsed();
    QString unit = "ms";
    if(times > 1000) {
         times /= 1000.0;
         unit = "s";
    }
    qDebug(" %s  [ FOUND ] [ %f %s ]", qPrintable(m_path), times, qPrintable(unit));
#endif
    emit findWorkFinished(filelist);
    waitForFinishedCondition.wakeAll();
}
