/*******************************************************************************
  Name: WinUart.c
  Copyright: null
  Author: unprince
  Date: 04-09-12 21:03
  Description: Win32�ϵĴ��ڲ���ģ��
*******************************************************************************/
#include <windows.h>
#include "WinUart.h"
#include "debug.h"

#ifdef _WINDOWS_H
#include "config.h"

static HANDLE uart = INVALID_HANDLE_VALUE;

int uart_Init(U32 port, U32 bps) {
    DCB dcb;
    U8 buffer[32];
    COMMTIMEOUTS timeout;

    sprintf(buffer, "\\\\.\\COM%d", port & 0xF);
    uart = CreateFile(buffer, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
    if (uart == INVALID_HANDLE_VALUE)
        return 0;
    SetupComm(uart, 8192, 8192);
    GetCommState(uart, &dcb);
    dcb.BaudRate = bps;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    SetCommState(uart, &dcb);

    timeout.ReadIntervalTimeout = 1000;			//�������ʱ 
	timeout.ReadTotalTimeoutMultiplier = 500;	//��ʱ��ϵ��
	timeout.ReadTotalTimeoutConstant = 5000;	//�������������ʱ�ܳ�ʱ  
	timeout.WriteTotalTimeoutMultiplier = 0;	//дʱ��ϵ�� 
	timeout.WriteTotalTimeoutConstant = 0;		//дʱ�䳣�� 
	SetCommTimeouts(uart, &timeout);

    return 1;
}

void uart_Close(void) {
    if (uart != INVALID_HANDLE_VALUE)
        CloseHandle(uart);
}

U32 uart_Write(U8 *buffer, U32 size) {
    U32 dwSize;
    if (uart == INVALID_HANDLE_VALUE) return 0;
    WriteFile(uart, buffer, size, &dwSize, NULL);
    return dwSize;
}

U32 uart_Read(U8 *buffer, U32 size) {
    COMSTAT state;
    if (uart == INVALID_HANDLE_VALUE) return 0;
    ClearCommError(uart, &dwRead, &state);
    if (state.cbInQue > size)
        ReadFile(uart, buffer, size, &dwRead, NULL);
    else ReadFile(uart, buffer, state.cbInQue, &dwRead, NULL);

}

#endif

