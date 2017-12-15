#include "gstreamervideoprobecontrol.h"

GstreamerVideoProbeControl::GstreamerVideoProbeControl()
{

}

/*

HS_BEGIN_NAMESPACE

GstreamerVideoProbeControl::GstreamerVideoProbeControl(QObject *parent)
    : MediaVideoProbeController(parent)
    , m_flushing(false)
    , m_frameProbed(false)
{
}

GstreamerVideoProbeControl::~GstreamerVideoProbeControl()
{
}

void GstreamerVideoProbeControl::startFlushing()
{
    m_flushing = true;

    {
        QMutexLocker locker(&m_frameMutex);
        m_pendingFrame = QVideoFrame();
    }

    // only emit flush if at least one frame was probed
    if (m_frameProbed)
        emit flush();
}

void GstreamerVideoProbeControl::stopFlushing()
{
    m_flushing = false;
}

void GstreamerVideoProbeControl::probeCaps(GstCaps *caps)
{
#if GST_CHECK_VERSION(1,0,0)
    GstVideoInfo videoInfo;
    QVideoSurfaceFormat format = QGstUtils::formatForCaps(caps, &videoInfo);

    QMutexLocker locker(&m_frameMutex);
    m_videoInfo = videoInfo;
#else
    int bytesPerLine = 0;
    QVideoSurfaceFormat format = QGstUtils::formatForCaps(caps, &bytesPerLine);

    QMutexLocker locker(&m_frameMutex);
    m_bytesPerLine = bytesPerLine;
#endif
    m_format = format;
}

bool GstreamerVideoProbeControl::probeBuffer(GstBuffer *buffer)
{
    QMutexLocker locker(&m_frameMutex);

    if (m_flushing || !m_format.isValid())
        return true;

    QVideoFrame frame(
#if GST_CHECK_VERSION(1,0,0)
                new QGstVideoBuffer(buffer, m_videoInfo),
#else
                new QGstVideoBuffer(buffer, m_bytesPerLine),
#endif
                m_format.frameSize(),
                m_format.pixelFormat());

    QGstUtils::setFrameTimeStamps(&frame, buffer);

    m_frameProbed = true;

    if (!m_pendingFrame.isValid())
        QMetaObject::invokeMethod(this, "frameProbed", Qt::QueuedConnection);
    m_pendingFrame = frame;

    return true;
}

void GstreamerVideoProbeControl::frameProbed()
{
    QVideoFrame frame;
    {
        QMutexLocker locker(&m_frameMutex);
        if (!m_pendingFrame.isValid())
            return;
        frame = m_pendingFrame;
        m_pendingFrame = QVideoFrame();
    }
    emit videoFrameProbed(frame);
}

HS_END_NAMESPACE

*/
