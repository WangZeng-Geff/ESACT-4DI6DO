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

#ifndef __LIBADC_H__
#define __LIBADC_H__

#include "HR8P296.h"
#include "type.h"

/* ADCʱ��Դѡ�� */
typedef enum
{
	ADC_ClkS_Pclk = 0x0 ,  //ʱ�ӣ�PCLK
	ADC_ClkS_32Khz = 0x1 , //ʱ�ӣ�ADCCLK(32KHZ)
}ADC_TYPE_CLKS;

/* ADCʱ��ԴԤ��Ƶ */
typedef enum
{
	ADC_ClkDiv_1 = 0x0 ,   //Ԥ��Ƶ��1:1
	ADC_ClkDiv_2 = 0x1 ,   //Ԥ��Ƶ��1:2
	ADC_ClkDiv_4 = 0x2 ,   //Ԥ��Ƶ��1:4
	ADC_ClkDiv_8 = 0x3 ,   //Ԥ��Ƶ��1:8
	ADC_ClkDiv_16 = 0x4 ,  //Ԥ��Ƶ��1:16
	ADC_ClkDiv_32 = 0x5 ,  //Ԥ��Ƶ��1:32
	ADC_ClkDiv_64 = 0x6 ,  //Ԥ��Ƶ��1:64
	ADC_ClkDiv_256 = 0x7 , //Ԥ��Ƶ��1:256
}ADC_TYPE_CLKDIV;

/* ADC������뷽ʽ */
typedef enum
{
	ADC_Align_Right = 0x1 , //������룺�Ҷ��루�Ͷ��룩
	ADC_Align_Left = 0x0 ,  //������룺����루�߶��룩
}ADC_TYPE_FM;

/* ����ο���ѹѡ�� */
typedef enum
{
	ADC_VrefP_Vcc = 0x0 ,    //����ο���ѹ��3.3V LDO��ѹ
	ADC_VrefP_Ref = 0x1 ,    //����ο���ѹ���ڲ��ο���ѹVref 2.048V ��AVREFP�˿ڸ���Ϊ��ͨIO��
	ADC_VrefP_Ref2 = 0x2 ,    //����ο���ѹ���ڲ��ο���ѹVref 2.048V ��AVREFP�˿����VREF
	ADC_VrefP_Exter = 0x3 ,  //����ο���ѹ���ⲿ�ο���ѹ
}ADC_TYPE_VREFP;

/* ����ģʽѡ�� */
typedef enum
{
	ADC_Samp_Soft = 0x0 ,     //AD����ģʽѡ�����
	ADC_Samp_Hardware = 0x1 , //AD����ģʽѡ��Ӳ��
}ADC_TYPE_SMPS;

/* ADC����ʱ��ѡ�� */
typedef enum
{
	ADC_SampClk_2 = 0x0 ,  //AD����ʱ��ѡ��2��TadClk
	ADC_SampClk_4 = 0x1 ,  //AD����ʱ��ѡ��4��TadClk
	ADC_SampClk_8 = 0x2 ,  //AD����ʱ��ѡ��8��TadClk
	ADC_SampClk_16 = 0x3 , //AD����ʱ��ѡ��16��TadClk
}ADC_TYPE_ST;

/* ת���ٶ�ѡ�� */
typedef enum
{
	ADC_ConvSpeed_Low = 0x0 ,  //ADת���ٶȣ�����
	ADC_ConvSpeed_High = 0x1 , //ADת���ٶȣ�����
}ADC_TYPE_HSEN;

/* ADCͨ��ѡ�� */
typedef enum
{
	ADC_Ch_0 = 0x0 ,       //ͨ��0
	ADC_Ch_1 = 0x1 ,       //ͨ��1
	ADC_Ch_2 = 0x2 ,       //ͨ��2
	ADC_Ch_3 = 0x3 ,       //ͨ��3
	ADC_Ch_4 = 0x4 ,       //ͨ��4
	ADC_Ch_5 = 0x5 ,       //ͨ��5
	ADC_Ch_6 = 0x6 ,       //ͨ��6
	ADC_Ch_7 = 0x7 ,       //ͨ��7
	ADC_Ch_8 = 0x8 ,       //ͨ��8
	ADC_Ch_9 = 0x9 ,       //ͨ��9
	ADC_Ch_10 = 0xa ,       //ͨ��10
}ADC_TYPE_CHS;



