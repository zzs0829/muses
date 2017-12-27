#ifndef MULTIMEDIAGLOBAL_H
#define MULTIMEDIAGLOBAL_H

#include <QtCore/qglobal.h>
#include <QLoggingCategory>


#if !defined(HS_NAMESPACE)  /* user namespace */

# define HS_PREPEND_NAMESPACE(name) ::name
# define HS_USE_NAMESPACE
# define HS_BEGIN_NAMESPACE
# define HS_END_NAMESPACE

#ifndef LOG_CATEGORY_MEDIA
#define LOG_CATEGORY_MEDIA "muses.media"
#endif

#else /* user namespace */

#define HS_PREPEND_NAMESPACE(name) ::HS_NAMESPACE::name
#define HS_USE_NAMESPACE using namespace ::HS_NAMESPACE;
#define HS_BEGIN_NAMESPACE namespace HS_NAMESPACE {
#define HS_END_NAMESPACE  }

# endif

//
#include "multimediaenumdebug.h"

HS_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(BUILD_MULTIMEDIA_LIB)
#      define MULTIMEDIA_EXPORT Q_DECL_EXPORT
#    else
#      define MULTIMEDIA_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define MULTIMEDIA_EXPORT
#endif

HS_END_NAMESPACE

#endif // MULTIMEDIAGLOBAL_H
