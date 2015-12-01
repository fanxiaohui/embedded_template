#ifndef __SD2405AL_PRIVATE_H__
#define __SD2405AL_PRIVATE_H__

#include "sd2405al.h"
#include "i2c/i2c_platform.h"

struct sd2405_platform {
    uint8_t addr;
    const struct i2c_bus bus;
    void (*delay_ms)(sd2405_platform_t platform,
                     uint16_t howmany);
};

#endif

