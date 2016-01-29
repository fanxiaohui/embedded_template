#include "dac816x_platform.h"


static void transmit(struct dac816x_platform const *__FAR platform, uint8_t cmd, uint16_t dat) {
    uint8_t buf;

    (void)spi_select(&platform->bus, platform->cs_index, 1);

    (void)spi_transmit(&platform->bus, &cmd);
    buf = dat >> 8;
    (void)spi_transmit(&platform->bus, &buf);
    buf = (uint8_t)dat;
    (void)spi_transmit(&platform->bus, &buf);

    (void)spi_select(&platform->bus, platform->cs_index, 0);
}

static char set_channel_power_status(struct dac816x_platform const *__FAR platform, uint8_t channel, uint8_t power_status) {
    uint8_t dat;
    if (power_status == DAC816X_CHANNEL_POWER_ON) {
        dat = 0x00;
    } else if (power_status == DAC816X_CHANNEL_POWER_DOWN_1K) {
        dat = 0x10;
    } else if (power_status == DAC816X_CHANNEL_POWER_DOWN_100K) {
        dat = 0x20;
    } else if (power_status == DAC816X_CHANNEL_POWER_DOWN_HiZ) {
        dat = 0x30;
    } else {
        return 0;
    }

    if (channel == DAC816X_OPERATION_ON_CHANNEL_A) {
        dat |= 0x01;
    } else if (channel == DAC816X_OPERATION_ON_CHANNEL_B) {
        dat |= 0x02;
    } else {
        return 0;
    }

    transmit(platform, 0x20, dat);
    return 1;
}

static uint8_t gain_to_command_data(char is_channel_a_gain_2, char is_channel_b_gain_2) {
    if (is_channel_a_gain_2 && is_channel_b_gain_2) {
        return 0x00;
    }

    if ((!is_channel_a_gain_2) && is_channel_b_gain_2) {
        return 0x01;
    }

    if (is_channel_a_gain_2 && (!is_channel_b_gain_2)) {
        return 0x02;
    }

    return 0x03;
}


static void set_gain(struct dac816x *__FAR dev) {
    uint8_t dat = gain_to_command_data(dev->is_channel_a_gain_2, dev->is_channel_b_gain_2);
    transmit(dev->platform, 0x02, dat);
}

char dac816x_init(struct dac816x *__FAR dev) {
    (void)spi_init(&dev->platform->bus, SPI_FLAG_CLK_IDLE_HIGH | SPI_FLAG_CLK_FIRST_EDGE);

    dev->channel_a_power_status = DAC816X_CHANNEL_POWER_DOWN_100K;
    dev->channel_b_power_status = DAC816X_CHANNEL_POWER_DOWN_100K;
    dev->is_channel_a_gain_2 = 0;
    dev->is_channel_b_gain_2 = 0;

    transmit(dev->platform, 0x48, 0x0001); // reset to power on status;
    transmit(dev->platform, 0xc0, 0x0000); // LDAC inactive for DAC-B and DAC-A


    (void)set_channel_power_status(dev->platform,
                                   DAC816X_OPERATION_ON_CHANNEL_A,
                                   dev->channel_a_power_status);
    (void)set_channel_power_status(dev->platform,
                                   DAC816X_OPERATION_ON_CHANNEL_B,
                                   dev->channel_a_power_status);

    if (dev->platform->ref_voltage_mv != 0) {
        transmit(dev->platform, 0x38, 0x00); // disable internal REF gain = 1
    } else {
        transmit(dev->platform, 0x38, 0x01); // enbale internal REF gain = 2
    }

    set_gain(dev);

    return 1;
}

char dac816x_set_power(struct dac816x *__FAR dev, uint8_t operation_channel, uint8_t power_status) {
    if (!set_channel_power_status(dev->platform, operation_channel, power_status)) {
        return 0;
    }

    if (operation_channel == DAC816X_OPERATION_ON_CHANNEL_A) {
        dev->channel_a_power_status = power_status;
    } else if (operation_channel == DAC816X_OPERATION_ON_CHANNEL_B) {
        dev->channel_b_power_status = power_status;
    } else {
        return 0;
    }
    return 1;
}


char dac816x_set_output(struct dac816x *__FAR dev, uint8_t operation_channel, uint16_t voltage) {
    uint32_t dat;
    char is_gain_2 = 0;
    uint16_t ref_vol = dev->platform->ref_voltage_mv == 0 ? 2500 : dev->platform->ref_voltage_mv;

    if (voltage >= ref_vol * 2) { // to large
        return 0;
    }

    if (operation_channel == DAC816X_OPERATION_ON_CHANNEL_A) {
        if (dev->channel_a_power_status != DAC816X_CHANNEL_POWER_ON) return 0;
    } else if (operation_channel == DAC816X_OPERATION_ON_CHANNEL_B) {
        if (dev->channel_b_power_status != DAC816X_CHANNEL_POWER_ON) return 0;
    } else {
        return 0;
    }

    (void)set_channel_power_status(dev->platform, operation_channel, DAC816X_CHANNEL_POWER_DOWN_HiZ);

    if (voltage >= ref_vol) { // gain = 2
        voltage = voltage / 2;
        is_gain_2 = 1;
    }

    if (operation_channel == DAC816X_OPERATION_ON_CHANNEL_A) {
        if (dev->is_channel_a_gain_2 != is_gain_2) {
            dev->is_channel_a_gain_2 = is_gain_2;
            set_gain(dev);
        }
    } else {
        if (dev->is_channel_b_gain_2 != is_gain_2) {
            dev->is_channel_b_gain_2 = is_gain_2;
            set_gain(dev);
        }
    }

    dat = (voltage * 65536  + ref_vol / 2) / ref_vol;
    transmit(dev->platform, operation_channel == DAC816X_OPERATION_ON_CHANNEL_A ? 0x18 : 0x19, (uint16_t)dat);
    (void)set_channel_power_status(dev->platform, operation_channel, DAC816X_CHANNEL_POWER_ON);
    return 1;
}
