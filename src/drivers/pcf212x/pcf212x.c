#include "pcf212x_platform.h"
#include "misc/bcd_code.h"
#include "misc/second_datetime.h"
#include <stdio.h>


static uint8_t write_regs(const struct pcf212x_platform *__FAR platform,
                          const uint8_t *reg_and_dat,
                          uint8_t len) {
    if (len != i2c_transmit(&platform->bus, platform->addr, reg_and_dat, len, 0, 0)) {
        return 0;
    }
    return 1;
}

static uint8_t read_regs(const struct pcf212x_platform *__FAR platform,
                         uint8_t reg,
                         uint8_t *__FAR dat,
                         uint8_t len) {
    if (1 != i2c_transmit(&platform->bus, platform->addr, &reg, 1, 0, 0)) {
        return 0;
    }

    if (len != i2c_transmit(&platform->bus, platform->addr, 0, 0, dat, len)) {
        return 0;
    }
    return 1;
}


uint8_t pcf212x_init(struct pcf212x *__FAR dev) {
    uint8_t dat[4];

    dev->status = PCF212X_STATUS_NOT_INIT;

    if (0 == i2c_init(&dev->platform->bus)) {
        return 0;
    }
    if (0 == read_regs(dev->platform, 0x00, dat, 4)) {
        return 0;
    }

    if (dat[0] & (1 << 3)) {
        // POR_OVRD = 0
        dat[0] &= ~(1 << 3);
        if (0 == write_regs(dev->platform, 0x00, dat[0])) {
            return 0;
        }
    }

    if (dat[3] & (1 << 7)) {
        dev->status = PCF212X_STATUS_OK;
        return 1;
    }

    dev->status = PCF212X_STATUS_RESET;
    return 1;
}


#if 0
static void dump_datetime(struct datetime *t) {
    (void)printf("%02d年%02d月%02d日%02d:%02d:%02d 星期%d\n",
                 t->year, t->month, t->day, t->hour, t->minute, t->second, t->weekday + 1);
}
#else
#define dump_datetime(t) do {;} while(0)
#endif

uint8_t pcf212x_write_time(struct pcf212x *__FAR dev,
                           uint32_t seconds) {
    struct datetime t;
    uint8_t reg_and_dat[8];

    (void)second_to_datetime(&t, seconds);
    dump_datetime(&t);


    reg_and_dat[0] = 0x03;
    reg_and_dat[1] = to_bcd(t.second);
    reg_and_dat[2] = to_bcd(t.minute);
    reg_and_dat[3] = to_bcd(t.hour);
    reg_and_dat[4] = to_bcd(t.day);
    reg_and_dat[5] = t.weekday;
    reg_and_dat[6] = to_bcd(t.month);
    reg_and_dat[7] = to_bcd(t.year);

    return write_regs(dev->platform, reg_and_dat, sizeof(reg_and_dat));
}


uint8_t pcf212x_read_time(struct pcf212x *__FAR dev,
                          uint32_t *seconds) {
    uint8_t treg[7];
    struct datetime t;

    if (seconds == 0) {
        return 0;
    }

    if (!read_regs(dev->platform, 0x03, treg, sizeof(treg))) {
        return 0;
    }

    t.second = from_bcd(treg[0]);
    t.minute = from_bcd(treg[1]);
    t.hour = from_bcd(treg[2] & 0x3F);
    t.day = from_bcd(treg[3]);
    t.weekday = treg[4];
    t.month = from_bcd(treg[5]);
    t.year = from_bcd(treg[6]);

    dump_datetime(&t);

    return datetime_to_second(seconds, &t);
}

uint8_t pcf212x_dump_regs(struct pcf212x *__FAR dev, uint8_t __FAR dat[0x1C]) {
    return read_regs(dev->platform, 0x00, dat, 0x1C);
}

uint8_t pcf212x_clear_alarm(struct pcf212x *__FAR dev) {
    uint8_t reg_and_dat[6] = {0x00};
    if (0 == write_regs(dev->platform, reg_and_dat, 4)) {
        return 0;
    }
    reg_and_dat[0] = 0x0A;
    reg_and_dat[1] = 0x80;
    reg_and_dat[2] = 0x80;
    reg_and_dat[3] = 0x80;
    reg_and_dat[4] = 0x80;
    reg_and_dat[5] = 0x80;


    if (0 == write_regs(dev->platform, reg_and_dat, sizeof(reg_and_dat))) {
        return 0;
    }

    return 1;
};

uint8_t pcf212x_set_alarm(struct pcf212x *__FAR dev, uint32_t seconds) {
    struct datetime t;
    uint8_t reg_and_dat[6];

    (void)second_to_datetime(&t, seconds);
    dump_datetime(&t);


    reg_and_dat[0] = 0x0A;
    reg_and_dat[1] = to_bcd(t.second);
    reg_and_dat[2] = to_bcd(t.minute);
    reg_and_dat[3] = to_bcd(t.hour);
    reg_and_dat[4] = to_bcd(t.day);
    reg_and_dat[5] = 0x80;

    if (0 == write_regs(dev->platform, reg_and_dat, sizeof(reg_and_dat))) {
        return 0;
    }

    reg_and_dat[0] = 0x01;
    reg_and_dat[1] = 1<<1;

    if (0 == write_regs(dev->platform, reg_and_dat, 2)) {
        return 0;
    }

    return 1;
}
