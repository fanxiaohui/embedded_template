#ifndef __DAC816X_PLATFORM_H__
#define __DAC816X_PLATFORM_H__

#include "dac816x.h"
#include "spi/spi_platform.h"


struct dac816x_platform {
    const struct spi_bus bus;
    uint16_t ref_voltage_mv;
    uint8_t init_channel_a_power_status : 2;
    uint8_t init_channel_b_power_status : 2;
};

struct dac816x {
    struct dac816x_platform const *__FAR platform;
    uint8_t channel_a_power_status : 2;
    uint8_t channel_b_power_status : 2;
    uint8_t is_channel_a_gain_2 : 1;
    uint8_t is_channel_b_gain_2 : 1;
};

#endif
