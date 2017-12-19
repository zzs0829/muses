#ifndef MULTIMEDIASERVICEPROVIDERPLUGIN_H
#define MULTIMEDIASERVICEPROVIDERPLUGIN_H

#include <Multimedia/multimediaservice.h>
#include <QtCore/qobject.h>

HS_BEGIN_NAMESPACE

struct MULTIMEDIA_EXPORT MultimediaServiceProviderInterface
{
    virtual ~MultimediaServiceProviderInterface();
    virtual MultimediaService* create(QString const& key) = 0;
    virtual void release(MultimediaService *service) = 0;
};

#define MultimediaServiceProviderInterface_iid \
    "module.muses.multimediaserviceproviderinterface/1.0"
Q_DECLARE_INTERFACE(MultimediaServiceProviderInterface, MultimediaServiceProviderInterface_iid)

class MULTIMEDIA_EXPORT MultimediaServiceProviderPlugin : public QObject, public MultimediaServiceProviderInterface
{
    Q_OBJECT
    Q_INTERFACES(MultimediaServiceProviderInterface)
public:
    virtual ~MultimediaServiceProviderPlugin();
    virtual MultimediaService* create(const QString& key) Q_DECL_OVERRIDE = 0;
    virtual void release(MultimediaService *service) Q_DECL_OVERRIDE = 0;
};

/*!
    Service with support for media playback
    Required Controls: QMediaPlayerControl
    Optional Controls: QMediaPlaylistControl, QAudioDeviceControl
    Video Output Controls (used by QWideoWidget and QGraphicsVideoItem):
                        Required: QVideoOutputControl
                        Optional: QVideoWindowControl, QVideoRendererControl, QVideoWidgetControl
*/
#define MEDIASERVICE_MEDIAPLAYER "module.muses.mediaplayer"
#define MEDIASERVICE_PLAYLIST "module.muses.playlist"

HS_END_NAMESPACE

#endif // MULTIMEDIASERVICEPROVIDERPLUGIN_H
