#ifndef __I2C_PLATFORM_H__
#define __I2C_PLATFORM_H__

#include "i2c.h"

typedef char (*i2c_init_func)(void *private_data);
typedef uint8_t (*i2c_transmit_func)(void *private_data,
                        uint8_t addr,
                        const uint8_t *w,
                        uint8_t wlen,
                        uint8_t *r,
                        uint8_t rlen);

struct i2c_operations {
    i2c_init_func init;
    i2c_transmit_func transmit;

};

struct i2c_bus {
    void *private_data;
    const struct i2c_operations *ops;
};

#endif
