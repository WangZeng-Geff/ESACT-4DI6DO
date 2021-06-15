/***************************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
 *�ļ�����  lib_iic.c
 *��  �ߣ�  JiangYz
 *��  ����  V1.00
 *��  �ڣ�  2016/02/03
 *��  ����  IICģ��⺯��
 *��  ע��  ������ HR8P296оƬ
 ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 ***************************************************************/

#include "lib_iiC.h"
#include "System_HR8P296.h"
/***************************************************************
  ��������I2C_Init
  ��  ����I2C��ʼ��
  ����ֵ��I2C_InitStruct����ʼ�����ýṹ���ַ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void I2C_Init(I2C_InitStruType* I2C_InitStruct)
{
	uint32_t temp;
	if(I2C_InitStruct->I2C_Mode == I2C_Mode_Master)
	{
		if(I2C_InitStruct->I2C_16XSamp != DISABLE)
		{
			temp = SystemCoreClock/24/I2C_InitStruct->I2C_Clk - 1;
		}
		else
		{
			temp = SystemCoreClock/16/I2C_InitStruct->I2C_Clk - 1;
		}
		if(temp > 254) temp = 255;

		I2C->CON.TJP = temp;      //����ʱ����ʱ������

		I2C->CON.TJE = 0x1;       //ʹ��ʱ����ʱ��
	}

	I2C->CON.SDASE = I2C_InitStruct->I2C_16XSamp;  //16����������
	I2C->CON.SCLSE = I2C_InitStruct->I2C_16XSamp; 
	I2C->CON.SCLOD = I2C_InitStruct->I2C_SclOd;    //�˿ڿ�©����
	I2C->CON.SDAOD = I2C_InitStruct->I2C_SdaOd;
	I2C->MOD.MS = I2C_InitStruct->I2C_Mode;       //����ģʽ����
	I2C->MOD.SPAE = I2C_InitStruct->I2C_AutoStop; //�Զ�ֹͣ����
	I2C->MOD.SRAE = I2C_InitStruct->I2C_AutoCall; //�Զ�Ѱ������

}

/***************************************************************
  ��������I2C_ITConfig
  ��  ����I2C�ж�����
  ����ֵ��I2C_IT����Ҫ���õ��ж� �� NewState��ʹ�ܻ�ر�
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void I2C_ITConfig(I2C_TYPE_IT I2C_IT,TYPE_FUNCEN NewState)
{
	if (NewState != DISABLE)
	{
		I2C->IE.Word |= (uint32_t)I2C_IT;
	}
	else
	{
		I2C->IE.Word &= ~((uint32_t)I2C_IT);
	}
}


/***************************************************************
  ��������I2C_SendAddress
  ��  ����I2C���ʹӻ���ַ
  ����ֵ��I2C_Address��7λ�ӻ���ַ������� 0x00~0xfe �� Mode������д
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void I2C_SendAddress(uint8_t I2C_Address,I2C_TYPE_RWMODE Mode)
{
	I2C->CON.SA = I2C_Address>>1;
	I2C->CON.RW = Mode;
}

/***************************************************************
  ��������I2C_SetAddress
  ��  ����I2C���õ�ַ�������ڴӻ�ģʽ��
  ����ֵ��I2C_Address��7λ�ӻ���ַ������� 0x00~0xfe
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void I2C_SetAddress(uint8_t I2C_Address)
{
	I2C->CON.SA = I2C_Address>>1;
}

/***************************************************************
  ��������I2C_RecModeConfig
  ��  ����I2C���ý���ģʽ
  ����ֵ��RecType������ģʽ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void I2C_RecModeConfig(I2C_TYPE_RECMODE RecType)
{
	I2C->MOD.RDM = RecType;
}

/***************************************************************
  ��������I2C_TBIMConfig
  ��  ����I2C���ͻ��������ж�ģʽѡ��
  ����ֵ��Type�����ж�ģʽ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void I2C_TBIMConfig(I2C_TYPE_TRBIM Type)
{
	I2C->IE.TBIM = Type;
}

/***************************************************************
  ��������I2C_RBIMConfig
  ��  ����I2C���ջ��������ж�ģʽѡ��
  ����ֵ��Type�����ж�ģʽ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void I2C_RBIMConfig(I2C_TYPE_TRBIM Type)
{
	I2C->IE.RBIM = Type;
}

/***************************************************************
  ��������I2C_AckDelay
  ��  ����I2CӦ����ʱ����
  ����ֵ��Type����ʱʱ�� �� NewStatus��ʹ�ܡ�ʧ��
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void I2C_AckDelay(I2C_TYPE_ADLY Type,TYPE_FUNCEN NewStatus)
{
	I2C->MOD.ADLY = Type;
	I2C->MOD.ADE = NewStatus;
}

/***************************************************************
  ��������I2C_TISConfig
  ��  ����I2C����֡����������
  ����ֵ��Time��������
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void I2C_TISConfig(I2C_TYPE_TIS Time)
{
	I2C->MOD.TIS = Time;
}

/***************************************************************
  ��������I2C_SendByte
  ��  ����I2C�ֽڷ���
  ����ֵ��Byte���ֽ�����
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void I2C_SendByte(uint8_t Byte)
{
	I2C->TBW.Byte[0] = Byte;
}

/***************************************************************
  ��������I2C_SendHalfWord
  ��  ����I2C���ַ���
  ����ֵ��HalfWord���������
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void I2C_SendHalfWord(uint16_t HalfWord)
{
	I2C->TBW.HalfWord[0] = HalfWord;
}

/***************************************************************
  ��������I2C_SendWord
  ��  ����I2C�ַ���
  ����ֵ��Word��������
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void I2C_SendWord(uint32_t Word)
{
	I2C->TBW.Word = Word;
}

/***************************************************************
  ��������I2C_RecByte
  ��  ����I2C����-�ֽ�
  ����ֵ����
  ���ֵ����
  ����ֵ�����յ�����
 ***************************************************************/
