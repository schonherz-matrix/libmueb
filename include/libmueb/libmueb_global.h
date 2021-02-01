#ifndef LIBMUEB_LIBMUEB_GLOBAL_H_
#define LIBMUEB_LIBMUEB_GLOBAL_H_

#include <QtCore/qglobal.h>

#include <QImage>

namespace libmueb {
// One frame to be transmitted/received. Just an alias for user convenience.
using Frame = QImage;
}  // namespace libmueb

#if defined(LIBMUEB_LIBRARY)
#define LIBMUEB_EXPORT Q_DECL_EXPORT
#else
#define LIBMUEB_EXPORT Q_DECL_IMPORT
#endif

#endif  // LIBMUEB_LIBMUEB_GLOBAL_H_
