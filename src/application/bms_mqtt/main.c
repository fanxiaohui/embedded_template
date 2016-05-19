#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <windows.h>

#include "dtu_m35_platform.h"
#include "dtu_m35_mqtt_impl.h"
#include "MQTTClient.h"
#include "MQTTConnect.h"
#include "platform_os.h"
#include "logger.h"

#define LOG_LEVEL LOG_LEVEL_TRACE

static uint8_t recv_buff[128];

static HANDLE uart = INVALID_HANDLE_VALUE;

static void uart_init(void) {
    DCB dcb;
    uint8_t buffer[32];
    COMMTIMEOUTS timeout;
    int port = 12;

    sprintf(buffer, "\\\\.\\COM%d", port & 0xF);
    uart = CreateFile(buffer, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
    if (uart == INVALID_HANDLE_VALUE) {
        return;
    }
    SetupComm(uart, 8192, 8192);
    GetCommState(uart, &dcb);
    dcb.BaudRate = 9600;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    SetCommState(uart, &dcb);

    timeout.ReadIntervalTimeout = 1000;         //读间隔超时
    timeout.ReadTotalTimeoutMultiplier = 500;   //读时间系数
    timeout.ReadTotalTimeoutConstant = 5000;    //不满足读缓冲区时总超时
    timeout.WriteTotalTimeoutMultiplier = 0;    //写时间系数
    timeout.WriteTotalTimeoutConstant = 0;      //写时间常量
    SetCommTimeouts(uart, &timeout);

    return;
}

static void uart_send(const uint8_t *buff, uint16_t size) {
    uint32_t dwSize;
    if (uart == INVALID_HANDLE_VALUE) {
        return;
    }
    WriteFile(uart, buff, size, (LPDWORD)&dwSize, NULL);
}

int uart_recv(uint8_t *buffer, uint32_t size) {
    uint32_t dwRead;
    COMSTAT state;
    if (uart == INVALID_HANDLE_VALUE) {
        return 0;
    }
    ClearCommError(uart, (LPDWORD)&dwRead, &state);
    if (state.cbInQue > size) {
        ReadFile(uart, buffer, size, (LPDWORD)&dwRead, NULL);
    } else {
        ReadFile(uart, buffer, state.cbInQue, (LPDWORD)&dwRead, NULL);
    }
    return dwRead;
}


static void *uart_recv_thread(void *p);

static void m35_platform_init(void) {
    uart_init();
    pthread_t tid;
    int ret;
    if ((ret = pthread_create(&tid, NULL, uart_recv_thread, NULL)) != 0) {
        fprintf(stderr, "pthread create: %s\n", strerror(ret));
        exit(-1);
    }
}

static void m35_platform_power_enable(uint8_t is_enable) {
}

static void m35_platform_power_key(uint8_t is_assert) {
}

static void m35_platform_serial_send(const uint8_t *dat, uint16_t len) {
    uint32_t dwSize;
    if (uart == INVALID_HANDLE_VALUE) {
        return;
    }
    WriteFile(uart, dat, len, (LPDWORD)&dwSize, NULL);
}

static uint8_t m35_is_poweron(void) {
    return 1;
}

const struct dtu_m35_platform dtu_m35_platform = {
    .init = m35_platform_init,
    .is_poweron = m35_is_poweron,
    .power_enable = m35_platform_power_enable,
    .power_key = m35_platform_power_key,
    .serial_send = m35_platform_serial_send,
};

static struct dtu_m35 m35;
static const struct mqtt_network mqtt_m35_network = M35_MQTT_NETOWK(&m35);
static struct mqtt_client client;
static uint8_t write_buffer[200];
static uint8_t read_buffer[200];

static void *uart_recv_thread(void *p) {
    static uint8_t buff[200];
    static uint8_t index = 0;
    uint8_t b;
    while (1) {
        while (1 != uart_recv(&b, 1));
        OS_CRITICAL(
            //dtum35_recv_byte(&m35, b);
            atcmd_recv_serial_byte(&m35.atcmd, b);
        );
    }
}

char payload[100];

static MQTTMessage msg = {
    QOS1,
    1,
    0,
    0,
};

void message_handler_test_d(MessageData *msg) {
    int i;
    MQTTMessage * m = msg->message;
    uint8_t *payload = m->payload;
    LOG(LOG_LEVEL_INFO, "%s", __func__);

    printf("LEN: %d\n", m->payloadlen);
    printf("DAT:");
    for (i=0; i<m->payloadlen; ++i) {
        printf(" %02X", *payload++);
    }
    printf("\n");
}

void mqtt_run(mqtt_client_t client) {
    static int i = 0;
    client->ipstack->connect(client->ipstack->priv, "104.224.149.201", 1883);
    os_sleep(3000);
    MQTTConnect(client, NULL);
    MQTTSubscribe(client, "test/d", QOS1, message_handler_test_d);
    do {
        sprintf(payload, "hello, mqtt %d", i++);
        msg.payload = payload;
        msg.payloadlen = strlen(payload);
        MQTTPublish(client, "test/u", &msg);
        MQTTYield(client, 20000);
    } while (SUCCESS ==  MQTTYield(client, 20000));
    client->ipstack->disconnect(client->ipstack->priv);
}

int main(int argc, char **argv) {
    static uint8_t buff[50];
    pthread_t tid;
    os_platform_init();
    dtum35_init(&m35, &dtu_m35_platform);
    pthread_create(&tid, NULL, (void *(*)(void *))dtum35_run, &m35);
    MQTTClientInit(&client,
                   &mqtt_m35_network,
                   20000,
                   write_buffer, sizeof(write_buffer),
                   read_buffer, sizeof(read_buffer));
    while (1) {
        mqtt_run(&client);
        os_sleep(4000);
    }
}

