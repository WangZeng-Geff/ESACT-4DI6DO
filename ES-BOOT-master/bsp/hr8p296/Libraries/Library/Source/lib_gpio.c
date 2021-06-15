#include "lib_gpio.h"

/***************************************************************
  ��������GPIO_Init
  ��  ����GPIO��ʼ��
  ����ֵ��GPIOx��������GPIOA/GPIOB/GPIOC �� PINx������оƬѡ����Ҫ������ �� GPIO_InitStruct����ʼ�����ýṹ���ַ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void GPIO_Init(GPIO_TYPE GPIOx,GPIO_TYPE_PIN PINx, GPIO_InitStruType* GPIO_InitStruct)
{
	if(GPIOx == GPIOA)
	{
		if(PINx/8 == 0)
		{
			GPIO->PAFUN0.Word &=~ ((uint32_t)0x3 << (PINx*4));
			GPIO->PAFUN0.Word |= ((uint32_t)GPIO_InitStruct->GPIO_Func << (PINx*4));

		}
		else if(PINx/8 == 1)
		{
			GPIO->PAFUN1.Word &=~ ((uint32_t)0x3 << (PINx%8*4));
			GPIO->PAFUN1.Word |= ((uint32_t)GPIO_InitStruct->GPIO_Func << (PINx%8*4));
		}
		else if(PINx/8 == 2)
		{
			GPIO->PAFUN2.Word &=~ ((uint32_t)0x3 << (PINx%8*4));
			GPIO->PAFUN2.Word |= ((uint32_t)GPIO_InitStruct->GPIO_Func << (PINx%8*4));
		}
		else 
		{
			GPIO->PAFUN3.Word &=~ ((uint32_t)0x3 << (PINx%8*4));
			GPIO->PAFUN3.Word |= ((uint32_t)GPIO_InitStruct->GPIO_Func << (PINx%8*4)) ; //����ѡ��
		}


		GPIO->PADIR.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PADIR.Word |= ((uint32_t)GPIO_InitStruct->GPIO_Direction <<PINx);     //����ѡ��

		GPIO->PAPUEN.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PAPUEN.Word |= ((uint32_t)GPIO_InitStruct->GPIO_PUEN <<PINx);         //������������ʹ��  


		GPIO->PAPDEN.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PAPDEN.Word |= ((uint32_t)GPIO_InitStruct->GPIO_PDEN <<PINx);   //������������ʹ�� 


		GPIO->PAOD.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PAOD.Word |= ((uint32_t)GPIO_InitStruct->GPIO_OD <<PINx);      //���ÿ�©���


	}
	else if(GPIOx == GPIOB)
	{
		if(PINx/8 == 0)
		{
			GPIO->PBFUN0.Word &=~ ((uint32_t)0x3 << (PINx*4));
			GPIO->PBFUN0.Word |= ((uint32_t)GPIO_InitStruct->GPIO_Func << (PINx*4));
		}
		else if(PINx/8 == 1)
		{
			GPIO->PBFUN1.Word &=~ ((uint32_t)0x3 << (PINx%8*4));
			GPIO->PBFUN1.Word |= ((uint32_t)GPIO_InitStruct->GPIO_Func << (PINx%8*4));
		}
		else 
		{
			GPIO->PBFUN2.Word &=~ ((uint32_t)0x3 << (PINx%8*4));
			GPIO->PBFUN2.Word |= ((uint32_t)GPIO_InitStruct->GPIO_Func << (PINx%8*4));                                                              
		}

		GPIO->PBDIR.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PBDIR.Word |= ((uint32_t)GPIO_InitStruct->GPIO_Direction <<PINx);     //����ѡ��

		GPIO->PBPUEN.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PBPUEN.Word |= ((uint32_t)GPIO_InitStruct->GPIO_PUEN <<PINx);         //������������ʹ��  


		GPIO->PBPDEN.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PBPDEN.Word |= ((uint32_t)GPIO_InitStruct->GPIO_PDEN <<PINx);   //������������ʹ�� 


		GPIO->PBOD.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PBOD.Word |= ((uint32_t)GPIO_InitStruct->GPIO_OD <<PINx);      //���ÿ�©���


	}
	else
	{
		if(PINx/8 == 0)
		{
			GPIO->PCFUN0.Word &=~ ((uint32_t)0x3 << (PINx*4));
			GPIO->PCFUN0.Word |= ((uint32_t)GPIO_InitStruct->GPIO_Func << (PINx*4));
		}
		else
		{
			GPIO->PCFUN1.Word &=~ ((uint32_t)0x3 << (PINx%8*4));
			GPIO->PCFUN1.Word |= ((uint32_t)GPIO_InitStruct->GPIO_Func << (PINx%8*4));
		}


		GPIO->PCDIR.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PCDIR.Word |= ((uint32_t)GPIO_InitStruct->GPIO_Direction <<PINx);     //����ѡ��

		GPIO->PCPUEN.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PCPUEN.Word |= ((uint32_t)GPIO_InitStruct->GPIO_PUEN <<PINx);         //������������ʹ��  


		GPIO->PCPDEN.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PCPDEN.Word |= ((uint32_t)GPIO_InitStruct->GPIO_PDEN <<PINx);   //������������ʹ�� 


		GPIO->PCOD.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PCOD.Word |= ((uint32_t)GPIO_InitStruct->GPIO_OD <<PINx);      //���ÿ�©���

	}

}


/***************************************************************
  ��������GPIO_Write
  ��  ����GPIO�˿�д����
  ����ֵ��GPIOx��������GPIOA/GPIOB/GPIOC �� ValueҪд�����ݡ���ע�⣺��Щ�����ڵ����ţ����õ�ֵ���Ӧ��λ�������õ�
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void GPIO_Write(GPIO_TYPE GPIOx, uint32_t Value)
{
	if(GPIOx == GPIOA)
		GPIO->PA.Word = Value;
	else if(GPIOx == GPIOB)
		GPIO->PB.Word = Value;
	else
		GPIO->PC.Word = Value;			
}


/***************************************************************
  ��������GPIO_Read
  ��  ����GPIO�˿ڶ�����
  ����ֵ��GPIOx��������GPIOA/GPIOB/GPIOC
  ���ֵ����
  ����ֵ�����������ݡ���ע�⣺��Щ�����ڵ����ţ�������ֵ���Ӧ��λ����Ч��
 ***************************************************************/
