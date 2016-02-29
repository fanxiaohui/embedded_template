#ifndef __SOFTSPI_H__
#define __SOFTSPI_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

typedef struct softspi *__FAR softspi_t;

uint8_t softspi_init(softspi_t bus, uint8_t mode_flag);
void softspi_deinit(softspi_t bus);
uint8_t softspi_select(softspi_t bus, uint8_t which, uint8_t is_select);
uint8_t softspi_transmit_byte(softspi_t bus, uint8_t *dat);
uint16_t softspi_transfer(softspi_t bus, uint8_t *__FAR r, const uint8_t *__FAR w, uint16_t len);

#endif
