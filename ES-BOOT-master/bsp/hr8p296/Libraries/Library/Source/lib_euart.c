/***************************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
 *文件名：  lib_euart.c
 *作  者:  ZouX
 *版  本:  V1.00
 *日  期:  2015/02/03
 *描  述：  Euart模块库函数
 *备  注：  适用于 HR8P296芯片
 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
 ***************************************************************/

#include "lib_euart.h"
#include "system_hr8p296.h"
/***************************************************************
  函数名：EUART_ModeConfig
  描  述：EUART工作模式选择
  输入值：EUARTx：可以是EUART0 、 Mode：工作模式
  输出值：无
  返回值：无
 ***************************************************************/
void EUART_ModeConfig(EUART_TypeDef* EUARTx,EUART_TYPE_MODE Mode)
{
	EUARTx->MOD.MOD  =  Mode;
}

/***************************************************************
  函数名：EUART_Init
  描  述：EUART初始化
  输入值：EUARTx：可以是EUART0 、 EUART_InitStruct：初始化配置结构体地址
  输出值：无
  返回值：无
 ***************************************************************/
void EUART_Init(EUART_TypeDef* EUARTx,EUART_InitStruType* EUART_InitStruct)
{
	uint32_t BaudTemp = 0;
	if(EUART_InitStruct->EUART_ClockSet == EUART_Clock_1)
	{
		BaudTemp = (SystemCoreClock/16/EUART_InitStruct->EUART_BaudRate)-1;
	}
	else if(EUART_InitStruct->EUART_ClockSet == EUART_Clock_2)
	{
		BaudTemp = (SystemCoreClock/32/EUART_InitStruct->EUART_BaudRate)-1;
	}
	else if(EUART_InitStruct->EUART_ClockSet == EUART_Clock_3)
	{
		BaudTemp = (SystemCoreClock/64/EUART_InitStruct->EUART_BaudRate)-1;
	}

	if(BaudTemp > 2046)
	{
		BaudTemp = 2047;
	}

	EUARTx->CON.BCS = EUART_InitStruct->EUART_ClockSet;
	EUARTx->BRR.BRR = BaudTemp;
	EUARTx->CON.TXFS   =  EUART_InitStruct->EUART_StopBits;
	EUARTx->CON.TXMOD  =  EUART_InitStruct->EUART_TxMode;
	EUARTx->CON.TXP    =  EUART_InitStruct->EUART_TxPolar;
	EUARTx->CON.RXMOD  =  EUART_InitStruct->EUART_RxMode;
	EUARTx->CON.RXP    =  EUART_InitStruct->EUART_RxPolar;
}

/***************************************************************
  函数名：EUART_BaudConfig
  描  述：EUART波特率设置
  输入值：EUARTx：可以是EUART0 、 BaudRate：目标波特率（HZ） 、 ClockSet：波特率发生器时钟选择
  输出值：无
  返回值：无
 ***************************************************************/
void EUART_BaudConfig(EUART_TypeDef* EUARTx, uint32_t BaudRate, EUART_TYPE_BCS ClockSet)
{
	uint32_t BaudTemp = 0;
	if(ClockSet == EUART_Clock_1)
	{
		BaudTemp = (SystemCoreClock/16/BaudRate)-1;
	}
	else if(ClockSet == EUART_Clock_2)
	{
		BaudTemp = (SystemCoreClock/32/BaudRate)-1;
	}
	else if(ClockSet == EUART_Clock_3)
	{
		BaudTemp = (SystemCoreClock/64/BaudRate)-1;
	}

	if(BaudTemp > 2046)
	{
		BaudTemp = 2047;
	}

	EUARTx->CON.BCS = ClockSet;
	EUARTx->BRR.BRR = BaudTemp;    
}

/***************************************************************
  函数名：EUART_ITConfig
  描  述：EUART中断配置
  输入值：EUARTx：可以是EUART0 、 EUART_IT：中断类型 、 NewState：使能/失能
  输出值：无
  返回值：无
 ***************************************************************/
void EUART_ITConfig(EUART_TypeDef* EUARTx, EUART_TYPE_IT EUART_IT, TYPE_FUNCEN NewState)
{
	if (NewState != DISABLE)
	{
		EUARTx->IE.Word |=  (uint32_t) EUART_IT;
	}
	else
	{
		EUARTx->IE.Word &=  ~((uint32_t) EUART_IT);
	}
}

