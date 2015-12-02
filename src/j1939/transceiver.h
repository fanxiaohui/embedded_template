#ifndef __J1939_TRANSCEIVER_H__
#define __J1939_TRANSCEIVER_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

struct can_extended_frame {
    uint32_t id;
    uint8_t len;
    uint8_t dat[8];
};

typedef const struct can_transceiver *__FAR can_transceiver_t;

inline uint8_t can_transceiver_init(can_transceiver_t can, uint32_t baudrate);
inline uint8_t can_transceiver_recv(can_transceiver_t can, struct can_extended_frame *__FAR frame);
inline uint8_t can_transceiver_send(can_transceiver_t can, const struct can_extended_frame *__FAR frame);

#endif
