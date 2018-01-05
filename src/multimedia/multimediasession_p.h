#ifndef MULTIMEDIASESSION_P_H
#define MULTIMEDIASESSION_P_H

#include <Multimedia/private/multimediaglobal_p.h>
#include "multimediasession.h"

HS_BEGIN_NAMESPACE

class MULTIMEDIA_PRIVATE_EXPORT MultimediaSessionPrivate
{
    Q_DECLARE_PUBLIC(MultimediaSession)
public:
    MultimediaSessionPrivate();
    virtual ~MultimediaSessionPrivate();

    MultimediaSession *q_ptr;
    QString id;
    QString key;
    MultimediaSession::State state;
    QVariantMap property;

    void setState(MultimediaSession::State s);
    void setProperty(const QString &k, const QVariant &v);
};

HS_END_NAMESPACE

#endif // MULTIMEDIASESSION_P_H
