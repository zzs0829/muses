#ifndef MULTIMEDIASESSION_H
#define MULTIMEDIASESSION_H

#include <Multimedia/multimediaglobal.h>
#include <QtCore/qobject.h>

HS_BEGIN_NAMESPACE

class MultimediaService;
class MultimediaSessionPrivate;
class MULTIMEDIA_EXPORT MultimediaSession : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
public:
    enum State
    {
        Invalid,
        Available,
        NotAvailable,
        Connecting,
        Connected,
        Disconnected
    };

    QString sessionId() const;
    QString sessionKey() const;
    State sessionState() const;

    QVariant sessionProperty(const QString &key) const;
    void setSessionProperty(const QString &key, const QVariant &value);

signals:
    void sessionStateChanged(MultimediaSession::State state);
    void sessionPropertyChanged(const QString &key, const QVariant &value);

protected:
    explicit MultimediaSession(QObject *parent = 0);
    MultimediaSession(MultimediaSessionPrivate &dd, QObject *parent);
    MultimediaSessionPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(MultimediaSession)
};

HS_END_NAMESPACE

Q_DECLARE_METATYPE(MultimediaSession::State)

MULTIMEDIA_ENUM_DEBUG(MultimediaSession, State)

#endif // MULTIMEDIASESSION_H
