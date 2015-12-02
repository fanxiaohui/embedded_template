#ifndef __J1939_TRANSPORT_H__
#define __J1939_TRANSPORT_H__

#ifndef __FAR
#define __FAR
#endif

typedef struct j1939_tp *__FAR j1939_tp_t;

void j1939_transport_run(j1939_tp_t tp);

#endif
