#include "transport.h"
#include "transceiver_platform.h"
#include "async/async.h"
#include "async/event.h"
#include "async/looper.h"
#include "mem_pool.h"

struct session {
    struct session *__FAR next;
    uint32_t pgn;
    async_event_t *event;
    uint16_t length;
    uint16_t offset;
    uint8_t dat[1];
};

static unsigned char mem_pool_buffer[2048];
static mem_pool_t mem_pool = (mem_pool_t)0;

struct j1939_transport {
    const struct transceiver  *__FAR can;
    uint8_t self_addr;
    async_event_t new_frame_event;
    struct session *__FAR sessions;
    async_looper_t looper;
};

static struct can_extended_frame *__FAR received_callback(j1939_transport_t tp, struct can_extended_frame *__FAR frame) {
    struct can_extended_frame *__FAR new_frame = mem_pool_malloc(&mem_pool, sizeof(struct can_extended_frame));
    if (0 == new_frame) {
        return frame;
    }

    async_event_trigger(tp->new_frame_event, frame);
    return new_frame;
}

static char on_new_frame(async_event_t event, struct can_extended_frame *__FAR frame) {
    j1939_transport_t tp = async_event_get_data(event);

    mem_pool_free(&mem_pool, frame);
    return 1;
}

void j1939_transport_run(j1939_transport_t tp, transceiver_t can, uint32_t baudrate, uint8_t self_addr) {
    struct can_extended_frame *__FAR frame ;

    mem_pool_add_buffer(&mem_pool, mem_pool_buffer, sizeof(mem_pool_buffer));
    frame = mem_pool_malloc(&mem_pool, sizeof(struct can_extended_frame));

    tp->can = can;
    tp->self_addr = self_addr;

    async_init();
    tp->looper = async_looper_create();
    tp->new_frame_event = async_event_register(tp->looper, on_new_frame, ASYNC_TIME_FOREVER, tp);
    transceiver_init(can, baudrate, received_callback, tp, frame);
}

