#ifndef MULTIMEDIA_H
#define MULTIMEDIA_H

#include <Multimedia/multimediaglobal.h>


HS_BEGIN_NAMESPACE

namespace Multimedia
{
    enum SupportEstimate
    {
        NotSupported,
        MaybeSupported,
        ProbablySupported,
        PreferredService
    };

    enum EncodingQuality
    {
        VeryLowQuality,
        LowQuality,
        NormalQuality,
        HighQuality,
        VeryHighQuality
    };

    enum EncodingMode
    {
        ConstantQualityEncoding,
        ConstantBitRateEncoding,
        AverageBitRateEncoding,
        TwoPassEncoding
    };

    enum AvailabilityStatus
    {
        Available,
        ServiceMissing,
        Busy,
        ResourceError
    };

}

HS_END_NAMESPACE

Q_DECLARE_METATYPE(Multimedia::AvailabilityStatus)
Q_DECLARE_METATYPE(Multimedia::SupportEstimate)
Q_DECLARE_METATYPE(Multimedia::EncodingMode)
Q_DECLARE_METATYPE(Multimedia::EncodingQuality)

#endif // MULTIMEDIA_H
