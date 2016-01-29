#include "hcs12_misc.h"
#include "derivative.h"

#ifndef HCS12_OSCFREQ
#define HCS12_OSCFREQ 8000000UL
#endif

uint32_t hcs12_get_bus_clock(void) {
    uint32_t clk_freq;
    uint8_t mul;
    uint8_t div;

    mul = (SYNR & 0x3F) + 1;
    div = (REFDV & 0x3F) + 1;

    if ((CLKSEL & CLKSEL_PLLSEL_MASK) > 0) {
        clk_freq = ((HCS12_OSCFREQ * 2) * mul) / div;
    } else {
        clk_freq = HCS12_OSCFREQ;
    }
    return clk_freq / 2;
}
