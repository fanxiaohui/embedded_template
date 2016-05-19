#include "dtu_m35_platform.h"
#include <stdio.h>
#include "misc.h"
#include "dtu_m35_mqtt_impl.h"

#define LOG_LEVEL LOG_LEVEL_TRACE

#include "logger.h"

static uint8_t start_and_config_modem(struct dtu_m35 *m35) {
    LOG(LOG_LEVEL_TRACE, "%s", __func__);
    RUNTIME_STATUS_SET(m35, DTUM35_RUNTIME_STATUS_POWERUP);
    while (!m35->platform->is_poweron()) {
        m35->platform->power_enable(1);
        os_sleep(1000);
        m35->platform->power_key(1);
        os_sleep(1800);
        m35->platform->power_key(0);
        os_sleep(800);
        if (m35->platform->is_poweron()) {
            os_sleep(5000);
            break;
        }
        m35->platform->power_enable(0);
        os_sleep(3000);
    }
    RUNTIME_STATUS_SET(m35, DTUM35_RUNTIME_STATUS_CONFIGURE);

    LOG(LOG_LEVEL_DEBUG, "测试AT命令");
    (void)atcmd_exec_command(&m35->atcmd, "AT", NULL, 500);
    (void)atcmd_retry_until_reply(&m35->atcmd, "AT", "Call Ready", 2000, 1);

    LOG(LOG_LEVEL_DEBUG, "关闭回显");
    if (!atcmd_retry_until_reply(&m35->atcmd, "ATE0", "OK", 1200, 4)) {
        goto __error;
    }

    LOG(LOG_LEVEL_DEBUG, "设置AT+CREG?的输出格式");
    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+CREG=2", "OK", 1200, 4)) {
        goto __error;
    }

    LOG(LOG_LEVEL_DEBUG, "设置AT+CGREG?的输出格式");
    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+CGREG=0", "OK", 1200, 4)) {
        goto __error;
    }

    LOG(LOG_LEVEL_DEBUG, "设置短信为TEXT格式");
    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+CMGF=1", "OK", 1200, 4)) {
        goto __error;
    }

    LOG(LOG_LEVEL_DEBUG, "设置GPRS数据头格式");
    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+CSDH=0", "OK", 1200, 4)) {
        goto __error;
    }

    LOG(LOG_LEVEL_DEBUG, "设置GPRS数据上报模式");
    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+QIHEAD=1", "OK", 1200, 1)) {
        //goto __error;
    }

    LOG(LOG_LEVEL_DEBUG, "设置短信上报模式");
    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+CNMI=2,2,0,0,0", "OK", 1000, 5)) {
        goto __error;
    }

    RUNTIME_STATUS_SET(m35, DTUM35_RUNTIME_STATUS_SIM_INIT);
    LOG(LOG_LEVEL_DEBUG, "检查SIM卡状态");
    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+CPIN?", "+CPIN: READY", 1000, 10)) {
        goto __error;
    }

    RUNTIME_STATUS_SET(m35, DTUM35_RUNTIME_STATUS_WAIT_REGISTER);
    {
        LOG(LOG_LEVEL_DEBUG, "检查GSM网络注册状态");
        static const char *__FAR const creg_replys[] = {"+CREG: 2,1", "+CREG: 2,5", (const char *)0};
        if (!atcmd_retry_until_replys(&m35->atcmd, "AT+CREG?", creg_replys, 1000, 30)) {
            goto __error;
        }
    }

    {
        LOG(LOG_LEVEL_DEBUG, "检查GPRS网络注册状态");
        static const char *__FAR const cgreg_replys[] = {"+CGREG: 0,1", "+CGREG: 0,5", (const char *)0};
        if (!atcmd_retry_until_replys(&m35->atcmd, "AT+CGREG?", cgreg_replys, 1000, 30)) {
            //runTimeStatusSetCurrentREGWarn();
            LOG(LOG_LEVEL_WARN, "GPRS网络注册失败");
        }
    }

    LOG(LOG_LEVEL_DEBUG, "设置短信为TEXT格式");
    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+CMGF=1", "OK", 1000, 5)) {
        goto __error;
    }

    LOG(LOG_LEVEL_DEBUG, "设置短信为GSM编码格式");
    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+CSCS=\"GSM\"", "OK", 1000, 5)) {
        goto __error;
    }

    (void)atcmd_retry_until_reply(&m35->atcmd, "AT+QIPROMPT=0", "OK", 500, 5);
    LOG(LOG_LEVEL_INFO, "配置成功");
    return 1;

__error:
    LOG(LOG_LEVEL_ERROR, "配置失败");
    return 0;
}


