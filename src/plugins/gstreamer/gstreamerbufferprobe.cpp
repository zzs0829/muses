#include "gstreamerbufferprobe.h"
#include "gstutils.h"

HS_BEGIN_NAMESPACE

GstreamerBufferProbe::GstreamerBufferProbe(Flags flags)
#if GST_CHECK_VERSION(1,0,0)
    : m_capsProbeId(-1)
#else
    : m_caps(0)
#endif
    , m_bufferProbeId(-1)
    , m_flags(flags)
{
}

GstreamerBufferProbe::~GstreamerBufferProbe()
{
#if !GST_CHECK_VERSION(1,0,0)
    if (m_caps)
        gst_caps_unref(m_caps);
#endif
}

void GstreamerBufferProbe::addProbeToPad(GstPad *pad, bool downstream)
{
    if (GstCaps *caps = hs_gst_pad_get_current_caps(pad)) {
        probeCaps(caps);
        gst_caps_unref(caps);
    }
#if GST_CHECK_VERSION(1,0,0)
    if (m_flags & ProbeCaps) {
        m_capsProbeId = gst_pad_add_probe(
                    pad,
                    downstream
                        ? GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM
                        : GST_PAD_PROBE_TYPE_EVENT_UPSTREAM,
                    capsProbe,
                    this,
                    NULL);
    }
    if (m_flags & ProbeBuffers) {
        m_bufferProbeId = gst_pad_add_probe(
                    pad, GST_PAD_PROBE_TYPE_BUFFER, bufferProbe, this, NULL);
    }
#else
    Q_UNUSED(downstream);

    m_bufferProbeId = gst_pad_add_buffer_probe(pad, G_CALLBACK(bufferProbe), this);
#endif
}

void GstreamerBufferProbe::removeProbeFromPad(GstPad *pad)
{
#if GST_CHECK_VERSION(1,0,0)
    if (m_capsProbeId != -1) {
        gst_pad_remove_probe(pad, m_capsProbeId);
        m_capsProbeId = -1;
    }
    if (m_bufferProbeId != -1) {
        gst_pad_remove_probe(pad, m_bufferProbeId);
        m_bufferProbeId = -1;
    }
#else
    if (m_bufferProbeId != -1) {
        gst_pad_remove_buffer_probe(pad, m_bufferProbeId);
        m_bufferProbeId = -1;
        if (m_caps) {
            gst_caps_unref(m_caps);
            m_caps = 0;
        }
    }
#endif
}

void GstreamerBufferProbe::probeCaps(GstCaps *)
{
}

bool GstreamerBufferProbe::probeBuffer(GstBuffer *)
{
    return true;
}

#if GST_CHECK_VERSION(1,0,0)
GstPadProbeReturn GstreamerBufferProbe::capsProbe(
        GstPad *, GstPadProbeInfo *info, gpointer user_data)
{
    GstreamerBufferProbe * const control = static_cast<GstreamerBufferProbe *>(user_data);

    if (GstEvent * const event = gst_pad_probe_info_get_event(info)) {
        if (GST_EVENT_TYPE(event) == GST_EVENT_CAPS) {
            GstCaps *caps;
            gst_event_parse_caps(event, &caps);

            control->probeCaps(caps);
        }
    }
    return GST_PAD_PROBE_OK;
}

GstPadProbeReturn GstreamerBufferProbe::bufferProbe(
        GstPad *, GstPadProbeInfo *info, gpointer user_data)
{
    GstreamerBufferProbe * const control = static_cast<GstreamerBufferProbe *>(user_data);
    if (GstBuffer * const buffer = gst_pad_probe_info_get_buffer(info))
        return control->probeBuffer(buffer) ? GST_PAD_PROBE_OK : GST_PAD_PROBE_DROP;
    return GST_PAD_PROBE_OK;
}
#else
gboolean GstreamerBufferProbe::bufferProbe(GstElement *, GstBuffer *buffer, gpointer user_data)
{
    GstreamerBufferProbe * const control = static_cast<GstreamerBufferProbe *>(user_data);

    if (control->m_flags & ProbeCaps) {
        GstCaps *caps = gst_buffer_get_caps(buffer);
        if (caps && (!control->m_caps || !gst_caps_is_equal(control->m_caps, caps))) {
            qSwap(caps, control->m_caps);
            control->probeCaps(control->m_caps);
        }
        if (caps)
            gst_caps_unref(caps);
    }

    if (control->m_flags & ProbeBuffers) {
        return control->probeBuffer(buffer) ? TRUE : FALSE;
    } else {
        return TRUE;
    }
}
#endif

HS_END_NAMESPACE
