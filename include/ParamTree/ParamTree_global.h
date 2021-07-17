#ifndef PARAMTREE_GLOBAL_H
#define PARAMTREE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PARAMTREE_LIBRARY)
#  define PARAMTREE_EXPORT Q_DECL_EXPORT
#else
#  define PARAMTREE_EXPORT Q_DECL_IMPORT
#endif

#endif // PARAMTREE_GLOBAL_H
