#ifndef GSTREAMERMESSAGE_H
#define GSTREAMERMESSAGE_H

#include <Multimedia/multimediaglobal.h>

#include <gst/gst.h>

HS_BEGIN_NAMESPACE

class GstreamerMessage
{
public:
    GstreamerMessage();
    GstreamerMessage(GstMessage* message);
    GstreamerMessage(GstreamerMessage const& m);
    ~GstreamerMessage();

    GstMessage* rawMessage() const;

    GstreamerMessage& operator=(GstreamerMessage const& rhs);

private:
    GstMessage* m_message;
};

HS_END_NAMESPACE

Q_DECLARE_METATYPE(GstreamerMessage)

#endif // GSTREAMERMESSAGE_H
