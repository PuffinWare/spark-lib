#ifndef COMMON_H
#define COMMON_H

typedef unsigned long ulong;

#ifdef __cplusplus
extern "C" {
#endif

ulong get_duration(ulong now, ulong last);

typedef void (*callback_t) (void);

#ifdef __cplusplus
}
#endif

#endif // COMMON_H
