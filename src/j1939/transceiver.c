#include "transceiver_platform.h"

inline uint8_t transceiver_init(transceiver_t can, uint32_t baudrate, transceiver_received_callback cb, void *__FAR dat, struct can_extended_frame *frame) {
    return can->ops->init(can->private_data, baudrate, cb, dat, frame);
}

inline uint8_t can_transceiver_send(transceiver_t can, const struct can_extended_frame *__FAR frame) {
    return can->ops->send(can->private_data, frame);
}


