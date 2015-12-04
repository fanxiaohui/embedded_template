#include <stdio.h>
#include "sd2405al_platform.h"
#include "misc/second_datetime.h"

//#define dprintf (void)printf
#define dprintf (void)

// 0x32 => 32(0x20)
static uint8_t bcd_to_hex(uint8_t bcd) {
    uint8_t h, l;
    h = bcd >> 4;
    if (h > 9) {
        return 0;
    }
    l = bcd & 0x0f;
    if (l > 9) {
        return 0;
    }
    return h * 10 + l;
}

// 32(0x20) => 0x32
static uint8_t hex_to_bcd(uint8_t hex) {
    uint8_t h;
    h = hex / 10;
    if (h > 9) {
        return 0;
    }
    return (h << 4) | (hex % 10);
}

static uint8_t write_regs(const struct sd2405_platform *__FAR platform,
                          const uint8_t *reg_and_dat,
                          uint8_t len) {
    if (len != i2c_transmit(&platform->bus, platform->addr, reg_and_dat, len, 0, 0)) {
        return 0;
    }
    return 1;
}

static uint8_t read_regs(const struct sd2405_platform *__FAR platform,
                         uint8_t reg,
                         uint8_t *__FAR dat,
                         uint8_t len) {
    if (len + 1 != i2c_transmit(&platform->bus, platform->addr, &reg, 1, dat, len)) {
        return 0;
    }
    return 1;
}

static uint8_t write_enable(struct sd2405 *__FAR dev, uint8_t is_enable) {
    if (is_enable) {
        uint8_t dat[2];
        dat[0] = 0x10;
        dat[1] = dev->ctr2 | 0x80;
        if (!write_regs(dev->platform, dat, 2)) {
            return 0;
        }

        dev->ctr2 = dat[1];

        dat[0] = 0x0F;
        dat[1] = dev->ctr1 | 0x84;
        if (!write_regs(dev->platform, dat, 2)) {
            return 0;
        }
        dev->ctr1 = dat[1];
    } else {
        uint8_t dat[3];
        dat[0] = 0x0F;
        dat[1] = dev->ctr1 & (~0x84);
        dat[2] = dev->ctr2 & (~0x80);
        if (!write_regs(dev->platform, dat, 3)) {
            return 0;
        }
        dev->ctr1 = dat[1];
        dev->ctr2 = dat[2];
    }

    return 1;
}

uint8_t sd2405_init(struct sd2405 *__FAR dev) {
    uint8_t dat[2];
    if (0 == i2c_init(&dev->platform->bus)) {
        return 0;
    }

    if (0 == read_regs(dev->platform, 0x0F, dat, 2)) {
        return 0;
    }

    dev->ctr1 = dat[0];
    dev->ctr2 = dat[1];
    return 1;
}


uint8_t sd2405_write_time(struct sd2405 *__FAR dev, uint32_t seconds) {
    datetime_t t;
    uint8_t reg_and_dat[8];

    if (!write_enable(dev, 1)) {
        return 0;
    }

    (void)second_to_datetime(&t, seconds);

    reg_and_dat[0] = 0x00;
    reg_and_dat[1] = hex_to_bcd(t.second);
    reg_and_dat[2] = hex_to_bcd(t.minute);
    reg_and_dat[3] = hex_to_bcd(t.hour) | 0x80;
    reg_and_dat[4] = t.weekday;
    reg_and_dat[5] = hex_to_bcd(t.day);
    reg_and_dat[6] = hex_to_bcd(t.month);
    reg_and_dat[7] = hex_to_bcd(t.year);

    if (!write_regs(dev->platform, reg_and_dat, sizeof(reg_and_dat))) {
        (void)write_enable(dev, 0);
        return 0;
    }

    (void)write_enable(dev, 0);

    return 1;
}


uint8_t sd2405_read_time(struct sd2405 *__FAR dev, uint32_t *__FAR seconds) {
    uint8_t treg[7];
    datetime_t t;

    if (seconds == 0) {
        return 0;
    }

    if (!read_regs(dev->platform, 0x00, treg, sizeof(treg))) {
        return 0;
    }

    t.second = bcd_to_hex(treg[0]);
    t.minute = bcd_to_hex(treg[1]);
    t.hour = bcd_to_hex(treg[2] & 0x3F);
    t.day = bcd_to_hex(treg[4]);
    t.month = bcd_to_hex(treg[5]);
    t.year = bcd_to_hex(treg[6]);

    if (!datetime_to_second(seconds, &t)) {
        return 0;
    }
    return 1;
}

