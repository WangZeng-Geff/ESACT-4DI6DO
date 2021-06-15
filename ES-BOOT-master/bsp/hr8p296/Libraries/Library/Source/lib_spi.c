/***************************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
 *�ļ�����  lib_spi.c
 *��  �ߣ�  WangBJ
 *��  ����  V1.00
 *��  �ڣ�  2014/10/24
 *��  ����  SPIģ��⺯��
 *��  ע��  ������ HR8P296оƬ
 ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 ***************************************************************/

#include "lib_spi.h"
#include "system_HR8P296.h"
/***************************************************************
  ��������SPI0_Init
  ��  ����SPI��ʼ��
  ����ֵ����ʼ�����ýṹ���ַ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void SPI_Init(SPI_InitStruType* SPI_InitStruct)
{
	uint32_t Temp = 0;
	Temp = SystemCoreClock/(SPI_InitStruct->SPI_Freq*2);
	if(Temp>254) 
	{
		Temp = 255;
	}
	SPI->CON.CKS = Temp;
	SPI->CON.DFS = SPI_InitStruct->SPI_Df;
	SPI->CON.MS = SPI_InitStruct->SPI_Mode;
	SPI->CON.DRE = SPI_InitStruct->SPI_DelayRec;
	SPI->CON.TME = SPI_InitStruct->SPI_DelaySend;
	Temp = (uint32_t)SPI_InitStruct->SPI_SendDelayPeroid-1;
	if(Temp>62) 
	{
		Temp=63;
	}
	SPI->CON.TMP = Temp;

}

/***************************************************************
  ��������SPI0_ITConfig
  ��  ����SPI�ж�����
  ����ֵ��SPI_IE���ж����� �� NewState��ʹ�ܡ�ʧ��
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void SPI_ITConfig(SPI_TYPE_IT SPI_IE,TYPE_FUNCEN NewState)
{
	if(NewState != DISABLE)
	{
		SPI->IE.Word |= (uint32_t)SPI_IE;
	}
	else
	{
		SPI->IE.Word &= ~((uint32_t)SPI_IE);
	}

}

/***************************************************************
  ��������SPI0_DataFormatConfig
  ��  ����SPI���ݸ�ʽ����
  ����ֵ�����ݸ�ʽ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void SPI_DataFormatConfig(SPI_TYPE_DFS Type)
{
	SPI->CON.DFS = Type;
}

/***************************************************************
  ��������SPI0_SendByte
  ��  ����SPI����һ���ֽڣ�8λ��
  ����ֵ��Ҫ���͵�����
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void SPI_SendByte(uint8_t Temp)
{
	SPI->TBW.Byte[0] = Temp;
}

/***************************************************************
  ��������SPI0_SendHalfWord
  ��  ����SPI����һ�����֣�16λ��
  ����ֵ��Ҫ���͵�����
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void SPI_SendHalfWord(uint16_t Temp)
{
	SPI->TBW.HalfWord[0] = Temp;
}

/***************************************************************
  ��������SPI0_SendWord
  ��  ����SPI����һ���֣�32λ��
  ����ֵ��Ҫ���͵�����
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void SPI_SendWord(uint32_t Temp)
{
	SPI->TBW.Word = Temp;
}

/***************************************************************
  ��������SPI0_RecByte
  ��  ����SPI����һ���ֽ�
  ����ֵ����
  ���ֵ����
  ����ֵ�����յ�����
 ***************************************************************/
uint8_t SPI_RecByte(void)
{
	return SPI->RBR.Byte[0];
}

/***************************************************************
  ��������SPI0_RecHalfWord
  ��  ����SPI����һ������
  ����ֵ����
  ���ֵ����
  ����ֵ�����յ�����
 ***************************************************************/
uint16_t SPI_RecHalfWord(void)
{
	return SPI->RBR.HalfWord[0];
}

/***************************************************************
  ��������SPI0_RecWord
  ��  ����SPI����һ����
  ����ֵ����
  ���ֵ����
  ����ֵ�����յ�����
 ***************************************************************/
uint32_t SPI_RecWord(void)
{
	return SPI->RBR.Word;
}

/***************************************************************
  ��������SPI0_TBIMConfig
  ��  ����SPI���ͻ��������ж�ģʽѡ��
  ����ֵ�����ж�ģʽ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void SPI_TBIMConfig(SPI_TYPE_TRBIM Type)
{
	SPI->IE.TBIM = Type;
}

/***************************************************************
  ��������SPI0_RBIMConfig
  ��  ����SPI���ջ��������ж�ģʽѡ��
  ����ֵ�����ж�ģʽ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void SPI_RBIMConfig(SPI_TYPE_TRBIM Type)
{
	SPI->IE.RBIM = Type;
}

/***************************************************************
  ��������SPI0_GetFlagStatus
  ��  ����SPI����־λ״̬
  ����ֵ����־λ
  ���ֵ����
  ����ֵ��SET/RESET
 ***************************************************************/
FlagStatus SPI_GetFlagStatus(SPI_TYPE_FLAG Flag)
{
	FlagStatus bitstatus = RESET;
	/* ��ȡ�Ĵ��� */
	if((uint32_t)Flag <= 0x00000040 )
	{
		if ((SPI->IF.Word & Flag) != (uint32_t)RESET)
		{
			bitstatus = SET;
		}
		else
		{
			bitstatus = RESET;
		}
	}

	else
	{
		if ((SPI->STA .Word & Flag) != (uint32_t)RESET)
		{
			bitstatus = SET;
		}
		else
		{
			bitstatus = RESET;
		}
	}

	return  bitstatus;
}

/***************************************************************
  ��������SPI0_GetITStatus
  ��  ����SPI����ж�״̬,δʹ����Ӧ�ж�ʱ���᷵��SET
  ����ֵ���жϱ�־λ
  ���ֵ����
  ����ֵ��SET/RESET
 ***************************************************************/
ITStatus SPI_GetITStatus(SPI_TYPE_IT Flag)
{
	FlagStatus bitstatus = RESET;
	uint32_t itstatus = 0x0, itenable = 0x0;

	itstatus = SPI->IF.Word & (uint32_t)Flag;
	itenable = SPI->IE.Word & (uint32_t)Flag;
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
  ��������SPI0_ClearITPendingBit
  ��  ����SPI�жϱ�־���
  ����ֵ����־λ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void SPI_ClearITPendingBit(SPI_CLR_IF Flag)
{
	SPI->IF.Word = (uint32_t)Flag;
}

/*************************END OF FILE**********************/
