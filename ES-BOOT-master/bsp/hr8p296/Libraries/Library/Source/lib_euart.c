/***************************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
 *�ļ�����  lib_euart.c
 *��  ��:  ZouX
 *��  ��:  V1.00
 *��  ��:  2015/02/03
 *��  ����  Euartģ��⺯��
 *��  ע��  ������ HR8P296оƬ
 ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 ***************************************************************/

#include "lib_euart.h"
#include "system_hr8p296.h"
/***************************************************************
  ��������EUART_ModeConfig
  ��  ����EUART����ģʽѡ��
  ����ֵ��EUARTx��������EUART0 �� Mode������ģʽ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void EUART_ModeConfig(EUART_TypeDef* EUARTx,EUART_TYPE_MODE Mode)
{
	EUARTx->MOD.MOD  =  Mode;
}

/***************************************************************
  ��������EUART_Init
  ��  ����EUART��ʼ��
  ����ֵ��EUARTx��������EUART0 �� EUART_InitStruct����ʼ�����ýṹ���ַ
  ���ֵ����
  ����ֵ����
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
  ��������EUART_BaudConfig
  ��  ����EUART����������
  ����ֵ��EUARTx��������EUART0 �� BaudRate��Ŀ�겨���ʣ�HZ�� �� ClockSet�������ʷ�����ʱ��ѡ��
  ���ֵ����
  ����ֵ����
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
  ��������EUART_ITConfig
  ��  ����EUART�ж�����
  ����ֵ��EUARTx��������EUART0 �� EUART_IT���ж����� �� NewState��ʹ��/ʧ��
  ���ֵ����
  ����ֵ����
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
  ��������EUART_TBIMConfig
  ��  ����EUART���ͻ��������ж�ģʽѡ��
  ����ֵ��EUARTx��������EUART0 �� Type�����ж�ģʽ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void EUART_TBIMConfig(EUART_TypeDef* EUARTx,EUART_TYPE_TRBIM Type)
{
	EUARTx->IE.TBIM = Type;
}

/***************************************************************
  ��������EUART_RBIMConfig
  ��  ����EUART���ջ��������ж�ģʽѡ��
  ����ֵ��EUARTx��������EUART0 �� Type�����ж�ģʽ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void EUART_RBIMConfig(EUART_TypeDef* EUARTx,EUART_TYPE_TRBIM Type)
{
	EUARTx->IE.RBIM = Type;
}

/***************************************************************
  ��������EUART_GetFlagStatus
  ��  ����EUART��ȡ��־λ״̬
  ����ֵ��EUARTx��������EUART0 �� EUART_Flag����־λ
  ���ֵ����
  ����ֵ��SET/RESET
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
  ��������EUART_GetITStatus
  ��  ����EUART��ȡ�ж�״̬,δʹ����Ӧ�ж�ʱ���᷵��SET
  ����ֵ��EUARTx��������EUART0 �� EUART_Flag����־λ
  ���ֵ����
  ����ֵ��SET/RESET
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
  ��������EUART_ClearITPendingBit
  ��  ����EUART��־λ���
  ����ֵ��EUARTx��������EUART0 �� EUART_Flag����־λ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void EUART_ClearITPendingBit(EUART_TypeDef* EUARTx,EUART_CLR_IF EUART_Flag)
{
	EUARTx->IF.Word = (uint32_t)EUART_Flag;
}


/*------------------------------------------------------------------------------------------------*/
/********************************U7816���ܿ�ģ����غ���*******************************************/
/*------------------------------------------------------------------------------------------------*/

