#ifndef __ADS1015_PRIVATE_H__
#define __ADS1015_PRIVATE_H__

#include "ads1015.h"
#include "i2c/i2c_platform.h"

struct ads1015_platform {
    uint8_t addr;
    const struct i2c_bus bus;
    void (*delay_ms)(const struct ads1015_platform *__FAR platform, unsigned short howmany);
};

#endif
