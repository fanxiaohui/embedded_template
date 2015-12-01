#include "spi_platform.h"

inline char spi_init(const struct spi_bus *__FAR bus, uint8_t flags) {
    return bus->ops->init(bus->private_data, flags);
}

inline char spi_config_clk_idle(const struct spi_bus *__FAR bus, uint8_t is_low) {
    return bus->ops->config_clk_idle(bus->private_data, is_low);
}

inline char spi_is_clk_idle_high(const struct spi_bus *__FAR bus) {
    return bus->ops->is_clk_idle_high(bus->private_data);
}

inline char spi_config_clk_edge(const struct spi_bus *__FAR bus, uint8_t is_first) {
    return bus->ops->config_clk_edge(bus->private_data, is_first);
}

inline char spi_is_clk_edge_first(const struct spi_bus *__FAR bus) {
    return bus->ops->is_clk_edge_first(bus->private_data);
}

inline char spi_config_first_bit(const struct spi_bus *__FAR bus, uint8_t is_lsb_first) {
    return bus->ops->config_first_bit(bus->private_data, is_lsb_first);
}

inline char spi_is_lsb_first(const struct spi_bus *__FAR bus) {
    return bus->ops->is_lsb_first(bus->private_data);
}

inline char spi_select(const struct spi_bus *__FAR bus, uint8_t which, char is_select) {
    return bus->ops->select(bus->private_data, which, is_select);
}

inline char spi_transmit(const struct spi_bus *__FAR bus, uint8_t *b) {
    return bus->ops->transmit(bus->private_data, b);
}

