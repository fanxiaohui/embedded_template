#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>

typedef const struct spi_bus *spi_bus_t;

#define SPI_FLAG_CLK_IDEL_HIGH     0x01
#define SPI_FLAG_CLK_FIRST_EDGE    0x02
#define SPI_FLAG_LSB_FIRST         0x04

inline char spi_init(spi_bus_t bus, uint8_t flags);
inline char spi_config_clk_idle(spi_bus_t bus, uint8_t is_low);
inline char spi_is_clk_idle_low(spi_bus_t bus);
inline char spi_config_clk_edge(spi_bus_t bus, uint8_t is_first);
inline char spi_is_clk_edge_first(spi_bus_t bus);
inline char spi_config_first_bit(spi_bus_t bus, uint8_t is_lsb_first);
inline char spi_is_lsb_first(spi_bus_t bus);
inline char spi_select(spi_bus_t bus, uint8_t which, char is_select);
inline char spi_transmit(spi_bus_t bus, uint8_t *b);

#endif

