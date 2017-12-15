#include "gstreamermessage.h"

HS_BEGIN_NAMESPACE

static void qRegisterGstreamerMessageMetaTypes()
{
    qRegisterMetaType<GstreamerMessage>();
}

Q_CONSTRUCTOR_FUNCTION(qRegisterGstreamerMessageMetaTypes)

/*!
    \class GstreamerMessage
    \internal
*/

GstreamerMessage::GstreamerMessage():
    m_message(0)
{
}

GstreamerMessage::GstreamerMessage(GstMessage* message):
    m_message(message)
{
    gst_message_ref(m_message);
}

GstreamerMessage::GstreamerMessage(GstreamerMessage const& m):
    m_message(m.m_message)
{
    gst_message_ref(m_message);
}


GstreamerMessage::~GstreamerMessage()
{
    if (m_message != 0)
        gst_message_unref(m_message);
}

GstMessage* GstreamerMessage::rawMessage() const
{
    return m_message;
}

GstreamerMessage& GstreamerMessage::operator=(GstreamerMessage const& rhs)
{
    if (rhs.m_message != m_message) {
        if (rhs.m_message != 0)
            gst_message_ref(rhs.m_message);

        if (m_message != 0)
            gst_message_unref(m_message);

        m_message = rhs.m_message;
    }

    return *this;
}


HS_END_NAMESPACE
