#include "lib_gpio.h"

/***************************************************************
  函数名：GPIO_Init
  描  述：GPIO初始化
  输入值：GPIOx：可以是GPIOA/GPIOB/GPIOC 、 PINx：根据芯片选择需要的引脚 、 GPIO_InitStruct：初始化配置结构体地址
  输出值：无
  返回值：无
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
			GPIO->PAFUN3.Word |= ((uint32_t)GPIO_InitStruct->GPIO_Func << (PINx%8*4)) ; //功能选择
		}


		GPIO->PADIR.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PADIR.Word |= ((uint32_t)GPIO_InitStruct->GPIO_Direction <<PINx);     //方向选择

		GPIO->PAPUEN.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PAPUEN.Word |= ((uint32_t)GPIO_InitStruct->GPIO_PUEN <<PINx);         //配置输入上拉使能  


		GPIO->PAPDEN.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PAPDEN.Word |= ((uint32_t)GPIO_InitStruct->GPIO_PDEN <<PINx);   //配置输入下拉使能 


		GPIO->PAOD.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PAOD.Word |= ((uint32_t)GPIO_InitStruct->GPIO_OD <<PINx);      //配置开漏输出


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
		GPIO->PBDIR.Word |= ((uint32_t)GPIO_InitStruct->GPIO_Direction <<PINx);     //方向选择

		GPIO->PBPUEN.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PBPUEN.Word |= ((uint32_t)GPIO_InitStruct->GPIO_PUEN <<PINx);         //配置输入上拉使能  


		GPIO->PBPDEN.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PBPDEN.Word |= ((uint32_t)GPIO_InitStruct->GPIO_PDEN <<PINx);   //配置输入下拉使能 


		GPIO->PBOD.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PBOD.Word |= ((uint32_t)GPIO_InitStruct->GPIO_OD <<PINx);      //配置开漏输出


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
		GPIO->PCDIR.Word |= ((uint32_t)GPIO_InitStruct->GPIO_Direction <<PINx);     //方向选择

		GPIO->PCPUEN.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PCPUEN.Word |= ((uint32_t)GPIO_InitStruct->GPIO_PUEN <<PINx);         //配置输入上拉使能  


		GPIO->PCPDEN.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PCPDEN.Word |= ((uint32_t)GPIO_InitStruct->GPIO_PDEN <<PINx);   //配置输入下拉使能 


		GPIO->PCOD.Word &=~((uint32_t)0x1 << PINx);
		GPIO->PCOD.Word |= ((uint32_t)GPIO_InitStruct->GPIO_OD <<PINx);      //配置开漏输出

	}

}


/***************************************************************
  函数名：GPIO_Write
  描  述：GPIO端口写数据
  输入值：GPIOx：可以是GPIOA/GPIOB/GPIOC 、 Value要写的数据——注意：有些不存在的引脚，设置的值相对应的位是无作用的
  输出值：无
  返回值：无
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
  函数名：GPIO_Read
  描  述：GPIO端口读数据
  输入值：GPIOx：可以是GPIOA/GPIOB/GPIOC
  输出值：无
  返回值：读出的数据——注意：有些不存在的引脚，读出的值相对应的位是无效的
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
  函数名：GPIO_ReadBit
  描  述：GPIO端口读某位数据
  输入值：GPIOx：可以是GPIOA/GPIOB/GPIOC 、 PINx:GPIO_Pin_0 —— GPIO_Pin_31
  输出值：无
  返回值：读出的数据——注意：有些不存在的引脚，读出的值是无效的
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
  函数名：GPIOA_SetBit
  描  述：GPIOA某引脚置1
  输入值：PINx：可以是GPIO_Pin_0 —— GPIO_Pin_23
  输出值：无
  返回值：无
 ***************************************************************/
void GPIOA_SetBit(GPIO_TYPE_PIN PINx)
{
	GPIO->PAS.Word = (uint32_t)0x1<<PINx;
}

/***************************************************************
  函数名：GPIOA_ResetBit
  描  述：GPIOA某引脚清0
  输入值：PINx：可以是GPIO_Pin_0 —— GPIO_Pin_23
  输出值：无
  返回值：无
 ***************************************************************/
void GPIOA_ResetBit(GPIO_TYPE_PIN PINx)
{
	GPIO->PAC.Word = (uint32_t)0x1<<PINx;
}

/***************************************************************
  函数名：GPIOA_ToggleBit
  描  述：GPIOA某引脚输出状态取反
  输入值：PINx：可以是GPIO_Pin_0 —— GPIO_Pin_23
  输出值：无
  返回值：无
 ***************************************************************/
void GPIOA_ToggleBit(GPIO_TYPE_PIN PINx)
{


	GPIO->PAI.Word = (uint32_t)0x1<<PINx;                            //b_GPIO_PAI(PINx) = 1;
}

/***************************************************************
  函数名：GPIOB_SetBit
  描  述：GPIOB某引脚置1
  输入值：PINx：可以是GPIO_Pin_0 —— GPIO_Pin_31
  输出值：无
  返回值：无
 ***************************************************************/
void GPIOB_SetBit(GPIO_TYPE_PIN PINx)
{
	GPIO->PBS.Word = (uint32_t)0x1<<PINx;
}

/***************************************************************
  函数名：GPIOB_ResetBit
  描  述：GPIOB某引脚清0
  输入值：PINx：可以是GPIO_Pin_0 —— GPIO_Pin_31
  输出值：无
  返回值：无
 ***************************************************************/
