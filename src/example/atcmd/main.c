#include "atcmd_platform.h"

#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include "platform_os.h"
#include <windows.h>

static struct atcmd atcmd;
static uint8_t recv_buff[128];

static HANDLE uart = INVALID_HANDLE_VALUE;

static void uart_init(void) {
    DCB dcb;
    uint8_t buffer[32];
    COMMTIMEOUTS timeout;
    int port = 12;

    sprintf(buffer, "\\\\.\\COM%d", port & 0xF);
    uart = CreateFile(buffer, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
    if (uart == INVALID_HANDLE_VALUE)
        return;
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
    if (uart == INVALID_HANDLE_VALUE) return;
    WriteFile(uart, buff, size, (LPDWORD)&dwSize, NULL);
}

int uart_recv(uint8_t *buffer, uint32_t size) {
    uint32_t dwRead;
    COMSTAT state;
    if (uart == INVALID_HANDLE_VALUE) return 0;
    ClearCommError(uart, (LPDWORD)&dwRead, &state);
    if (state.cbInQue > size) {
        ReadFile(uart, buffer, size, (LPDWORD)&dwRead, NULL);
    } else {
        ReadFile(uart, buffer, state.cbInQue, (LPDWORD)&dwRead, NULL);
    }
    return dwRead;
}

static void *uart_recv_thread(void *p) {
    static uint8_t buff[200];
    static uint8_t index = 0;
    uint8_t b;
    while (1) {
        while (1 != uart_recv(&b, 1));
        atcmd.serial_recv(&atcmd, b);
    }
}

int main(int argc, char **argv) {
    pthread_t tid;
    static uint8_t buff[50];
    int ret;

    struct atcmd_init_param param = {
        uart_send,
        recv_buff,
        sizeof(recv_buff),
    };

    uart_init();

    atcmd_init(&atcmd, &param);

    if ((ret = pthread_create(&tid, NULL, uart_recv_thread, NULL)) != 0) {
        fprintf(stderr, "pthread create: %s\n", strerror(ret));
        exit(-1);
    }

    atcmd_retry_until_reply(&atcmd, "ATE0", "OK", 500, 5);
    while (1) {
        os_sleep(1000);
        atcmd_get_imei(&atcmd, buff, sizeof(buff));
        printf("imei: %s\n", buff);
    }
}

