#include "gstreamerbushelper.h"
#include "gstreamerbushelper_p.h"

HS_BEGIN_NAMESPACE


static GstBusSyncReply syncGstBusFilter(GstBus* bus, GstMessage* message,
                                        GstreamerBusHelperPrivate *d)
{
    Q_UNUSED(bus);
    QMutexLocker lock(&d->filterMutex);

    for (GstreamerSyncMessageFilter *filter : qAsConst(d->syncFilters)) {
        if (filter->processSyncMessage(GstreamerMessage(message)))
            return GST_BUS_DROP;
    }

    return GST_BUS_PASS;
}

GstreamerBusHelper::GstreamerBusHelper(GstBus* bus, QObject *parent) :
    QObject(parent)
{
    d = new GstreamerBusHelperPrivate(this, bus);
#if GST_CHECK_VERSION(1,0,0)
    gst_bus_set_sync_handler(bus, (GstBusSyncHandler)syncGstBusFilter, d, 0);
#else
    gst_bus_set_sync_handler(bus, (GstBusSyncHandler)syncGstBusFilter, d);
#endif
    gst_object_ref(GST_OBJECT(bus));
}

GstreamerBusHelper::~GstreamerBusHelper()
{
#if GST_CHECK_VERSION(1,0,0)
    gst_bus_set_sync_handler(d->bus(), 0, 0, 0);
#else
    gst_bus_set_sync_handler(d->bus(),0,0);
#endif
    gst_object_unref(GST_OBJECT(d->bus()));
}

void GstreamerBusHelper::installMessageFilter(QObject *filter)
{
    GstreamerSyncMessageFilter *syncFilter = qobject_cast<GstreamerSyncMessageFilter*>(filter);
    if (syncFilter) {
        QMutexLocker lock(&d->filterMutex);
        if (!d->syncFilters.contains(syncFilter))
            d->syncFilters.append(syncFilter);
    }

    GstreamerBusMessageFilter *busFilter = qobject_cast<GstreamerBusMessageFilter*>(filter);
    if (busFilter && !d->busFilters.contains(busFilter))
        d->busFilters.append(busFilter);
}

void GstreamerBusHelper::removeMessageFilter(QObject *filter)
{
    GstreamerSyncMessageFilter *syncFilter = qobject_cast<GstreamerSyncMessageFilter*>(filter);
    if (syncFilter) {
        QMutexLocker lock(&d->filterMutex);
        d->syncFilters.removeAll(syncFilter);
    }

    GstreamerBusMessageFilter *busFilter = qobject_cast<GstreamerBusMessageFilter*>(filter);
    if (busFilter)
        d->busFilters.removeAll(busFilter);
}

HS_END_NAMESPACE
