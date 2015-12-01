#ifndef __SOFT_I2C_PLATFORM_H__
#define __SOFT_I2C_PLATFORM_H__

#include "softi2c.h"
#include "i2c/i2c_platform.h"
#include "gpio/gpio_platform.h"

struct softi2c_platform {
    void *scl;
    void *sda;
    const struct gpio_operations *gpio_ops;
};

extern const struct i2c_operations softi2c_ops;

#endif