/***************************************************************
  函数名：EUART_TBIMConfig
  描  述：EUART发送缓冲器空中断模式选择
  输入值：EUARTx：可以是EUART0 、 Type：空中断模式
  输出值：无
  返回值：无
 ***************************************************************/
void EUART_TBIMConfig(EUART_TypeDef* EUARTx,EUART_TYPE_TRBIM Type)
{
	EUARTx->IE.TBIM = Type;
}

/***************************************************************
  函数名：EUART_RBIMConfig
  描  述：EUART接收缓冲器满中断模式选择
  输入值：EUARTx：可以是EUART0 、 Type：满中断模式
  输出值：无
  返回值：无
 ***************************************************************/
void EUART_RBIMConfig(EUART_TypeDef* EUARTx,EUART_TYPE_TRBIM Type)
{
	EUARTx->IE.RBIM = Type;
}

/***************************************************************
  函数名：EUART_GetFlagStatus
  描  述：EUART获取标志位状态
  输入值：EUARTx：可以是EUART0 、 EUART_Flag：标志位
  输出值：无
  返回值：SET/RESET
 ***************************************************************/
FlagStatus EUART_GetFlagStatus(EUART_TypeDef* EUARTx,EUART_TYPE_FLAG EUART_Flag)
{
	FlagStatus bitstatus = RESET;

	if ((EUARTx->IF.Word & ((uint32_t)EUART_Flag )) != RESET)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}

	return  bitstatus;
}

/***************************************************************
  函数名：EUART_GetITStatus
  描  述：EUART获取中断状态,未使能相应中断时不会返回SET
  输入值：EUARTx：可以是EUART0 、 EUART_Flag：标志位
  输出值：无
  返回值：SET/RESET
 ***************************************************************/
ITStatus EUART_GetITStatus(EUART_TypeDef* EUARTx,EUART_TYPE_IT EUART_Flag)
{
	FlagStatus bitstatus = RESET;
	uint32_t itstatus = 0x0, itenable = 0x0;

	itenable = EUARTx->IE.Word & (uint32_t)EUART_Flag;
	itstatus = EUARTx->IF.Word & (uint32_t)EUART_Flag;

	if ((itstatus != (uint32_t)RESET) && (itenable != (uint32_t)RESET))
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}

	return  bitstatus; 
}

/***************************************************************
  函数名：EUART_ClearITPendingBit
  描  述：EUART标志位清除
  输入值：EUARTx：可以是EUART0 、 EUART_Flag：标志位
  输出值：无
  返回值：无
 ***************************************************************/
void EUART_ClearITPendingBit(EUART_TypeDef* EUARTx,EUART_CLR_IF EUART_Flag)
{
	EUARTx->IF.Word = (uint32_t)EUART_Flag;
}


/*------------------------------------------------------------------------------------------------*/
/********************************U7816智能卡模块相关函数*******************************************/
/*------------------------------------------------------------------------------------------------*/

/***************************************************************
  函数名：U7816_Init
  描  述：初始化U7816模块
  输入值：EUARTx：可以是EUART0 、 U7816_InitStruct：初始化配置结构体地址
  输出值：无
  返回值：无
 ***************************************************************/
