#ifndef MULTIMEDIARESOURCESETINTERFACE_H
#define MULTIMEDIARESOURCESETINTERFACE_H

#include <Multimedia/multimediaglobal.h>
#include <QtCore/qobject.h>


HS_BEGIN_NAMESPACE

#define MultimediaResourceSetInterface_iid \
    "module.muses.multimediaresourcesetinterface/1.0"

class MULTIMEDIA_EXPORT MultimediaResourceSetInterface : public QObject
{
    Q_OBJECT
public:
    virtual ~MultimediaResourceSetInterface();

    virtual bool isVideoEnabled() const = 0; // 是否能播放视频
    virtual bool isGranted() const = 0; // 是否获取
    virtual bool isAvailable() const = 0; // 是否可获取

    virtual void acquire() = 0;
    virtual void release() = 0;
    virtual void setVideoEnabled(bool enabled) = 0;

    static QString iid();

signals:
    void resourcesGranted(); // 已获取
    void resourcesLost(); // 已失去
    void resourcesDenied(); // 已拒绝
    void resourcesReleased(); // 已释放
    void availabilityChanged(bool available); // 可获取

protected:
    MultimediaResourceSetInterface(QObject *parent = 0);
};

HS_END_NAMESPACE

#endif // MULTIMEDIARESOURCESETINTERFACE_H
