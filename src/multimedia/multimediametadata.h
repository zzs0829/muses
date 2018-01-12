#ifndef MULTIMEDIAMETADATA_H
#define MULTIMEDIAMETADATA_H

#include <Multimedia/multimediaglobal.h>

#include <QtCore/qpair.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qstring.h>

#define HS_DECLARE_METADATA(key) MULTIMEDIA_EXPORT extern const QString key

namespace MultimediaMetaData
{
    // Common
    HS_DECLARE_METADATA(Title);
    HS_DECLARE_METADATA(SubTitle);
    HS_DECLARE_METADATA(Author);
    HS_DECLARE_METADATA(Comment);
    HS_DECLARE_METADATA(Description);
    HS_DECLARE_METADATA(Category);
    HS_DECLARE_METADATA(Genre);
    HS_DECLARE_METADATA(Year);
    HS_DECLARE_METADATA(Date);
    HS_DECLARE_METADATA(UserRating);
    HS_DECLARE_METADATA(Keywords);
    HS_DECLARE_METADATA(Language);
    HS_DECLARE_METADATA(Publisher);
    HS_DECLARE_METADATA(Copyright);
    HS_DECLARE_METADATA(ParentalRating);
    HS_DECLARE_METADATA(RatingOrganization);

    // Media
    HS_DECLARE_METADATA(Size);
    HS_DECLARE_METADATA(MediaType);
    HS_DECLARE_METADATA(Duration);

    // Audio
    HS_DECLARE_METADATA(AudioBitRate);
    HS_DECLARE_METADATA(AudioCodec);
    HS_DECLARE_METADATA(AverageLevel);
    HS_DECLARE_METADATA(ChannelCount);
    HS_DECLARE_METADATA(PeakValue);
    HS_DECLARE_METADATA(SampleRate);

    // Music
    HS_DECLARE_METADATA(AlbumTitle);
    HS_DECLARE_METADATA(AlbumArtist);
    HS_DECLARE_METADATA(ContributingArtist);
    HS_DECLARE_METADATA(Composer);
    HS_DECLARE_METADATA(Conductor);
    HS_DECLARE_METADATA(Lyrics);
    HS_DECLARE_METADATA(Mood);
    HS_DECLARE_METADATA(TrackNumber);
    HS_DECLARE_METADATA(TrackCount);

    HS_DECLARE_METADATA(CoverArtUrlSmall);
    HS_DECLARE_METADATA(CoverArtUrlLarge);

    // Image/Video
    HS_DECLARE_METADATA(Resolution);
    HS_DECLARE_METADATA(PixelAspectRatio);

    // Video
    HS_DECLARE_METADATA(VideoFrameRate);
    HS_DECLARE_METADATA(VideoBitRate);
    HS_DECLARE_METADATA(VideoCodec);

    HS_DECLARE_METADATA(PosterUrl);

    // Movie
    HS_DECLARE_METADATA(ChapterNumber);
    HS_DECLARE_METADATA(Director);
    HS_DECLARE_METADATA(LeadPerformer);
    HS_DECLARE_METADATA(Writer);

    // Photos
    HS_DECLARE_METADATA(CameraManufacturer);
    HS_DECLARE_METADATA(CameraModel);
    HS_DECLARE_METADATA(Event);
    HS_DECLARE_METADATA(Subject);
    HS_DECLARE_METADATA(Orientation);
    HS_DECLARE_METADATA(ExposureTime);
    HS_DECLARE_METADATA(FNumber);
    HS_DECLARE_METADATA(ExposureProgram);
    HS_DECLARE_METADATA(ISOSpeedRatings);
    HS_DECLARE_METADATA(ExposureBiasValue);
    HS_DECLARE_METADATA(DateTimeOriginal);
    HS_DECLARE_METADATA(DateTimeDigitized);
    HS_DECLARE_METADATA(SubjectDistance);
    HS_DECLARE_METADATA(MeteringMode);
    HS_DECLARE_METADATA(LightSource);
    HS_DECLARE_METADATA(Flash);
    HS_DECLARE_METADATA(FocalLength);
    HS_DECLARE_METADATA(ExposureMode);
    HS_DECLARE_METADATA(WhiteBalance);
    HS_DECLARE_METADATA(DigitalZoomRatio);
    HS_DECLARE_METADATA(FocalLengthIn35mmFilm);
    HS_DECLARE_METADATA(SceneCaptureType);
    HS_DECLARE_METADATA(GainControl);
    HS_DECLARE_METADATA(Contrast);
    HS_DECLARE_METADATA(Saturation);
    HS_DECLARE_METADATA(Sharpness);
    HS_DECLARE_METADATA(DeviceSettingDescription);

    // Location
    HS_DECLARE_METADATA(GPSLatitude);
    HS_DECLARE_METADATA(GPSLongitude);
    HS_DECLARE_METADATA(GPSAltitude);
    HS_DECLARE_METADATA(GPSTimeStamp);
    HS_DECLARE_METADATA(GPSSatellites);
    HS_DECLARE_METADATA(GPSStatus);
    HS_DECLARE_METADATA(GPSDOP);
    HS_DECLARE_METADATA(GPSSpeed);
    HS_DECLARE_METADATA(GPSTrack);
    HS_DECLARE_METADATA(GPSTrackRef);
    HS_DECLARE_METADATA(GPSImgDirection);
    HS_DECLARE_METADATA(GPSImgDirectionRef);
    HS_DECLARE_METADATA(GPSMapDatum);
    HS_DECLARE_METADATA(GPSProcessingMethod);
    HS_DECLARE_METADATA(GPSAreaInformation);

    HS_DECLARE_METADATA(PosterImage);
    HS_DECLARE_METADATA(CoverArtImage);
    HS_DECLARE_METADATA(ThumbnailImage);
}

#undef HS_DECLARE_METADATA

#endif // MULTIMEDIAMETADATA_H
