#ifndef __SPI_PLATFORM_H__
#define __SPI_PLATFORM_H__

#include "spi.h"

typedef uint8_t (*spi_init_func)(void *__FAR private_data, uint8_t flags);
typedef void (*spi_deinit_func)(void *__FAR private_data);
typedef uint8_t (*spi_select_func)(void *__FAR private_data, uint8_t which, uint8_t is_select);
typedef uint8_t (*spi_transmit_byte_func)(void *__FAR private_data, uint8_t *b);
typedef uint16_t (*spi_transfer_func)(void *__FAR private_data, uint8_t *__FAR r, const uint8_t *__FAR w, uint16_t len);

struct spi_operations {
    spi_init_func init;
    spi_deinit_func deinit;
    spi_select_func select;
    spi_transmit_byte_func transmit_byte;
    spi_transfer_func transfer;
};

struct spi_bus {
    void *__FAR private_data;
    const struct spi_operations *ops;
};

uint16_t spi_transfer_use_transmit_onebyte(void *__FAR private_data, const struct spi_operations *ops, uint8_t *__FAR r, const uint8_t *__FAR w, uint16_t len);
#endif
