#ifndef ABSTRACTEXECUTOR_H
#define ABSTRACTEXECUTOR_H

#include <Multimedia/multimediaglobal.h>
#include <QtCore/qobject.h>


HS_BEGIN_NAMESPACE

class AbstractExecutor : public QObject
{
    Q_OBJECT
public:
    explicit AbstractExecutor(QObject *parent = 0);

    virtual void execute(const QString &command) = 0;

signals:

public slots:
};

HS_END_NAMESPACE

#endif // ABSTRACTEXECUTOR_H
