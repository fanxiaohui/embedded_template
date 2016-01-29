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
inline uint8_t spi_config_clk_idle(spi_bus_t bus, uint8_t is_low);
inline uint8_t spi_is_clk_idle_high(spi_bus_t bus);
inline uint8_t spi_config_clk_edge(spi_bus_t bus, uint8_t is_first);
inline uint8_t spi_is_clk_edge_first(spi_bus_t bus);
inline uint8_t spi_config_first_bit(spi_bus_t bus, uint8_t is_lsb_first);
inline uint8_t spi_is_lsb_first(spi_bus_t bus);
inline uint8_t spi_select(spi_bus_t bus, uint8_t which, uint8_t is_select);
inline uint8_t spi_transmit(spi_bus_t bus, uint8_t *b);

#endif

