/***************************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd.
 *文件名： lib_euart.h
 *作 者： ZhangGB
 *版 本： V1.00
 *日 期： 2014/10/24
 *描 述： Euart模块库函数头文件
 *备 注： 适用于 HR8P296芯片
 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
 ***************************************************************/

#ifndef __LIBU7816_H__
#define __LIBU7816_H__

#include "HR8P296.h"
#include "lib_uart.h"
#include "type.h"

/* 发送帧停止位选择 */
typedef enum
{
	EUART_StopBits_1 = 0x0 , //发送帧停止位：1位
	EUART_StopBits_2 = 0x1 , //发送帧停止位：2位
}EUART_TYPE_TXFS;

/* 数据格式 */
typedef enum
{
	EUART_DataMode_7 = 0x0 ,     //数据格式:7位数据
	EUART_DataMode_8 = 0x1 ,     //数据格式:8位数据
	EUART_DataMode_9 = 0x2 ,     //数据格式:9位数据
	EUART_DataMode_7Odd = 0x4 ,  //数据格式:7位数据+寄校验
	EUART_DataMode_7Even = 0x5 , //数据格式:7位数据+偶校验
	EUART_DataMode_8Odd = 0x6 ,  //数据格式:8位数据+寄校验
	EUART_DataMode_8Even = 0x7 , //数据格式:8位数据+偶校验
}EUART_TYPE_DATAMOD;

/* 端口极性 */
typedef enum
{
	EUART_Polar_Normal = 0x0 ,   //发送端口极性:正常
	EUART_Polar_Opposite = 0x1 , //发送端口极性:反向
}EUART_TYPE_RTXP;

/* 波特率发生器时钟选择 */
typedef enum
{
	EUART_Clock_1 = 0x1 , //波特率发生器时钟:PCLK
	EUART_Clock_2 = 0x2 , //波特率发生器时钟:PCLK/2
	EUART_Clock_3 = 0x3 , //波特率发生器时钟:PCLK/4
}EUART_TYPE_BCS;

/* EUART初始化配置结构体定义 */
typedef struct
{
	EUART_TYPE_TXFS  EUART_StopBits;  //发送帧停止位选择

	EUART_TYPE_DATAMOD  EUART_TxMode; //发送数据帧格式

	EUART_TYPE_RTXP  EUART_TxPolar;   //发送端口极性

	EUART_TYPE_DATAMOD  EUART_RxMode; //接收数据帧格式

	EUART_TYPE_RTXP  EUART_RxPolar;   //接收端口极性

	uint32_t  EUART_BaudRate;         //传输波特率

	EUART_TYPE_BCS  EUART_ClockSet;   //波特率发生器时钟选择

}EUART_InitStruType;

/* 中断选择 */
typedef enum
{
	EUART_IT_TB = 0x0001 ,
	EUART_IT_RB = 0x0002 ,
	EUART_IT_RO = 0x0004 ,
	EUART_IT_FE = 0x0008 ,
	EUART_IT_PE = 0x0010 ,
	EUART_IT_TBWE = 0x0020 ,
	EUART_IT_ARTE = 0x0040 ,
	EUART_IT_RNA = 0x0080 ,
	EUART_IT_TXIDLE = 0x0100 ,
	EUART_IT_RXIDLE = 0x0200 ,
}EUART_TYPE_IT;

/* 标志位 */
typedef enum
{
	EUART_FLAG_TB = 0x0001 ,
	EUART_FLAG_RB = 0x0002 ,
	EUART_FLAG_RO = 0x0004 ,
	EUART_FLAG_FE = 0x0008 ,
	EUART_FLAG_PE = 0x0010 ,
	EUART_FLAG_TBWE = 0x0020 ,
	EUART_FLAG_ARTE = 0x0040 ,
	EUART_FLAG_RNA = 0x0080 ,
	EUART_FLAG_TXIDLE = 0x0100 ,
	EUART_FLAG_RXIDLE = 0x0200 ,
}EUART_TYPE_FLAG;

