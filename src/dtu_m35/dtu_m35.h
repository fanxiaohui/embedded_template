#ifndef __DTU_M35_H__
#define __DTU_M35_H__

#include "stdint.h"
#include "platform_os.h"

enum DTUM35_RUNTIME_STATUS {
    DTUM35_RUNTIME_STATUS_UNKNOWN = 0,
    DTUM35_RUNTIME_STATUS_POWERUP,
    DTUM35_RUNTIME_STATUS_CONFIGURE,
    DTUM35_RUNTIME_STATUS_WAIT_REGISTER,
    DTUM35_RUNTIME_STATUS_SEND_SMS,
    DTUM35_RUNTIME_STATUS_CONNECT_SERVER,
    DTUM35_RUNTIME_STATUS_GET_SIGNAL_QUALITY,
    DTUM35_RUNTIME_STATUS_CHECK_CONNECTION,
    DTUM35_RUNTIME_STATUS_WAIT_EVENT,
    DTUM35_RUNTIME_STATUS_SEND_DATA,
    DTUM35_RUNTIME_STATUS_RECV_DATA,
    DTUM35_RUNTIME_STATUS_RECV_SMS,
    DTUM35_RUNTIME_STATUS_DELAT_TO_RESTART,
    DTUM35_RUNTIME_STATUS_SIM_INIT,
};

enum DTUM35_OPS {
    /// 未知运营商.
    DTU_OPS_UNKNOWN = 0,
    /// 中国联通.
    DTU_OPS_CHINA_UNIONCOM = 1,
    /// 中国移动.
    DTU_OPS_CHINA_MOBILE = 2,
    /// 中国电信.
    DTU_OPS_CHINA_TELECOM = 3,
};

typedef struct dtu_m35 *dtu_m35_t;
typedef const struct dtu_m35_platform *dtu_m35_platform_t;

uint8_t dtum35_init(dtu_m35_t m35, dtu_m35_platform_t platform);
uint8_t dtum35_run(dtu_m35_t m35);

int dtum35_tcp_read(dtu_m35_t m35, unsigned char* buffer, int len, os_time_t timeout_ms);
int dtum35_tcp_write(dtu_m35_t m35, unsigned char* buffer, int len, os_time_t timeout_ms);
int dtum35_tcp_connect(dtu_m35_t m35, const char *host, uint16_t port);
int dtum35_tcp_disconnect(dtu_m35_t m35);
void dtum35_recv_byte(dtu_m35_t m35, uint8_t b);

#endif
