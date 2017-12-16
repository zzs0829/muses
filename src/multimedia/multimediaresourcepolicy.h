#ifndef MULTIMEDIARESOURCEPOLICY_H
#define MULTIMEDIARESOURCEPOLICY_H

#include <Multimedia/multimediaglobal.h>

HS_BEGIN_NAMESPACE

class MULTIMEDIA_EXPORT MultimediaResourcePolicy
{
public:
    template<typename T>
    static T* createResourceSet();
    static void destroyResourceSet(QObject* resourceSet);
private:
    static QObject* createResourceSet(const QString& interfaceId);
};

template<typename T>
T* MultimediaResourcePolicy::createResourceSet()
{
    return qobject_cast<T*>(MultimediaResourcePolicy::createResourceSet(T::iid()));
}

HS_END_NAMESPACE

#endif // MULTIMEDIARESOURCEPOLICY_H
