#ifndef NATIVEPLAYLISTSERVICEPLUGIN_H
#define NATIVEPLAYLISTSERVICEPLUGIN_H

#include <Multimedia/multimediaserviceproviderplugin.h>


class NativePlaylistServicePlugin
        : public MultimediaServiceProviderPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "module.muses.multimediaserviceproviderinterface/1.0" FILE "playlist.json")
public:
    MultimediaService* create(const QString &path) Q_DECL_OVERRIDE;
    void release(MultimediaService *service) Q_DECL_OVERRIDE;
};

#endif // NATIVEPLAYLISTSERVICEPLUGIN_H
