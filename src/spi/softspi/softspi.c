#include "softspi_platform.h"
#include "spi/spi_platform.h"

char softspi_init(struct softspi *bus, uint8_t flags) {
    uint8_t i;
    const struct softspi_platform *plat = bus->platform;
    gpio_ops_t gpio_ops = plat->gpio_ops;

    bus->flags = flags;

    for (i = 0; i < plat->cs_num; ++i) {
        gpio_init(gpio_ops, plat->cs_pin[i], GPIO_MODE_OUTPUT_PUSHPULL);
        gpio_set_output(gpio_ops, plat->cs_pin[i], 1);
    }

    gpio_init(gpio_ops, plat->clk, GPIO_MODE_OUTPUT_PUSHPULL);
    gpio_set_output(gpio_ops, plat->clk, GPIO_MODE_OUTPUT_PUSHPULL);
    gpio_init(gpio_ops, plat->dat, GPIO_MODE_OUTPUT_PUSHPULL);
    gpio_set_output(gpio_ops, plat->dat, GPIO_MODE_OUTPUT_PUSHPULL);

    gpio_set_output(gpio_ops, plat->clk, flags & SPI_FLAG_CLK_IDEL_HIGH);

    return 1;
}

char softspi_config_clk_idle(struct softspi *bus, uint8_t is_high) {
    const struct softspi_platform *plat = bus->platform;
    gpio_ops_t gpio_ops = plat->gpio_ops;

    if (is_high) {
        bus->flags |= SPI_FLAG_CLK_IDEL_HIGH;
        gpio_set_output(gpio_ops, plat->clk, 1);
    } else {
        bus->flags &= ~SPI_FLAG_CLK_IDEL_HIGH;
        gpio_set_output(gpio_ops, plat->clk, 0);
    }

    return 1;
}

char softspi_is_clk_idle_high(struct softspi *bus) {
    return (bus->flags & SPI_FLAG_CLK_IDEL_HIGH) != 0;
}

char softspi_config_clk_edge(struct softspi *bus, uint8_t is_first) {
    if (is_first) {
        bus->flags |= SPI_FLAG_CLK_FIRST_EDGE;
    } else {
        bus->flags &= ~SPI_FLAG_CLK_FIRST_EDGE;
    }
    return 1;
}

char softspi_is_clk_edge_first(struct softspi *bus) {
    return (bus->flags & SPI_FLAG_CLK_FIRST_EDGE) != 0;
}

char softspi_config_first_bit(struct softspi *bus, uint8_t is_lsb_first) {
    if (is_lsb_first) {
        bus->flags |= SPI_FLAG_LSB_FIRST;
    } else {
        bus->flags &= ~SPI_FLAG_LSB_FIRST;
    }

    return 1;

}

char softspi_is_lsb_first(struct softspi *bus) {
    return (bus->flags & SPI_FLAG_LSB_FIRST ) != 0;
}


char softspi_select(struct softspi *bus, uint8_t which, char is_select) {
    const struct softspi_platform *plat = bus->platform;
    gpio_ops_t gpio_ops = plat->gpio_ops;

    if (which >= plat->cs_num) {
        return 0;
    }

    gpio_set_output(gpio_ops, plat->cs_pin[which], is_select == 0);
}

inline static char __transmit_1st_edge_msb_first(struct softspi_platform const *plat, uint8_t *dat) {
}

inline static char __transmit_2st_edge_msb_first(struct softspi_platform const *plat, uint8_t *dat) {
}

inline static char __transmit_1st_edge_lsb_first(struct softspi_platform const *plat, uint8_t *dat) {
}

inline static char __transmit_2st_edge_lsb_first(struct softspi_platform const *plat, uint8_t *dat) {
}

char softspi_transmit(struct softspi *bus, uint8_t *dat) {
    if (bus->flags & SPI_FLAG_LSB_FIRST) {
        if (bus->flags & SPI_FLAG_CLK_FIRST_EDGE) {
            return __transmit_1st_edge_lsb_first(bus->platform, dat);
        } else {
            return __transmit_2st_edge_lsb_first(bus->platform, dat);
        }
    } else {
        if (bus->flags & SPI_FLAG_CLK_FIRST_EDGE) {
            return __transmit_1st_edge_msb_first(bus->platform, dat);
        } else {
            return __transmit_2st_edge_msb_first(bus->platform, dat);
        }
    }
}

const struct spi_operations softspi_ops = {
    .init = softspi_init,
    .config_clk_idle = softspi_config_clk_idle,
    .is_clk_idle_high = softspi_is_clk_idle_high,
    .config_clk_edge = softspi_config_clk_edge,
    .is_clk_edge_first = softspi_is_clk_edge_first,
    .config_first_bit = softspi_config_first_bit,
    .is_lsb_first = softspi_is_lsb_first,
    .select = softspi_select,
    .transmit = softspi_transmit,
};

