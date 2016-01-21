#ifndef __DAC856X_PLATFORM_H__
#define __DAC856X_PLATFORM_H__

#include "dac856x.h"
#include "spi/spi_platform.h"


struct dac856x_platform {
    /// SPI����;
    const struct spi_bus bus;
    uint8_t cs_index;
    /// �ο���ѹ, �����ʹ���ڲ��ο�, ��ֵӦ��ֵΪ0;
    uint16_t ref_voltage_uv;
};

struct dac856x {
    struct dac856x_platform const *platform;
    uint8_t power_status;
    uint16_t dac_value;
    uint16_t ref_vol_dot_1mv;
};

#endif
