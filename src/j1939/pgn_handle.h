#ifndef __J1939_PNG_HANDLE_H__
#define __J1939_PNG_HANDLE_H__

#include <stdint.h>

typedef uint8_t (*j1939_png_handler)(const uint8_t *recv, uint16_t recv_len, uint8_t reply, uint16_t *reply_len);

#endif
