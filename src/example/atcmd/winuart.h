/*******************************************************************************
  Name: WinUart.h
  Copyright: null
  Author: unprince
  Date: 04-09-12 21:04
  Description: Win32上串口操作头文件 
*******************************************************************************/
#ifndef __WIN_UART_H__
#define __WIN_UART_H__

#include <windows.h>

int uart_Init(U32 port,U32 bps);
void uart_Close(void);
U32 uart_Write(U8 *buffer, U32 size);
U32 uart_Read(U8 *buffer,U32 size);


#endif
