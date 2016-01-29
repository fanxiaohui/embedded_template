#ifndef __SPI_PLATFORM_H__
#define __SPI_PLATFORM_H__

#include "spi.h"

typedef uint8_t (*spi_init_func)(void *__FAR private_data, uint8_t flags);
typedef uint8_t (*spi_config_clk_idle_func)(void *__FAR private_data, uint8_t is_high);
typedef uint8_t (*spi_is_clk_idle_high_fucn)(void *__FAR private_data);
typedef uint8_t (*spi_config_clk_edge_func)(void *__FAR private_data, uint8_t is_first_edge);
typedef uint8_t (*spi_is_clk_first_edge_func)(void *__FAR private_data);
typedef uint8_t (*spi_config_first_bit_func)(void *__FAR private_data, uint8_t is_lsb_first);
typedef uint8_t (*spi_is_lsb_first_func)(void *__FAR private_data);
typedef uint8_t (*spi_select_func)(void *__FAR private_data, uint8_t which, uint8_t is_select);
typedef uint8_t (*spi_transmit_func)(void *__FAR private_data, uint8_t *b);

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
    const struct spi_operations *ops;
};

#endif
