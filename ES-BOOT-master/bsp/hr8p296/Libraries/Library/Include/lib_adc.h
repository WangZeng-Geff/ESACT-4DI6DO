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

#ifndef __LIBADC_H__
#define __LIBADC_H__

#include "HR8P296.h"
#include "type.h"

/* ADC时钟源选择 */
typedef enum
{
	ADC_ClkS_Pclk = 0x0 ,  //时钟：PCLK
	ADC_ClkS_32Khz = 0x1 , //时钟：ADCCLK(32KHZ)
}ADC_TYPE_CLKS;

/* ADC时钟源预分频 */
typedef enum
{
	ADC_ClkDiv_1 = 0x0 ,   //预分频：1:1
	ADC_ClkDiv_2 = 0x1 ,   //预分频：1:2
	ADC_ClkDiv_4 = 0x2 ,   //预分频：1:4
	ADC_ClkDiv_8 = 0x3 ,   //预分频：1:8
	ADC_ClkDiv_16 = 0x4 ,  //预分频：1:16
	ADC_ClkDiv_32 = 0x5 ,  //预分频：1:32
	ADC_ClkDiv_64 = 0x6 ,  //预分频：1:64
	ADC_ClkDiv_256 = 0x7 , //预分频：1:256
}ADC_TYPE_CLKDIV;

/* ADC结果对齐方式 */
typedef enum
{
	ADC_Align_Right = 0x1 , //结果对齐：右对齐（低对齐）
	ADC_Align_Left = 0x0 ,  //结果对齐：左对齐（高对齐）
}ADC_TYPE_FM;

/* 正向参考电压选择 */
typedef enum
{
	ADC_VrefP_Vcc = 0x0 ,    //正向参考电压：3.3V LDO电压
	ADC_VrefP_Ref = 0x1 ,    //正向参考电压：内部参考电压Vref 2.048V ，AVREFP端口复用为普通IO口
	ADC_VrefP_Ref2 = 0x2 ,    //正向参考电压：内部参考电压Vref 2.048V ，AVREFP端口输出VREF
	ADC_VrefP_Exter = 0x3 ,  //正向参考电压：外部参考电压
}ADC_TYPE_VREFP;

/* 采样模式选择 */
typedef enum
{
	ADC_Samp_Soft = 0x0 ,     //AD采样模式选择：软件
	ADC_Samp_Hardware = 0x1 , //AD采样模式选择：硬件
}ADC_TYPE_SMPS;

/* ADC采样时间选择 */
typedef enum
{
	ADC_SampClk_2 = 0x0 ,  //AD采样时间选择：2个TadClk
	ADC_SampClk_4 = 0x1 ,  //AD采样时间选择：4个TadClk
	ADC_SampClk_8 = 0x2 ,  //AD采样时间选择：8个TadClk
	ADC_SampClk_16 = 0x3 , //AD采样时间选择：16个TadClk
}ADC_TYPE_ST;

/* 转换速度选择 */
typedef enum
{
	ADC_ConvSpeed_Low = 0x0 ,  //AD转换速度：低速
	ADC_ConvSpeed_High = 0x1 , //AD转换速度：高速
}ADC_TYPE_HSEN;

/* ADC通道选择 */
typedef enum
{
	ADC_Ch_0 = 0x0 ,       //通道0
	ADC_Ch_1 = 0x1 ,       //通道1
	ADC_Ch_2 = 0x2 ,       //通道2
	ADC_Ch_3 = 0x3 ,       //通道3
	ADC_Ch_4 = 0x4 ,       //通道4
	ADC_Ch_5 = 0x5 ,       //通道5
	ADC_Ch_6 = 0x6 ,       //通道6
	ADC_Ch_7 = 0x7 ,       //通道7
	ADC_Ch_8 = 0x8 ,       //通道8
	ADC_Ch_9 = 0x9 ,       //通道9
	ADC_Ch_10 = 0xa ,       //通道10
}ADC_TYPE_CHS;



/*ADC core功耗选择位*/
typedef enum
{
	ADC_LP_86ua = 2,     //ADC Core 功耗为86，推荐使用
	ADC_LP_215ua = 0,    //ADC Core 功耗为215uA
	ADC_LP_120ua = 1,    //ADC Core 功耗为120uA
	ADC_LP_70ua = 3,    //ADC Core 功耗为70uA
}ADC_TYPE_BUFLP;

/*ADC 输入buff功耗选择位*/
typedef enum
{
	ADC_BUFF_0ua = 0,     //ADC 输入buff功耗为0uA，推荐使用
	ADC_BUFF_34ua = 1,    //ADC 输入buff功耗为34uA
	ADC_BUFF_95ua = 2,     //ADC 输入buff功耗为95uA
}ADC_TYPE_BUFEN;

/* ADC初始化配置结构体定义 */
typedef struct
{
	ADC_TYPE_CLKS  ADC_ClkS;      //ADCCON1:bit3 ADC时钟源选择 

	ADC_TYPE_CLKDIV  ADC_ClkDiv;  //ADCCON1:bit2-0 ADC时钟源预分频 

	ADC_TYPE_FM  ADC_Align;       //ADCCON1:bit6 ADC结果对齐方式 

	ADC_TYPE_VREFP  ADC_VrefP;    //ADCCON1:bit9-8 ADC正向参考电压选择

	ADC_TYPE_SMPS  ADC_SampS;     //ADCCON1:bit12 ADC采样模式选择

	ADC_TYPE_ST  ADC_SampClk;     //ADCCON1:bit15-14 ADC采样时间选择

	ADC_TYPE_HSEN  ADC_ConvSpeed; //ADCCON1:bit16 ADC转换速度选择

	ADC_TYPE_CHS  ADC_ChS;        //ADCCHS:bit0-3 ADC模拟通道选择

	ADC_TYPE_BUFLP  ADC_Lp;       //ADC_BUF_LP    ADC core功耗选择   

	ADC_TYPE_BUFEN ADC_Buf;       //ADC_BUF_EN    ADC输入buffer选择

}ADC_InitStruType;



/************ADC模块宏定义***********/

/* ADC使能控制 */
#define ADC_Enable()  (ADC->CON0.EN = 0x1)
#define ADC_Disable() (ADC->CON0.EN = 0x0)

/* ADC开始转换 */
#define ADC_Start() (ADC->CON0.TRG = 0x1)

/* ADC采样软件控制 */
#define ADC_SampStart() (ADC->CON1.SMPON = 0x1)
#define ADC_SampStop()  (ADC->CON1.SMPON = 0x0)

/* 清除中断标志 */
#define ADC_ClearITPendingBit() (ADC->IF.IF = 0x1)

/* ADC中断使能控制 */
#define ADC_IT_Enable()  (ADC->IE.IE = 0x1)
#define ADC_IT_Disable() (ADC->IE.IE = 0x0)

/************ADC模块函数声明***********/
void ADC_Init(ADC_InitStruType * ADC_InitStruct);
void ADC_Set_CH(ADC_TYPE_CHS AdcCH);
uint16_t ADC_GetConvValue(void);
FlagStatus ADC_GetConvStatus(void);
FlagStatus ADC_GetFlagStatus(void);
ITStatus ADC_GetITStatus(void);
void ADC_Reset(void);
#endif

/*************************END OF FILE**********************/
