/***************************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
 *�ļ�����  lib_wdt.c
 *��  �ߣ�  JiangYz
 *��  ����  V1.00
 *��  �ڣ�  2016/02/04
 *��  ����  ���Ź�ģ��⺯��
 *��  ע��  ������ HR8P296оƬ
 ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 ***************************************************************/

#include "lib_wdt.h"
#include "system_HR8P296.h"

/*************************************
  ��������WDT_Init
  ��  ����WDT��ʼ��
  ����ֵ����ʼ���ṹ��
  ����ֵ����
 **************************************/
void WDT_Init(WDT_InitStruType *WDT_InitStruct)
{
	uint32_t tmp;

	if (WDT_InitStruct->WDT_Clock != WDT_CLOCK_PCLK)
		tmp = (uint32_t)(32 * WDT_InitStruct->WDT_Tms);
	else
		tmp = SystemCoreClock / 1000 * WDT_InitStruct->WDT_Tms;

	WDT->LOAD.LOAD=tmp;
	WDT->CON.CLKS = WDT_InitStruct->WDT_Clock;
	WDT->CON.IE = WDT_InitStruct->WDT_IE;
	WDT->CON.RSTEN = WDT_InitStruct->WDT_Rst;

	return;
}

/*************************************
  ��������WDT_SetReloadValue
  ��  ��������WDT���صĳ�ֵ
  ����ֵ����ʼֵ��32-bit�޷�������
  ����ֵ����
 **************************************/
void WDT_SetReloadValue(uint32_t Value)
{
	WDT->LOAD.LOAD = Value;
	return;
}

/*************************************
  ��������WDT_GetValue
  ��  ������ȡWDT��ǰֵ
  ����ֵ����
  ����ֵ����ǰֵ
 **************************************/
uint32_t WDT_GetValue(void)
{
	return (uint32_t)WDT->VALUE.VALUE;

}

/*************************************
  ��������WDT_GetFlagStatus
  ��  ������ȡWDT�ж�״̬
  ����ֵ����
  ����ֵ��SET/RESET
 **************************************/
FlagStatus WDT_GetFlagStatus(void)
{
	FlagStatus bitstatus = RESET;

	if (WDT->RIS.WDTIF != RESET)
		bitstatus = SET;

	return  bitstatus;
}
