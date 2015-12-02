#ifndef __J1939_TRANSPORT_H__
#define __J1939_TRANSPORT_H__

#include "transport.h"
#include "transceiver.h"
#include "list/list.h"

struct j1939_tp {
    struct can_transceiver transceiver;
    LIST_HEAD tps;
};

#endif
