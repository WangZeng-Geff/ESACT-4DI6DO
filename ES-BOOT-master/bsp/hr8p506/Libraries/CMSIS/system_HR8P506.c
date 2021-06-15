/******************************************************************************************
* Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
* �ļ���:  system_HR8P506.c
* ��  ��:  ESM Application Team in
* ��  ��:  V1.00
* ��  ��:  2015/10/27
* ��  ��:  HR8P506ϵ��оƬ��ʼ���ļ�
* ��  ע:  ������HR8P506ϵ��оƬ
*          ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
*******************************************************************************************/
#include "HR8P506.h"
#include "lib_scu.h"

unsigned long SystemCoreClock = 48000000;

void SystemInit(void)
{	
	PLLClock_Config(Enable, SCU_PLL_OUT8M, SCU_PLL_48M, Enable);
	DeviceClockAllEnable();
}
