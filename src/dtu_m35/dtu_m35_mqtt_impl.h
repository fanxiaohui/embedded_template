#ifndef __DTU_M35_MQTT_IMPL_H__
#define __DTU_M35_MQTT_IMPL_H__

#include "stdint.h"
#include "platform_os.h"
#include "dtu_m35.h"

struct m35_mqtt {
    dtu_m35_t m35;
    uint8_t socket_id;
};

typedef struct dtu_m35_t m35_mqtt_t;

#define M35_MQTT_NETOWK(m35_mqtt) { \
    m35_mqtt, \
    (mqtt_network_read)dtum35_tcp_read, \
    (mqtt_network_write)dtum35_tcp_write, \
    (mqtt_network_connect)dtum35_tcp_connect, \
    (mqtt_network_disconnect)dtum35_tcp_disconnect, \
}

#endif

