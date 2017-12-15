#include "avabstractadaptor.h"
#include "avabstractadaptor_p.h"

HS_BEGIN_NAMESPACE

AVAbstractAdaptor::AVAbstractAdaptor(QObject *parent) :
    QObject(*new AVAbstractAdaptorPrivate, parent)
{

}

AVAbstractAdaptor::~AVAbstractAdaptor()
{

}


HS_END_NAMESPACE
