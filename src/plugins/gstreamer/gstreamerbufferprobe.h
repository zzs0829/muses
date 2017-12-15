#ifndef GSTREAMERBUFFERPROBE_H
#define GSTREAMERBUFFERPROBE_H

#include <Multimedia/multimediaglobal.h>
#include <gst/gst.h>

HS_BEGIN_NAMESPACE

class GstreamerBufferProbe
{
public:
    enum Flags
    {
        ProbeCaps       = 0x01,
        ProbeBuffers    = 0x02,
        ProbeAll    = ProbeCaps | ProbeBuffers
    };

    explicit GstreamerBufferProbe(Flags flags = ProbeAll);
    virtual ~GstreamerBufferProbe();

    void addProbeToPad(GstPad *pad, bool downstream = true);
    void removeProbeFromPad(GstPad *pad);

protected:
    virtual void probeCaps(GstCaps *caps);
    virtual bool probeBuffer(GstBuffer *buffer);

private:
#if GST_CHECK_VERSION(1,0,0)
    static GstPadProbeReturn capsProbe(GstPad *pad, GstPadProbeInfo *info, gpointer user_data);
    static GstPadProbeReturn bufferProbe(GstPad *pad, GstPadProbeInfo *info, gpointer user_data);
    int m_capsProbeId;
#else
    static gboolean bufferProbe(GstElement *element, GstBuffer *buffer, gpointer user_data);
    GstCaps *m_caps;
#endif
    int m_bufferProbeId;
    const Flags m_flags;
};

HS_END_NAMESPACE

#endif // GSTREAMERBUFFERPROBE_H