/*ADC core����ѡ��λ*/
typedef enum
{
	ADC_LP_86ua = 2,     //ADC Core ����Ϊ86���Ƽ�ʹ��
	ADC_LP_215ua = 0,    //ADC Core ����Ϊ215uA
	ADC_LP_120ua = 1,    //ADC Core ����Ϊ120uA
	ADC_LP_70ua = 3,    //ADC Core ����Ϊ70uA
}ADC_TYPE_BUFLP;

/*ADC ����buff����ѡ��λ*/
typedef enum
{
	ADC_BUFF_0ua = 0,     //ADC ����buff����Ϊ0uA���Ƽ�ʹ��
	ADC_BUFF_34ua = 1,    //ADC ����buff����Ϊ34uA
	ADC_BUFF_95ua = 2,     //ADC ����buff����Ϊ95uA
}ADC_TYPE_BUFEN;

/* ADC��ʼ�����ýṹ�嶨�� */
typedef struct
{
	ADC_TYPE_CLKS  ADC_ClkS;      //ADCCON1:bit3 ADCʱ��Դѡ�� 

	ADC_TYPE_CLKDIV  ADC_ClkDiv;  //ADCCON1:bit2-0 ADCʱ��ԴԤ��Ƶ 

	ADC_TYPE_FM  ADC_Align;       //ADCCON1:bit6 ADC������뷽ʽ 

	ADC_TYPE_VREFP  ADC_VrefP;    //ADCCON1:bit9-8 ADC����ο���ѹѡ��

	ADC_TYPE_SMPS  ADC_SampS;     //ADCCON1:bit12 ADC����ģʽѡ��

	ADC_TYPE_ST  ADC_SampClk;     //ADCCON1:bit15-14 ADC����ʱ��ѡ��

	ADC_TYPE_HSEN  ADC_ConvSpeed; //ADCCON1:bit16 ADCת���ٶ�ѡ��

	ADC_TYPE_CHS  ADC_ChS;        //ADCCHS:bit0-3 ADCģ��ͨ��ѡ��

	ADC_TYPE_BUFLP  ADC_Lp;       //ADC_BUF_LP    ADC core����ѡ��   

	ADC_TYPE_BUFEN ADC_Buf;       //ADC_BUF_EN    ADC����bufferѡ��

}ADC_InitStruType;



/************ADCģ��궨��***********/

/* ADCʹ�ܿ��� */
#define ADC_Enable()  (ADC->CON0.EN = 0x1)
#define ADC_Disable() (ADC->CON0.EN = 0x0)

/* ADC��ʼת�� */
#define ADC_Start() (ADC->CON0.TRG = 0x1)

/* ADC����������� */
#define ADC_SampStart() (ADC->CON1.SMPON = 0x1)
#define ADC_SampStop()  (ADC->CON1.SMPON = 0x0)

/* ����жϱ�־ */
#define ADC_ClearITPendingBit() (ADC->IF.IF = 0x1)

/* ADC�ж�ʹ�ܿ��� */
#define ADC_IT_Enable()  (ADC->IE.IE = 0x1)
#define ADC_IT_Disable() (ADC->IE.IE = 0x0)

/************ADCģ�麯������***********/
void ADC_Init(ADC_InitStruType * ADC_InitStruct);
void ADC_Set_CH(ADC_TYPE_CHS AdcCH);
uint16_t ADC_GetConvValue(void);
FlagStatus ADC_GetConvStatus(void);
FlagStatus ADC_GetFlagStatus(void);
ITStatus ADC_GetITStatus(void);
void ADC_Reset(void);
#endif

/*************************END OF FILE**********************/
