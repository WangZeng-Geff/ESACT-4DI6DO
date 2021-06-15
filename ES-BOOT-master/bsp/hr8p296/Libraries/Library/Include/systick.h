/*********************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd.
 *�ļ���:  systick.h
 *��  ��:  WangMX
 *��  ��:  V1.00
 *��  ��:  2014/11/11
 *��  ��:  SysTickģ�����ͷ�ļ�
 *��  ע:  ������HRSDK-GDB-HR8P296 V1.1
 ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 **********************************************************/
#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "lib_config.h"

void SysTickInit(void);
void Delay_100us(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#endif
