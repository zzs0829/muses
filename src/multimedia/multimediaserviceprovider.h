#ifndef MULTIMEDIASERVICEPROVIDER_H
#define MULTIMEDIASERVICEPROVIDER_H

#include <QObject>
#include <Multimedia/multimediaservice.h>

class MultimediaServiceProvider : public QObject
{
    Q_OBJECT
public:
    virtual MultimediaService* requestService(const QByteArray &type) = 0;
    virtual void releaseService(MultimediaService *service) = 0;


    static MultimediaServiceProvider* defaultServiceProvider();
    static void setDefaultServiceProvider(MultimediaServiceProvider *provider);
};

#endif // MULTIMEDIASERVICEPROVIDER_H
