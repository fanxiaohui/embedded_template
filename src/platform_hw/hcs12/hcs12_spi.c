#include "hcs12_spi_platform.h"

uint8_t hcs12_spi_init(const struct hcs12_spi_platform *__FAR platform, uint8_t flags) {
    uint8_t temp;

    temp = (1 << 6) | (1 << 4); // enable, master
    if (flags & SPI_FLAG_CLK_IDLE_HIGH) temp |= 1 << 3;
    if ((flags & SPI_FLAG_CLK_FIRST_EDGE) == 0) temp |= 1 << 2;
    if (flags & SPI_FLAG_LSB_FIRST) temp |= 1 << 0;
    platform->regs->cr1.Byte = temp;

    temp = (1 << 1); //8bit, stop in wait mode
    platform->regs->cr2.Byte = temp;
    platform->regs->br.Byte = platform->baud_reg_value;

    for (temp = 0; temp < platform->cs_num; ++temp) {
        (void)hcs12_gpio_init(&platform->cs_pins[temp], GPIO_MODE_OUTPUT_PUSHPULL);
        (void)hcs12_gpio_set_output(&platform->cs_pins[temp], 1);
    }


    return 1;
}

void hcs12_spi_deinit(const struct hcs12_spi_platform *__FAR platform) {
    (void)platform;
}

uint8_t hcs12_spi_select(const struct hcs12_spi_platform *__FAR platform, uint8_t which, uint8_t is_select) {
    if (which >= platform->cs_num) return 0;
    (void)hcs12_gpio_set_output(&platform->cs_pins[which], 0 == is_select);
    return 1;
}

uint8_t hcs12_spi_transmit_byte(const struct hcs12_spi_platform *__FAR platform, uint8_t *dat) {
    uint16_t i;

    i = 0;
    for (;;) { // wait transmit emtpy
        if (platform->regs->sr.Bits.SPTEF) break;
        if (i > 10000) return 0;
        ++i;
    }


    platform->regs->dr.Overlap_STR.SPI0DRLSTR.Byte = *dat;


    i = 0;
    for (;;) { // wait data recved
        if (platform->regs->sr.Bits.SPIF) break;
        if (i > 10000) return 0;
        ++i;
    }

    *dat = platform->regs->dr.Overlap_STR.SPI0DRLSTR.Byte;
    return 1;
}

uint16_t hcs12_spi_transfer(const struct hcs12_spi_platform *__FAR platform, uint8_t *__FAR r, const uint8_t *__FAR w, uint16_t len) {
    return spi_transfer_use_transmit_onebyte(platform, &hcs12_spi_ops, r, w, len);
}


struct spi_operations hcs12_spi_ops = {
    hcs12_spi_init,
    hcs12_spi_deinit,
    hcs12_spi_select,
    hcs12_spi_transmit_byte,
    hcs12_spi_transfer,
};
