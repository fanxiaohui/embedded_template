#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

typedef const struct i2c_bus *__FAR i2c_bus_t;

inline uint8_t i2c_init(i2c_bus_t bus);
inline uint8_t i2c_transmit(i2c_bus_t bus,
                            uint8_t addr,
                            const uint8_t *__FAR w,
                            uint8_t wlen,
                            uint8_t *__FAR r,
                            uint8_t rlen);
#endif

