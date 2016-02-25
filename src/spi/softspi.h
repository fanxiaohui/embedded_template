#ifndef __SOFTSPI_H__
#define __SOFTSPI_H__

#include <stdint.h>
#include "spi.h"

#ifndef __FAR
#define __FAR
#endif

typedef struct softspi *__FAR softspi_t;

uint8_t softspi_init(softspi_t bus, uint8_t mode_flag);
uint8_t softspi_config_clk_idle(softspi_t bus, uint8_t is_high);
uint8_t softspi_is_clk_idle_high(softspi_t bus);
uint8_t softspi_config_clk_edge(softspi_t bus, uint8_t is_first);
uint8_t softspi_is_clk_edge_first(softspi_t bus);
uint8_t softspi_config_first_bit(softspi_t bus, uint8_t is_lsb_first);
uint8_t softspi_is_lsb_first(softspi_t bus);
uint8_t softspi_select(softspi_t bus, uint8_t which, uint8_t is_select);
uint8_t softspi_transmit(softspi_t bus, uint8_t *dat);

#endif
