#ifndef MULTIMEDIARESOURCESETINTERFACE_H
#define MULTIMEDIARESOURCESETINTERFACE_H

#include <Multimedia/multimediaglobal.h>
#include <QtCore/qobject.h>


HS_BEGIN_NAMESPACE

class MultimediaResourceSetInterface : public QObject
{
    Q_OBJECT
public:
    virtual bool isVideoEnabled() const = 0;
    virtual bool isGranted() const = 0;
    virtual bool isAvailable() const = 0;

    virtual void acquire() = 0;
    virtual void release() = 0;
    virtual void setVideoEnabled(bool enabled) = 0;

signals:
    void resourcesGranted();
    void resourcesLost();
    void resourcesDenied();
    void resourcesReleased();
    void availabilityChanged(bool available);

protected:
    MultimediaResourceSetInterface(QObject *parent = 0);
};

HS_END_NAMESPACE

#endif // MULTIMEDIARESOURCESETINTERFACE_H
