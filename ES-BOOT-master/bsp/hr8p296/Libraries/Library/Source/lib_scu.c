/***************************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
 *�ļ�����  lib_scu.c
 *��  �ߣ�  WangBJ
 *��  ����  V1.00
 *��  �ڣ�  2014/10/24
 *��  ����  ϵͳ����ģ��⺯��
 *��  ע��  ������ HR8P296оƬ
 ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 ***************************************************************/

#include "lib_scu.h"
#include "lib_gpio.h"
#include "system_HR8P296.h"
/***************************************************************
  ��������SCU_NMISelect
  ��  ��������NMI���������ж�
  ����ֵ�����������ж�
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void SCU_NMISelect(SCU_TYPE_NMICS NMI_Type)
{
	SCU->NMIC.NMICS = NMI_Type;
}

/***************************************************************
  ��������SCU_GetPWRCFlagStatus
  ��  ������ȡPWRC��λ״̬�Ĵ�����־λ״̬
  ����ֵ��PWRC�Ĵ�����־λ
  ���ֵ����
  ����ֵ��RESET/SET
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
  ��������SCU_ClearPWRCFlagBit
  ��  �������PWRC��λ״̬�Ĵ�����־λ
  ����ֵ��PWRC�Ĵ�����־λ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void SCU_ClearPWRCFlagBit(SCU_TYPE_PWRC PWRC_Flag)
{
	SCU->PWRC.Word &= ~((uint32_t)PWRC_Flag);
}

/***************************************************************
  ��������SCU_GetLVDFlagStatus
  ��  ������ȡLVDD�Ĵ�����־λ״̬
  ����ֵ��LVD�Ĵ�����־λ
  ���ֵ����
  ����ֵ��RESET/SET
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
  ��������SCU_GetCFGWord
  ��  ������ȡоƬ������
  ����ֵ����
  ���ֵ����
  ����ֵ��32λ������
 ***************************************************************/
uint32_t SCU_GetCFGWord(void)
{
	return (SCU->CFGWORD0.Word);
}

/***************************************************************
  ��������SCU_SysClkSelect
  ��  ����ѡ��ϵͳʱ��
  ����ֵ��ʱ��Դ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void SCU_SysClkSelect(SCU_TYPE_SYSCLK Sysclk)
{
	SCU->SCLKEN0.SYSCLK_SSE= Sysclk;
}

/***************************************************************
  ��������SCU_GetSysClk
  ��  ������ȡϵͳʱ��Դ
  ����ֵ����
  ���ֵ����
  ����ֵ��ϵͳʱ��Դ
 ***************************************************************/
SCU_TYPE_SYSCLK SCU_GetSysClk(void)
{
	return (SCU_TYPE_SYSCLK)(SCU->SCLKEN0.SYSCLK_SST);
}

