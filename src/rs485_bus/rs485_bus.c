#include "rs485_bus_platform.h"

inline uint8_t rs485_bus_init(struct rs485_bus *bus, uint16_t baud, void *dat, void (*on_recv_byte)(void *, uint8_t)) {
    if (!bus->ops->init(bus->private_data, dat, on_recv_byte)) {
        return 0;
    }

    if (!bus->ops->set_baud(bus->private_data, baud)) {
        return 0;
    }

    bus->baud = baud;
    return 1;
}

inline uint8_t rs485_bus_tx_byte(struct rs485_bus *bus, uint8_t b) {
    return bus->ops->tx_byte(bus->private_data, b);
}


inline uint8_t rs485_bus_tx_string(struct rs485_bus *bus, const uint8_t *s) {
    uint8_t rc = 0;
    while (*s) {
        bus->ops->tx_byte(bus->private_data, *s++);
        ++rc;
    }
    
    return rc;
}

inline uint8_t rs485_bus_tx_bytes(struct rs485_bus *bus, const uint8_t *dat, uint8_t len) {
    uint8_t i;
    for (i = 0; i < len; ++i) {
        bus->ops->tx_byte(bus->private_data, *dat++);
    }
    return i;
}
