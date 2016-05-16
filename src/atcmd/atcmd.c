#include "atcmd_platform.h"
#include "platform_os.h"
#include "string.h"
#include "stdio.h"

#define DEBUG_LEVEL DEBUG_LEVEL_TRACE

#include "debug.h"

static void send_string(atcmd_t atcmd, const char *s) {
    atcmd->send((const unsigned char *)s, strlen(s));
}

static void send_byte(atcmd_t atcmd, uint8_t b) {
    atcmd->send(&b, 1);
}

static void send_bytes(atcmd_t atcmd, const uint8_t *s, uint16_t size) {
    atcmd->send(s, size);
}

static void clear_recv_buffer(atcmd_t atcmd) {
    OS_CRITICAL(
        ringbuffer_clear(&atcmd->rb);
    );
    os_clear_sem(atcmd->sem);
}

static uint8_t expect(atcmd_t atcmd, const struct atcmd_expect *exp, os_time_t ms) {
    RINGBUFFER_SIZE_TYPE size;
    uint8_t rc;

    os_time_t now = os_get_time();
    os_time_t end = now + ms;

    for (; now < end; now = os_get_time()) {
        dprintf(DEBUG_LEVEL_DEBUG, "wait for reply\n");
        if (0 == os_pend_sem(atcmd->sem, end - now)) {
            dprintf(DEBUG_LEVEL_DEBUG, "wait for reply timeout\n");
            return 0;
        }

        size = exp->buff_size;
        OS_CRITICAL(
            rc =  ringbuffer_read_string_and_is_with_prefix(
                      &atcmd->rb,
                      exp->expect,
                      exp->recv_buff,
                      &size);
        );
        if (rc) {
            dprintf(DEBUG_LEVEL_DEBUG, "get reply \"%s\"\n", exp->recv_buff);
            return 1;
        }
    }
    return 0;
}

void atcmd_init(atcmd_t atcmd, uint8_t *buff, uint8_t buff_size, void (*send)(const uint8_t *dat, uint16_t size)) {
    atcmd->send = send;
    ringbuffer_init(&atcmd->rb, buff, buff_size);
    atcmd->sem = os_create_sem();
}

void atcmd_recv_line(atcmd_t atcmd, const char *line, uint8_t size) {
    dprintf(DEBUG_LEVEL_DEBUG, "ATCMD<- [%d]%s\n", size, line);
    OS_CRITICAL(
        (void)ringbuffer_write(&atcmd->rb, line, size);
    );
    os_post_sem(atcmd->sem);
}

uint8_t atcmd_exec_command(atcmd_t atcmd, const char *cmd, const struct atcmd_expect *exp, os_time_t ms) {
    if (cmd != NULL) {
        clear_recv_buffer(atcmd);
        dprintf(DEBUG_LEVEL_DEBUG, "ATCMD-> %s\n", cmd);
        send_string(atcmd, cmd);
        if (*cmd) {
            send_byte(atcmd, '\r');
        }
    }

    if (exp == NULL || exp->expect == NULL) {
        os_sleep(ms);
        return 1;
    }

    return expect(atcmd, exp, ms);
}

uint8_t atcmd_retry_until_reply(atcmd_t atcmd, const char *cmd, const char *reply, os_time_t ms, uint8_t times) {
    struct atcmd_expect exp;

    exp.expect = reply;
    exp.recv_buff = NULL;
    exp.buff_size = 0;

    while (times--) {
        if (atcmd_exec_command(atcmd, cmd, &exp, ms)) {
            return 1;
        }
    }
    return 0;
}


