#include "multimediasession_p.h"

MultimediaSessionPrivate::MultimediaSessionPrivate(const QString &id, const QString &key):
    q_ptr(0),
    id(id),
    key(key),
    state(MultimediaSession::Invalid)
{

}

MultimediaSessionPrivate::~MultimediaSessionPrivate()
{

}

void MultimediaSessionPrivate::setState(MultimediaSession::State s)
{
    Q_Q(MultimediaSession);

    if(s == state)
        return;

    state = s;
    emit q->sessionStateChanged(state);
}

void MultimediaSessionPrivate::setProperty(const QString &k, const QVariant &v)
{
    Q_Q(MultimediaSession);
    if( v == property[k])
        return;

    property[k] = v;
    emit q->sessionPropertyChanged(k, v);
}
