#include "hcs12_spi_platform.h"

char hcs12_spi_init(const struct hcs12_spi_platform *__FAR platform, uint8_t flags) {
    uint8_t temp;

    temp = (1 << 6) | (1 << 4); // enable, master
    if (flags & SPI_FLAG_CLK_IDLE_HIGH) temp |= 1 << 3;
    if (flags & SPI_FLAG_CLK_FIRST_EDGE) temp |= 1 << 2;
    if (flags & SPI_FLAG_LSB_FIRST) temp |= 1 << 0;
    platform->regs->cr1.Byte = temp;

    temp = (1 << 6) | (1 << 1); //8bit, stop in wait mode
    platform->regs->cr2.Byte = temp;
    platform->regs->br.Byte = platform->baud_reg_value;

    for (temp = 0; temp < platform->cs_num; ++temp) {
        hcs12_gpio_init(&platform->cs_pins[temp], GPIO_MODE_OUTPUT_PUSHPULL);
        hcs12_gpio_set_output(&platform->cs_pins[temp], 1);
    }


    return 1;
}

char hcs12_spi_select(const struct hcs12_spi_platform *__FAR platform, uint8_t which, uint8_t is_select) {
    if (which >= platform->cs_num) return 0;
    hcs12_gpio_set_output(&platform->cs_pins[which], 0 == is_select);
    return 1;
}

char hcs12_spi_transmit(const struct hcs12_spi_platform *__FAR platform, uint8_t *dat) {
    uint16_t i;
    for (;;) { // wait transmit emtpy
        if (platform->regs->sr.Bits.SPTEF) break;
        if (i > 1000) return 0;
        ++i;
    }


    platform->regs->dr.Overlap_STR.SPI0DRLSTR.Byte = *dat;


    for (;;) { // wait data recved
        if (platform->regs->sr.Bits.SPIF) break;
        if (i > 1000) return 0;
        ++i;
    }

    *dat = platform->regs->dr.Overlap_STR.SPI0DRLSTR.Byte;
    return 1;
}

struct spi_operations hcs12_spi_ops = {
    hcs12_spi_init,
    0, //spi_config_clk_idle_func config_clk_idle;
    0, //spi_is_clk_idle_high_fucn is_clk_idle_high;
    0, //spi_config_clk_edge_func config_clk_edge;
    0, //spi_is_clk_idle_high_fucn is_clk_edge_first;
    0, //spi_config_first_bit_func config_first_bit;
    0, //spi_is_lsb_first_func is_lsb_first;
    hcs12_spi_select,
    hcs12_spi_transmit,
};
