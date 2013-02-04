#ifndef GEOC_GLOBAL_H
#define GEOC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GEOC_LIBRARY)
#  define GEOCSHARED_EXPORT Q_DECL_EXPORT
#else
#  define GEOCSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // GEOC_GLOBAL_H
