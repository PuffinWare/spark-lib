#ifndef COMMON_H
#define COMMON_H
#include "spark_wiring.h"

typedef unsigned long ulong;

#ifdef __cplusplus
extern "C" {
#endif

ulong get_duration(ulong now, ulong last);

#ifdef __cplusplus
}
#endif

#endif // COMMON_H
