#include "bcd_code.h"

// 0x32 => 32(0x20)
uint8_t from_bcd(uint8_t bcd) {
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
uint8_t to_bcd(uint8_t hex) {
    uint8_t h;
    h = hex / 10;
    if (h > 9) {
        return 0;
    }
    return (h << 4) | (hex % 10);
}
