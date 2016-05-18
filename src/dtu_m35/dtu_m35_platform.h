#ifndef __DTU_M35_PLATFORM_H__
#define __DTU_M35_PLATFORM_H__

#include "dtu_m35.h"
#include "atcmd_platform.h"

struct dtu_m35_platform {
    void (*init)(void);
    uint8_t (*is_poweron)(void);
    void (*power_enable)(uint8_t is_enable);
    void (*power_key)(uint8_t is_assert);
    void (*serial_send)(const uint8_t *dat, uint16_t len);
};

struct dtu_m35 {
    const struct dtu_m35_platform *platform;
    struct atcmd atcmd;
    uint8_t atcmd_buff[100];
    uint8_t ipd_buff[200];
    uint8_t recv_line_index;
    void (*fsm_recv_byte)(struct dtu_m35 *m35, uint8_t b);
    os_time_t time_recv_default;
    os_sem_t cmd_sem;
    uint8_t cmd_rb_buffer[100];
    struct ringbuffer cmd_rb;
    enum DTUM35_RUNTIME_STATUS runtime_status;
    uint8_t wait_time_index_for_restart;
    enum DTUM35_OPS ops;
    char imei[20];
    char ccid[24];
    uint16_t lacci[2];
    uint8_t rssi, ber;
    uint32_t data_acked;
};

#define RUNTIME_STATUS_SET(m35, status) do { m35->runtime_status = status; } while(0)

#endif
