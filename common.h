#ifndef COMMON_H
#define COMMON_H

#define PUFFIN_DEBUG true
typedef unsigned long ulong;

typedef enum I2C_MODE {
  START,
  READY,
  REQUEST,
  RESPONSE
} I2C_MODE;

#define I2C_READING_DELAY  5000

#ifdef __cplusplus
extern "C" {
#endif


ulong get_duration(ulong now, ulong last);

typedef void (*callback_t) (void);

#ifdef __cplusplus
}
#endif

#endif // COMMON_H
