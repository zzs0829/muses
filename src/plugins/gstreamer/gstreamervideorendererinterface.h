#ifndef GSTREAMERVIDEORENDERERINTERFACE_H
#define GSTREAMERVIDEORENDERERINTERFACE_H

#include <Multimedia/multimediaglobal.h>
#include <QtCore/qobject.h>
#include <gst/gst.h>


HS_BEGIN_NAMESPACE

class GstreamerVideoRendererInterface : public QObject
{
    Q_OBJECT
public:
    explicit GstreamerVideoRendererInterface(QObject *parent = 0);
    virtual GstElement *videoSink() = 0;

    //stopRenderer() is called when the renderer element is stopped.
    //it can be reimplemented when video renderer can't detect
    //changes to NULL state but has to free video resources.
    virtual void stopRenderer() = 0;

    //the video output is configured, usually after the first paint event
    //(winId is known,
    virtual bool isReady() const  = 0;

signals:
    void sinkChanged();
    void readyChanged(bool);
};

HS_END_NAMESPACE

#endif // GSTREAMERVIDEORENDERERINTERFACE_H
