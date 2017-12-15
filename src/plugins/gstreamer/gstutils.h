#ifndef GSTUTILS_H
#define GSTUTILS_H

#include <QtCore/qmap.h>
#include <QtCore/qset.h>
#include <QtCore/qvector.h>
#include <QtCore/qvariant.h>
#include <gst/gst.h>

#if GST_CHECK_VERSION(1,0,0)
# define HS_GSTREAMER_PLAYBIN_ELEMENT_NAME "playbin"
# define HS_GSTREAMER_CAMERABIN_ELEMENT_NAME "camerabin"
# define HS_GSTREAMER_COLORCONVERSION_ELEMENT_NAME "videoconvert"
# define HS_GSTREAMER_RAW_AUDIO_MIME "audio/x-raw"
# define HS_GSTREAMER_VIDEOOVERLAY_INTERFACE_NAME "GstVideoOverlay"
#else
# define HS_GSTREAMER_PLAYBIN_ELEMENT_NAME "playbin2"
# define HS_GSTREAMER_CAMERABIN_ELEMENT_NAME "camerabin2"
# define HS_GSTREAMER_COLORCONVERSION_ELEMENT_NAME "ffmpegcolorspace"
# define HS_GSTREAMER_RAW_AUDIO_MIME "audio/x-raw-int"
# define HS_GSTREAMER_VIDEOOVERLAY_INTERFACE_NAME "GstXOverlay"
#endif

typedef enum {
  GST_PLAY_FLAG_VIDEO         = (1 << 0),
  GST_PLAY_FLAG_AUDIO         = (1 << 1),
  GST_PLAY_FLAG_TEXT          = (1 << 2),
  GST_PLAY_FLAG_VIS           = (1 << 3),
  GST_PLAY_FLAG_SOFT_VOLUME   = (1 << 4),
  GST_PLAY_FLAG_NATIVE_AUDIO  = (1 << 5),
  GST_PLAY_FLAG_NATIVE_VIDEO  = (1 << 6),
  GST_PLAY_FLAG_DOWNLOAD      = (1 << 7),
  GST_PLAY_FLAG_BUFFERING     = (1 << 8),
  GST_PLAY_FLAG_DEINTERLACE   = (1 << 9),
  GST_PLAY_FLAG_SOFT_COLORBALANCE = (1 << 10)
} GstPlayFlags;

namespace GstUtils {

    void initializeGst();

    QMap<QByteArray, QVariant> gstTagListToMap(const GstTagList *list);

}

void hs_gst_object_ref_sink(gpointer object);
GstStructure *hs_gst_structure_new_empty(const char *name);
GstCaps *hs_gst_pad_get_current_caps(GstPad *pad);
gboolean hs_gst_element_query_position(GstElement *element, GstFormat format, gint64 *cur);
gboolean hs_gst_element_query_duration(GstElement *element, GstFormat format, gint64 *cur);

#endif // GSTUTILS_H
