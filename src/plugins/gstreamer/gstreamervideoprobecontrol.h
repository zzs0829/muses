#ifndef GSTREAMERVIDEOPROBECONTROL_H
#define GSTREAMERVIDEOPROBECONTROL_H

class GstreamerVideoProbeControl
{
public:
    GstreamerVideoProbeControl();

};

/*

#include <Multimedia/mediavideoprobecontroller.h>
#include "gstreamerbufferprobe.h"

#include <QtCore/qmutex.h>

HS_BEGIN_NAMESPACE

class GstreamerVideoProbeControl
        : public MediaVideoProbeController
        , public GstreamerBufferProbe
{
    Q_OBJECT
public:
    explicit GstreamerVideoProbeControl(QObject *parent = 0);
    virtual ~QGstreamerVideoProbeControl();

    void probeCaps(GstCaps *caps) override;
    bool probeBuffer(GstBuffer *buffer) override;

    void startFlushing();
    void stopFlushing();

private slots:
    void frameProbed();

private:
    VideoSurfaceFormat m_format;
    VideoFrame m_pendingFrame;
    QMutex m_frameMutex;
#if GST_CHECK_VERSION(1,0,0)
    GstVideoInfo m_videoInfo;
#else
    int m_bytesPerLine;
#endif
    bool m_flushing;
    bool m_frameProbed; // true if at least one frame was probed
};

HS_END_NAMESPACE

*/

#endif // GSTREAMERVIDEOPROBECONTROL_H