void  U7816_Init(EUART_TypeDef* EUARTx, U7816_InitStruType* U7816_InitStruct)
{
	EUARTx->MOD.CK0E =  U7816_InitStruct->U7816_ECK0;        // EUARTxMOD：bit2   7816ECK0使能位

	EUARTx->MOD.CK1E =  U7816_InitStruct->U7816_ECK1;        // EUARTxMOD：bit3   7816ECK1使能位

	EUARTx->MOD.CHS  =  U7816_InitStruct->U7816_EIOCh;       // EUARTxMOD：bit4   EIO通讯通道选择位

	EUARTx->MOD.IOC  =  U7816_InitStruct->U7816_EIODir;      // EUARTxMOD：bit5   EIO端口方向控制

	EUARTx->MOD.DAS  =  U7816_InitStruct->U7816_DataForm;    // EUARTxMOD：bit6   数据格式选择位

	EUARTx->MOD.PS   =  U7816_InitStruct->U7816_DataVerify;  // EUARTxMOD：bit7   数据奇偶校验位

	EUARTx->MOD.ARTE =  U7816_InitStruct->U7816_AutoRetryTX; // EUARTxMOD：bit8   自动重发使能位

	EUARTx->MOD.ARRE =  U7816_InitStruct->U7816_AutoRetryRX; // EUARTxMOD：bit9   自动重收使能位

	EUARTx->MOD.TNAS =  U7816_InitStruct->U7816_NACK_Width;  // EUARTxMOD：bit11~10  NACK信号宽度

	EUARTx->MOD.ARTS =  U7816_InitStruct->U7816_RetryTimes;  // EUARTxMOD：bit12~13  自动重发次数

	EUARTx->MOD.CKS  =  U7816_InitStruct->U7816_CLKS;        // EUARTxMOD：bit15~14  时钟源选择

	//设置波特率
	EUART_BaudConfig(EUARTx,U7816_InitStruct->U7816_BaudRate,U7816_InitStruct->U7816_ClockSet);

	/* EUARTxMOD：bit23~16  ETU保护时间选择 2~257 个ETU */     
	if(U7816_InitStruct->U7816_ETUTime >255)
	{
		U7816_InitStruct->U7816_ETUTime = 255;
	}
	EUARTx->MOD.ETUS  =  U7816_InitStruct->U7816_ETUTime;

}

/***************************************************************
  函数名：EUART_EIOChConfig
  描  述：EUART EIO 通讯通道选择
  输入值：EUARTx：可以是EUART0 、 U7816_IO：IO端口号
  输出值：无
  返回值：无
 ***************************************************************/
void  EUART_EIOChConfig(EUART_TypeDef* EUARTx, U7816_TYPE_CHS U7816_IO)
{
	EUARTx->MOD.CHS  =  U7816_IO;
}

/***************************************************************
  函数名：EUART_EIODirection
  描  述：EUART EIO 端口方向控制
  输入值：EUARTx：可以是EUART0 、 direction：IO端口状态
  输出值：无
  返回值：无
 ***************************************************************/
void EUART_EIODirection(EUART_TypeDef* EUARTx, U7816_TYPE_EIOC Dir)
{
	EUARTx->MOD.IOC  =  Dir;
}

/***************************************************************
  函数名：EUART_SendByte
  描  述：EUART Byte写数据
  输入值：EUARTx：可以是EUART0 、 ByteData：写入数据
  输出值：无
  返回值：无
 ***************************************************************/
void EUART_SendByte(EUART_TypeDef* EUARTx, uint8_t ByteData)
{
	EUARTx->TBW.Byte[0] = ByteData;
}

/***************************************************************
  函数名：EUART_SendHalfWord
  描  述：EUART HalfWord写数据
  输入值：EUARTx：可以是EUART0 、 HalfWordData：写入数据
  输出值：无
  返回值：无
 ***************************************************************/
void EUART_SendHalfWord(EUART_TypeDef* EUARTx, uint16_t HalfWordData)
{
	EUARTx->TBW.HalfWord[0] = HalfWordData;
}

/***************************************************************
  函数名：EUART_SendWord
  描  述：EUART word写数据
  输入值：EUARTx：可以是EUART0 、 WordData：写入数据
  输出值：无
  返回值：无
 ***************************************************************/
void EUART_SendWord(EUART_TypeDef* EUARTx, uint32_t WordData)
{
	EUARTx->TBW.Word = WordData;
}

/***************************************************************
  函数名：EUART_RecByte
  描  述：EUART Byte读数据
  输入值：EUARTx：可以是EUART0
  输出值：无
  返回值：读的数据
 ***************************************************************/
uint8_t EUART_RecByte(EUART_TypeDef* EUARTx)
{
	return(EUARTx->RBR.Byte[0]);
}

/***************************************************************
  函数名：EUART_RecHalfWord
  描  述：EUART HalfWord读数据
  输入值：EUARTx：可以是EUART0
  输出值：无
  返回值：读的数据
 ***************************************************************/
uint16_t EUART_RecHalfWord(EUART_TypeDef* EUARTx)
{
	return(EUARTx->RBR.HalfWord[0]);
}

/***************************************************************
  函数名：EUART_RecWord
  描  述：EUART Word读数据
  输入值：EUARTx：可以是EUART0
  输出值：无
  返回值：读的数据
 ***************************************************************/
uint32_t EUART_RecWord(EUART_TypeDef* EUARTx)
{
	return(EUARTx->RBR.Word);
}

/*************************END OF FILE**********************/
