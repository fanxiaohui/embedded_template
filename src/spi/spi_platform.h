#ifndef __SPI_PLATFORM_H__
#define __SPI_PLATFORM_H__

#include "spi.h"

struct spi_operations {
    char (*init)(void *private_data, uint8_t flags);
    char (*config_clk_idle)(void *private_data, uint8_t is_high);
    char (*is_clk_idle_high)(void *private_data);
    char (*config_clk_edge)(void *private_data, uint8_t is_first);
    char (*is_clk_edge_first)(void *private_data);
    char (*config_first_bit)(void *private_data, uint8_t is_lsb_first);
    char (*is_lsb_first)(void *private_data);
    char (*select)(void *private_data, uint8_t which, char is_select);
    char (*transmit)(void *private_data, uint8_t *b);
};

struct spi_bus {
    void *private_data;
    const struct spi_ops *ops;
};

#endif
