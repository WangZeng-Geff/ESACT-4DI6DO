/***************************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd.
 *�ļ����� lib_euart.h
 *�� �ߣ� ZhangGB
 *�� ���� V1.00
 *�� �ڣ� 2014/10/24
 *�� ���� Euartģ��⺯��ͷ�ļ�
 *�� ע�� ������ HR8P296оƬ
 ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 ***************************************************************/

#ifndef __LIBU7816_H__
#define __LIBU7816_H__

#include "HR8P296.h"
#include "lib_uart.h"
#include "type.h"

/* ����ֹ֡ͣλѡ�� */
typedef enum
{
	EUART_StopBits_1 = 0x0 , //����ֹ֡ͣλ��1λ
	EUART_StopBits_2 = 0x1 , //����ֹ֡ͣλ��2λ
}EUART_TYPE_TXFS;

/* ���ݸ�ʽ */
typedef enum
{
	EUART_DataMode_7 = 0x0 ,     //���ݸ�ʽ:7λ����
	EUART_DataMode_8 = 0x1 ,     //���ݸ�ʽ:8λ����
	EUART_DataMode_9 = 0x2 ,     //���ݸ�ʽ:9λ����
	EUART_DataMode_7Odd = 0x4 ,  //���ݸ�ʽ:7λ����+��У��
	EUART_DataMode_7Even = 0x5 , //���ݸ�ʽ:7λ����+żУ��
	EUART_DataMode_8Odd = 0x6 ,  //���ݸ�ʽ:8λ����+��У��
	EUART_DataMode_8Even = 0x7 , //���ݸ�ʽ:8λ����+żУ��
}EUART_TYPE_DATAMOD;

/* �˿ڼ��� */
typedef enum
{
	EUART_Polar_Normal = 0x0 ,   //���Ͷ˿ڼ���:����
	EUART_Polar_Opposite = 0x1 , //���Ͷ˿ڼ���:����
}EUART_TYPE_RTXP;

/* �����ʷ�����ʱ��ѡ�� */
typedef enum
{
	EUART_Clock_1 = 0x1 , //�����ʷ�����ʱ��:PCLK
	EUART_Clock_2 = 0x2 , //�����ʷ�����ʱ��:PCLK/2
	EUART_Clock_3 = 0x3 , //�����ʷ�����ʱ��:PCLK/4
}EUART_TYPE_BCS;

/* EUART��ʼ�����ýṹ�嶨�� */
typedef struct
{
	EUART_TYPE_TXFS  EUART_StopBits;  //����ֹ֡ͣλѡ��

	EUART_TYPE_DATAMOD  EUART_TxMode; //��������֡��ʽ

	EUART_TYPE_RTXP  EUART_TxPolar;   //���Ͷ˿ڼ���

	EUART_TYPE_DATAMOD  EUART_RxMode; //��������֡��ʽ

	EUART_TYPE_RTXP  EUART_RxPolar;   //���ն˿ڼ���

	uint32_t  EUART_BaudRate;         //���䲨����

	EUART_TYPE_BCS  EUART_ClockSet;   //�����ʷ�����ʱ��ѡ��

}EUART_InitStruType;

/* �ж�ѡ�� */
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

/* ��־λ */
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

/* ����жϱ�־λ */
typedef enum
{
	EUART_Clr_RO = 0x0004 ,
	EUART_Clr_FE = 0x0008 ,
	EUART_Clr_PE = 0x0010 ,
	EUART_Clr_TBWE = 0x0020 ,
	EUART_Clr_ARTE = 0x0040 ,
	EUART_Clr_RNA = 0x0080 ,
}EUART_CLR_IF;

/* ���͡������ж�ģʽ */
typedef enum
{
	EUART_TRBIM_Byte = 0x0 ,     //�ж�:�ֽڿ�
	EUART_TRBIM_HalfWord = 0x1 , //�ж�:���ֿ�
	EUART_TRBIM_Word = 0x2 ,     //�ж�:�ֿ�
}EUART_TYPE_TRBIM;


/*----------------------U7816���ܿ��ӿ�ģ�����----------------------*/

/* EUART����ģʽ */
typedef enum
{
	EUART_Mode_U7816 = 0x1 , //7816ģʽ
	EUART_Mode_Uart = 0x0 ,  //��ͨuartģʽ
}EUART_TYPE_MODE;

/* EIOͨѶͨ��ѡ��λ */ 
typedef enum
{
	U7816_CHS_EIO0 = 0x0 , //7816 IO�˿�0
	U7816_CHS_EIO1 = 0x1 , //7816 IO�˿�1
}U7816_TYPE_CHS;

/* EIO�˿ڷ������ */ 
typedef enum
{
	U7816_EIODir_In = 0x0 ,  //7816 EIO ��������
	U7816_EIODir_Out = 0x1 , //7816 EIO ��������
}U7816_TYPE_EIOC;

