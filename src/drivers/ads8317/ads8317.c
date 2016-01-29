#include "ads8317.h"
#include "ads8317_platform.h"

void ads8317_init(const struct ads8317_platform *__FAR dev) {
    (void)spi_init(&dev->bus, SPI_FLAG_CLK_IDLE_HIGH | SPI_FLAG_CLK_SECOND_EDGE | SPI_FLAG_MSB_FIRST);
}


int16_t ads8317_read(const struct ads8317_platform *__FAR dev) {
    uint8_t buf[3];
    uint16_t ret;

    (void)spi_select(&dev->bus, 0, 1);
    (void)spi_transmit(&dev->bus, &buf[0]);
    (void)spi_transmit(&dev->bus, &buf[1]);
    (void)spi_transmit(&dev->bus, &buf[2]);
    (void)spi_select(&dev->bus, 0, 0);

    ret = ((uint16_t)buf[0]) << 14;
    ret |= ((uint16_t)buf[1]) << 6;
    ret |= buf[2] >> 2;
    return (int16_t)ret;
}
