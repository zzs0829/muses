#ifndef AVABSTRACTADAPTOR_H
#define AVABSTRACTADAPTOR_H

#include <Multimedia/multimediaglobal.h>
#include <QtCore/qobject.h>


HS_BEGIN_NAMESPACE

class AVAbstractAdaptorPrivate;
class MULTIMEDIA_EXPORT AVAbstractAdaptor : public QObject
{
    Q_OBJECT
public:
    ~AVAbstractAdaptor();

protected:
    explicit AVAbstractAdaptor(QObject *parent = 0);

    // 准备资源，不需立即播放
    virtual void prepareSource(const QUrl &source) = 0;

signals:

private:
    Q_DECLARE_PRIVATE(AVAbstractAdaptor)

};

HS_END_NAMESPACE

#endif // AVABSTRACTADAPTOR_H
