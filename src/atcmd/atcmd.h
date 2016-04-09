#ifndef __ATCMD_H__
#define __ATCMD_H__

#include "stdint.h"
#include "platform_os.h"

struct atcmd_interface {
    void (*init)(void);
    void (*send)(const uint8_t *buff, uint16_t size);
};

struct atcmd_expect {
    const char *expect;
    char *recv_buff;
    uint8_t buff_size;
};

typedef struct atcmd *atcmd_t;

typedef uint16_t (*cb_get_send_data)(void *private_data, char *buf, uint16_t len);


void atcmd_init(atcmd_t atcmd, uint8_t *buff, uint8_t buff_size, const struct atcmd_interface *iface);
void atcmd_recv_line(atcmd_t atcmd, const char *line, uint8_t size);
uint8_t atcmd_exec_command(atcmd_t atcmd, const char *cmd, const struct atcmd_expect *exp, os_time_t ms);
uint8_t atcmd_retry_until_reply(atcmd_t atcmd, const char *cmd, const char *reply, os_time_t ms, uint8_t times);
uint8_t atmc_retry_until_replys(atcmd_t atcmd, const char *cmd, const char *const *replys, os_time_t ms, uint8_t times);
uint8_t atcmd_get_ccid(atcmd_t atcmd, char *buf, uint8_t len);
uint8_t atcmd_get_imei(atcmd_t atcmd, char *buf, uint8_t len);
uint8_t atcmd_connect_tcp(atcmd_t atcmd, const char *addr, uint16_t port);
uint8_t atcmd_send_tcp_cb(atcmd_t atcmd, void *private_data, cb_get_send_data cb_get, uint16_t total_len);
uint8_t atcmd_get_signal_quality(atcmd_t atcmd, uint8_t *rssi, uint8_t *ber) ;
uint8_t atcmd_get_data_sent_info(atcmd_t atcmd, unsigned long *sent, unsigned long *acked, unsigned long *unacked) ;
uint8_t atcmd_get_ops(atcmd_t atcmd, char *buf, uint8_t len);
uint8_t atcmd_get_lacci(atcmd_t atcmd, uint16_t lac_ci[2]);

#endif