/***************************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
 *文件名：  lib_flashiap.c
 *作  者：  WangBJ
 *版  本：  V1.00
 *日  期：  2014/10/24
 *描  述：  flash读写库函数
 *备  注：  适用于 HR8P296芯片
 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
 ***************************************************************/

#include "HR8P296.h"
#include "lib_flashiap.h"

/***************************************************************
  函数名：FlashIap_Unlock
  描  述：IAP解锁与访问请求
  输入值：无
  输出值：无
  返回值：成功、失败
 ***************************************************************/
ErrorStatus FlashIap_Unlock(void)
{
	uint16_t  Temp16;

	FlashIAP_RegUnLock();              //解锁 IAP

	FlashIAP_Enable();                 //使能IAP

	FlashIAP_REQ();                    //访问请求

	//等待FLASH应答信号
	for(Temp16 = 0; Temp16 < 0xFFFF; Temp16++)      
	{
		if(IAP->CON.FLASH_ACK != 0) break;
	}
	if(Temp16 == 0xFFFF)
	{
		return  ERROR;
	}
	else
	{
		return  SUCCESS;
	}

}

/***************************************************************
  函数名：FlashIap_WriteEnd
  描  述：IAP写结束
  输入值：无
  输出值：无
  返回值：成功、失败
 ***************************************************************/
ErrorStatus FlashIap_WriteEnd(void)
{
	uint32_t  Temp32;

	FlashIAP_RegUnLock();                             //IAP解锁

	IAP->CON.Word &= 0xFFFFFFEE;             				//IAP访问FLASH请求(结束)

	for(Temp32 = 0; Temp32 < 0xFFFF; Temp32++)      //等待FLASH应答信号(结束)
	{
		if(IAP->CON.FLASH_ACK == 0) break;
	} 

	if(Temp32 == 0xFFFF)
	{
		return  ERROR;
	}
	else
	{
		return  SUCCESS;
	}        
}

/***************************************************************
  函数名：FlashIap_ErasePage
  描  述：IAP页擦出
  输入值：Page_Addr：页地址
  输出值：无
  返回值：成功、失败
 ***************************************************************/
ErrorStatus FlashIap_ErasePage(uint8_t Page_Addr)
{
	uint16_t  Temp16;
	uint32_t temp;

	temp = __get_PRIMASK();         		//获取PRIMASK寄存器当前状态
	__disable_irq(); //屏蔽所有中断

	if(FlashIap_Unlock() == ERROR)
	{
		__set_PRIMASK(temp);           	//恢复PRIMASK寄存器状态
		return  ERROR;
	}

	IAP->ADDR.IAPPA = Page_Addr;    		//输入页地址   

	IAP->TRIG.IAPT = 0x00005EA1;      	//输入编程命令 

	for(Temp16 = 0; Temp16 < 0xFFFF; Temp16++)  
	{                                  	//判断IAP工作状态
		if((IAP->STA.Word & (uint32_t)0x01) == (uint32_t)0x00) break; 
	}

	if(Temp16 == 0xFFFF)
	{
		__set_PRIMASK(temp);           	//恢复PRIMASK寄存器状态
		return  ERROR;
	}

	for(Temp16 = 0; Temp16 < 0xFFFF; Temp16++)  
	{                                 	//判断IAP页擦除标志
		if((IAP->STA.Word & (uint32_t)0x02) == (uint32_t)0x02) break;  
	}       

	if(Temp16 == 0xFFFF)
	{
		__set_PRIMASK(temp);           //恢复PRIMASK寄存器状态
		return  ERROR;
	}

	if(FlashIap_WriteEnd() == ERROR)
	{
		__set_PRIMASK(temp);           //恢复PRIMASK寄存器状态
		return  ERROR;
	}

	__set_PRIMASK(temp);           		//恢复PRIMASK寄存器状态

	return  SUCCESS;        
}  

/***************************************************************
  函数名：FlashIap_WriteCont
  描  述：IAP连续写
  输入值：Unit_addr：单元地址 、 Page_addr：页地址 、 Data32：数据
  输出值：无
  返回值：成功、失败
 ***************************************************************/
ErrorStatus FlashIap_WriteCont(uint8_t Unit_addr, uint8_t Page_addr, uint32_t Data32)
{
	uint16_t  temp16;

	IAP->ADDR.IAPPA = Page_addr;       //输入地址 
	IAP->ADDR.IAPCA = Unit_addr;

	IAP->DATA.IAPD    = Data32;          //输入数据
	IAP->TRIG.IAPT    = 0x00005DA2;      //输入编程命令 

	for(temp16 = 0; temp16 < 0xFFFF; temp16++)  
	{
		if((IAP->STA.Word & (uint32_t)0x01) == (uint32_t)0x00) break;  //判断IAP工作状态
	}

	if(temp16 == 0xFFFF)
	{
		return  ERROR;
	}


	for(temp16 = 0; temp16 < 0xFFFF; temp16++)  
	{
		if((IAP->STA.Word & 0x04)==0x04) break;  //判断IAP编程结束标志
	}       

	if(temp16 == 0xFFFF)
	{
		return  ERROR;
	}

	return  SUCCESS;
}

/***************************************************************
  函数名：FlashIap_WriteWord
  描  述：IAP写一个字
  输入值：Unit_addr：单元地址 、 Page_addr：页地址 、 Data32：数据
  输出值：无
  返回值：成功、失败
 ***************************************************************/
ErrorStatus FlashIap_WriteWord(uint8_t Unit_addr, uint8_t Page_addr, uint32_t Data32)
{
	uint32_t temp;

	temp = __get_PRIMASK();         		//获取PRIMASK寄存器当前状态
	__disable_irq(); //屏蔽所有中断

	if(FlashIap_Unlock() == ERROR)
	{
		__set_PRIMASK(temp);           	//恢复PRIMASK寄存器状态
		return  ERROR;	
	}		

	if(FlashIap_WriteCont(Unit_addr, Page_addr, Data32) == ERROR)
	{
		__set_PRIMASK(temp);           //恢复PRIMASK寄存器状态
		return  ERROR;
	}

	if(FlashIap_WriteEnd() == ERROR)
	{
		__set_PRIMASK(temp);           //恢复PRIMASK寄存器状态
		return  ERROR;
	}

	__set_PRIMASK(temp);           		//恢复PRIMASK寄存器状态
	return  SUCCESS;		

}

/***************************************************************
  函数名：Flash_Read
  描  述：Flash读数据
  输入值：Ram_Addr：读出数据的存放地址 、 Flash_Addr：Flash地址（0x00000000 ~ 0x0001FFFF）、 Len：读取的字长度
  输出值：读出的数据
  返回值：成功、失败
 ***************************************************************/
ErrorStatus Flash_Read(uint32_t * Ram_Addr, uint32_t Flash_Addr,  uint8_t Len)
{
	uint8_t  i;
	uint32_t *  ram_addr32;
	const uint32_t  *  flash_addr32;

	ram_addr32 = (uint32_t *)Ram_Addr;
	flash_addr32 = (const uint32_t  * )Flash_Addr;
	if((Len == 0)&(Len>(0x20000-Flash_Addr)/4))  //判断读取长度是否合法
	{
		return  ERROR;
	}
	for(i=0; i<Len; i++)
	{
		*ram_addr32 = *flash_addr32 ;
		ram_addr32++;
		flash_addr32++;
	}
	return  SUCCESS;
}

/*************************END OF FILE**********************/
