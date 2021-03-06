/*********************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd.
 *文件名:  systick.c
 *作  者:  WangMX
 *版  本:  V1.00
 *日  期:  2014/11/11
 *描  述:  SysTick模块程序
 *备  注:  适用于HRSDK-GDB-HR8P296 V1.1
 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
 **********************************************************/
#include "systick.h"

__IO uint32_t TimingDelay;

/***************************************************************
  函数名: void SysTickInit(void)
  描  述：系统滴答初始化：100us
  输入值：无
  输出值：无
  返回值：无
 ***************************************************************/
void SysTickInit(void)
{
	SYSTICK_InitStruType x;
	x.SysTick_ClkSource = SysTick_ClkS_Cpu;
	x.SysTick_Value = 2000;                 //100us
	x.SysTick_ITEnable = ENABLE;            //中断使能
	SysTick_Init(&x);
	SysTick_Disable();                        //使用时才开启
}

/***************************************************************
  函数名: void Delay_100us(__IO uint32_t nTime)
  描  述：系统滴答100us定时
  输入值：定时100us个数
  输出值：无
  返回值：无
 ***************************************************************/
void Delay_100us(__IO uint32_t nTime)
{
	TimingDelay = nTime;
	SysTick_Enable();
	while(TimingDelay != 0);
	SysTick_Disable();
}

/***************************************************************
  函数名: void TimingDelay_Decrement(void)
  描  述：系统滴答中断调用
  输入值：无
  输出值：无
  返回值：无
 ***************************************************************/
void TimingDelay_Decrement(void)
{
	if(TimingDelay != 0)
		TimingDelay --;
}
