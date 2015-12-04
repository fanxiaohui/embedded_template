#ifndef __HCS12_SPI_H__
#define __HCS12_SPI_H__

#include <stdint.h>
#include "spi/spi.h"


typedef const struct hcs12_spi_platform *__FAR hcs12_spi_t;

char hcs12_spi_init(hcs12_spi_t spi, uint8_t flags);
char hcs12_spi_transfer(hcs12_spi_t spi, uint8_t *dat);

#endif
