/******************************************************************************************
* Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
* 文件名:  system_HR8P506.c
* 作  者:  ESM Application Team in
* 版  本:  V1.00
* 日  期:  2015/10/27
* 描  述:  HR8P506系列芯片初始化文件
* 备  注:  适用于HR8P506系列芯片
*          本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
*******************************************************************************************/
#include "HR8P506.h"
#include "lib_scu.h"

unsigned long SystemCoreClock = 48000000;

void SystemInit(void)
{	
	PLLClock_Config(Enable, SCU_PLL_OUT8M, SCU_PLL_48M, Enable);
	DeviceClockAllEnable();
}
