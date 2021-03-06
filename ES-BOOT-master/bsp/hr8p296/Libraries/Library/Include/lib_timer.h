/******************************************************************************************
 * Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd.
 * 文件名:  lib_timer.h
 * 作  者:  JiangYz
 * 版  本:  V1.00
 * 日  期:  2016/01/27
 * 描  述:  16位定时器/计数器、32位定时器/计数器、实时时钟库函数头文件
 * 备  注:  适用于HR8P296系列芯片
 *          本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
 *******************************************************************************************/
#ifndef __LIBTIMER_H__
#define __LIBTIMER_H__

#include "HR8P296.h"
#include "type.h"
//*************************************************************

//*************************************************************


/* 时钟源选择 */
typedef enum
{

	TIM_ClkS_PCLK = 0x0 , //时钟源选择:内部PCLK
	TIM_ClkS_CK0 = 0x1 ,  //时钟源选择:外部CK0时钟输入
	TIM_ClkS_CK1 = 0x2 ,  //时钟源选择:外部CK1时钟输入
}TIM_TYPE_CLKS;

/* 外部时钟计数边沿选择 */
typedef enum
{
	TIM_EDGE_Rise = 0x0 , //外部时钟计数边沿选择:上升沿
	TIM_EDGE_Fall = 0x1 , //外部时钟计数边沿选择:下降沿
	TIM_EDGE_All = 0x2 ,  //外部时钟计数边沿选择:所有
}TIM_TYPE_EDGE;


/* 工作模式选择 */
typedef enum
{
	TIM_Mode_TC0 = 0x0 , //工作模式:定时、计数模式
	TIM_Mode_TC1 = 0x1 , //工作模式:定时、计数模式
	TIM_Mode_CAP = 0x2 , //工作模式:捕捉模式
	TIM_Mode_PWM = 0x3 , //工作模式:调制模式
}TIM_TYPE_MODE;

/* TIM初始化配置结构体定义 */
typedef struct
{
	TIM_TYPE_CLKS  TIM_ClkS; //时钟源选择

	TYPE_FUNCEN  TIM_SYNC;   //外部时钟同步

	TIM_TYPE_EDGE  TIM_EDGE; //外部时钟计数边沿选择

	TIM_TYPE_MODE  TIM_Mode; //工作模式选择

}TIM_BaseInitStruType;

/* 匹配寄存器值匹配后的工作模式 */
typedef enum
{
	TIM_Go_No = 0x0 ,    //匹配寄存器值匹配后的工作模式:继续计数，不产生中断
	TIM_Hold_Int = 0x1 , //匹配寄存器值匹配后的工作模式:保持计数，产生中断
	TIM_Clr_Int = 0x2 ,  //匹配寄存器值匹配后的工作模式:清零并重新计数，产生中断
	TIM_Go_Int = 0x3 ,   //匹配寄存器值匹配后的工作模式:继续计数，产生中断
}TIM_TYPE_MATCON;

/* 匹配寄存器值匹配后输出端口的工作模式 */
typedef enum
{
	TIM_Out_Hold = 0x0 ,   //匹配寄存器值匹配后输出端口的工作模式：保持
	TIM_Out_Low = 0x1 ,    //匹配寄存器值匹配后输出端口的工作模式：清0
	TIM_Out_High = 0x2 ,   //匹配寄存器值匹配后输出端口的工作模式：置1
	TIM_Out_Switch = 0x3 , //匹配寄存器值匹配后输出端口的工作模式：取反
}TIM_TYPE_MATOUT;

/* 捕捉次数控制 */
typedef enum
{
	TIM_CapTime_1 = 0x0 , //捕捉次数控制:1
	TIM_CapTime_2 = 0x1 , //捕捉次数控制:2
	TIM_CapTime_3 = 0x2 , //捕捉次数控制:3
	TIM_CapTime_4 = 0x3 , //捕捉次数控制:4
	TIM_CapTime_5 = 0x4 , //捕捉次数控制:5
	TIM_CapTime_6 = 0x5 , //捕捉次数控制:6
	TIM_CapTime_7 = 0x6 , //捕捉次数控制:7
	TIM_CapTime_8 = 0x7 , //捕捉次数控制:8
	TIM_CapTime_9 = 0x8 , //捕捉次数控制:9
	TIM_CapTime_10 = 0x9 , //捕捉次数控制:10
	TIM_CapTime_11 = 0xA , //捕捉次数控制:11
	TIM_CapTime_12 = 0xB , //捕捉次数控制:12
	TIM_CapTime_13 = 0xC , //捕捉次数控制:13
	TIM_CapTime_14 = 0xD , //捕捉次数控制:14
	TIM_CapTime_15 = 0xE , //捕捉次数控制:15
	TIM_CapTime_16 = 0xF , //捕捉次数控制:16
}TIM_TYPE_CAPT;

