#include "ads8317.h"
#include "ads8317_platform.h"

void ads8317_init(const struct ads8317_platform *__FAR dev) {
    spi_init(&dev->bus, SPI_FLAG_CLK_IDLE_HIGH);
}


int16_t ads8317_read(const struct ads8317_platform *__FAR dev) {
    uint8_t buf[3];
    uint16_t ret;

    spi_select(&dev->bus, 0, 1);
    spi_transmit(&dev->bus, &buf[0]);
    spi_transmit(&dev->bus, &buf[1]);
    spi_transmit(&dev->bus, &buf[2]);
    spi_select(&dev->bus, 0, 0);

    ret = ((uint16_t)buf[0]) << 14;
    ret |= ((uint16_t)buf[1]) << 6;
    ret |= (uint16_t)buf[2] & 0x03;
    return (int16_t)ret;
}