uint8_t I2C_RecByte(void)
{
	return I2C->RBR.Byte[0] ;
}

/***************************************************************
  ��������I2C_RecHalfWord
  ��  ����I2C����-����
  ����ֵ����
  ���ֵ����
  ����ֵ�����յ�����
 ***************************************************************/
uint16_t I2C_RecHalfWord(void)
{
	return I2C->RBR.HalfWord[0];
}

/***************************************************************
  ��������I2C_RecWord
  ��  ����I2C����-��
  ����ֵ����
  ���ֵ����
  ����ֵ�����յ�����
 ***************************************************************/
uint32_t I2C_RecWord(void)
{
	return I2C->RBR.Word ;
}

/***************************************************************
  ��������I2C_GetRWMode
  ��  ����I2C������д״̬��ȡ
  ����ֵ����
  ���ֵ����
  ����ֵ����/д
 ***************************************************************/
I2C_TYPE_RWMODE I2C_GetRWMode(void)
{
	return (I2C->CON.RW==0 ? I2C_Mode_Write : I2C_Mode_Read);
}

/***************************************************************
  ��������I2C_GetTBStatus
  ��  ����I2C��ȡ���ͻ���Ĵ���״̬,TB0-TB3ȫ���򷵻�SET,���򷵻�RESET
  ����ֵ����
  ���ֵ����
  ����ֵ��SET/RESET
 ***************************************************************/
FlagStatus I2C_GetTBStatus(void)
{
	FlagStatus bitstatus = RESET;
	if((I2C->STA.Word&(uint32_t)0x0f00) != (uint32_t)0x0f00)
	{
		bitstatus = RESET;
	}
	else
	{
		bitstatus = SET;
	}
	return  bitstatus;
}

/***************************************************************
  ��������I2C_GetFlagStatus
  ��  ����I2C��ȡ��־λ״̬
  ����ֵ��I2C_Flag��ѡ����Ҫ�ı�־λ
  ���ֵ����
  ����ֵ��SET/RESET
 ***************************************************************/
FlagStatus I2C_GetFlagStatus(I2C_TYPE_FLAG I2C_Flag)
{
	return ( (I2C->IF.Word & (uint32_t)I2C_Flag)? SET : RESET );
}

/*************************************
  ��������I2C_GetITStatus
  ��  ������ȡ�ж�ʹ��״̬
  ����ֵ��I2C_Flag �ж�ʹ��λ
  �ж�����
  ����ֵ��SET/RESET
 **************************************/
FlagStatus I2C_GetITStatus( I2C_TYPE_IT I2C_Flag)
{
	FlagStatus status = RESET;

	if ((I2C->IE.Word & (uint32_t)I2C_Flag) == RESET)
		return status;
    
    if(I2C_Flag == 0x01000)
    {
        if ((I2C->IF.Word & 0x00000800) != RESET)
        {
            status = SET;
            return status;
        }
    }
    
	if ((I2C->IF.Word & (uint32_t)I2C_Flag) != RESET)
		status = SET;

	return status;
}

/***************************************************************
  ��������I2C_ClearITPendingBit
  ��  ����I2C�жϱ�־���
  ����ֵ��I2C_IT����־λ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void I2C_ClearITPendingBit(I2C_CLR_IF I2C_IT)
{
	I2C->IF.Word = (uint32_t)I2C_IT;

}
