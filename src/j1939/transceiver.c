#include "transceiver_platform.h"

inline uint8_t can_transceiver_init(can_transceiver_t can, uint32_t baudrate) {
    return can->ops->init(can->private_data, baudrate);
}

inline uint8_t can_transceiver_recv(can_transceiver_t can, struct can_extended_frame *__FAR frame) {
    return can->ops->recv(can->private_data, frame);
}

inline uint8_t can_transceiver_send(can_transceiver_t can, const struct can_extended_frame *__FAR frame) {
    return can->ops->send(can->private_data, frame);
}


