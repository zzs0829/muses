#include "multimediaresourcesetinterface.h"

HS_BEGIN_NAMESPACE

MultimediaResourceSetInterface::MultimediaResourceSetInterface(QObject *parent) :
    QObject(parent)
{

}

MultimediaResourceSetInterface::~MultimediaResourceSetInterface()
{

}

QString MultimediaResourceSetInterface::iid()
{
    return QLatin1String(MultimediaResourceSetInterface_iid);
}

HS_END_NAMESPACE
