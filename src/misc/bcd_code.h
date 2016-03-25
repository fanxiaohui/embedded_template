#ifndef __BCD_CODE_H__
#define __BCD_CODE_H__

#include "stdint.h"

// 0x32 => 32(0x20)
uint8_t from_bcd(uint8_t bcd);

// 32(0x20) => 0x32
uint8_t to_bcd(uint8_t v);

#endif