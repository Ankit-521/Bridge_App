#ifndef VIDEOCHAT_GLOBAL_H
#define VIDEOCHAT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VIDEOCHAT_LIBRARY)
#  define VIDEOCHAT_EXPORT Q_DECL_EXPORT
#else
#  define VIDEOCHAT_EXPORT Q_DECL_IMPORT
#endif

#endif // VIDEOCHAT_GLOBAL_H
