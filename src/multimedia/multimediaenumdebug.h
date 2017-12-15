#ifndef MULTIMEDIAENUMDEBUG_H
#define MULTIMEDIAENUMDEBUG_H

#include <QtCore/qmetaobject.h>
#include <QtCore/qdebug.h>


HS_BEGIN_NAMESPACE


#ifndef QT_NO_DEBUG_STREAM

#define MULTIMEDIA_ENUM_DEBUG(Class,Enum) \
inline QDebug operator<<(QDebug dbg, Class::Enum value) \
{ \
    int index = Class::staticMetaObject.indexOfEnumerator(#Enum); \
    dbg.nospace() << Class::staticMetaObject.className() << "::" << Class::staticMetaObject.enumerator(index).valueToKey(value); \
    return dbg.space(); \
}

#else

#define MULTIMEDIA_ENUM_DEBUG(Class,Enum)

#endif //QT_NO_DEBUG_STREAM

HS_END_NAMESPACE

#endif // MULTIMEDIAENUMDEBUG_H