void GPIOB_ResetBit(GPIO_TYPE_PIN PINx)
{
	GPIO->PBC.Word = (uint32_t)0x1<<PINx;
}

/***************************************************************
  函数名：GPIOB_ToggleBit
  描  述：GPIOB某引脚输出状态取反
  输入值：PINx：可以是GPIO_Pin_0 —— GPIO_Pin_31
  输出值：无
  返回值：无
 ***************************************************************/
void GPIOB_ToggleBit(GPIO_TYPE_PIN PINx)
{

	GPIO->PBI.Word = (uint32_t)0x1<<PINx; 
}


/***************************************************************
  函数名：GPIOC_SetBit
  描  述：GPIOC某引脚置1
  输入值：PINx：可以是GPIO_Pin_0 —— GPIO_Pin_16
  输出值：无
  返回值：无
 ***************************************************************/
void GPIOC_SetBit(GPIO_TYPE_PIN PINx)
{
	GPIO->PCS.Word = (uint32_t)0x1<<PINx;
}

/***************************************************************
  函数名：GPIOC_ResetBit
  描  述：GPIOC某引脚清0
  输入值：PINx：可以是GPIO_Pin_0 —— GPIO_Pin_16
  输出值：无
  返回值：无
 ***************************************************************/
void GPIOC_ResetBit(GPIO_TYPE_PIN PINx)
{
	GPIO->PCC.Word = (uint32_t)0x1<<PINx;
}

/***************************************************************
  函数名：GPIOC_ToggleBit
  描  述：GPIOC某引脚输出状态取反
  输入值：PINx：可以是GPIO_Pin_0 —— GPIO_Pin_16
  输出值：无
  返回值：无
 ***************************************************************/
void GPIOC_ToggleBit(GPIO_TYPE_PIN PINx)
{

	GPIO->PCI.Word = (uint32_t)0x1<<PINx; 
}

/***************************************************************
  函数名：GPIOA_SetDirection
  描  述：GPIOA某引脚设置方向
  输入值：PINx：可以是GPIO_Pin_0 —— GPIO_Pin_23 、 Dir_Type：GPIO_Dir_Out/GPIO_Dir_In
  输出值：无
  返回值：无
 ***************************************************************/
void GPIOA_SetDirection(GPIO_TYPE_PIN PINx, GPIO_TYPE_DIR Dir_Type)
{
	GPIO->PADIR.Word &=~((uint32_t)0x1 << PINx);
	GPIO->PADIR.Word |= ((uint32_t)Dir_Type <<PINx);
}

/***************************************************************
  函数名：GPIOB_SetDirection
  描  述：GPIOB某引脚设置方向
  输入值：PINx：可以是GPIO_Pin_0 —— GPIO_Pin_31 、 Dir_Type：GPIO_Dir_Out/GPIO_Dir_In
  输出值：无
  返回值：无
 ***************************************************************/
void GPIOB_SetDirection(GPIO_TYPE_PIN PINx, GPIO_TYPE_DIR Dir_Type)
{
	GPIO->PBDIR.Word &=~((uint32_t)0x1 << PINx);
	GPIO->PBDIR.Word |= ((uint32_t)Dir_Type <<PINx);
}

/***************************************************************
  函数名：GPIOC_SetDirection
  描  述：GPIOC某引脚设置方向
  输入值：PINx：可以是GPIO_Pin_0 —— GPIO_Pin_16 、 Dir_Type：GPIO_Dir_Out/GPIO_Dir_In
  输出值：无
  返回值：无
 ***************************************************************/
void GPIOC_SetDirection(GPIO_TYPE_PIN PINx, GPIO_TYPE_DIR Dir_Type)
{
	GPIO->PCDIR.Word &=~((uint32_t)0x1 << PINx);
	GPIO->PCDIR.Word |= ((uint32_t)Dir_Type <<PINx);
}

/***************************************************************
  函数名：PINT_Config
  描  述：PINT配置
  输入值：PINTx：可选PINT0 —— PINT7 、 SELx：输入选择 、 TRIGx：触发选择
  输出值：无
  返回值：无
 ***************************************************************/
void PINT_Config(PINT_TYPE PINTx, PINT_TYPE_SEL SELx, PINT_TYPE_TRIG TRIGx)
{
	GPIO->PSEL.Word &= ~((uint32_t)0x07<<(PINTx*4));
	GPIO->PSEL.Word |=((uint32_t)SELx << (PINTx*4));   //sel选择

	GPIO->PCFG.Word &= ~((uint32_t)0x07<<(PINTx*4));	
	GPIO->PCFG.Word |=((uint32_t)TRIGx << (PINTx*4));  //触发方式选择
}



/***************************************************************
  函数名：PINT_GetITStatus
  描  述：PINT读取中断标志
  输入值：PINTx：PINT0-PINT7
  输出值：无
  返回值：SET/RESET
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
  函数名：PINT_ClearITPendingBit
  描  述：PINT清除中断标志
  输入值：PINT中断类型
  输出值：无
  返回值：无
 ***************************************************************/
void PINT_ClearITPendingBit(PINT_TYPE_IT PINT_Flag)
{
	GPIO->PIF.Word = (uint32_t)PINT_Flag;
}

/*************************END OF FILE**********************/



