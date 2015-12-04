#ifndef __HCS12_SPI_PRIVATE_H__
#define __HCS12_SPI_PRIVATE_H__

#include "spi/spi_platform.h"
#include "hcs12_spi.h"
#include "hcs12_gpio_platform.h"
#include "derivative.h"

struct hcs12_spi_regs {
    volatile SPI0CR1STR cr1;
    volatile SPI0CR2STR cr2;
    volatile SPI0BRSTR br;
    volatile SPI0SRSTR sr;
    volatile SPI0DRSTR dr;
};

struct hcs12_spi_platform {
    struct hcs12_spi_regs *regs;
    uint8_t baud_reg_value;
    uint8_t cs_num;
    const struct hcs12_gpio *__FAR cs_pins;
};

extern struct spi_operations hcs12_spi_ops;



#endif
