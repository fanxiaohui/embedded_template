#ifndef __PCF212X_PLATFORM_H__
#define __PCF212X_PLATFORM_H__

#include "pcf212x.h"
#include "i2c/i2c_platform.h"

struct pcf212x_platform {
    uint8_t addr;
    const struct i2c_bus bus;
};

#define PCF212X_STATUS_NOT_INIT 0
#define PCF212X_STATUS_OK    1
#define PCF212X_STATUS_RESET 2

struct pcf212x {
    const struct pcf212x_platform *platform;
    uint8_t status;
};

#endif