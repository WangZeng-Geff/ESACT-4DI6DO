/***************************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
 *�ļ�����  lib_timer.c
 *��  �ߣ�  ZouX
 *��  ����  V1.00
 *��  �ڣ�  2016/01/31
 *��  ����  ADCģ��⺯��
 *��  ע��  ������ HR8P296оƬ
 ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 ***************************************************************/

#include "lib_adc.h"

/***************************************************************
  ��������ADC_Init
  ��  ������ʼ��ADCģ��
  ����ֵ����ʼ�����ýṹ���ַ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void ADC_Init(ADC_InitStruType * ADC_InitStruct)
{

	ADC->CON1.CLKS = ADC_InitStruct->ADC_ClkS;     //ADCCON1:bit3 ADCʱ��Դѡ��

	ADC->CON1.CLKDIV = ADC_InitStruct->ADC_ClkDiv; //ADCCON1:bit2-0 ADCʱ��ԴԤ��Ƶ

	ADC->CON1.FM = ADC_InitStruct->ADC_Align;      //ADCCON1:bit6 ADC������뷽ʽ

	ADC->CON1.VREFP = ADC_InitStruct->ADC_VrefP;   //ADCCON1:bit9-8 ADC����ο���ѹѡ��

	ADC->CON1.SMPS = ADC_InitStruct->ADC_SampS;    //ADCCON1:bit12 ADC����ģʽѡ��

	ADC->CON1.ST = ADC_InitStruct->ADC_SampClk;    //ADCCON1:bit15-14 ADC����ʱ��ѡ��

	ADC->CON1.HSEN = ADC_InitStruct->ADC_ConvSpeed;//ADCCON1:bit16 ADCת���ٶ�ѡ��

	ADC->CHS.CHS = ADC_InitStruct->ADC_ChS;        //ADCCHS:bit0-3 ADCģ��ͨ��ѡ��

	ADC->BUF.LP =  ADC_InitStruct->ADC_Lp;         //ADC_BUF_LP    ADC core����ѡ�� 

	ADC->BUF.BUF_EN = ADC_InitStruct->ADC_Buf;     //ADC_BUF_EN    ADC����bufferѡ��
}

/***************************************************************
  ��������ADC_Set_CH
  ��  ����ѡ��ADCģ��ͨ��
  ����ֵ��ͨ��
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void ADC_Set_CH(ADC_TYPE_CHS AdcCH)
{

	ADC->CHS.CHS = AdcCH;  //ADCCHS:bit0-3 ADCģ��ͨ��ѡ��
}

/***************************************************************
  ��������ADC_GetConvValue
  ��  ������ȡADCת�����
  ����ֵ����
  ���ֵ����
  ����ֵ��ת��ֵ
 ***************************************************************/
uint16_t ADC_GetConvValue(void)
{   
	return  ((uint16_t)ADC->DR.DR);
}

/***************************************************************
  ��������ADC_GetConvStatus
  ��  ������ȡADCת��״̬
  ����ֵ����
  ���ֵ����
  ����ֵ��RESET(���)/SET(����ת��)
 ***************************************************************/
FlagStatus ADC_GetConvStatus(void)
{
	FlagStatus bitstatus = RESET;
	/* ���ת��״̬�Ĵ��� */
	if ((ADC->CON0.TRG != (uint32_t)RESET))
	{
		bitstatus = SET; //����ת��
	}
	else
	{
		bitstatus = RESET; //ת�����
	}
	return  bitstatus;
}

/***************************************************************
  ��������ADC_GetFlagStatus
  ��  ������ȡADC��־λ״̬
  ����ֵ����
  ���ֵ����
  ����ֵ��SET/RESET
 ***************************************************************/
FlagStatus ADC_GetFlagStatus(void)
{
	FlagStatus bitstatus = RESET;
	/* ����жϱ�־λ */
	if ((ADC->IF.IF != (uint32_t)RESET))
	{
		bitstatus = SET; //ת�����
	}
	else
	{
		bitstatus = RESET; //���ж�
	}
	return  bitstatus;
}

/***************************************************************
  ��������ADC_GetITStatus
  ��  ������ȡADC�ж�״̬��δʹ����Ӧ�ж�ʱ���᷵��SET
  ����ֵ����
  ���ֵ����
  ����ֵ��SET���жϣ�/RESET�����жϣ�
 ***************************************************************/
ITStatus ADC_GetITStatus(void)
{
	ITStatus bitstatus = RESET;
	/* ����жϱ�־λ */
	if ((ADC->IF.IF != (uint32_t)RESET)&&(ADC->IE.IE != (uint32_t)RESET))
	{
		bitstatus = SET; //ת����ɣ����ж�
	}
	else
	{
		bitstatus = RESET; //���ж�
	}
	return  bitstatus;
}

/***************************************************************
  ��������ADC_Reset
  ��  ����ADCģ������:�Ĵ����ָ��ϵ��ʼֵ
  ����ֵ����
  ���ֵ����
  ����ֵ����
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