static void stop_modem(struct dtu_m35 *m35) {
    LOG(LOG_LEVEL_TRACE, "%s", __func__);
    if (m35->platform->is_poweron()) {
        m35->platform->power_key(1);
        os_sleep(1200);
        m35->platform->power_key(0);
        os_sleep(12000);
    }
    m35->platform->power_enable(0);
}

static void wait_to_restart(struct dtu_m35 *m35) {
    LOG(LOG_LEVEL_TRACE, "%s", __func__);
    static const os_time_t wait_time_table_for_restart [] = {
        1000UL * 10UL,
        1000UL * 30UL,
        1000UL * 100UL,
        1000UL * 300UL,
        1000UL * 900UL,
        1000UL * 1800UL,
        1000UL * 3600UL
    };

    os_sleep(wait_time_table_for_restart[m35->wait_time_index_for_restart]);

    if (m35->wait_time_index_for_restart < (ARRAY_SIZE(wait_time_table_for_restart) - 1)) {
        ++m35->wait_time_index_for_restart;
    }
}

static uint8_t update_ops(struct dtu_m35 *m35) {
    LOG(LOG_LEVEL_TRACE, "%s", __func__);
    char ops[32];
    if (!atcmd_get_ops(&m35->atcmd, ops, sizeof(ops))) {
        return 0;
    }

    if (0 == strcmp(ops, "CHINA MOBILE")) {
        m35->ops = DTU_OPS_CHINA_MOBILE;
    } else if (0 == strcmp(ops, "CHINA UNIONCOM")) {
        m35->ops = DTU_OPS_CHINA_UNIONCOM;
    } else if (0 == strcmp(ops, "CHINA TELECOM")) {
        m35->ops = DTU_OPS_CHINA_TELECOM;
    } else {
        m35->ops = DTU_OPS_UNKNOWN;
        return 0;
    }
    return 1;
}

static uint8_t update_imei(struct dtu_m35 *m35) {
    LOG(LOG_LEVEL_TRACE, "%s", __func__);
    uint8_t i;
    memset(m35->imei, 0, sizeof(m35->imei));
    for (i = 0; i < 10; ++i) {
        if (atcmd_get_imei(&m35->atcmd, m35->imei, sizeof(m35->imei) - 1)) {
            return 1 ;
        }
    }
    return 0;
}


static uint8_t update_ccid(struct dtu_m35 *m35) {
    LOG(LOG_LEVEL_TRACE, "%s", __func__);
    uint8_t i;
    memset(m35->ccid, 0, sizeof(m35->ccid));
    for (i = 0; i < 10; ++i) {
        if (atcmd_get_ccid(&m35->atcmd, m35->ccid, sizeof(m35->ccid) - 1)) {
            return 1;
        }
    }
    return 0;
}

static uint8_t update_signal_quality(struct dtu_m35 *m35) {
    return atcmd_get_signal_quality(&m35->atcmd, &m35->rssi, &m35->ber);
}

static uint8_t update_lacci(struct dtu_m35 *m35) {
    return atcmd_get_lacci(&m35->atcmd, m35->lacci);
}

static uint8_t update_data_send_info(struct dtu_m35 *m35) {
    unsigned long acked;
    unsigned long unacked;

    if (!atcmd_get_data_sent_info(&m35->atcmd, NULL, &acked, &unacked)) {
        return 0;
    }

    if (acked > m35->data_acked) {
        m35->data_acked = acked;
        return 1;
    }

    if (unacked > 0) {
        return 0;
    }

    return 1;
}


uint8_t dtum35_init(struct dtu_m35 *m35, const struct dtu_m35_platform *platform) {
    struct atcmd_init_param param = {
        platform->serial_send,
        m35->atcmd_buff,
        sizeof(m35->atcmd_buff),
        m35->ipd_buff,
        sizeof(m35->ipd_buff),
    };
    LOG(LOG_LEVEL_TRACE, "%s", __func__);
    memset(m35, 0, sizeof(*m35));
    ringbuffer_init(&m35->cmd_rb, m35->cmd_rb_buffer, sizeof(m35->cmd_rb_buffer));
    m35->cmd_sem = os_create_sem();
    //m35->fsm_recv_byte = recv_at_default;
    platform->init();
    m35->platform = platform;
    atcmd_init(&m35->atcmd, &param);
}

#if 0
void dtum35_recv_byte(struct dtu_m35 *m35, uint8_t b) {
    m35->atcmd.serial_recv(&m35->atcmd, b);
}
#endif

struct cmd_info {
    uint8_t cmd;
    uint8_t param_len;
};

struct cmd_param_disconnect_tcp {
    char *ret;
};

