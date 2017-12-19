#include "multimedia.h"

HS_BEGIN_NAMESPACE

static void qRegisterMultimediaMetaTypes()
{
    qRegisterMetaType<Multimedia::AvailabilityStatus>();
    qRegisterMetaType<Multimedia::SupportEstimate>();
    qRegisterMetaType<Multimedia::EncodingMode>();
    qRegisterMetaType<Multimedia::EncodingQuality>();
}

Q_CONSTRUCTOR_FUNCTION(qRegisterMultimediaMetaTypes)

HS_END_NAMESPACE
