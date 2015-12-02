#include <stdio.h>
#include "sd2405al_platform.h"

#include "misc/second_datetime.h"

//#define dprintf (void)printf
#define dprintf (void)

#define DELAY_MS 100


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

static char write_regs(const struct sd2405_platform *__FAR platform,
                       const uint8_t *reg_and_dat,
                       uint8_t len) {
    if (len != i2c_transmit(&platform->bus, platform->addr, reg_and_dat, len, 0, 0)) {
        return 0;
    }
    return 1;
}

static char read_regs(const struct sd2405_platform *__FAR platform,
                      uint8_t reg,
                      uint8_t *__FAR dat,
                      uint8_t len) {
    if (len + 1 != i2c_transmit(&platform->bus, platform->addr, &reg, 1, dat, len)) {
        return 0;
    }
    return 1;
}

static char write_enable(const struct sd2405_platform *__FAR platform, char is_enable) {
    if (is_enable) {
        uint8_t dat[2];
        dat[0] = 0x10;
        dat[1] = 0x80;
        if (!write_regs(platform, dat, 2)) {
            return 0;
        }

        dat[0] = 0x0F;
        dat[1] = 0x84;
        if (!write_regs(platform, dat, 2)) {
            return 0;
        }
    }

    return 1;
}

char sd2405_init(const struct sd2405_platform *__FAR platform) {
    if (platform == 0) {
        return 0;
    }
    return i2c_init(&platform->bus);
}


char sd2405_write_time(const struct sd2405_platform *__FAR platform, uint32_t seconds) {
    datetime_t t;
    uint8_t reg_and_dat[8];

    if (!write_enable(platform, 1)) {
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

    if (!write_regs(platform, reg_and_dat, sizeof(reg_and_dat))) {
        (void)write_enable(platform, 0);
        return 0;
    }

    (void)write_enable(platform, 0);

    return 1;
}


char sd2405_read_time(const struct sd2405_platform *__FAR platform, uint32_t *__FAR seconds) {
    uint8_t treg[7];
    datetime_t t;

    if (seconds == 0) {
        return 0;
    }

    if (!read_regs(platform, 0x00, treg, sizeof(treg))) {
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

char sd2405_store_data(const struct sd2405_platform *__FAR platform,
                       const uint8_t *__FAR dat,
                       uint8_t offset,
                       uint8_t len) {
    uint8_t i;
    uint8_t reg_and_dat[13];

    if ((!dat) || (offset + len > 12)) {
        return 0;
    }

    if (!write_enable(platform, 1)) {
        return 0;
    }

    reg_and_dat[0] = 0x14 + offset;
    for (i = 0; i < len; ++i) {
        reg_and_dat[i + 1] = dat[i];
    }

    if (!write_regs(platform, reg_and_dat, len + 1)) {
        (void)write_enable(platform, 0);
        return 0;
    }

    (void)write_enable(platform, 0);
    return 1;
}

char sd2405_restore_data(const struct sd2405_platform *__FAR platform,
                         uint8_t *__FAR dat,
                         uint8_t offset,
                         uint8_t len) {
    if ((!dat) || (offset + len > 12)) {
        return 0;
    }
    return read_regs(platform, (uint8_t)(0x14 + offset), dat, len);
}

