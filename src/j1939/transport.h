#ifndef __J1939_TRANSPORT_H__
#define __J1939_TRANSPORT_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

typedef const struct transceiver *__FAR transceiver_t;
typedef struct j1939_transport *__FAR j1939_transport_t;

void j1939_transport_run(j1939_transport_t tp, transceiver_t can, uint32_t baudrate, uint8_t self_addr);


#endif
