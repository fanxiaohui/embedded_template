#ifndef __J1939_PRIVATE_H__
#define __J1939_PRIVATE_H__

#include "transceiver.h"

struct can_extended_frame *malloc_frame_buffer(void);
void free_frame_buffer(struct can_extended_frame *frame);

#endif
