#ifndef __I2C_PLATFORM_H__
#define __I2C_PLATFORM_H__

#include "i2c.h"

typedef uint8_t (*i2c_init_func)(void *__FAR private_data);
typedef uint8_t (*i2c_transmit_func)(void *__FAR private_data,
                                     uint8_t addr,
                                     const uint8_t *__FAR w,
                                     uint8_t wlen,
                                     uint8_t *__FAR r,
                                     uint8_t rlen);
typedef void (*i2c_deinit_func)(void *__FAR private_data);                                      

struct i2c_operations {
    i2c_init_func init;
    i2c_deinit_func deinit;
    i2c_transmit_func transmit;
};

struct i2c_bus {
    void *__FAR private_data;
    const struct i2c_operations *__FAR ops;
};

#endif