struct cmd_param_connect_tcp {
    const char *host;
    uint16_t port;
    char *ret;
};

struct cmd_param_recv_tcp {
    char *buf;
    uint16_t len;
    os_time_t timeout;
    uint16_t *len_read;
};

struct cmd_param_send_tcp {
    const uint8_t *dat;
    uint16_t len;
    os_time_t timeout;
    char *ret;
};

union cmd_param {
    struct cmd_param_connect_tcp connect_tcp;
    struct cmd_param_disconnect_tcp disconnect_tcp;
    struct cmd_param_send_tcp send_tcp;
    struct cmd_param_recv_tcp recv_tcp;
};

struct cmd_handle_map {
    uint8_t cmd;
    uint8_t param_len;
    void (*handle)(dtu_m35_t m35, union cmd_param *param);
};

enum {
    CMD_CONNECT_TCP,
    CMD_DISCONNECT_TCP,
    CMD_SEND_TCP,
    CMD_RECV_TCP,
};

static void handle_disconnect_tcp(dtu_m35_t m35, union cmd_param *param) {
    LOG(LOG_LEVEL_TRACE, "%s", __func__);
    struct cmd_param_disconnect_tcp *p = &param->disconnect_tcp;
    *(p->ret) = atcmd_disconnect_tcp(&m35->atcmd);
}

static void handle_connect_tcp(dtu_m35_t m35, union cmd_param *param) {
    LOG(LOG_LEVEL_TRACE, "%s", __func__);
    struct cmd_param_connect_tcp *p = &param->connect_tcp;
    *(p->ret) = atcmd_connect_tcp(&m35->atcmd, p->host, p->port);
}

static void handle_send_tcp(dtu_m35_t m35, union cmd_param *param) {
    LOG(LOG_LEVEL_TRACE, "%s", __func__);
    struct cmd_param_send_tcp *p = &param->send_tcp;
    *(p->ret) = atcmd_send_tcp(&m35->atcmd, p->dat, p->len, p->timeout);
}

static const struct cmd_handle_map cmd_handle_maps[] = {
    {CMD_SEND_TCP, sizeof(struct cmd_param_send_tcp), handle_send_tcp},
    {CMD_CONNECT_TCP, sizeof(struct cmd_param_connect_tcp), handle_connect_tcp},
    {CMD_DISCONNECT_TCP, sizeof(struct cmd_param_disconnect_tcp), handle_disconnect_tcp},
};


static const struct cmd_handle_map *get_cmd_handle(uint8_t cmd) {
    const struct cmd_handle_map *handle_map;
    for (handle_map = cmd_handle_maps; handle_map < &cmd_handle_maps[ARRAY_SIZE(cmd_handle_maps)]; ++handle_map) {
        if (handle_map->cmd == cmd) {
            return handle_map;
        }
    }
    return NULL;
}

uint8_t dtum35_run(struct dtu_m35 *m35) {
    struct cmd_info cmd_info;
    union cmd_param cmd_param;
    const struct cmd_handle_map *handle_map;

    LOG(LOG_LEVEL_TRACE, "%s", __func__);
    while (1) {
        if (!start_and_config_modem(m35)) {
            goto __need_restart;
        }
        while (1) {
            if (!os_pend_sem(m35->cmd_sem, 5000)) {
#if 0
                update_ops(m35);
                update_imei(m35);
                update_ccid(m35);
                update_signal_quality(m35);
                update_lacci(m35);
                update_data_send_info(m35);
#endif
                goto _clear_rb;
            }
            LOG(LOG_LEVEL_TRACE, "%s", __func__);
            if (sizeof(cmd_info) != ringbuffer_read(&m35->cmd_rb, (uint8_t *)&cmd_info, sizeof(cmd_info))) {
                goto _clear_rb;
            }

            LOG(LOG_LEVEL_TRACE, "%s", __func__);
            if (cmd_info.param_len > sizeof(union cmd_param)) {
                goto _clear_rb;
            }

            LOG(LOG_LEVEL_TRACE, "%s", __func__);
            if (cmd_info.param_len != ringbuffer_read(&m35->cmd_rb, (uint8_t *)&cmd_param, cmd_info.param_len)) {
                goto _clear_rb;
            }

            LOG(LOG_LEVEL_TRACE, "%s", __func__);
            handle_map = get_cmd_handle(cmd_info.cmd);
            if (handle_map == NULL) {
                goto _clear_rb;
            }

            LOG(LOG_LEVEL_TRACE, "%s", __func__);
            if (cmd_info.param_len != handle_map->param_len) {
                goto _clear_rb;
            }

            LOG(LOG_LEVEL_TRACE, "%s", __func__);
            handle_map->handle(m35, &cmd_param);
            continue;
_clear_rb:
            ringbuffer_clear(&m35->cmd_rb);
        }
__need_restart:
        RUNTIME_STATUS_SET(m35, DTUM35_RUNTIME_STATUS_DELAT_TO_RESTART);
        stop_modem(m35);
        wait_to_restart(m35);
    }
}