/* 清除中断标志位 */
typedef enum
{
	EUART_Clr_RO = 0x0004 ,
	EUART_Clr_FE = 0x0008 ,
	EUART_Clr_PE = 0x0010 ,
	EUART_Clr_TBWE = 0x0020 ,
	EUART_Clr_ARTE = 0x0040 ,
	EUART_Clr_RNA = 0x0080 ,
}EUART_CLR_IF;

/* 发送、接收中断模式 */
typedef enum
{
	EUART_TRBIM_Byte = 0x0 ,     //中断:字节空
	EUART_TRBIM_HalfWord = 0x1 , //中断:半字空
	EUART_TRBIM_Word = 0x2 ,     //中断:字空
}EUART_TYPE_TRBIM;


/*----------------------U7816智能卡接口模块相关----------------------*/

/* EUART工作模式 */
typedef enum
{
	EUART_Mode_U7816 = 0x1 , //7816模式
	EUART_Mode_Uart = 0x0 ,  //普通uart模式
}EUART_TYPE_MODE;

/* EIO通讯通道选择位 */ 
typedef enum
{
	U7816_CHS_EIO0 = 0x0 , //7816 IO端口0
	U7816_CHS_EIO1 = 0x1 , //7816 IO端口1
}U7816_TYPE_CHS;

/* EIO端口方向控制 */ 
typedef enum
{
	U7816_EIODir_In = 0x0 ,  //7816 EIO 接收数据
	U7816_EIODir_Out = 0x1 , //7816 EIO 发送数据
}U7816_TYPE_EIOC;

/* 数据格式选择位 */ 
typedef enum
{
	U7816_DataForm_Normal = 0x0 ,   //数据格式为正向
	U7816_DataForm_Contrary = 0x1 , //数据格式为反向
}U7816_TYPE_DAS ;

/* 数据奇偶校验位 */
typedef enum
{
	U7816_Verify_Odd = 0x0 ,  //数据奇数校验
	U7816_Verify_Even = 0x1 , //数据偶数校验
}U7816_TYPE_PS;

/* NACK信号宽度 */
typedef enum
{
	U7816_NACKWidth_1ETU = 0x0 ,   //NACK信号宽度 1个ETU
	U7816_NACKWidth_1P5ETU = 0x1 , //NACK信号宽度 1.5个ETU
	U7816_NACKWidth_2ETU = 0x2 ,   //NACK信号宽度 2个ETU
}U7816_TYPE_TNAS;

/* 自动重发次数 */
typedef enum
{
	U7816_RetryTimes_1 = 0x0 , //7816自动重发次数1次
	U7816_RetryTimes_2 = 0x1 , //7816自动重发次数2次
	U7816_RetryTimes_3 = 0x2 , //7816自动重发次数3次
	U7816_RetryTimes_N = 0x3 , //7816自动重发次数无限次，直到发送成功
}U7816_TYPE_ARTS;

/* 时钟源选择 */
typedef enum
{
	U7816_PCLK_1 = 0x0 , //PCLK时钟
	U7816_PCLK_2 = 0x1 , //PCLK时钟2分频
	U7816_PCLK_4 = 0x2 , //PCLK时钟4分频
	U7816_PCLK_8 = 0x3 , //PCLK时钟8分频
}U7816_TYPE_CKS;



/* U7816模块初始化配置结构体定义 */
typedef struct
{    
	EUART_TYPE_BCS  U7816_ClockSet;     //波特率发生器时钟选择

	uint32_t  U7816_BaudRate;           //传输波特率

	TYPE_FUNCEN  U7816_ECK0;            // EUARTxMOD：bit2 7816ECK0使能位

	TYPE_FUNCEN  U7816_ECK1;            // EUARTxMOD：bit3 7816ECK1使能位

	U7816_TYPE_CHS  U7816_EIOCh;        // EUARTxMOD：bit4 EIO通讯通道选择位

	U7816_TYPE_EIOC  U7816_EIODir;      // EUARTxMOD：bit5 EIO端口方向控制

	U7816_TYPE_DAS  U7816_DataForm;     // EUARTxMOD：bit6 数据格式选择位

	U7816_TYPE_PS  U7816_DataVerify;    // EUARTxMOD：bit7 数据奇偶校验位

	TYPE_FUNCEN  U7816_AutoRetryTX;     // EUARTxMOD：bit8 自动重发使能位

	TYPE_FUNCEN  U7816_AutoRetryRX;     // EUARTxMOD：bit9 自动重收使能位

	U7816_TYPE_TNAS  U7816_NACK_Width;  // EUARTxMOD：bit11~10 NACK信号宽度

	U7816_TYPE_ARTS  U7816_RetryTimes;  // EUARTxMOD：bit13~12 自动重发次数

	U7816_TYPE_CKS  U7816_CLKS;         // EUARTxMOD：bit15~14 时钟源选择

	uint8_t  U7816_ETUTime;             // EUARTxMOD：bit23~16 ETU保护时间选择 2~257 个ETU		

}U7816_InitStruType;