/* 捕捉功能初始化结构体定义 */
typedef struct
{
	TYPE_FUNCEN  TIM_CapRise; //上升沿捕捉使能

	TYPE_FUNCEN  TIM_CapFall; //下降沿捕捉使能

	TYPE_FUNCEN  TIM_CapIS0;  //输入端口0使能

	TYPE_FUNCEN  TIM_CapIS1;  //输入端口1使能

	TIM_TYPE_CAPT  TIM_CapTime; //捕捉次数控制

}TIM_CapInitStruType;

/* 中断配置 */
typedef enum
{
	TIM_IT_MAT0 = 0x01 ,
	TIM_IT_MAT1 = 0x02 ,
	TIM_IT_MAT2 = 0x04 ,
	TIM_IT_MAT3 = 0x08 ,
	TIM_IT_N =    0x10 ,
	TIM_IT_CAP0 = 0x20 ,
	TIM_IT_CAP1 = 0x40 ,
}TIM_TYPE_IT;

/* 匹配寄存器 */
typedef enum
{
	TIM_MAT0 = 0x00 ,
	TIM_MAT1 = 0x01 ,
	TIM_MAT2 = 0x02 ,
	TIM_MAT3 = 0x03 ,
}TIM_TYPE_MATX;

/* PWM输出 */
typedef enum
{
	T16N0_OUT = 0x00 ,
	T16N1_OUT = 0x01 ,
	T16N2_OUT = 0x02 ,
	T16N3_OUT = 0x03 ,
}TIM_TYPE_PWMOC;
/*PWM输出类型*/
typedef enum
{
	NO_PWM  =0x0,   //非调制输出
	IR38K_PWM =0x1, //固定38K调制输出
	PWM_OUT0=0x2,   //T16NxOUT0 PWM调制输出
	PWM_OUT1=0x3,   //T16NxOUT1 PWM调制输出
}PWM_TYPE_OUT;
/*PWM调制电平*/
typedef enum
{
	LOW_LEVEL = 0x0,//  低电平
	HIGH_LEVEL  = 0x1,//   高电平
}TYPE_LEVEL;

/*************TIM模块宏定义************/

/* TIM模块使能控制 */
#define T16N0_Enable() (T16N0->CON0.EN = 1)
#define T16N1_Enable() (T16N1->CON0.EN = 1)
#define T16N2_Enable() (T16N2->CON0.EN = 1)
#define T16N3_Enable() (T16N3->CON0.EN = 1)
#define T32N0_Enable() (T32N0->CON0.EN = 1)
#define T32N1_Enable() (T32N1->CON0.EN = 1)
#define T32N2_Enable() (T32N2->CON0.EN = 1)
#define T16N0_Disable() (T16N0->CON0.EN = 0)
#define T16N1_Disable() (T16N1->CON0.EN = 0)
#define T16N2_Disable() (T16N2->CON0.EN = 0)
#define T16N3_Disable() (T16N3->CON0.EN = 0)
#define T32N0_Disable() (T32N0->CON0.EN = 0)
#define T32N1_Disable() (T32N1->CON0.EN = 0)
#define T32N2_Disable() (T32N2->CON0.EN = 0)

/* 异步写使能控制 */
#define T16N0_ASYNCWR_Enable() (T16N0->CON0.ASYWEN = 1)
#define T16N1_ASYNCWR_Enable() (T16N1->CON0.ASYWEN = 1)
#define T16N2_ASYNCWR_Enable() (T16N2->CON0.ASYWEN = 1)
#define T16N3_ASYNCWR_Enable() (T16N3->CON0.ASYWEN = 1)
#define T32N0_ASYNCWR_Enable() (T32N0->CON0.ASYNCWREN = 1)
#define T32N1_ASYNCWR_Enable() (T32N1->CON0.ASYNCWREN = 1)
#define T32N2_ASYNCWR_Enable() (T32N2->CON0.ASYNCWREN = 1)
#define T16N0_ASYNCWR_Disable() (T16N0->CON0.ASYWEN= 0)
#define T16N1_ASYNCWR_Disable() (T16N1->CON0.ASYWEN = 0)
#define T16N2_ASYNCWR_Disable() (T16N2->CON0.ASYWEN = 0)
#define T16N3_ASYNCWR_Disable() (T16N3->CON0.ASYWEN = 0)
#define T32N0_ASYNCWR_Disable() (T32N0->CON0.ASYNCWREN = 0)
#define T32N1_ASYNCWR_Disable() (T32N1->CON0.ASYNCWREN = 0)
#define T32N2_ASYNCWR_Disable() (T32N2->CON0.ASYNCWREN = 0)

