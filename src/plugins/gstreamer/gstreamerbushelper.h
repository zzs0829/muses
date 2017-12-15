#ifndef GSTREAMERBUSHELPER_H
#define GSTREAMERBUSHELPER_H

#include <Multimedia/multimediaglobal.h>
#include "gstreamermessage.h"

#include <QtCore/qobject.h>
#include <gst/gst.h>


HS_BEGIN_NAMESPACE

class GstreamerSyncMessageFilter {
public:
    //returns true if message was processed and should be dropped, false otherwise
    virtual bool processSyncMessage(const GstreamerMessage &message) = 0;
};
#define GstreamerSyncMessageFilter_iid "module.muses.gstreamersyncmessagefilter/1.0"
Q_DECLARE_INTERFACE(GstreamerSyncMessageFilter, GstreamerSyncMessageFilter_iid)

class GstreamerBusMessageFilter {
public:
    //returns true if message was processed and should be dropped, false otherwise
    virtual bool processBusMessage(const GstreamerMessage &message) = 0;
};
#define GstreamerBusMessageFilter_iid "module.muses.gstreamerbusmessagefilter/1.0"
Q_DECLARE_INTERFACE(GstreamerBusMessageFilter, GstreamerBusMessageFilter_iid)

class GstreamerBusHelperPrivate;
class GstreamerBusHelper : public QObject
{
    Q_OBJECT
    friend class GstreamerBusHelperPrivate;
public:
    explicit GstreamerBusHelper(GstBus* bus, QObject *parent = 0);
    ~GstreamerBusHelper();

    void installMessageFilter(QObject *filter);
    void removeMessageFilter(QObject *filter);

signals:
    void message(GstreamerMessage const& message);

private:
    GstreamerBusHelperPrivate* d;
};

HS_END_NAMESPACE

#endif // GSTREAMERBUSHELPER_H
