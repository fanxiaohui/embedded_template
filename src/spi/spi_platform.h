#ifndef __SPI_PLATFORM_H__
#define __SPI_PLATFORM_H__

#include "spi.h"

typedef char (*spi_init_func)(void *__FAR private_data, uint8_t flags);
typedef char (*spi_config_clk_idle_func)(void *__FAR private_data, uint8_t is_high);
typedef char (*spi_is_clk_idle_high_fucn)(void *__FAR private_data);
typedef char (*spi_config_clk_edge_func)(void *__FAR private_data, uint8_t is_first_edge);
typedef char (*spi_is_clk_first_edge_func)(void *__FAR private_data);
typedef char (*spi_config_first_bit_func)(void *__FAR private_data, uint8_t is_lsb_first);
typedef char (*spi_is_lsb_first_func)(void *__FAR private_data);
typedef char (*spi_select_func)(void *__FAR private_data, uint8_t which, uint8_t is_select);
typedef char (*spi_transmit_func)(void *__FAR private_data, uint8_t *b);

struct spi_operations {
    spi_init_func init;
    spi_config_clk_idle_func config_clk_idle;
    spi_is_clk_idle_high_fucn is_clk_idle_high;
    spi_config_clk_edge_func config_clk_edge;
    spi_is_clk_idle_high_fucn is_clk_edge_first;
    spi_config_first_bit_func config_first_bit;
    spi_is_lsb_first_func is_lsb_first;
    spi_select_func select;
    spi_transmit_func transmit;
};

struct spi_bus {
    void *__FAR private_data;
    const struct spi_operations *__FAR ops;
};

#endif