uint32_t GPIO_Read(GPIO_TYPE GPIOx)
{
	if(GPIOx == GPIOA)
		return	GPIO->PA.Word;
	else if(GPIOx == GPIOB)
		return GPIO->PB.Word;
	else
		return GPIO->PC.Word;			
}

/***************************************************************
  ��������GPIO_ReadBit
  ��  ����GPIO�˿ڶ�ĳλ����
  ����ֵ��GPIOx��������GPIOA/GPIOB/GPIOC �� PINx:GPIO_Pin_0 ���� GPIO_Pin_31
  ���ֵ����
  ����ֵ�����������ݡ���ע�⣺��Щ�����ڵ����ţ�������ֵ����Ч��
 ***************************************************************/
PinStatus GPIO_ReadBit(GPIO_TYPE GPIOx,GPIO_TYPE_PIN PINx)
{
	PinStatus bitstatus = RESET;
	if(GPIOx == GPIOA)
	{
		if((GPIO->PA.Word&((uint32_t)0x1<<PINx)) != RESET)
			bitstatus = SET;
		else
			bitstatus = RESET;
	}
	else if(GPIOx == GPIOB)
	{
		if((GPIO->PB.Word&((uint32_t)0x1<<PINx)) != RESET)
			bitstatus = SET;
		else
			bitstatus = RESET;
	}
	else 
	{
		if((GPIO->PC.Word&((uint32_t)0x1<<PINx)) != RESET)
			bitstatus = SET;
		else
			bitstatus = RESET;
	}
	return bitstatus;
}

