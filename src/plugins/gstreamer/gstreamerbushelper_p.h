#ifndef GSTREAMERBUSHELPER_P_H
#define GSTREAMERBUSHELPER_P_H

#include "gstreamerbushelper.h"
#include <QtCore/qmap.h>
#include <QtCore/qtimer.h>
#include <QtCore/qmutex.h>
#include <QtCore/qlist.h>

HS_BEGIN_NAMESPACE

class GstreamerBusHelperPrivate : public QObject
{
    Q_OBJECT
public:
    GstreamerBusHelperPrivate(GstreamerBusHelper *parent, GstBus* bus);
    ~GstreamerBusHelperPrivate();

    GstBus* bus() const { return m_bus; }

public:
    QMutex filterMutex;
    QList<GstreamerSyncMessageFilter*> syncFilters;
    QList<GstreamerBusMessageFilter*> busFilters;


private slots:
    void interval();
    void doProcessMessage(const GstreamerMessage& msg);

private:
    void processMessage(GstMessage* message);
    void queueMessage(GstMessage* message);
    static gboolean busCallback(GstBus *bus, GstMessage *message, gpointer data);


    guint m_tag;
    GstBus* m_bus;
    GstreamerBusHelper*  m_helper;
    QTimer* m_intervalTimer;
};

HS_END_NAMESPACE

#endif // GSTREAMERBUSHELPER_P_H
