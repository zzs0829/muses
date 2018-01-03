#include "mediaplayerfactory.h"
#include "mediaplayer_p.h"
#include <Multimedia/multimediaserviceprovider.h>
#include <Multimedia/multimediaserviceproviderplugin.h>

MediaPlayerFactory::MediaPlayerFactory(QObject *parent)
    : QObject(parent),
      control(0)
{
    MultimediaServiceProvider *provider = MultimediaServiceProvider::defaultServiceProvider();
    MultimediaService* service = provider->requestService(MEDIASERVICE_MEDIAPLAYER);
    if(service) {
        control = service->requestController<MediaPlayerController *>();
    }
}

MediaPlayerFactory *MediaPlayerFactory::instance()
{
    // C++11 Thread safe
    static MediaPlayerFactory playerFactory;
    return &playerFactory;
}

MediaPlayer *MediaPlayerFactory::create(const QString &key)
{
    if(!m_players.contains(key)) {
        m_players.insert(key, new MediaPlayer(this));
    }
    return m_players.value(key);
}

void MediaPlayerFactory::destory(MediaPlayer *player)
{
    QString key = m_players.key(player);
    bool contains = m_players.contains(key);
    Q_ASSERT_X(contains, "destory player", "The play is not in factory.");

    if(contains) {
        m_players.remove(key);
        player->deleteLater();
    }
}

bool MediaPlayerFactory::checkAvailable(MediaPlayer *player)
{
    QString key = m_players.key(player);
    bool contains = m_players.contains(key);
    Q_ASSERT_X(contains, "check available player", "The play is not in factory.");

    if(!contains) {
        qWarning("The play is not in factory to check available player.");
        return false;
    }

    return key == m_availableKey;
}

bool MediaPlayerFactory::available(MediaPlayer *player)
{
    QString key = m_players.key(player);
    bool contains = m_players.contains(key);
    Q_ASSERT_X(contains, "available player", "The play is not in factory.");

    if(!control){
        qWarning("The control is null to available player.");
        return false;
    }

    if(!contains) {
        qWarning("The play is not in factory to available player.");
        return false;
    }

    if(key == m_availableKey)
        return true;

    if(!m_availableKey.isEmpty()) {
        MediaPlayer *old = m_players.value(m_availableKey);
        old->d_func()->unbindControl(control);
    }
    player->d_func()->bindControl(control);
    m_availableKey = key;
    return true;
}

bool MediaPlayerFactory::unavailable(MediaPlayer *player)
{
    QString key = m_players.key(player);
    bool contains = m_players.contains(key);
    Q_ASSERT_X(contains, "unavailable player", "The play is not in factory.");

    if(!control){
        qWarning("The control is null to unavailable player.");
        return false;
    }

    if(!contains) {
        qWarning("The play is not in factory to unavailable player.");
        return false;
    }

    if(key != m_availableKey)
        return true;

    player->d_func()->unbindControl(control);
    m_availableKey = QString();
    return true;
}