uint8_t sd2405_store_data(struct sd2405 *__FAR dev,
                          const uint8_t *__FAR dat,
                          uint8_t offset,
                          uint8_t len) {
    uint8_t i;
    uint8_t reg_and_dat[13];

    if ((!dat) || (offset + len > 12)) {
        return 0;
    }

    if (!write_enable(dev, 1)) {
        return 0;
    }

    reg_and_dat[0] = 0x14 + offset;
    for (i = 0; i < len; ++i) {
        reg_and_dat[i + 1] = dat[i];
    }

    if (!write_regs(dev->platform, reg_and_dat, len + 1)) {
        (void)write_enable(dev, 0);
        return 0;
    }

    (void)write_enable(dev, 0);
    return 1;
}

uint8_t sd2405_restore_data(struct sd2405 *__FAR dev,
                            uint8_t *__FAR dat,
                            uint8_t offset,
                            uint8_t len) {
    if ((!dat) || (offset + len > 12)) {
        return 0;
    }
    return read_regs(dev->platform, (uint8_t)(0x14 + offset), dat, len);
}


uint8_t sd2405_dump_regs(struct sd2405 *__FAR dev, uint8_t __FAR dat[32]) {
    return read_regs(dev->platform, 0, dat, 32);
}

uint8_t sd2405_config_interrupt(struct sd2405 *__FAR dev, enum sd2405_interrupt_type int_type, enum sd2405_interrupt_single single) {
    uint8_t ret = 0;
    uint8_t dat[2];
    if (!write_enable(dev, 1)) {
        return 0;
    }
    dat[0] = 0x10; // CTR2
    dat[1] = dev->ctr2 | 0x08;

    if (single == SD2405_INTERRUPT_SINGLE_PAUSE) {
        dat[1] |= 0x40;
    }

    switch (int_type ) {
    case SD2405_INTERRUPT_DISABLE:
        dat[1] &= ~(0x30);
        break;
    case SD2405_INTERRUPT_ALARM:
        dat[1] |= 0x10 | 0x02;
        break;
    case SD2405_INTERRUPT_FREQUENCY:
        dat[1] |= 0x20 | 0x01;
        break;
    case SD2405_INTERRUPT_COUNTDOWN:
        dat[1] |= 0x30 | 0x04;
        break;
    default:
        goto __ret;
    }

    ret = write_regs(dev->platform, dat, sizeof(dat));
    if (ret) {
        dev->ctr2 = dat[1];
    }
__ret:
    (void)write_enable(dev, 0);
    return ret;
}

uint8_t sd2405_clear_interrupt(struct sd2405 *__FAR dev) {
    uint8_t ret;
    uint8_t reg_and_dat[2];
    if (!write_enable(dev, 1)) {
        return 0;
    }

    reg_and_dat[0] = 0x0F;
    reg_and_dat[1] = dev->ctr1 & (~0x03);
    ret = write_regs(dev->platform, reg_and_dat, 2);
    (void)write_enable(dev, 0);
    return ret;
}

uint8_t sd2405_set_alarm(struct sd2405 *__FAR dev, uint32_t seconds) {
    datetime_t t;
    uint8_t reg_and_dat[9];

    if (!write_enable(dev, 1)) {
        return 0;
    }

    (void)second_to_datetime(&t, seconds);

    reg_and_dat[0] = 0x07; // alarm register
    reg_and_dat[1] = 0;
    reg_and_dat[2] = hex_to_bcd(t.minute);
    reg_and_dat[3] = hex_to_bcd(t.hour);
    reg_and_dat[4] = 0;
    reg_and_dat[5] = hex_to_bcd(t.day);
    reg_and_dat[6] = hex_to_bcd(t.month);
    reg_and_dat[7] = hex_to_bcd(t.year);
    reg_and_dat[8] = 0x76; // alarm match Year Month Day Hour Minite

    if (!write_regs(dev->platform, reg_and_dat, sizeof(reg_and_dat))) {
        (void)write_enable(dev, 0);
        return 0;
    }

    (void)write_enable(dev, 0);
    return 1;
}

