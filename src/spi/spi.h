#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

typedef const struct spi_bus *__FAR spi_bus_t;

#define SPI_FLAG_CLK_IDLE_HIGH     0x01
#define SPI_FLAG_CLK_IDLE_LOW      0x00
#define SPI_FLAG_CLK_FIRST_EDGE    0x02
#define SPI_FLAG_CLK_SECOND_EDGE   0x00
#define SPI_FLAG_LSB_FIRST         0x04
#define SPI_FLAG_MSB_FIRST         0x00

inline uint8_t spi_init(spi_bus_t bus, uint8_t flags);
inline void spi_deinit(spi_bus_t bus);
inline uint8_t spi_select(spi_bus_t bus, uint8_t which, uint8_t is_select);
inline uint8_t spi_transmit_byte(const struct spi_bus *__FAR bus, uint8_t *b);
inline uint16_t spi_transfer(spi_bus_t bus, uint8_t *__FAR r, const uint8_t *__FAR w, uint16_t len);

#endif

