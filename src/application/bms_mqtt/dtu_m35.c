#include "dtu_m35_platform.h"
#include <stdio.h>
#include "misc.h"

#define dprintf printf


static void recv_at_default(struct dtu_m35 *m35, unsigned char b) {
    if (b == '\n') {
        if (m35->recv_line_index < 2) {
            m35->recv_line_index = 0;
            return;
        }

        m35->recv_line_buff[m35->recv_line_index++] = 0;

        atcmd_recv_line(&m35->atcmd, m35->recv_line_buff, m35->recv_line_index);
        m35->recv_line_index = 0;
        return;
    }

    if (b == '\r') {
        return;
    }
    if (m35->recv_line_index >= (sizeof(m35->recv_line_buff) - 2)) {
        return;
    }

    m35->recv_line_buff[m35->recv_line_index++] = b;
    if (m35->recv_line_index == 3) {
        if (memcmp(m35->recv_line_buff, "IPD", 3) != 0) {
            return;
        }
        // GOT "IPD" header, enter IPD recv flow.
        // dataLen = 0;
        //recvByte = recvByte_RECV_IPD_LENGTH;
        //tickEnterRecvAtReply = OSTimeGet() + OS_TICKS_PER_SEC;
        return;
    }

    if (m35->recv_line_index == 5) {
        if (memcmp(m35->recv_line_buff, "+CMT:", 5) != 0) {
            return;
        }
        // GOT "+CMT" header, enter CMT recv flow.
        // dataLen = 0;
        //recvByte = recvByte_RECV_CMT_NUM_PRE_QUOTE;
        //tickEnterRecvAtReply = OSTimeGet() + OS_TICKS_PER_SEC * 3 / 2;
        return;
    }
}

static uint8_t start_and_config_modem(struct dtu_m35 *m35) {
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
    // dprintf("[DTUM35]: Power is on, test AT\n");
    (void)atcmd_exec_command(&m35->atcmd, "AT", NULL, 500);
    (void)atcmd_retry_until_reply(&m35->atcmd, "AT", "Call Ready", 2000, 1);

    // 关闭回显.
    if (!atcmd_retry_until_reply(&m35->atcmd, "ATE0", "OK", 1200, 4)) {
        return 0;
    }

    // 设置AT+CREG?的输出格式.
    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+CREG=2", "OK", 1200, 4)) {
        return 0;
    }

    // 设置AT+CGREG?的输出格式.
    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+CGREG=0", "OK", 1200, 4)) {
        return 0;
    }

    // 设置短信为TEXT格式.
    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+CMGF=1", "OK", 1200, 4)) {
        return 0;
    }

    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+CSDH=0", "OK", 1200, 4)) {
        return 0;
    }

    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+CNMI=2,2,0,0,0", "OK", 1000, 5)) {
        return 0;
    }

    RUNTIME_STATUS_SET(m35, DTUM35_RUNTIME_STATUS_SIM_INIT);
    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+CPIN?", "+CPIN: READY", 1000, 10)) {
        return 0;
    }

    RUNTIME_STATUS_SET(m35, DTUM35_RUNTIME_STATUS_WAIT_REGISTER);
    {
        static const char *__FAR const creg_replys[] = {"+CREG: 2,1", "+CREG: 2,5", (const char *)0};
        if (!atcmd_retry_until_replys(&m35->atcmd, "AT+CREG?", creg_replys, 1000, 30)) {
            return 0;
        }
    }

    {
        static const char *__FAR const cgreg_replys[] = {"+CGREG: 0,1", "+CGREG: 0,5", (const char *)0};
        if (!atcmd_retry_until_replys(&m35->atcmd, "AT+CGREG?", cgreg_replys, 1000, 30)) {
            //runTimeStatusSetCurrentREGWarn();
            // dprintf("[M35]: Query CGREG failture\n");
        }
    }

    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+CMGF=1", "OK", 1000, 5)) {
        return 0;
    }

    if (!atcmd_retry_until_reply(&m35->atcmd, "AT+CSCS=\"GSM\"", "OK", 1000, 5)) {
        return 0;
    }

    (void)atcmd_retry_until_reply(&m35->atcmd, "AT+QIPROMPT=0", "OK", 500, 5);
    return 1;
}

static void stop_modem(struct dtu_m35 *m35) {
    if (m35->platform->is_poweron()) {
        m35->platform->power_key(1);
        os_sleep(1200);
        m35->platform->power_key(0);
        os_sleep(12000);
    }
    m35->platform->power_enable(0);
}

static void wait_to_restart(struct dtu_m35 *m35) {
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
    unsigned char i;
    memset(m35->imei, 0, sizeof(m35->imei));
    for (i = 0; i < 10; ++i) {
        if (atcmd_get_imei(&m35->atcmd, m35->imei, sizeof(m35->imei) - 1)) {
            return 1 ;
        }
    }
    return 0;
}


static uint8_t update_ccid(struct dtu_m35 *m35) {
    unsigned char i;
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
    memset(m35, 0, sizeof(*m35));
    m35->fsm_recv_byte = recv_at_default;
    platform->init();
    m35->platform = platform;
    atcmd_init(&m35->atcmd, m35->atcmd_buff, sizeof(m35->atcmd_buff), platform->serial_send);
}

void dtum35_recv_byte(struct dtu_m35 *m35, uint8_t b) {
    if (m35->fsm_recv_byte != recv_at_default) {
        if (os_get_time() > m35->time_recv_default) {
            m35->recv_line_index = 0;
            m35->fsm_recv_byte = recv_at_default;
        }
    }

    m35->fsm_recv_byte(m35, b);
}

uint8_t dtum35_run(struct dtu_m35 *m35) {
    while (1) {
        dprintf(__FILE__ "[%d]\n", __LINE__);
        if (!start_and_config_modem(m35)) goto __need_restart;
        while (1) {
            dprintf(__FILE__ "[%d]\n", __LINE__);
            update_ops(m35);
            dprintf(__FILE__ "[%d]\n", __LINE__);
            update_imei(m35);
            dprintf(__FILE__ "[%d]\n", __LINE__);
            update_ccid(m35);
            dprintf(__FILE__ "[%d]\n", __LINE__);
            update_signal_quality(m35);
            dprintf(__FILE__ "[%d]\n", __LINE__);
            update_lacci(m35);
            dprintf(__FILE__ "[%d]\n", __LINE__);
            update_data_send_info(m35);
            os_sleep(1000);
        }
__need_restart:
        dprintf(__FILE__ "[%d]\n", __LINE__);
        RUNTIME_STATUS_SET(m35, DTUM35_RUNTIME_STATUS_DELAT_TO_RESTART);
        stop_modem(m35);
        dprintf(__FILE__ "[%d]\n", __LINE__);
        wait_to_restart(m35);
    }
}