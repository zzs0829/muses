#ifndef MEDIAPLAYERFACTORY_H
#define MEDIAPLAYERFACTORY_H

#include <Multimedia/mediaplayer.h>
#include <QObject>

class MediaPlayerFactory : public QObject
{
    Q_OBJECT
public:
    static MediaPlayerFactory *instance();

    MediaPlayer *create(const QString &key);
    void destory(MediaPlayer *player);

    bool available(MediaPlayer *player);
    bool unavailable(MediaPlayer *player);

signals:

public slots:

private:
    explicit MediaPlayerFactory(QObject *parent = 0);

    QMap<QString, MediaPlayer *> m_players;
    QString m_availableKey;
};

#endif // MEDIAPLAYERFACTORY_H
