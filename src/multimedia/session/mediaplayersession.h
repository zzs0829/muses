#ifndef MEDIAPLAYERSESSION_H
#define MEDIAPLAYERSESSION_H


#include <Multimedia/multimediasession.h>
#include <Multimedia/multimediaplayer.h>


HS_BEGIN_NAMESPACE

class MediaPlayerSessionPrivate;
class MULTIMEDIA_EXPORT MediaPlayerSession : public MultimediaSession
{
    Q_OBJECT
public:
    enum PropertyType {
        PlaybackState,
        MediaStatus,
        Duration,
        Position,
        Volume,
        Muted,
        BufferStatus,
        AudioAvailable,
        VideoAvailable,
        Seekable,
        PlaybackRate,
        MediaContent
    };

    static PropertyType propertyType(const QString &key);
    static QString propertyKey(MediaPlayerSession::PropertyType type);


protected:
    explicit MediaPlayerSession(QObject *parent = 0);
    MediaPlayerSession(MediaPlayerSessionPrivate &dd, QObject *parent);

private:
    Q_DISABLE_COPY(MediaPlayerSession)
    Q_DECLARE_PRIVATE(MediaPlayerSession)

    static QMap<PropertyType, QString> propertyMap;
    static void initPropertyMap();
};

HS_END_NAMESPACE

#endif // MEDIAPLAYERSESSION_H
