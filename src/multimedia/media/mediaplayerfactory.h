#ifndef MEDIAPLAYERFACTORY_H
#define MEDIAPLAYERFACTORY_H

#include <Multimedia/mediaplayer.h>
#include <Multimedia/mediaplayercontroller.h>
#include <QObject>

HS_BEGIN_NAMESPACE

class MULTIMEDIA_EXPORT MediaPlayerFactory : public QObject
{
    Q_OBJECT
public:
    static MediaPlayerFactory *instance();

    MediaPlayer *create(const QString &key);
    void destory(MediaPlayer *player);

    bool checkAvailable(MediaPlayer *player);
    bool available(MediaPlayer *player);
    bool unavailable(MediaPlayer *player);

signals:

public slots:

private:
    explicit MediaPlayerFactory(QObject *parent = 0);

    MediaPlayerController *control;
    QMap<QString, MediaPlayer *> m_players;
    QString m_availableKey;
};

HS_END_NAMESPACE

#endif // MEDIAPLAYERFACTORY_H
