#ifndef __DAC816X_PLATFORM_H__
#define __DAC816X_PLATFORM_H__

#include "dac816x.h"
#include "spi/spi_platform.h"


struct dac816x_platform {
    /// SPI总线;
    const struct spi_bus bus;
    uint16_t cs_index;
    /// 参考电压, 如果是使用内部参考, 该值应赋值为0;
    uint16_t ref_voltage_mv;
};

struct dac816x {
    struct dac816x_platform const *__FAR platform;
    uint8_t channel_a_power_status : 2;
    uint8_t channel_b_power_status : 2;
    uint8_t is_channel_a_gain_2 : 1;
    uint8_t is_channel_b_gain_2 : 1;
};

#endif