/* PWM输出使能控制 */
#define T16N0_PwmOut0_Enable() (T16N0->CON1.MOE0 = 1)
#define T16N1_PwmOut0_Enable() (T16N1->CON1.MOE0 = 1)
#define T16N2_PwmOut0_Enable() (T16N2->CON1.MOE0 = 1)
#define T16N3_PwmOut0_Enable() (T16N3->CON1.MOE0 = 1)
#define T32N0_PwmOut0_Enable() (T32N0->CON1.MOE0 = 1)
#define T32N1_PwmOut0_Enable() (T32N1->CON1.MOE0 = 1)
#define T32N2_PwmOut0_Enable() (T32N2->CON1.MOE0 = 1)
#define T16N0_PwmOut1_Enable() (T16N0->CON1.MOE1 = 1)
#define T16N1_PwmOut1_Enable() (T16N1->CON1.MOE1 = 1)
#define T16N2_PwmOut1_Enable() (T16N2->CON1.MOE1 = 1)
#define T16N3_PwmOut1_Enable() (T16N3->CON1.MOE1 = 1)
#define T32N0_PwmOut1_Enable() (T32N0->CON1.MOE1 = 1)
#define T32N1_PwmOut1_Enable() (T32N1->CON1.MOE1 = 1)
#define T32N2_PwmOut1_Enable() (T32N2->CON1.MOE1 = 1)
#define T16N0_PwmOut0_Disable() (T16N0->CON1.MOE0 = 0)
#define T16N1_PwmOut0_Disable() (T16N1->CON1.MOE0 = 0)
#define T16N2_PwmOut0_Disable() (T16N2->CON1.MOE0 = 0)
#define T16N3_PwmOut0_Disable() (T16N3->CON1.MOE0 = 0)
#define T32N0_PwmOut0_Disable() (T32N0->CON1.MOE0 = 0)
#define T32N1_PwmOut0_Disable() (T32N1->CON1.MOE0 = 0)
#define T32N2_PwmOut0_Disable() (T32N2->CON1.MOE0 = 0)
#define T16N0_PwmOut1_Disable() (T16N0->CON1.MOE1 = 0)
#define T16N1_PwmOut1_Disable() (T16N1->CON1.MOE1 = 0)
#define T16N2_PwmOut1_Disable() (T16N2->CON1.MOE1 = 0)
#define T16N3_PwmOut1_Disable() (T16N3->CON1.MOE1 = 0)
#define T32N0_PwmOut1_Disable() (T32N0->CON1.MOE1 = 0)
#define T32N1_PwmOut1_Disable() (T32N1->CON1.MOE1 = 0)
#define T32N2_PwmOut1_Disable() (T32N2->CON1.MOE1 = 0)