/***************************************************************
  ��������U7816_Init
  ��  ������ʼ��U7816ģ��
  ����ֵ��EUARTx��������EUART0 �� U7816_InitStruct����ʼ�����ýṹ���ַ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void  U7816_Init(EUART_TypeDef* EUARTx, U7816_InitStruType* U7816_InitStruct)
{
	EUARTx->MOD.CK0E =  U7816_InitStruct->U7816_ECK0;        // EUARTxMOD��bit2   7816ECK0ʹ��λ

	EUARTx->MOD.CK1E =  U7816_InitStruct->U7816_ECK1;        // EUARTxMOD��bit3   7816ECK1ʹ��λ

	EUARTx->MOD.CHS  =  U7816_InitStruct->U7816_EIOCh;       // EUARTxMOD��bit4   EIOͨѶͨ��ѡ��λ

	EUARTx->MOD.IOC  =  U7816_InitStruct->U7816_EIODir;      // EUARTxMOD��bit5   EIO�˿ڷ������

	EUARTx->MOD.DAS  =  U7816_InitStruct->U7816_DataForm;    // EUARTxMOD��bit6   ���ݸ�ʽѡ��λ

	EUARTx->MOD.PS   =  U7816_InitStruct->U7816_DataVerify;  // EUARTxMOD��bit7   ������żУ��λ

	EUARTx->MOD.ARTE =  U7816_InitStruct->U7816_AutoRetryTX; // EUARTxMOD��bit8   �Զ��ط�ʹ��λ

	EUARTx->MOD.ARRE =  U7816_InitStruct->U7816_AutoRetryRX; // EUARTxMOD��bit9   �Զ�����ʹ��λ

	EUARTx->MOD.TNAS =  U7816_InitStruct->U7816_NACK_Width;  // EUARTxMOD��bit11~10  NACK�źſ��

	EUARTx->MOD.ARTS =  U7816_InitStruct->U7816_RetryTimes;  // EUARTxMOD��bit12~13  �Զ��ط�����

	EUARTx->MOD.CKS  =  U7816_InitStruct->U7816_CLKS;        // EUARTxMOD��bit15~14  ʱ��Դѡ��

	//���ò�����
	EUART_BaudConfig(EUARTx,U7816_InitStruct->U7816_BaudRate,U7816_InitStruct->U7816_ClockSet);

	/* EUARTxMOD��bit23~16  ETU����ʱ��ѡ�� 2~257 ��ETU */     
	if(U7816_InitStruct->U7816_ETUTime >255)
	{
		U7816_InitStruct->U7816_ETUTime = 255;
	}
	EUARTx->MOD.ETUS  =  U7816_InitStruct->U7816_ETUTime;

}

/***************************************************************
  ��������EUART_EIOChConfig
  ��  ����EUART EIO ͨѶͨ��ѡ��
  ����ֵ��EUARTx��������EUART0 �� U7816_IO��IO�˿ں�
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void  EUART_EIOChConfig(EUART_TypeDef* EUARTx, U7816_TYPE_CHS U7816_IO)
{
	EUARTx->MOD.CHS  =  U7816_IO;
}

/***************************************************************
  ��������EUART_EIODirection
  ��  ����EUART EIO �˿ڷ������
  ����ֵ��EUARTx��������EUART0 �� direction��IO�˿�״̬
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void EUART_EIODirection(EUART_TypeDef* EUARTx, U7816_TYPE_EIOC Dir)
{
	EUARTx->MOD.IOC  =  Dir;
}

/***************************************************************
  ��������EUART_SendByte
  ��  ����EUART Byteд����
  ����ֵ��EUARTx��������EUART0 �� ByteData��д������
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void EUART_SendByte(EUART_TypeDef* EUARTx, uint8_t ByteData)
{
	EUARTx->TBW.Byte[0] = ByteData;
}

/***************************************************************
  ��������EUART_SendHalfWord
  ��  ����EUART HalfWordд����
  ����ֵ��EUARTx��������EUART0 �� HalfWordData��д������
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void EUART_SendHalfWord(EUART_TypeDef* EUARTx, uint16_t HalfWordData)
{
	EUARTx->TBW.HalfWord[0] = HalfWordData;
}

/***************************************************************
  ��������EUART_SendWord
  ��  ����EUART wordд����
  ����ֵ��EUARTx��������EUART0 �� WordData��д������
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void EUART_SendWord(EUART_TypeDef* EUARTx, uint32_t WordData)
{
	EUARTx->TBW.Word = WordData;
}

/***************************************************************
  ��������EUART_RecByte
  ��  ����EUART Byte������
  ����ֵ��EUARTx��������EUART0
  ���ֵ����
  ����ֵ����������
 ***************************************************************/
uint8_t EUART_RecByte(EUART_TypeDef* EUARTx)
{
	return(EUARTx->RBR.Byte[0]);
}

/***************************************************************
  ��������EUART_RecHalfWord
  ��  ����EUART HalfWord������
  ����ֵ��EUARTx��������EUART0
  ���ֵ����
  ����ֵ����������
 ***************************************************************/
uint16_t EUART_RecHalfWord(EUART_TypeDef* EUARTx)
{
	return(EUARTx->RBR.HalfWord[0]);
}

/***************************************************************
  ��������EUART_RecWord
  ��  ����EUART Word������
  ����ֵ��EUARTx��������EUART0
  ���ֵ����
  ����ֵ����������
 ***************************************************************/
uint32_t EUART_RecWord(EUART_TypeDef* EUARTx)
{
	return(EUARTx->RBR.Word);
}

/*************************END OF FILE**********************/
