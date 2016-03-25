#ifndef __HCS12_SPI_H__
#define __HCS12_SPI_H__

#include "stdint.h"
#include "spi/spi.h"

typedef const struct hcs12_spi_platform *__FAR hcs12_spi_t;

uint8_t hcs12_spi_init(hcs12_spi_t spi, uint8_t flags);
uint8_t hcs12_spi_transmit_byte(hcs12_spi_t spi, uint8_t *dat);
uint16_t hcs12_spi_transfer(hcs12_spi_t spi, uint8_t *__FAR r, const uint8_t *__FAR w, uint16_t len);

#endif