/************T16模块函数声明***********/
void T16Nx_BaseInit(T16N_TypeDef* T16Nx,TIM_BaseInitStruType* TIM_BaseInitStruct);
void T16Nx_CapInit(T16N_TypeDef* T16Nx,TIM_CapInitStruType* TIM_CapInitStruct);
void T16Nx_MAT0ITConfig(T16N_TypeDef* T16Nx,TIM_TYPE_MATCON Type);
void T16Nx_MAT1ITConfig(T16N_TypeDef* T16Nx,TIM_TYPE_MATCON Type);
void T16Nx_MAT2ITConfig(T16N_TypeDef* T16Nx,TIM_TYPE_MATCON Type);
void T16Nx_MAT3ITConfig(T16N_TypeDef* T16Nx,TIM_TYPE_MATCON Type);
void T16Nx_MAT0Out0Config(T16N_TypeDef* T16Nx,TIM_TYPE_MATOUT Type);
void T16Nx_MAT1Out0Config(T16N_TypeDef* T16Nx,TIM_TYPE_MATOUT Type);
void T16Nx_MAT2Out1Config(T16N_TypeDef* T16Nx,TIM_TYPE_MATOUT Type);
void T16Nx_MAT3Out1Config(T16N_TypeDef* T16Nx,TIM_TYPE_MATOUT Type);
void T16Nx_ITConfig(T16N_TypeDef* T16Nx,TIM_TYPE_IT Type,TYPE_FUNCEN NewState);
void T16Nx_PWMOutConfig(TIM_TYPE_PWMOC Pwms,PWM_TYPE_OUT PwmType,TYPE_LEVEL PwmLevel);
void T16Nx_SetCNT(T16N_TypeDef* T16Nx,uint16_t Value);
void T16Nx_SetPRECNT(T16N_TypeDef* T16Nx,uint8_t Value);
void T16Nx_SetPREMAT(T16N_TypeDef* T16Nx,uint8_t Value);
void T16Nx_SetMAT0(T16N_TypeDef* T16Nx,uint16_t Value);
void T16Nx_SetMAT1(T16N_TypeDef* T16Nx,uint16_t Value);
void T16Nx_SetMAT2(T16N_TypeDef* T16Nx,uint16_t Value);
void T16Nx_SetMAT3(T16N_TypeDef* T16Nx,uint16_t Value);
uint16_t T16Nx_GetMAT0(T16N_TypeDef* T16Nx);
uint16_t T16Nx_GetMAT1(T16N_TypeDef* T16Nx);
uint16_t T16Nx_GetMAT2(T16N_TypeDef* T16Nx);
uint16_t T16Nx_GetMAT3(T16N_TypeDef* T16Nx);
uint16_t T16Nx_GetCNT(T16N_TypeDef* T16Nx);
uint8_t T16Nx_GetPRECNT(T16N_TypeDef* T16Nx);
FlagStatus T16Nx_GetFlagStatus(T16N_TypeDef* T16Nx,TIM_TYPE_IT TIM_Flag);
ITStatus T16Nx_GetITStatus(T16N_TypeDef* T16Nx,TIM_TYPE_IT TIM_Flag);
void T16Nx_ClearITPendingBit(T16N_TypeDef* T16Nx,TIM_TYPE_IT TIM_Flag);
/************************************T32模块函数声明********************************************************/
void T32Nx_BaseInit(T32N_TypeDef* T32Nx,TIM_BaseInitStruType* TIM_BaseInitStruct);
void T32Nx_CapInit(T32N_TypeDef* T32Nx,TIM_CapInitStruType* TIM_CapInitStruct);
void T32Nx_MAT0ITConfig(T32N_TypeDef* T32Nx,TIM_TYPE_MATCON Type);
void T32Nx_MAT1ITConfig(T32N_TypeDef* T32Nx,TIM_TYPE_MATCON Type);
void T32Nx_MAT2ITConfig(T32N_TypeDef* T32Nx,TIM_TYPE_MATCON Type);
void T32Nx_MAT3ITConfig(T32N_TypeDef* T32Nx,TIM_TYPE_MATCON Type);
void T32Nx_MAT0Out0Config(T32N_TypeDef* T32Nx,TIM_TYPE_MATOUT Type);
void T32Nx_MAT1Out0Config(T32N_TypeDef* T32Nx,TIM_TYPE_MATOUT Type);
void T32Nx_MAT2Out1Config(T32N_TypeDef* T32Nx,TIM_TYPE_MATOUT Type);
void T32Nx_MAT3Out1Config(T32N_TypeDef* T32Nx,TIM_TYPE_MATOUT Type);
void T32Nx_ITConfig(T32N_TypeDef* T32Nx,TIM_TYPE_IT Type,TYPE_FUNCEN NewState);
void T32Nx_SetCNT(T32N_TypeDef* T16Nx,uint32_t Value);
void T32Nx_SetPRECNT(T32N_TypeDef* T32Nx,uint8_t Value);
void T32Nx_SetPREMAT(T32N_TypeDef* T32Nx,uint8_t Value);
void T32Nx_SetMAT0(T32N_TypeDef* T32Nx,uint32_t Value);
void T32Nx_SetMAT1(T32N_TypeDef* T32Nx,uint32_t Value);
void T32Nx_SetMAT2(T32N_TypeDef* T32Nx,uint32_t Value);
void T32Nx_SetMAT3(T32N_TypeDef* T32Nx,uint32_t Value);
uint32_t T32Nx_GetMAT0(T32N_TypeDef* T32Nx);
uint32_t T32Nx_GetMAT1(T32N_TypeDef* T32Nx);
uint32_t T32Nx_GetMAT2(T32N_TypeDef* T32Nx);
uint32_t T32Nx_GetMAT3(T32N_TypeDef* T32Nx);
uint32_t T32Nx_GetCNT(T32N_TypeDef* T32Nx);
uint8_t T32Nx_GetPRECNT(T32N_TypeDef* T32Nx);
FlagStatus T32Nx_GetFlagStatus(T32N_TypeDef* T32Nx,TIM_TYPE_IT TIM_Flag);
ITStatus T32Nx_GetITStatus(T32N_TypeDef* T32Nx,TIM_TYPE_IT TIM_Flag);
void T32Nx_ClearITPendingBit(T32N_TypeDef* T32Nx,TIM_TYPE_IT TIM_Flag);

#endif

/*************************END OF FILE**********************/