int dtum35_tcp_read(dtu_m35_t m35, uint8_t *buf, int len, os_time_t timeout_ms) {
    if (timeout_ms > 0) {
        LOG(LOG_LEVEL_TRACE, "%s %d bytes with timeout %dms", __func__, len, timeout_ms);
    }
    return atcmd_recv_tcp(&m35->atcmd, buf, len, timeout_ms);
}

int dtum35_tcp_write(dtu_m35_t m35, uint8_t *buffer, int len, os_time_t timeout_ms) {
    char ret;
    struct cmd_param_send_tcp param = {buffer, len, timeout_ms, &ret};
    struct cmd_info cmd_info = {CMD_SEND_TCP, sizeof(param)};
    RINGBUFFER_SIZE_TYPE size;
    LOG(LOG_LEVEL_DEBUG, "Try to send %d bytes to server with timeout %d", len, timeout_ms);

    OS_CRITICAL(
        size = ringbuffer_get_left_space(&m35->cmd_rb);
    );
    if (size < (sizeof(param) + sizeof(cmd_info))) {
        LOG(LOG_LEVEL_WARN, "Command buffer full");
        return 0;
    }

    ret = 0x7F;
    OS_CRITICAL(
        ringbuffer_write(&m35->cmd_rb, (uint8_t *)&cmd_info, sizeof(cmd_info));
        ringbuffer_write(&m35->cmd_rb, (uint8_t *)&param, sizeof(param));
        os_post_sem(m35->cmd_sem);
    );

    LOG(LOG_LEVEL_DEBUG, "Wait for send...");
    do {
        os_sleep(2);
    } while (ret == 0x7F);

    LOG(LOG_LEVEL_INFO, "Send TCP return %d", ret);
    if (ret) {
        return len;
    }
    return -1;
}

int dtum35_tcp_connect(dtu_m35_t m35, const char *host, uint16_t port) {
    char ret;
    struct cmd_param_connect_tcp param = {host, port, &ret};
    struct cmd_info cmd_info = {CMD_CONNECT_TCP, sizeof(param)};
    RINGBUFFER_SIZE_TYPE size;
    LOG(LOG_LEVEL_DEBUG, "Try to connect %s:%d", host, port);

    OS_CRITICAL(
        size = ringbuffer_get_left_space(&m35->cmd_rb);
    );
    if (size < (sizeof(param) + sizeof(cmd_info))) {
        LOG(LOG_LEVEL_WARN, "Command buffer full");
        return 0;
    }

    ret = 0x7F;
    OS_CRITICAL(
        ringbuffer_write(&m35->cmd_rb, (uint8_t *)&cmd_info, sizeof(cmd_info));
        ringbuffer_write(&m35->cmd_rb, (uint8_t *)&param, sizeof(param));
        os_post_sem(m35->cmd_sem);
    );

    LOG(LOG_LEVEL_DEBUG, "Wait for connet ...");
    do {
        os_sleep(2);
    } while (ret == 0x7F);

    LOG(LOG_LEVEL_INFO, "Connect TCP return %d", ret);
    return ret;
}

int dtum35_tcp_disconnect(dtu_m35_t m35) {
    char ret;
    struct cmd_param_disconnect_tcp param = {&ret};
    struct cmd_info cmd_info = {CMD_DISCONNECT_TCP, sizeof(param)};
    RINGBUFFER_SIZE_TYPE size;
    LOG(LOG_LEVEL_DEBUG, "Try to disconnect TCP");

    OS_CRITICAL(
        size = ringbuffer_get_left_space(&m35->cmd_rb);
    );
    if (size < (sizeof(param) + sizeof(cmd_info))) {
        LOG(LOG_LEVEL_WARN, "Command buffer full");
        return 0;
    }

    ret = 0x7F;
    OS_CRITICAL(
        ringbuffer_write(&m35->cmd_rb, (uint8_t *)&cmd_info, sizeof(cmd_info));
        ringbuffer_write(&m35->cmd_rb, (uint8_t *)&param, sizeof(param));
        os_post_sem(m35->cmd_sem);
    );

    LOG(LOG_LEVEL_DEBUG, "Wait for disconnet ...");
    do {
        os_sleep(2);
    } while (ret == 0x7F);

    LOG(LOG_LEVEL_INFO, "Disconnect TCP return %d", ret);
    return ret;
}

