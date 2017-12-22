#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../../../../src/plugins/nativeplaylist/nativefilefindworker.h"

static QStringList allnames = QString("*.mp3,*.mp4,*.mp5,*.so,*.so1,*.so3,*.so4,*.so5,*.so7").split(",");
static QStringList names = QString("*.mp3,*.mp4,*.mp5,*.so").split(",");

class NativeFileFindWorkerTest : public QObject
{
    Q_OBJECT

public:
    NativeFileFindWorkerTest();
    ~NativeFileFindWorkerTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1_data();
    void testCase1();

private:
    QList<QTemporaryFile *> allfiles;
    QList<QTemporaryFile *> aviafiles;
    QScopedPointer<QTemporaryDir> tempdir;
    NativeFileFindWorker *worker;
};

NativeFileFindWorkerTest::NativeFileFindWorkerTest():
    tempdir(new QTemporaryDir)
{
}

NativeFileFindWorkerTest::~NativeFileFindWorkerTest()
{
    qDeleteAll(allfiles);
    allfiles.clear();
    qDeleteAll(aviafiles);
    aviafiles.clear();
    tempdir.reset();
}

void NativeFileFindWorkerTest::initTestCase()
{
}

void NativeFileFindWorkerTest::cleanupTestCase()
{
}
#include <time.h>
void NativeFileFindWorkerTest::testCase1_data()
{

    QTest::addColumn<QString>("path");
    if(!tempdir->isValid())
        return;

    for(int i = 0; i < 300; ++i) {
        qsrand(QDateTime::currentMSecsSinceEpoch());
        int rand = qrand() % allnames.count();
        QString suffix = allnames.at(rand);
        bool findin = names.contains(suffix);
        suffix.remove("*");
        QTemporaryFile *file = new QTemporaryFile;
        file->setFileTemplate(tempdir->path() + "/XXXXXX" + suffix);
        file->open();
        file->close();
        QThread::msleep(1);
        if(findin) {
            aviafiles << file;
        }else
            allfiles << file;
    }

    QTest::newRow("path") << tempdir->path();
}

void NativeFileFindWorkerTest::testCase1()
{
    QFETCH(QString, path);
    worker = new NativeFileFindWorker(path, names);
    worker->start();
    worker->waitForFinished();


    qDebug() << worker->filelist().count() << aviafiles.count() << path;
    QVERIFY2(worker->filelist().count() == aviafiles.count(), "Failure");
}

QTEST_MAIN(NativeFileFindWorkerTest)

#include "tst_nativefilefindworkertest.moc"
