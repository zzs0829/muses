#include "multimediasession.h"
#include "multimediasession_p.h"

HS_BEGIN_NAMESPACE

MultimediaSession::MultimediaSession(const QString &id, const QString &key, QObject *parent) :
    QObject(parent)
  , d_ptr(new MultimediaSessionPrivate(id, key))
{
    d_ptr->q_ptr = this;
}

MultimediaSession::MultimediaSession(MultimediaSessionPrivate &dd,
                                     QObject *parent)  :
    QObject(parent)
  , d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

MultimediaSession::~MultimediaSession()
{
    delete d_ptr;
}

QString MultimediaSession::sessionId() const
{
    Q_D(const MultimediaSession);
    return d->id;
}

QString MultimediaSession::sessionKey() const
{
    Q_D(const MultimediaSession);
    return d->key;
}

MultimediaSession::State MultimediaSession::sessionState() const
{
    Q_D(const MultimediaSession);
    return d->state;
}

QVariant MultimediaSession::sessionProperty(const QString &key) const
{
    Q_D(const MultimediaSession);
    return d->property[key];
}

void MultimediaSession::setSessionProperty(const QString &key, const QVariant &value)
{
    Q_D(MultimediaSession);
    d->setProperty(key, value);
}

HS_END_NAMESPACE
