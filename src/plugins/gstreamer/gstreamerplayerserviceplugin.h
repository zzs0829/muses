#ifndef GSTREAMERPLAYERSERVICEPLUGIN_H
#define GSTREAMERPLAYERSERVICEPLUGIN_H

#include <Multimedia/multimediaserviceproviderplugin.h>

class GstreamerPlayerServicePlugin
        : public MultimediaServiceProviderPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "module.muses.multimediaserviceproviderinterface/1.0" FILE "mediaplayer.json")
public:
    MultimediaService* create(const QString &key) Q_DECL_OVERRIDE;
    void release(MultimediaService *service) Q_DECL_OVERRIDE;
};

#endif // GSTREAMERPLAYERSERVICEPLUGIN_H