uint8_t atcmd_retry_until_replys(atcmd_t atcmd, const char *cmd, const char *const *replys, os_time_t ms, uint8_t times) {
    struct atcmd_expect exp;
    char buf[24];

    exp.expect = "";
    exp.recv_buff = buf;
    exp.buff_size = sizeof(buf);

    while (times--) {
        const char *const *reply;
        volatile os_time_t now;
        os_time_t end = os_get_time() + ms;

        (void)atcmd_exec_command(atcmd, cmd, NULL, 0);
        for (now = os_get_time(); now < end; now = os_get_time()) {
            if (!atcmd_exec_command(atcmd, NULL, &exp, end - now)) {
                continue;
            }

            for (reply = replys; *reply != 0; ++reply) {
                if (memcmp(buf, *reply, strlen(*reply)) == 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

uint8_t atcmd_get_ccid(atcmd_t atcmd, char *buf, uint8_t len) {
    uint8_t i;
    struct atcmd_expect exp;
    volatile os_time_t now = os_get_time();
    os_time_t end = now + 1000;

    exp.expect = "";
    exp.recv_buff = buf;
    exp.buff_size = len;

    (void)atcmd_exec_command(atcmd, "AT+QCCID", NULL, 0);
    for (; now < end;  now = os_get_time()) {
        if (!atcmd_exec_command(atcmd, NULL, &exp, end - now)) {
            continue;
        }
        if (strlen(buf) > 18 && strlen(buf) <= 21) {
            return 1;
        }
    }

    return 0;
}


uint8_t atcmd_get_imei(atcmd_t atcmd, char *buf, uint8_t len) {
    uint8_t i;
    struct atcmd_expect exp;
    volatile os_time_t now = os_get_time();
    os_time_t end = now + 1000;

    exp.expect = "";
    exp.recv_buff = buf;
    exp.buff_size = len;

    (void)atcmd_exec_command(atcmd, "AT+GSN", NULL, 0);
    for (; now < end;  now = os_get_time()) {
        memset(buf, 0, len);
        if (!atcmd_exec_command(atcmd, NULL, &exp, end - now)) {
            continue;
        }
        if (strlen(buf) > 12 && strlen(buf) < 30) {
            return 1;
        }
    }
    return 0;
}

static unsigned char is_ip_string(const char *s) {
    long ipseg;
    const char *p = s;
    if (!s) {
        return 0;
    }
    ipseg = strtol(p, &p, 10);
    if (ipseg < 0 || ipseg > 255 || *p != '.') {
        return 0;
    }
    ipseg = strtol(p + 1, &p, 10);
    if (ipseg < 0 || ipseg > 255 || *p != '.') {
        return 0;
    }
    ipseg = strtol(p + 1, &p, 10);
    if (ipseg < 0 || ipseg > 255 || *p != '.') {
        return 0;
    }
    ipseg = strtol(p + 1, &p, 10);
    if (ipseg < 0 || ipseg > 255 || *p != 0) {
        return 0;
    }
    return 1;
}

uint8_t atcmd_connect_tcp(atcmd_t atcmd, const char *addr, uint16_t port) {
    if (!atcmd_retry_until_reply(atcmd, is_ip_string(addr) ? "AT+QIDNSIP=0" : "AT+QIDNSIP=1", "OK", 1000, 5)) {
        return 0;
    }

    if (addr && (port != 0)) {
        struct atcmd_expect exp;
        char line[48];
        exp.recv_buff = line;
        exp.buff_size = sizeof(line);

        send_string(atcmd, "AT+QIOPEN=\"TCP\",\"");
        send_string(atcmd, addr);
        (void)sprintf(line, "\",\"%d\"", port);
        exp.expect = "";
        if (!atcmd_exec_command(atcmd, line, &exp, 1000)) {
            return 0;
        }

        if (strcmp(line, "ALREADY CONNECT") == 0) {
            return 1;
        }
        if (strcmp(line, "OK") != 0) {
            return 0;
        }

        exp.expect = "CONNECT";
        if (!atcmd_exec_command(atcmd, NULL, &exp, 100UL * 1000UL)) {
            return 0;
        }
        if (strcmp(line, "CONNECT OK") == 0) {
            return 1;
        }
        return 0;
    }
    return 0;
}



uint8_t atcmd_send_tcp_cb(atcmd_t atcmd, void *private_data, cb_get_send_data cb_get, uint16_t total_len) {
    char buf[20];
    os_time_t t, timeout;
    struct atcmd_expect exp;
    uint16_t sent = 0;

    if (!cb_get || total_len <= 0) {
        return 0;
    }

    exp.expect = "";
    exp.recv_buff = buf;
    exp.buff_size = sizeof(buf);

    while (sent < total_len) {
        uint16_t this_len = total_len - sent;
        if (this_len > 1320) {
            this_len = 1320;
        }
        sent += this_len;

        (void)sprintf((char *)buf, "AT+QISEND=%d", this_len);
        clear_recv_buffer(atcmd);
        (void)atcmd_exec_command(atcmd, (char *)buf, NULL, 250);

        do {
            uint16_t i;
            i = cb_get(private_data, buf, (this_len > sizeof(buf)) ? sizeof(buf) : this_len);
            if (i == 0) {
                break;
            }
            send_bytes(atcmd, (unsigned char *)buf, i);
            this_len -= i;
        } while (this_len > 0);

        while (this_len-- > 0) {
            send_byte(atcmd, 0x00);
        }

        timeout = os_get_time() + 1000;
        for (t = os_get_time(); t < timeout; t = os_get_time()) {
            if (!atcmd_exec_command(atcmd, NULL, &exp, timeout - t )) {
                return 0;
            }

            if (strcmp("SEND OK", (char *)buf) == 0) {
                break;
            }
            if (strcmp("SEND FAIL", (char *)buf) == 0) {
                return 0;
            }
            if (strcmp("ERROR", (char *)buf) == 0) {
                return 0;
            }
        }
    }

    return 1;
}

static long atoi_skip_bank_prefix(const char *s) {
    long ret = 0;
    if (!s) {
        return -1;
    }

    while (*s == ' ' || *s == '\t') {
        ++s;
    }

    return atoi(s);
}


uint8_t atcmd_get_signal_quality(atcmd_t atcmd, uint8_t *rssi, uint8_t *ber) {
    char line[20];
    char *p;
    struct atcmd_expect exp;
    unsigned char this_rssi;
    unsigned char this_ber;

    exp.expect = "+CSQ:";
    exp.recv_buff = line;
    exp.buff_size = sizeof(line);

    if (!atcmd_exec_command(atcmd, "AT+CSQ", &exp, 500)) {
        return 0;
    }

    // "+CSQ: 28, 0"
    p = &line[5];
    this_rssi = atoi_skip_bank_prefix(p);
    if (this_rssi > 31 && this_rssi != 99) {
        return 0;
    }


    p = strchr(p + 1, ',');
    if (!p) {
        return 0;
    }

    this_ber = atoi_skip_bank_prefix(++p);
    if (this_ber > 7 && this_ber != 99) {
        return 0;
    }

    if (rssi) {
        *rssi = this_rssi;
    }

    if (ber) {
        *ber = this_ber;
    }

    return 1;
}

uint8_t atcmd_get_data_sent_info(atcmd_t atcmd, unsigned long *sent, unsigned long *acked, unsigned long *unacked) {
    char line[48];
    char *p;
    struct atcmd_expect exp;
    long this_sent;
    long this_acked;
    long this_unacked;


    exp.recv_buff = line;
    exp.buff_size = sizeof(line);

    exp.expect = "+QISACK:";
    if (!atcmd_exec_command(atcmd, "AT+QISACK", &exp, 1000)) {
        return 0;
    }

    //+QISACK: <sent>, <acked>, <nAcked>
    p = &line[8];
    this_sent = atoi_skip_bank_prefix(p);
    if (this_sent == -1) {
        return 0;
    }
    p = strchr(p, ',');
    if (!p) {
        return 0;
    }

    this_acked = atoi_skip_bank_prefix(++p);
    if (this_acked == -1) {
        return 0;
    }

    p = strchr(p, ',');
    if (!p) return 0;

    this_unacked = atoi_skip_bank_prefix(++p);
    if (this_unacked == -1) return 0;

    if (sent) {
        *sent = this_sent;
    }
    if (acked) {
        *acked = this_acked;
    }
    if (unacked) {
        *unacked = this_unacked;
    }

    return 1;

}


uint8_t atcmd_get_ops(atcmd_t atcmd, char *buf, uint8_t len) {
    struct atcmd_expect exp;

    exp.expect = "+COPS:";
    exp.recv_buff = buf;
    exp.buff_size = len;

    if (!atcmd_exec_command(atcmd, "AT+COPS?", &exp, 1000)) {
        char *ops = strchr(buf, '\"');
        if (!ops) {
            return 0;
        }
        ++ops;

        while (*ops && *ops != '\"' && len > 1) {
            *buf++ = *ops++;
        }
        *buf = 0;
        return 1;
    }

    return 0;
}

uint8_t atcmd_get_lacci(atcmd_t atcmd, uint16_t lac_ci[2]) {
    char buf[28];
    struct atcmd_expect exp;

    if (!lac_ci) {
        return 0;
    }

    exp.expect = "+CREG:";
    exp.recv_buff = buf;
    exp.buff_size = sizeof(buf);

    if (atcmd_exec_command(atcmd, "AT+CREG?", &exp, 1000)) {
        char *ops = strchr(buf, '\"');
        if (!ops) {
            return 0;
        }

        lac_ci[0] = (unsigned short)strtol(ops + 1, &ops, 16);
        if (!ops || *ops != '\"') {
            return 0;
        }

        ops = strchr(ops + 1, '\"');
        if (!ops) {
            return 0;
        }
        lac_ci[1] = (unsigned short)strtol(ops + 1, &ops, 16);
        if (!ops || *ops != '\"') {
            return 0;
        }
        return 1;
    }

    return 0;
}