/***************************************************************
  ��������SCU_SysClkChangeBusy
  ��  ������ȡϵͳʱ���л���־λ
  ����ֵ����
  ���ֵ����
  ����ֵ��RESET/SET
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
  ��������SCU_GetHRC1Flag
  ��  ������ȡHRC������־λ
  ����ֵ����
  ���ֵ����
  ����ֵ��RESET/SET
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
  ��������SCU_GetLRCFlag
  ��  ������ȡLRC������־λ
  ����ֵ����
  ���ֵ����
  ����ֵ��RESET/SET
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
  ��������SCU_HOSCReadyFlag
  ��  ������ȡHOSC�ȶ���־λ
  ����ֵ����
  ���ֵ����
  ����ֵ��RESET�����ȶ���/SET���ȶ���
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
  ��������SCU_LOSCReadyFlag
  ��  ������ȡLOSC�ȶ���־λ
  ����ֵ����
  ���ֵ����
  ����ֵ��RESET�����ȶ���/SET���ȶ���
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
  ��������SCU_LOSCReadyFlag
  ��  ������ȡLOSC�ȶ���־λ
  ����ֵ����
  ���ֵ����
  ����ֵ��RESET�����ȶ���/SET���ȶ���
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
  ��������SystemClockConfig
  ��  ����ϵͳʱ�����ã��ڲ�ʱ�ӣ�16MHZ������������ʱ��
  ����ֵ����
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void SystemClockConfig(void)
{
	uint32_t Prot_Temp;

	Prot_Temp = SCU->PROT.PROT;
	if(Prot_Temp != 0)   //д������
	{SCU_RegUnLock();}  //����
	SCU_HRC_Enable();                      //ʹ���ڲ�16MHZ
	while(SCU_GetHRC1Flag() != SET);      //�ȴ�ʱ�ӿ���
	SCU_SysClkSelect(SCU_SysClk_HRC);    //ѡ���ڲ�16MHZΪϵͳʱ��
	while(SCU_SysClkChangeBusy() != RESET); //�ȴ��л����
	SCU_SysClk_Div1();                        //ϵͳʱ�Ӻ��Ƶ1:1
	SCU_ADC32K_RtcOsc();                      //ADC��LCDC��WDT 32Kʱ��ѡ��RTCOSC
	SCU_LCD32K_RtcOsc();
	SCU_WDT32K_RtcOsc();
	SystemCoreClock = 16000000;
	if(Prot_Temp != 0)   //д������
	{SCU_RegLock();}   //��д����
}

/***************************************************************
  ��������DeviceClockAllEnable
  ��  ��������������ʱ��
  ����ֵ����
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void DeviceClockAllEnable(void)
{
	uint32_t Prot_Temp;

	Prot_Temp = SCU->PROT.PROT;
	if(Prot_Temp != 0)   //д������
	{SCU_RegUnLock();}  //����

	SCU->PCLKEN0.Word = 0x000000f7;     
	SCU->PCLKEN1.Word = 0x111F070F;        //����������ʱ��

	if(Prot_Temp != 0)   //д������
	{SCU_RegLock();}   //��д����
}

/***************************************************************
  ��������DeviceClockAllDisable
  ��  �����ر���������ʱ��
  ����ֵ����
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void DeviceClockAllDisable(void)
{
	uint32_t Prot_Temp;

	Prot_Temp = SCU->PROT.PROT;
	if(Prot_Temp != 0)   //д������
	{SCU_RegUnLock();}  //����

	SCU->PCLKEN0.Word = 0x00000000;     //�ر���������ʱ�ӣ�scu�޷��ر�
	SCU->PCLKEN0.Word = 0x00000000;

	if(Prot_Temp != 0)   //д������
	{SCU_RegLock();}   //��д����
}


/***************************************************************
  ��������SystemClockConfig
  ��  ����ϵͳʱ��ѡ��
  ����ֵ��CLKx ϵͳʱ��Դѡ��
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void SystemClockSelect(SCU_TYPE_SYSCLK CLKx)
{
	uint32_t sysclk;
	sysclk=CLKx;
	SCU_RegUnLock(); //����
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
	while(SCU_SysClkChangeBusy() != RESET); //�ȴ��л����
	SCU_RegLock();
}

/***************************************************************
  ��������SysclkPLL
  ��  ����ϵͳʱ�����໷��Ƶ
  ����ֵ��CLKx:PLLʱ��Դѡ��mode��PLLʧ��ʱ�Ĵ���ģʽ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void SysclkPLL(PLL_TYPE_CLK CLKx,PLL_TYPE_UNLOCK mode)
{
	SCU_RegUnLock(); //����
	SCU->SCLKEN1.PLL_SSE=CLKx;
	SCU->SCLKEN1.PLL_ULOCK_SET=mode;
	SCU_PLL_Enable();
	while(SCU->SCLKEN1.PLL_RDY==RESET);
	SCU_SysClkSelect(SCU_SysClk_PLLCLK);
	while(SCU_SysClkChangeBusy() != RESET); 
	SCU_RegLock();//д����
}
/*************************END OF FILE**********************/



