/***************************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
 *文件名：  lib_scu.c
 *作  者：  WangBJ
 *版  本：  V1.00
 *日  期：  2014/10/24
 *描  述：  系统控制模块库函数
 *备  注：  适用于 HR8P296芯片
 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
 ***************************************************************/

#include "lib_scu.h"
#include "lib_gpio.h"
#include "system_HR8P296.h"
/***************************************************************
  函数名：SCU_NMISelect
  描  述：设置NMI不可屏蔽中断
  输入值：不可屏蔽中断
  输出值：无
  返回值：无
 ***************************************************************/
void SCU_NMISelect(SCU_TYPE_NMICS NMI_Type)
{
	SCU->NMIC.NMICS = NMI_Type;
}

/***************************************************************
  函数名：SCU_GetPWRCFlagStatus
  描  述：获取PWRC复位状态寄存器标志位状态
  输入值：PWRC寄存器标志位
  输出值：无
  返回值：RESET/SET
 ***************************************************************/
FlagStatus  SCU_GetPWRCFlagStatus(SCU_TYPE_PWRC PWRC_Flag)
{
	FlagStatus bitstatus = RESET;
	if((SCU->PWRC.Word & (uint32_t)PWRC_Flag) != (uint32_t)RESET)
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
  函数名：SCU_ClearPWRCFlagBit
  描  述：清除PWRC复位状态寄存器标志位
  输入值：PWRC寄存器标志位
  输出值：无
  返回值：无
 ***************************************************************/
void SCU_ClearPWRCFlagBit(SCU_TYPE_PWRC PWRC_Flag)
{
	SCU->PWRC.Word &= ~((uint32_t)PWRC_Flag);
}

/***************************************************************
  函数名：SCU_GetLVDFlagStatus
  描  述：获取LVDD寄存器标志位状态
  输入值：LVD寄存器标志位
  输出值：无
  返回值：RESET/SET
 ***************************************************************/
FlagStatus  SCU_GetLVDFlagStatus(SCU_TYPE_LVD0CON LVD_Flag)
{
	FlagStatus bitstatus = RESET;
	if((SCU->LVDC.Word & (uint32_t)LVD_Flag) != (uint32_t)RESET)
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
  函数名：SCU_GetCFGWord
  描  述：获取芯片配置字
  输入值：无
  输出值：无
  返回值：32位配置字
 ***************************************************************/
uint32_t SCU_GetCFGWord(void)
{
	return (SCU->CFGWORD0.Word);
}

/***************************************************************
  函数名：SCU_SysClkSelect
  描  述：选择系统时钟
  输入值：时钟源
  输出值：无
  返回值：无
 ***************************************************************/
void SCU_SysClkSelect(SCU_TYPE_SYSCLK Sysclk)
{
	SCU->SCLKEN0.SYSCLK_SSE= Sysclk;
}

/***************************************************************
  函数名：SCU_GetSysClk
  描  述：获取系统时钟源
  输入值：无
  输出值：无
  返回值：系统时钟源
 ***************************************************************/
SCU_TYPE_SYSCLK SCU_GetSysClk(void)
{
	return (SCU_TYPE_SYSCLK)(SCU->SCLKEN0.SYSCLK_SST);
}

/***************************************************************
  函数名：SCU_SysClkChangeBusy
  描  述：获取系统时钟切换标志位
  输入值：无
  输出值：无
  返回值：RESET/SET
 ***************************************************************/
FlagStatus SCU_SysClkChangeBusy(void)
{
	FlagStatus bitstatus = RESET;
	if((SCU->SCLKEN0.SYSCLK_CHG) != (uint32_t)RESET)
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
  函数名：SCU_GetHRC1Flag
  描  述：获取HRC工作标志位
  输入值：无
  输出值：无
  返回值：RESET/SET
 ***************************************************************/
FlagStatus  SCU_GetHRC1Flag(void)
{
	FlagStatus bitstatus = RESET;
	if((SCU->SCLKEN0.HRC_ON) != (uint32_t)RESET)
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
  函数名：SCU_GetLRCFlag
  描  述：获取LRC工作标志位
  输入值：无
  输出值：无
  返回值：RESET/SET
 ***************************************************************/
FlagStatus  SCU_GetLRCFlag(void)
{
	FlagStatus bitstatus = RESET;
	if((SCU->SCLKEN0.LRC_ON) != (uint32_t)RESET)
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
  函数名：SCU_HOSCReadyFlag
  描  述：获取HOSC稳定标志位
  输入值：无
  输出值：无
  返回值：RESET（不稳定）/SET（稳定）
 ***************************************************************/
FlagStatus  SCU_HOSCReadyFlag(void)
{
	FlagStatus bitstatus = RESET;
	if((SCU->SCLKEN1.HOSC_RDY) != (uint32_t)RESET)
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
  函数名：SCU_LOSCReadyFlag
  描  述：获取LOSC稳定标志位
  输入值：无
  输出值：无
  返回值：RESET（不稳定）/SET（稳定）
 ***************************************************************/
FlagStatus  SCU_LOSCReadyFlag(void)
{
	FlagStatus bitstatus = RESET;
	if((SCU->SCLKEN1.LOSC_RDY) != (uint32_t)RESET)
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
  函数名：SCU_LOSCReadyFlag
  描  述：获取LOSC稳定标志位
  输入值：无
  输出值：无
  返回值：RESET（不稳定）/SET（稳定）
 ***************************************************************/
FlagStatus  SCU_PLLReadyFlag(void)
{
	FlagStatus bitstatus = RESET;
	if((SCU->SCLKEN1.PLL_RDY) != (uint32_t)RESET)
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
  函数名：SystemClockConfig
  描  述：系统时钟配置：内部时钟，16MHZ，打开所有外设时钟
  输入值：无
  输出值：无
  返回值：无
 ***************************************************************/
void SystemClockConfig(void)
{
	uint32_t Prot_Temp;

	Prot_Temp = SCU->PROT.PROT;
	if(Prot_Temp != 0)   //写保护了
	{SCU_RegUnLock();}  //解锁
	SCU_HRC_Enable();                      //使能内部16MHZ
	while(SCU_GetHRC1Flag() != SET);      //等待时钟开启
	SCU_SysClkSelect(SCU_SysClk_HRC);    //选择内部16MHZ为系统时钟
	while(SCU_SysClkChangeBusy() != RESET); //等待切换完成
	SCU_SysClk_Div1();                        //系统时钟后分频1:1
	SCU_ADC32K_RtcOsc();                      //ADC、LCDC、WDT 32K时钟选择RTCOSC
	SCU_LCD32K_RtcOsc();
	SCU_WDT32K_RtcOsc();
	SystemCoreClock = 16000000;
	if(Prot_Temp != 0)   //写保护了
	{SCU_RegLock();}   //打开写保护
}

/***************************************************************
  函数名：DeviceClockAllEnable
  描  述：打开所有外设时钟
  输入值：无
  输出值：无
  返回值：无
 ***************************************************************/
void DeviceClockAllEnable(void)
{
	uint32_t Prot_Temp;

	Prot_Temp = SCU->PROT.PROT;
	if(Prot_Temp != 0)   //写保护了
	{SCU_RegUnLock();}  //解锁

	SCU->PCLKEN0.Word = 0x000000f7;     
	SCU->PCLKEN1.Word = 0x111F070F;        //打开所有外设时钟

	if(Prot_Temp != 0)   //写保护了
	{SCU_RegLock();}   //打开写保护
}

/***************************************************************
  函数名：DeviceClockAllDisable
  描  述：关闭所有外设时钟
  输入值：无
  输出值：无
  返回值：无
 ***************************************************************/
void DeviceClockAllDisable(void)
{
	uint32_t Prot_Temp;

	Prot_Temp = SCU->PROT.PROT;
	if(Prot_Temp != 0)   //写保护了
	{SCU_RegUnLock();}  //解锁

	SCU->PCLKEN0.Word = 0x00000000;     //关闭所有外设时钟，scu无法关闭
	SCU->PCLKEN0.Word = 0x00000000;

	if(Prot_Temp != 0)   //写保护了
	{SCU_RegLock();}   //打开写保护
}


/***************************************************************
  函数名：SystemClockConfig
  描  述：系统时钟选择
  输入值：CLKx 系统时钟源选择
  输出值：无
  返回值：无
 ***************************************************************/
void SystemClockSelect(SCU_TYPE_SYSCLK CLKx)
{
	uint32_t sysclk;
	sysclk=CLKx;
	SCU_RegUnLock(); //解锁
	switch(sysclk)
	{
		case 0:
			SCU_HRC_Enable();
			while(SCU_GetHRC1Flag() != SET);
			break;
		case 1:
			SCU_LRC_Enable();
			while(SCU_GetLRCFlag() != SET); 
			break;
		case 2:
			SCU_HOSC_Enable();
			while(SCU_HOSCReadyFlag() != SET); 
			break;
		case 3:
			SCU_PLL_Enable();
			while(SCU_PLLReadyFlag() != SET);
			break;
		case 4:
			SCU_LOSCLP_Enable();
			while(SCU_LOSCReadyFlag() != SET);
			break;
		default:break;
	}
	SCU_SysClkSelect(CLKx);   
	while(SCU_SysClkChangeBusy() != RESET); //等待切换完成
	SCU_RegLock();
}

/***************************************************************
  函数名：SysclkPLL
  描  述：系统时钟锁相环倍频
  输入值：CLKx:PLL时钟源选择；mode：PLL失锁时的处理模式
  输出值：无
  返回值：无
 ***************************************************************/
void SysclkPLL(PLL_TYPE_CLK CLKx,PLL_TYPE_UNLOCK mode)
{
	SCU_RegUnLock(); //解锁
	SCU->SCLKEN1.PLL_SSE=CLKx;
	SCU->SCLKEN1.PLL_ULOCK_SET=mode;
	SCU_PLL_Enable();
	while(SCU->SCLKEN1.PLL_RDY==RESET);
	SCU_SysClkSelect(SCU_SysClk_PLLCLK);
	while(SCU_SysClkChangeBusy() != RESET); 
	SCU_RegLock();//写保护
}
/*************************END OF FILE**********************/



