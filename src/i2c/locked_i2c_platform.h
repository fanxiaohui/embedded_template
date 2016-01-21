#ifndef __LOCKED_I2C_PLATFORM_H__
#define __LOCKED_I2C_PLATFORM_H__


#include "platform.h"
#include "i2c/i2c_platform.h"

struct lockedi2c_platform {
    const struct i2c_bus *bus;
    os_mutex_t *mutex;
};

extern const struct i2c_operations lockedi2c_ops;

#endif

