#include "transport_platform.h"

struct session {
    uint8_t status;
};

void j1939_transport_run(struct j1939_tp *__FAR tp) {
    struct can_extended_frame frame;
    can_transceiver_init(&tp->transceiver);

    for (;;) {
        can_transceiver_recv(&tp->transceiver, &frame);
    }
}

