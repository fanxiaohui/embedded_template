#include "dac856x_platform.h"


static void update_to_device(struct dac856x *__FAR dev) {
    uint8_t buf[3];
    struct dac856x_platform const *__FAR platform = dev->platform;

    buf[0] = dev->power_status;
    buf[1] = dev->dac_value >> 8;
    buf[2] = (uint8_t)dev->dac_value;

    (void)spi_select(&platform->bus, platform->cs_index, 1);
    (void)spi_transfer(&platform->bus, 0, buf, 3);
    (void)spi_select(&platform->bus, platform->cs_index, 0);
}

char dac856x_init(struct dac856x *__FAR dev) {
    (void)spi_init(&dev->platform->bus, SPI_FLAG_CLK_IDLE_HIGH | SPI_FLAG_CLK_FIRST_EDGE);

    dev->power_status = DAC856X_POWER_DOWN_100K;
    dev->dac_value = 0x00;
    dev->ref_vol_dot_1mv = dev->platform->ref_voltage_uv == 0 ? 25000 : (dev->platform->ref_voltage_uv / 100);
    update_to_device(dev);
    return 1;
}

char dac856x_set_power(struct dac856x *__FAR dev, enum dac856x_power power_status) {
    if (dev->power_status != (uint8_t)power_status) {
        dev->power_status = (uint8_t)power_status;
        update_to_device(dev);
    }
    return 1;
}

char dac856x_set_regvalue(struct dac856x *__FAR dev, uint16_t regval) {
    if (dev->dac_value != regval) {
        dev->dac_value = regval;
        update_to_device(dev);
    }
    return 1;
}

uint16_t dac856x_get_regvalue(struct dac856x *__FAR dev) {
    return dev->dac_value;
}

char dac856x_set_output(struct dac856x *__FAR dev, uint32_t voltage) {
    uint32_t dac_value = (voltage / 100 * 65536  + dev->ref_vol_dot_1mv / 2) / dev->ref_vol_dot_1mv;
    return dac856x_set_regvalue(dev, (uint16_t)dac_value);
}
