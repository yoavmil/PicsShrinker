#ifndef COMMON
#define COMMON

#include <QDebug>

#define DBGF qDebug() << __FUNCTION__ <<':' <<__LINE__
#define WRNF qWarning() << __FUNCTION__ <<':' <<__LINE__

#endif // COMMON

