#include "./private.h"
#include "./transceiver.h"

#include "misc/list.h"

static LIST_HEAD(free_list);

struct can_extended_frame_with_head {
    struct list_head head;
    struct can_extended_frame frame;
};

static struct can_extended_frame_with_head frame_buffer_pool[20];

void j1939_databuffer_init(void) {
    uint8_t i;
    for (i = 0; i < sizeof(frame_buffer_pool)/sizeof(frame_buffer_pool[0]); ++i) {
        INIT_LIST_HEAD(&frame_buffer_pool[i].head);
        list_add(&frame_buffer_pool[i].head, &free_list);
    }
}

struct can_extended_frame *__FAR malloc_frame_buffer(void) {
    struct can_extended_frame_with_head *__FAR ret;
    if (list_empty(&free_list)) {
        return 0;
    }

    ret = (struct can_extended_frame_with_head *__FAR)free_list.next;
    list_del(&ret->head);
    return &ret->frame;
}

void free_frame_buffer(struct can_extended_frame *__FAR frame) {  
    struct can_extended_frame_with_head *__FAR frame_with_head = container_of(frame, struct can_extended_frame_with_head, frame);
    list_add(&frame_with_head->head, &free_list);
}