/***************************************************************
  ��������GPIOA_SetBit
  ��  ����GPIOAĳ������1
  ����ֵ��PINx��������GPIO_Pin_0 ���� GPIO_Pin_23
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void GPIOA_SetBit(GPIO_TYPE_PIN PINx)
{
	GPIO->PAS.Word = (uint32_t)0x1<<PINx;
}

/***************************************************************
  ��������GPIOA_ResetBit
  ��  ����GPIOAĳ������0
  ����ֵ��PINx��������GPIO_Pin_0 ���� GPIO_Pin_23
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void GPIOA_ResetBit(GPIO_TYPE_PIN PINx)
{
	GPIO->PAC.Word = (uint32_t)0x1<<PINx;
}

/***************************************************************
  ��������GPIOA_ToggleBit
  ��  ����GPIOAĳ�������״̬ȡ��
  ����ֵ��PINx��������GPIO_Pin_0 ���� GPIO_Pin_23
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void GPIOA_ToggleBit(GPIO_TYPE_PIN PINx)
{


	GPIO->PAI.Word = (uint32_t)0x1<<PINx;                            //b_GPIO_PAI(PINx) = 1;
}

/***************************************************************
  ��������GPIOB_SetBit
  ��  ����GPIOBĳ������1
  ����ֵ��PINx��������GPIO_Pin_0 ���� GPIO_Pin_31
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void GPIOB_SetBit(GPIO_TYPE_PIN PINx)
{
	GPIO->PBS.Word = (uint32_t)0x1<<PINx;
}

/***************************************************************
  ��������GPIOB_ResetBit
  ��  ����GPIOBĳ������0
  ����ֵ��PINx��������GPIO_Pin_0 ���� GPIO_Pin_31
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void GPIOB_ResetBit(GPIO_TYPE_PIN PINx)
{
	GPIO->PBC.Word = (uint32_t)0x1<<PINx;
}

/***************************************************************
  ��������GPIOB_ToggleBit
  ��  ����GPIOBĳ�������״̬ȡ��
  ����ֵ��PINx��������GPIO_Pin_0 ���� GPIO_Pin_31
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void GPIOB_ToggleBit(GPIO_TYPE_PIN PINx)
{

	GPIO->PBI.Word = (uint32_t)0x1<<PINx; 
}


/***************************************************************
  ��������GPIOC_SetBit
  ��  ����GPIOCĳ������1
  ����ֵ��PINx��������GPIO_Pin_0 ���� GPIO_Pin_16
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void GPIOC_SetBit(GPIO_TYPE_PIN PINx)
{
	GPIO->PCS.Word = (uint32_t)0x1<<PINx;
}

/***************************************************************
  ��������GPIOC_ResetBit
  ��  ����GPIOCĳ������0
  ����ֵ��PINx��������GPIO_Pin_0 ���� GPIO_Pin_16
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void GPIOC_ResetBit(GPIO_TYPE_PIN PINx)
{
	GPIO->PCC.Word = (uint32_t)0x1<<PINx;
}

/***************************************************************
  ��������GPIOC_ToggleBit
  ��  ����GPIOCĳ�������״̬ȡ��
  ����ֵ��PINx��������GPIO_Pin_0 ���� GPIO_Pin_16
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void GPIOC_ToggleBit(GPIO_TYPE_PIN PINx)
{

	GPIO->PCI.Word = (uint32_t)0x1<<PINx; 
}

/***************************************************************
  ��������GPIOA_SetDirection
  ��  ����GPIOAĳ�������÷���
  ����ֵ��PINx��������GPIO_Pin_0 ���� GPIO_Pin_23 �� Dir_Type��GPIO_Dir_Out/GPIO_Dir_In
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void GPIOA_SetDirection(GPIO_TYPE_PIN PINx, GPIO_TYPE_DIR Dir_Type)
{
	GPIO->PADIR.Word &=~((uint32_t)0x1 << PINx);
	GPIO->PADIR.Word |= ((uint32_t)Dir_Type <<PINx);
}

/***************************************************************
  ��������GPIOB_SetDirection
  ��  ����GPIOBĳ�������÷���
  ����ֵ��PINx��������GPIO_Pin_0 ���� GPIO_Pin_31 �� Dir_Type��GPIO_Dir_Out/GPIO_Dir_In
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void GPIOB_SetDirection(GPIO_TYPE_PIN PINx, GPIO_TYPE_DIR Dir_Type)
{
	GPIO->PBDIR.Word &=~((uint32_t)0x1 << PINx);
	GPIO->PBDIR.Word |= ((uint32_t)Dir_Type <<PINx);
}

/***************************************************************
  ��������GPIOC_SetDirection
  ��  ����GPIOCĳ�������÷���
  ����ֵ��PINx��������GPIO_Pin_0 ���� GPIO_Pin_16 �� Dir_Type��GPIO_Dir_Out/GPIO_Dir_In
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void GPIOC_SetDirection(GPIO_TYPE_PIN PINx, GPIO_TYPE_DIR Dir_Type)
{
	GPIO->PCDIR.Word &=~((uint32_t)0x1 << PINx);
	GPIO->PCDIR.Word |= ((uint32_t)Dir_Type <<PINx);
}

/***************************************************************
  ��������PINT_Config
  ��  ����PINT����
  ����ֵ��PINTx����ѡPINT0 ���� PINT7 �� SELx������ѡ�� �� TRIGx������ѡ��
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void PINT_Config(PINT_TYPE PINTx, PINT_TYPE_SEL SELx, PINT_TYPE_TRIG TRIGx)
{
	GPIO->PSEL.Word &= ~((uint32_t)0x07<<(PINTx*4));
	GPIO->PSEL.Word |=((uint32_t)SELx << (PINTx*4));   //selѡ��

	GPIO->PCFG.Word &= ~((uint32_t)0x07<<(PINTx*4));	
	GPIO->PCFG.Word |=((uint32_t)TRIGx << (PINTx*4));  //������ʽѡ��
}



/***************************************************************
  ��������PINT_GetITStatus
  ��  ����PINT��ȡ�жϱ�־
  ����ֵ��PINTx��PINT0-PINT7
  ���ֵ����
  ����ֵ��SET/RESET
 ***************************************************************/
FlagStatus  PINT_GetITStatus(PINT_TYPE_IT PINT_Flag)
{
	FlagStatus bitstatus = RESET;

	if((GPIO->PIF.Word & (uint32_t)PINT_Flag) != (uint32_t)RESET)
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
  ��������PINT_ClearITPendingBit
  ��  ����PINT����жϱ�־
  ����ֵ��PINT�ж�����
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void PINT_ClearITPendingBit(PINT_TYPE_IT PINT_Flag)
{
	GPIO->PIF.Word = (uint32_t)PINT_Flag;
}

/*************************END OF FILE**********************/



