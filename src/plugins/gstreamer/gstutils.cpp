#include "gstutils.h"
#include <QDate>
#include <QImage>


//internal
static void addTagToMap(const GstTagList *list,
                        const gchar *tag,
                        gpointer user_data)
{
    QMap<QByteArray, QVariant> *map = reinterpret_cast<QMap<QByteArray, QVariant>* >(user_data);

    GValue val;
    val.g_type = 0;
    gst_tag_list_copy_value(&val,list,tag);

    switch( G_VALUE_TYPE(&val) ) {
        case G_TYPE_STRING:
        {
            const gchar *str_value = g_value_get_string(&val);
            map->insert(QByteArray(tag), QString::fromUtf8(str_value));
            break;
        }
        case G_TYPE_INT:
            map->insert(QByteArray(tag), g_value_get_int(&val));
            break;
        case G_TYPE_UINT:
            map->insert(QByteArray(tag), g_value_get_uint(&val));
            break;
        case G_TYPE_LONG:
            map->insert(QByteArray(tag), qint64(g_value_get_long(&val)));
            break;
        case G_TYPE_BOOLEAN:
            map->insert(QByteArray(tag), g_value_get_boolean(&val));
            break;
        case G_TYPE_CHAR:
#if GLIB_CHECK_VERSION(2,32,0)
            map->insert(QByteArray(tag), g_value_get_schar(&val));
#else
            map->insert(QByteArray(tag), g_value_get_char(&val));
#endif
            break;
        case G_TYPE_DOUBLE:
            map->insert(QByteArray(tag), g_value_get_double(&val));
            break;
        default:
            // GST_TYPE_DATE is a function, not a constant, so pull it out of the switch
#if GST_CHECK_VERSION(1,0,0)
            if (G_VALUE_TYPE(&val) == G_TYPE_DATE) {
                const GDate *date = (const GDate *)g_value_get_boxed(&val);
#else
            if (G_VALUE_TYPE(&val) == GST_TYPE_DATE) {
                const GDate *date = gst_value_get_date(&val);
#endif
                if (g_date_valid(date)) {
                    int year = g_date_get_year(date);
                    int month = g_date_get_month(date);
                    int day = g_date_get_day(date);
                    map->insert(QByteArray(tag), QDate(year,month,day));
                    if (!map->contains("year"))
                        map->insert("year", year);
                }
#if GST_CHECK_VERSION(1,0,0)
            } else if (G_VALUE_TYPE(&val) == GST_TYPE_DATE_TIME) {
                const GstDateTime *dateTime = (const GstDateTime *)g_value_get_boxed(&val);
                int year = gst_date_time_has_year(dateTime) ? gst_date_time_get_year(dateTime) : 0;
                int month = gst_date_time_has_month(dateTime) ? gst_date_time_get_month(dateTime) : 0;
                int day = gst_date_time_has_day(dateTime) ? gst_date_time_get_day(dateTime) : 0;
                if (gst_date_time_has_time(dateTime)) {
                    int hour = gst_date_time_get_hour(dateTime);
                    int minute = gst_date_time_get_minute(dateTime);
                    int second = gst_date_time_get_second(dateTime);
                    float tz = gst_date_time_get_time_zone_offset(dateTime);
                    QDateTime dateTime(QDate(year, month, day), QTime(hour, minute, second),
                                       Qt::OffsetFromUTC, tz * 60 * 60);
                    map->insert(QByteArray(tag), dateTime);
                } else if (year > 0 && month > 0 && day > 0) {
                    map->insert(QByteArray(tag), QDate(year,month,day));
                }
                if (!map->contains("year") && year > 0)
                    map->insert("year", year);
            } else if (G_VALUE_TYPE(&val) == GST_TYPE_SAMPLE) {
                GstSample *sample = (GstSample *)g_value_get_boxed(&val);
                GstCaps* caps = gst_sample_get_caps(sample);
                if (caps && !gst_caps_is_empty(caps)) {
                    GstStructure *structure = gst_caps_get_structure(caps, 0);
                    const gchar *name = gst_structure_get_name(structure);
                    if (QByteArray(name).startsWith("image/")) {
                        GstBuffer *buffer = gst_sample_get_buffer(sample);
                        if (buffer) {
                            GstMapInfo info;
                            gst_buffer_map(buffer, &info, GST_MAP_READ);
                            map->insert(QByteArray(tag), QImage::fromData(info.data, info.size, name));
                            gst_buffer_unmap(buffer, &info);
                        }
                    }
                }
#endif
            } else if (G_VALUE_TYPE(&val) == GST_TYPE_FRACTION) {
                int nom = gst_value_get_fraction_numerator(&val);
                int denom = gst_value_get_fraction_denominator(&val);

                if (denom > 0) {
                    map->insert(QByteArray(tag), double(nom)/denom);
                }
            }
            break;
    }

    g_value_unset(&val);
}

void GstUtils::initializeGst()
{
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        gst_init(NULL, NULL);
    }
}
/*!
  Convert GstTagList structure to QMap<QByteArray, QVariant>.

  Mapping to int, bool, char, string, fractions and date are supported.
  Fraction values are converted to doubles.
*/
QMap<QByteArray, QVariant> GstUtils::gstTagListToMap(const GstTagList *tags)
{
    QMap<QByteArray, QVariant> res;
    gst_tag_list_foreach(tags, addTagToMap, &res);

    return res;
}

void hs_gst_object_ref_sink(gpointer object)
{
#if GST_CHECK_VERSION(0,10,24)
    gst_object_ref_sink(object);
#else
    g_return_if_fail (GST_IS_OBJECT(object));

    GST_OBJECT_LOCK(object);
    if (G_LIKELY(GST_OBJECT_IS_FLOATING(object))) {
        GST_OBJECT_FLAG_UNSET(object, GST_OBJECT_FLOATING);
        GST_OBJECT_UNLOCK(object);
    } else {
        GST_OBJECT_UNLOCK(object);
        gst_object_ref(object);
    }
#endif
}

GstStructure *hs_gst_structure_new_empty(const char *name)
{
#if GST_CHECK_VERSION(1,0,0)
    return gst_structure_new_empty(name);
#else
    return gst_structure_new(name, NULL);
#endif
}

GstCaps *hs_gst_pad_get_current_caps(GstPad *pad)
{
#if GST_CHECK_VERSION(1,0,0)
    return gst_pad_get_current_caps(pad);
#else
    return gst_pad_get_negotiated_caps(pad);
#endif
}

gboolean hs_gst_element_query_position(GstElement *element, GstFormat format, gint64 *cur)
{
#if GST_CHECK_VERSION(1,0,0)
    return gst_element_query_position(element, format, cur);
#else
    return gst_element_query_position(element, &format, cur);
#endif
}

gboolean hs_gst_element_query_duration(GstElement *element, GstFormat format, gint64 *cur)
{
#if GST_CHECK_VERSION(1,0,0)
    return gst_element_query_duration(element, format, cur);
#else
    return gst_element_query_duration(element, &format, cur);
#endif
}