/*************EUART模块宏定义************/

/* 发送使能控制 */
#define EUART0_TxEnable()  (EUART0->CON.TXEN = 1)
#define EUART1_TxEnable()  (EUART1->CON.TXEN = 1)

#define EUART0_TxDisable() (EUART0->CON.TXEN = 0)
#define EUART1_TxDisable() (EUART1->CON.TXEN = 0)


/* 接收使能控制 */
#define EUART0_RxEnable()  (EUART0->CON.RXEN = 1)
#define EUART1_RxEnable()  (EUART1->CON.RXEN = 1)

#define EUART0_RxDisable() (EUART0->CON.RXEN = 0)
#define EUART1_RxDisable() (EUART1->CON.RXEN = 0)


/* 发送器复位 */
#define EUART0_TxRst() (EUART0->CON.TRST = 1)
#define EUART1_TxRst() (EUART1->CON.TRST = 1)

/* 接收器复位 */
#define EUART0_RxRst() (EUART0->CON.RRST = 1)
#define EUART1_RxRst() (EUART1->CON.RRST = 1)

/* U7816 软件复位 */
#define EUART0_U7816_REST() ( EUART0->MOD.ERST = 1)
#define EUART1_U7816_REST() ( EUART1->MOD.ERST = 1)

/************EUART模块函数声明***********/

void EUART_ModeConfig(EUART_TypeDef* EUARTx,EUART_TYPE_MODE Mode);
void EUART_Init(EUART_TypeDef* EUARTx,EUART_InitStruType* EUART_InitStruct);
void EUART_BaudConfig(EUART_TypeDef* EUARTx, uint32_t BaudRate, EUART_TYPE_BCS ClockSet);
void EUART_ITConfig(EUART_TypeDef* EUARTx, EUART_TYPE_IT EUART_IT, TYPE_FUNCEN NewState);
void EUART_TBIMConfig(EUART_TypeDef* EUARTx,EUART_TYPE_TRBIM Type);
void EUART_RBIMConfig(EUART_TypeDef* EUARTx,EUART_TYPE_TRBIM Type);
FlagStatus EUART_GetFlagStatus(EUART_TypeDef* EUARTx,EUART_TYPE_FLAG EUART_Flag);
ITStatus EUART_GetITStatus(EUART_TypeDef* EUARTx,EUART_TYPE_IT EUART_Flag);
void EUART_ClearITPendingBit(EUART_TypeDef* EUARTx,EUART_CLR_IF EUART_Flag);

void U7816_Init(EUART_TypeDef* EUARTx, U7816_InitStruType* U7816_InitStruct);
void EUART_EIOChConfig(EUART_TypeDef* EUARTx, U7816_TYPE_CHS U7816_IO);
void EUART_EIODirection(EUART_TypeDef* EUARTx, U7816_TYPE_EIOC Dir);
void EUART_SendByte(EUART_TypeDef* EUARTx, uint8_t ByteData);
void EUART_SendHalfWord(EUART_TypeDef* EUARTx, uint16_t HalfWordData);
void EUART_SendWord(EUART_TypeDef* EUARTx, uint32_t WordData);
uint8_t EUART_RecByte(EUART_TypeDef* EUARTx);
uint16_t EUART_RecHalfWord(EUART_TypeDef* EUARTx);
uint32_t EUART_RecWord(EUART_TypeDef* EUARTx);

#endif

/*************************END OF FILE**********************/
