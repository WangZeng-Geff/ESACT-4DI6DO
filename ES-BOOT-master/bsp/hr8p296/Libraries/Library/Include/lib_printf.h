/***************************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd.
 *�ļ�����  lib_printf.h
 *��  �ߣ�  WangBJ
 *��  ����  V1.00
 *��  �ڣ�  2014/10/24
 *��  ����  ���ڴ�ӡ�⺯��ͷ�ļ�
 *��  ע��  ������ HR8P296оƬ
 ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 ***************************************************************/

#ifndef __LIBPRINTF_H__
#define __LIBPRINTF_H__


#include "lib_uart.h"
#include <stdio.h>
#include "type.h"


#ifdef __clang__

ErrorStatus UART_printf(uint8_t *Data,...);

#elif defined __CC_ARM

#define UART_printf  printf

#endif


#endif

/*************************END OF FILE**********************/
