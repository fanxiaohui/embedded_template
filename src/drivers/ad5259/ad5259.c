#include "ad5259_platform.h"


#define RESISTANCE_W 75

static char init_get_resistance_ab(ad5259_t ad5259) {
    // read back Tolerance
    uint8_t buf[2];
    uint16_t tolerance;
    buf[0] = 0x3E; // read tolerance Consecutively
    if (1 != ad5259->platform->i2c_write(ad5259->platform, buf, 1)) {
        return 0;
    }
    if (2 != ad5259->platform->i2c_read(ad5259->platform, buf, 2)) {
        return 0;
    }
    tolerance = (uint16_t)(buf[0] & 0x7F) << 8;
    tolerance += buf[1];

    if (buf[0] & 0x80) {
        tolerance = 25600 - tolerance;
    } else {
        tolerance = 25600 + tolerance;
    }

    ad5259->resistance_ab = ((uint32_t)tolerance * ad5259->platform->nominal_resistance + 12800) / 25600;
    return 1;
}

char ad5259_init(ad5259_t ad5259) {
    if (!ad5259->platform->init(ad5259->platform)) {
        return 0;
    }
    if (!init_get_resistance_ab(ad5259)) {
        return 0;
    }
    return 1;
}

uint32_t ad5259_get_resistance_ab(ad5259_t ad5259) {
    return ad5259->resistance_ab;
}

char ad5259_set_resistance_wb(ad5259_t ad5259, uint32_t rwb) {
    uint8_t buf[2];

    if (rwb <= 2 * RESISTANCE_W) {
        rwb = 0;
    } else {
        rwb = rwb - 2 * RESISTANCE_W;
    }

    uint32_t pos = (rwb * 256 + ad5259->resistance_ab / 2) / ad5259->resistance_ab;
    buf[0] = 0x00;
    buf[1] = pos & 0xff;

    if (2 != ad5259->platform->i2c_write(ad5259->platform, buf, 2)) {
        return 0;
    }
    return 1;
}

char ad5259_get_resistance_wb(ad5259_t ad5259, uint32_t *__FAR rwb) {
    uint8_t buf;
    buf = 0x00;
    if (1 != ad5259->platform->i2c_write(ad5259->platform, &buf, 1)) {
        return 0;
    }
    if (1 != ad5259->platform->i2c_read(ad5259->platform, &buf, 1)) {
        return 0;
    }
    *rwb = (ad5259->resistance_ab * buf + 128) / 256 + 2 * RESISTANCE_W;
    return 1;
}


