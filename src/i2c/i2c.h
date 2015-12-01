#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>

typedef const struct i2c_bus *i2c_bus_t;
inline char i2c_init(const struct i2c_bus *bus);
inline uint8_t i2c_transmit(const struct i2c_bus *bus,
                            uint8_t addr,
                            const uint8_t *w,
                            uint8_t wlen,
                            uint8_t *r,
                            uint8_t rlen);
#endif

