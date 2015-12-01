#ifndef __I2C_PLATFORM_H__
#define __I2C_PLATFORM_H__

#include "i2c.h"

struct i2c_ops {
    char (*init)(void *private_data);
    uint8_t (*transmit)(void *private_data,
                        uint8_t addr,
                        const uint8_t *w,
                        uint8_t wlen,
                        uint8_t *r,
                        uint8_t rlen);

};

struct i2c_bus {
    void *private_data;
    const struct i2c_ops *ops;
};

#endif
