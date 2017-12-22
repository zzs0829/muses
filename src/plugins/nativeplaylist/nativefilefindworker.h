#ifndef NATIVEFILEFINDWORKER_H
#define NATIVEFILEFINDWORKER_H

#include <QThread>
#include <QDir>
#include <QReadWriteLock>
#include <QMutex>
#include <QWaitCondition>
#include <QFileInfo>

class NativeFileFindWorker : public QThread
{
    Q_OBJECT
public:
    NativeFileFindWorker(const QString &path,
                         const QStringList &nameFilters = QStringList(),
                         bool hidden = false,
                         bool caseSensitive = false);
    NativeFileFindWorker(QObject *parent,
                         const QString &path,
                         const QStringList &nameFilters = QStringList(),
                         bool hidden = false,
                         bool caseSensitive = false);
    ~NativeFileFindWorker();

    QFileInfoList filelist();
    QString path() const;
    void waitForFinished();

protected:
    void run() Q_DECL_OVERRIDE;

private:
    mutable QMutex mutex;
    QWaitCondition waitForFinishedCondition;
#if (QT_VERSION < QT_VERSION_CHECK(5, 2, 0))
    volatile bool abort;
#endif
    QFileInfoList m_filelist;
    QString m_path;
    QStringList m_nameFilters;
    QDir::Filters m_filters;
};

#endif // NATIVEFILEFINDWORKER_H
