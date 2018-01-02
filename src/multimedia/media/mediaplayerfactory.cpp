#include "mediaplayerfactory.h"

Q_GLOBAL_STATIC(MediaPlayerFactory, playerFactory)


MediaPlayerFactory::MediaPlayerFactory(QObject *parent)
    : QObject(parent)
{

}

MediaPlayerFactory *MediaPlayerFactory::instance()
{
    return playerFactory();
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

bool MediaPlayerFactory::available(MediaPlayer *player)
{
    QString key = m_players.key(player);
    bool contains = m_players.contains(key);
    Q_ASSERT_X(contains, "available player", "The play is not in factory.");

    if(!contains) {
        qWarning("The play is not in factory to available player.");
        return false;
    }
    if(key == m_availableKey)
        return true;

}

bool MediaPlayerFactory::unavailable(MediaPlayer *player)
{
    QString key = m_players.key(player);
    bool contains = m_players.contains(key);
    Q_ASSERT_X(contains, "unavailable player", "The play is not in factory.");

    if(!contains) {
        qWarning("The play is not in factory to unavailable player.");
        return false;
    }
    if(key != m_availableKey)
        return true;

}
