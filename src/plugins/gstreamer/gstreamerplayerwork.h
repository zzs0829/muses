#ifndef GSTREAMERPLAYERWORK_H
#define GSTREAMERPLAYERWORK_H

#include <QObject>
#include "gstreamerplayersession.h"
#include "gstreamerplayercontrol.h"

class GstreamerPlayerWork : public QObject
{
    Q_OBJECT
public:
    explicit GstreamerPlayerWork(GstreamerPlayerSession *session,
                                 GstreamerPlayerControl *control,
                                 QObject *parent = Q_NULLPTR);

    GstreamerPlayerSession *session() { return m_session; }
    GstreamerPlayerControl *control() { return m_control; }

public slots:
    void start();
    void stop();

signals:
    void finished();

private slots:
    void _q_sessionPropertyChanged(const QString &key, const QVariant &value);
    void _q_handlePlay();
    void _q_handlePause();
    void _q_handleStop();

    void _q_durationChanged(qint64 duration);
    void _q_positionChanged(qint64 position);
    void _q_stateChanged(MultimediaPlayer::State newState);
    void _q_mediaStatusChanged(MultimediaPlayer::MediaStatus status);
    void _q_volumeChanged(int volume);
    void _q_mutedChanged(bool muted);
    void _q_audioAvailableChanged(bool audioAvailable);
    void _q_videoAvailableChanged(bool videoAvailable);
    void _q_bufferStatusChanged(int percentFilled);
    void _q_seekableChanged(bool);
    void _q_playbackRateChanged(qreal rate);
    void _q_error(int error, const QString &errorString);

private:
    GstreamerPlayerSession *m_session;
    GstreamerPlayerControl *m_control;
};

#endif // GSTREAMERPLAYERWORK_H
