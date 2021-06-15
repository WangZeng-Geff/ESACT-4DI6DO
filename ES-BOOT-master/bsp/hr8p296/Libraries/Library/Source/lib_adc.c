/***************************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
 *文件名：  lib_timer.c
 *作  者：  ZouX
 *版  本：  V1.00
 *日  期：  2016/01/31
 *描  述：  ADC模块库函数
 *备  注：  适用于 HR8P296芯片
 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
 ***************************************************************/

#include "lib_adc.h"

/***************************************************************
  函数名：ADC_Init
  描  述：初始化ADC模块
  输入值：初始化配置结构体地址
  输出值：无
  返回值：无
 ***************************************************************/
void ADC_Init(ADC_InitStruType * ADC_InitStruct)
{

	ADC->CON1.CLKS = ADC_InitStruct->ADC_ClkS;     //ADCCON1:bit3 ADC时钟源选择

	ADC->CON1.CLKDIV = ADC_InitStruct->ADC_ClkDiv; //ADCCON1:bit2-0 ADC时钟源预分频

	ADC->CON1.FM = ADC_InitStruct->ADC_Align;      //ADCCON1:bit6 ADC结果对齐方式

	ADC->CON1.VREFP = ADC_InitStruct->ADC_VrefP;   //ADCCON1:bit9-8 ADC正向参考电压选择

	ADC->CON1.SMPS = ADC_InitStruct->ADC_SampS;    //ADCCON1:bit12 ADC采样模式选择

	ADC->CON1.ST = ADC_InitStruct->ADC_SampClk;    //ADCCON1:bit15-14 ADC采样时间选择

	ADC->CON1.HSEN = ADC_InitStruct->ADC_ConvSpeed;//ADCCON1:bit16 ADC转换速度选择

	ADC->CHS.CHS = ADC_InitStruct->ADC_ChS;        //ADCCHS:bit0-3 ADC模拟通道选择

	ADC->BUF.LP =  ADC_InitStruct->ADC_Lp;         //ADC_BUF_LP    ADC core功耗选择 

	ADC->BUF.BUF_EN = ADC_InitStruct->ADC_Buf;     //ADC_BUF_EN    ADC输入buffer选择
}

/***************************************************************
  函数名：ADC_Set_CH
  描  述：选择ADC模拟通道
  输入值：通道
  输出值：无
  返回值：无
 ***************************************************************/
void ADC_Set_CH(ADC_TYPE_CHS AdcCH)
{

	ADC->CHS.CHS = AdcCH;  //ADCCHS:bit0-3 ADC模拟通道选择
}

/***************************************************************
  函数名：ADC_GetConvValue
  描  述：获取ADC转换结果
  输入值：无
  输出值：无
  返回值：转换值
 ***************************************************************/
uint16_t ADC_GetConvValue(void)
{   
	return  ((uint16_t)ADC->DR.DR);
}

/***************************************************************
  函数名：ADC_GetConvStatus
  描  述：获取ADC转换状态
  输入值：无
  输出值：无
  返回值：RESET(完成)/SET(正在转换)
 ***************************************************************/
FlagStatus ADC_GetConvStatus(void)
{
	FlagStatus bitstatus = RESET;
	/* 检测转换状态寄存器 */
	if ((ADC->CON0.TRG != (uint32_t)RESET))
	{
		bitstatus = SET; //正在转换
	}
	else
	{
		bitstatus = RESET; //转换完成
	}
	return  bitstatus;
}

/***************************************************************
  函数名：ADC_GetFlagStatus
  描  述：读取ADC标志位状态
  输入值：无
  输出值：无
  返回值：SET/RESET
 ***************************************************************/
FlagStatus ADC_GetFlagStatus(void)
{
	FlagStatus bitstatus = RESET;
	/* 检查中断标志位 */
	if ((ADC->IF.IF != (uint32_t)RESET))
	{
		bitstatus = SET; //转换完成
	}
	else
	{
		bitstatus = RESET; //无中断
	}
	return  bitstatus;
}

/***************************************************************
  函数名：ADC_GetITStatus
  描  述：读取ADC中断状态，未使能相应中断时不会返回SET
  输入值：无
  输出值：无
  返回值：SET（中断）/RESET（无中断）
 ***************************************************************/
ITStatus ADC_GetITStatus(void)
{
	ITStatus bitstatus = RESET;
	/* 检查中断标志位 */
	if ((ADC->IF.IF != (uint32_t)RESET)&&(ADC->IE.IE != (uint32_t)RESET))
	{
		bitstatus = SET; //转换完成，进中断
	}
	else
	{
		bitstatus = RESET; //无中断
	}
	return  bitstatus;
}

/***************************************************************
  函数名：ADC_Reset
  描  述：ADC模块重置:寄存器恢复上电初始值
  输入值：无
  输出值：无
  返回值：无
 ***************************************************************/
void ADC_Reset(void)
{

	ADC->CON0.Word = 0x00000000;
	ADC->CON1.Word = 0x00009000;
	ADC->CHS.Word = 0x00400000;
	ADC->IE.Word = 0x00000000;
	ADC->IF.IF = 0x1;
	ADC->BUF.Word = 0x00000009;
}

/*************************END OF FILE**********************/
