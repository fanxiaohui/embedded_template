#include "spi_platform.h"


uint8_t spi_init(const struct spi_bus *__FAR bus, uint8_t flags) {
    return bus->ops->init(bus->private_data, flags);
}

void spi_deinit(spi_bus_t bus) {
    bus->ops->deinit(bus->private_data);
}

uint8_t spi_select(const struct spi_bus *__FAR bus, uint8_t which, uint8_t is_select) {
    return bus->ops->select(bus->private_data, which, is_select);
}

uint8_t spi_transmit_byte(const struct spi_bus *__FAR bus, uint8_t *b) {
    return bus->ops->transmit_byte(bus->private_data, b);
}

uint16_t spi_transfer(const struct spi_bus *__FAR bus, uint8_t *__FAR r, const uint8_t *__FAR w, uint16_t len) {
    return bus->ops->transfer(bus->private_data, r, w, len);
}

static uint16_t send(void *__FAR private_data, const struct spi_operations *ops, const uint8_t *__FAR w, uint16_t len) {
    uint8_t tmp;
    uint16_t i;

    spi_transmit_byte_func transmit_byte = ops->transmit_byte;

    for (i = 0; i < len; ++i) {
        tmp = *w++;
        if (!transmit_byte(private_data, &tmp)) {
            break;
        }
    }
    return i;
}

static uint16_t recv(void *__FAR private_data, const struct spi_operations *ops, uint8_t *__FAR r, uint16_t len) {
    uint8_t tmp;
    uint16_t i;

    spi_transmit_byte_func transmit_byte = ops->transmit_byte;

    for (i = 0; i < len; ++i) {
        if (!transmit_byte(private_data, &tmp)) {
            break;
        }
        *r++ = tmp;
    }
    return i;
}

static uint16_t send_and_recv(void *__FAR private_data, const struct spi_operations *ops, uint8_t *__FAR r, const uint8_t *__FAR w, uint16_t len) {
    uint8_t tmp;
    uint16_t i;

    spi_transmit_byte_func transmit_byte = ops->transmit_byte;

    for (i = 0; i < len; ++i) {
        tmp = *w++;
        if (!transmit_byte(private_data, &tmp)) {
            break;
        }
        *r++ = tmp;
    }
    return i;
}

uint16_t spi_transfer_use_transmit_onebyte(void *__FAR private_data, const struct spi_operations *ops, uint8_t *__FAR r, const uint8_t *__FAR w, uint16_t len) {
    if (r && w) return send_and_recv(private_data, ops, r, w, len);
    if (r) return recv(private_data, ops, r, len);
    if (w) return send(private_data, ops, w, len);
    return 0;
}