/* ���ݸ�ʽѡ��λ */ 
typedef enum
{
	U7816_DataForm_Normal = 0x0 ,   //���ݸ�ʽΪ����
	U7816_DataForm_Contrary = 0x1 , //���ݸ�ʽΪ����
}U7816_TYPE_DAS ;

/* ������żУ��λ */
typedef enum
{
	U7816_Verify_Odd = 0x0 ,  //��������У��
	U7816_Verify_Even = 0x1 , //����ż��У��
}U7816_TYPE_PS;

/* NACK�źſ�� */
typedef enum
{
	U7816_NACKWidth_1ETU = 0x0 ,   //NACK�źſ�� 1��ETU
	U7816_NACKWidth_1P5ETU = 0x1 , //NACK�źſ�� 1.5��ETU
	U7816_NACKWidth_2ETU = 0x2 ,   //NACK�źſ�� 2��ETU
}U7816_TYPE_TNAS;

/* �Զ��ط����� */
typedef enum
{
	U7816_RetryTimes_1 = 0x0 , //7816�Զ��ط�����1��
	U7816_RetryTimes_2 = 0x1 , //7816�Զ��ط�����2��
	U7816_RetryTimes_3 = 0x2 , //7816�Զ��ط�����3��
	U7816_RetryTimes_N = 0x3 , //7816�Զ��ط��������޴Σ�ֱ�����ͳɹ�
}U7816_TYPE_ARTS;

/* ʱ��Դѡ�� */
typedef enum
{
	U7816_PCLK_1 = 0x0 , //PCLKʱ��
	U7816_PCLK_2 = 0x1 , //PCLKʱ��2��Ƶ
	U7816_PCLK_4 = 0x2 , //PCLKʱ��4��Ƶ
	U7816_PCLK_8 = 0x3 , //PCLKʱ��8��Ƶ
}U7816_TYPE_CKS;



/* U7816ģ���ʼ�����ýṹ�嶨�� */
typedef struct
{    
	EUART_TYPE_BCS  U7816_ClockSet;     //�����ʷ�����ʱ��ѡ��

	uint32_t  U7816_BaudRate;           //���䲨����

	TYPE_FUNCEN  U7816_ECK0;            // EUARTxMOD��bit2 7816ECK0ʹ��λ

	TYPE_FUNCEN  U7816_ECK1;            // EUARTxMOD��bit3 7816ECK1ʹ��λ

	U7816_TYPE_CHS  U7816_EIOCh;        // EUARTxMOD��bit4 EIOͨѶͨ��ѡ��λ

	U7816_TYPE_EIOC  U7816_EIODir;      // EUARTxMOD��bit5 EIO�˿ڷ������

	U7816_TYPE_DAS  U7816_DataForm;     // EUARTxMOD��bit6 ���ݸ�ʽѡ��λ

	U7816_TYPE_PS  U7816_DataVerify;    // EUARTxMOD��bit7 ������żУ��λ

	TYPE_FUNCEN  U7816_AutoRetryTX;     // EUARTxMOD��bit8 �Զ��ط�ʹ��λ

	TYPE_FUNCEN  U7816_AutoRetryRX;     // EUARTxMOD��bit9 �Զ�����ʹ��λ

	U7816_TYPE_TNAS  U7816_NACK_Width;  // EUARTxMOD��bit11~10 NACK�źſ��

	U7816_TYPE_ARTS  U7816_RetryTimes;  // EUARTxMOD��bit13~12 �Զ��ط�����

	U7816_TYPE_CKS  U7816_CLKS;         // EUARTxMOD��bit15~14 ʱ��Դѡ��

	uint8_t  U7816_ETUTime;             // EUARTxMOD��bit23~16 ETU����ʱ��ѡ�� 2~257 ��ETU		

}U7816_InitStruType;


/*************EUARTģ��궨��************/

/* ����ʹ�ܿ��� */
#define EUART0_TxEnable()  (EUART0->CON.TXEN = 1)
#define EUART1_TxEnable()  (EUART1->CON.TXEN = 1)

#define EUART0_TxDisable() (EUART0->CON.TXEN = 0)
#define EUART1_TxDisable() (EUART1->CON.TXEN = 0)


/* ����ʹ�ܿ��� */
#define EUART0_RxEnable()  (EUART0->CON.RXEN = 1)
#define EUART1_RxEnable()  (EUART1->CON.RXEN = 1)

#define EUART0_RxDisable() (EUART0->CON.RXEN = 0)
#define EUART1_RxDisable() (EUART1->CON.RXEN = 0)


/* ��������λ */
#define EUART0_TxRst() (EUART0->CON.TRST = 1)
#define EUART1_TxRst() (EUART1->CON.TRST = 1)

/* ��������λ */
#define EUART0_RxRst() (EUART0->CON.RRST = 1)
#define EUART1_RxRst() (EUART1->CON.RRST = 1)

/* U7816 �����λ */
#define EUART0_U7816_REST() ( EUART0->MOD.ERST = 1)
#define EUART1_U7816_REST() ( EUART1->MOD.ERST = 1)

/************EUARTģ�麯������***********/

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
