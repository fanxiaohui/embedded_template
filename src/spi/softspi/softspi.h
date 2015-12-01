#ifndef __SOFTSPI_H__
#define __SOFTSPI_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

typedef struct softspi *__FAR softspi_t;

char softspi_init(softspi_t bus, uint8_t mode_flag);
char softspi_config_clk_idle(softspi_t bus, uint8_t is_high);
char softspi_is_clk_idle_high(softspi_t bus);
char softspi_config_clk_edge(softspi_t bus, uint8_t is_first);
char softspi_is_clk_edge_first(softspi_t bus);
char softspi_config_first_bit(softspi_t bus, uint8_t is_lsb_first);
char softspi_is_lsb_first(softspi_t bus);
char softspi_select(softspi_t bus, uint8_t which, char is_select);
char softspi_transmit(softspi_t bus, uint8_t *__FAR dat);

extern const struct spi_ops softspi_ops;

#endif
