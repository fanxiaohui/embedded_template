#include "dac856x_platform.h"


static void update_to_device(struct dac856x *__FAR dev) {
    uint8_t buf;    
    struct dac856x_platform const *__FAR platform = dev->platform;
    
    spi_select(&platform->bus, platform->cs_index, 1);
    spi_transmit(&platform->bus, &dev->power_status);
    buf = dev->dac_value >> 8;
    spi_transmit(&platform->bus, &buf);
    buf = (uint8_t)dev->dac_value;
    spi_transmit(&platform->bus, &buf);

    spi_select(&platform->bus, platform->cs_index, 0);
}

char dac856x_init(struct dac856x *__FAR dev) {
    spi_init(&dev->platform->bus, SPI_FLAG_CLK_IDLE_HIGH | SPI_FLAG_CLK_FIRST_EDGE);
    
    dev->power_status = DAC856X_POWER_DOWN_100K;
    dev->dac_value = 0x00;
    dev->ref_vol_dot_1mv = dev->platform->ref_voltage_uv == 0 ? 25000 : (dev->platform->ref_voltage_uv / 100);
    update_to_device(dev);
    return 1;
}

char dac856x_set_power(struct dac856x *__FAR dev, enum dac856x_power power_status) {
    dev->power_status = (uint8_t)power_status;
    update_to_device(dev);
    return 1;
}


char dac856x_set_output(struct dac856x *__FAR dev, uint32_t voltage) {
    uint32_t dat;
    char is_gain_2 = 0;
    dev->dac_value = (voltage / 100 * 65536  + dev->ref_vol_dot_1mv / 2) / dev->ref_vol_dot_1mv;
    update_to_device(dev);    
    return 1;
}

uint16_t dac856x_get_regvalue(struct dac856x *__FAR dev) {
    return dev->dac_value;
}
