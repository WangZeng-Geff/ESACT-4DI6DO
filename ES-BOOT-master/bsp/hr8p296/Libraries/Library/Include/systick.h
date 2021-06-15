/*********************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd.
 *文件名:  systick.h
 *作  者:  WangMX
 *版  本:  V1.00
 *日  期:  2014/11/11
 *描  述:  SysTick模块程序头文件
 *备  注:  适用于HRSDK-GDB-HR8P296 V1.1
 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
 **********************************************************/
#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "lib_config.h"

void SysTickInit(void);
void Delay_100us(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#endif
