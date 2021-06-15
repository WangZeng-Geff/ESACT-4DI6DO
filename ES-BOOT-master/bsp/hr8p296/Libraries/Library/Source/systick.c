/*********************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd.
 *�ļ���:  systick.c
 *��  ��:  WangMX
 *��  ��:  V1.00
 *��  ��:  2014/11/11
 *��  ��:  SysTickģ�����
 *��  ע:  ������HRSDK-GDB-HR8P296 V1.1
 ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 **********************************************************/
#include "systick.h"

__IO uint32_t TimingDelay;

/***************************************************************
  ������: void SysTickInit(void)
  ��  ����ϵͳ�δ��ʼ����100us
  ����ֵ����
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void SysTickInit(void)
{
	SYSTICK_InitStruType x;
	x.SysTick_ClkSource = SysTick_ClkS_Cpu;
	x.SysTick_Value = 2000;                 //100us
	x.SysTick_ITEnable = ENABLE;            //�ж�ʹ��
	SysTick_Init(&x);
	SysTick_Disable();                        //ʹ��ʱ�ſ���
}

/***************************************************************
  ������: void Delay_100us(__IO uint32_t nTime)
  ��  ����ϵͳ�δ�100us��ʱ
  ����ֵ����ʱ100us����
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void Delay_100us(__IO uint32_t nTime)
{
	TimingDelay = nTime;
	SysTick_Enable();
	while(TimingDelay != 0);
	SysTick_Disable();
}

/***************************************************************
  ������: void TimingDelay_Decrement(void)
  ��  ����ϵͳ�δ��жϵ���
  ����ֵ����
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void TimingDelay_Decrement(void)
{
	if(TimingDelay != 0)
		TimingDelay --;
}
