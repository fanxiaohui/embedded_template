#ifndef __DAC856X_PLATFORM_H__
#define __DAC856X_PLATFORM_H__

#include "dac856x.h"
#include "spi/spi_platform.h"


struct dac856x_platform {
    /// SPI总线;
    const struct spi_bus bus;
    uint8_t cs_index;
    /// 参考电压, 如果是使用内部参考, 该值应赋值为0;
    uint16_t ref_voltage_uv;
};

struct dac856x {
    struct dac856x_platform const *platform;
    uint8_t power_status;
    uint16_t dac_value;
    uint16_t ref_vol_dot_1mv;
};

#endif
