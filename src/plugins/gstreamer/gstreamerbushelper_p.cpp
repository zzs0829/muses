#include "gstreamerbushelper_p.h"

#include <QtCore/qabstracteventdispatcher.h>
#include <QtCore/qcoreapplication.h>

HS_BEGIN_NAMESPACE

GstreamerBusHelperPrivate::GstreamerBusHelperPrivate(GstreamerBusHelper *parent, GstBus* bus) :
    QObject(parent),
    m_tag(0),
    m_bus(bus),
    m_helper(parent),
    m_intervalTimer(Q_NULLPTR)
{
    // glib event loop can be disabled either by env variable or QT_NO_GLIB define, so check the dispacher
    QAbstractEventDispatcher *dispatcher = QCoreApplication::eventDispatcher();
    const bool hasGlib = dispatcher && dispatcher->inherits("QEventDispatcherGlib");
    if (!hasGlib) {
        m_intervalTimer = new QTimer(this);
        m_intervalTimer->setInterval(250);
        connect(m_intervalTimer, SIGNAL(timeout()), SLOT(interval()));
        m_intervalTimer->start();
    } else {
        m_tag = gst_bus_add_watch_full(bus, G_PRIORITY_DEFAULT, busCallback, this, NULL);
    }

}

GstreamerBusHelperPrivate::~GstreamerBusHelperPrivate()
{
    m_helper = 0;
    delete m_intervalTimer;

    if (m_tag)
        g_source_remove(m_tag);
}

// private slots
void GstreamerBusHelperPrivate::interval()
{
    GstMessage* message;
    while ((message = gst_bus_poll(m_bus, GST_MESSAGE_ANY, 0)) != 0) {
        processMessage(message);
        gst_message_unref(message);
    }
}

void GstreamerBusHelperPrivate::doProcessMessage(const GstreamerMessage& msg)
{
    for (GstreamerBusMessageFilter *filter : qAsConst(busFilters)) {
        if (filter->processBusMessage(msg))
            break;
    }
    emit m_helper->message(msg);
}

// private
void GstreamerBusHelperPrivate::processMessage(GstMessage* message)
{
    GstreamerMessage msg(message);
    doProcessMessage(msg);
}

void GstreamerBusHelperPrivate::queueMessage(GstMessage* message)
{
    GstreamerMessage msg(message);
    QMetaObject::invokeMethod(this, "doProcessMessage", Qt::QueuedConnection,
                              Q_ARG(GstreamerMessage, msg));
}

gboolean GstreamerBusHelperPrivate::busCallback(GstBus *bus, GstMessage *message, gpointer data)
{
    Q_UNUSED(bus);
    reinterpret_cast<GstreamerBusHelperPrivate*>(data)->queueMessage(message);
    return TRUE;
}


HS_END_NAMESPACE
