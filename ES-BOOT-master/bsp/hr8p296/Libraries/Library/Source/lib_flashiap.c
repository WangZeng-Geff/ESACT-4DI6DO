/***************************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
 *�ļ�����  lib_flashiap.c
 *��  �ߣ�  WangBJ
 *��  ����  V1.00
 *��  �ڣ�  2014/10/24
 *��  ����  flash��д�⺯��
 *��  ע��  ������ HR8P296оƬ
 ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 ***************************************************************/

#include "HR8P296.h"
#include "lib_flashiap.h"

/***************************************************************
  ��������FlashIap_Unlock
  ��  ����IAP�������������
  ����ֵ����
  ���ֵ����
  ����ֵ���ɹ���ʧ��
 ***************************************************************/
ErrorStatus FlashIap_Unlock(void)
{
	uint16_t  Temp16;

	FlashIAP_RegUnLock();              //���� IAP

	FlashIAP_Enable();                 //ʹ��IAP

	FlashIAP_REQ();                    //��������

	//�ȴ�FLASHӦ���ź�
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
  ��������FlashIap_WriteEnd
  ��  ����IAPд����
  ����ֵ����
  ���ֵ����
  ����ֵ���ɹ���ʧ��
 ***************************************************************/
ErrorStatus FlashIap_WriteEnd(void)
{
	uint32_t  Temp32;

	FlashIAP_RegUnLock();                             //IAP����

	IAP->CON.Word &= 0xFFFFFFEE;             				//IAP����FLASH����(����)

	for(Temp32 = 0; Temp32 < 0xFFFF; Temp32++)      //�ȴ�FLASHӦ���ź�(����)
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
  ��������FlashIap_ErasePage
  ��  ����IAPҳ����
  ����ֵ��Page_Addr��ҳ��ַ
  ���ֵ����
  ����ֵ���ɹ���ʧ��
 ***************************************************************/
ErrorStatus FlashIap_ErasePage(uint8_t Page_Addr)
{
	uint16_t  Temp16;
	uint32_t temp;

	temp = __get_PRIMASK();         		//��ȡPRIMASK�Ĵ�����ǰ״̬
	__disable_irq(); //���������ж�

	if(FlashIap_Unlock() == ERROR)
	{
		__set_PRIMASK(temp);           	//�ָ�PRIMASK�Ĵ���״̬
		return  ERROR;
	}

	IAP->ADDR.IAPPA = Page_Addr;    		//����ҳ��ַ   

	IAP->TRIG.IAPT = 0x00005EA1;      	//���������� 

	for(Temp16 = 0; Temp16 < 0xFFFF; Temp16++)  
	{                                  	//�ж�IAP����״̬
		if((IAP->STA.Word & (uint32_t)0x01) == (uint32_t)0x00) break; 
	}

	if(Temp16 == 0xFFFF)
	{
		__set_PRIMASK(temp);           	//�ָ�PRIMASK�Ĵ���״̬
		return  ERROR;
	}

	for(Temp16 = 0; Temp16 < 0xFFFF; Temp16++)  
	{                                 	//�ж�IAPҳ������־
		if((IAP->STA.Word & (uint32_t)0x02) == (uint32_t)0x02) break;  
	}       

	if(Temp16 == 0xFFFF)
	{
		__set_PRIMASK(temp);           //�ָ�PRIMASK�Ĵ���״̬
		return  ERROR;
	}

	if(FlashIap_WriteEnd() == ERROR)
	{
		__set_PRIMASK(temp);           //�ָ�PRIMASK�Ĵ���״̬
		return  ERROR;
	}

	__set_PRIMASK(temp);           		//�ָ�PRIMASK�Ĵ���״̬

	return  SUCCESS;        
}  

/***************************************************************
  ��������FlashIap_WriteCont
  ��  ����IAP����д
  ����ֵ��Unit_addr����Ԫ��ַ �� Page_addr��ҳ��ַ �� Data32������
  ���ֵ����
  ����ֵ���ɹ���ʧ��
 ***************************************************************/
ErrorStatus FlashIap_WriteCont(uint8_t Unit_addr, uint8_t Page_addr, uint32_t Data32)
{
	uint16_t  temp16;

	IAP->ADDR.IAPPA = Page_addr;       //�����ַ 
	IAP->ADDR.IAPCA = Unit_addr;

	IAP->DATA.IAPD    = Data32;          //��������
	IAP->TRIG.IAPT    = 0x00005DA2;      //���������� 

	for(temp16 = 0; temp16 < 0xFFFF; temp16++)  
	{
		if((IAP->STA.Word & (uint32_t)0x01) == (uint32_t)0x00) break;  //�ж�IAP����״̬
	}

	if(temp16 == 0xFFFF)
	{
		return  ERROR;
	}


	for(temp16 = 0; temp16 < 0xFFFF; temp16++)  
	{
		if((IAP->STA.Word & 0x04)==0x04) break;  //�ж�IAP��̽�����־
	}       

	if(temp16 == 0xFFFF)
	{
		return  ERROR;
	}

	return  SUCCESS;
}

/***************************************************************
  ��������FlashIap_WriteWord
  ��  ����IAPдһ����
  ����ֵ��Unit_addr����Ԫ��ַ �� Page_addr��ҳ��ַ �� Data32������
  ���ֵ����
  ����ֵ���ɹ���ʧ��
 ***************************************************************/
ErrorStatus FlashIap_WriteWord(uint8_t Unit_addr, uint8_t Page_addr, uint32_t Data32)
{
	uint32_t temp;

	temp = __get_PRIMASK();         		//��ȡPRIMASK�Ĵ�����ǰ״̬
	__disable_irq(); //���������ж�

	if(FlashIap_Unlock() == ERROR)
	{
		__set_PRIMASK(temp);           	//�ָ�PRIMASK�Ĵ���״̬
		return  ERROR;	
	}		

	if(FlashIap_WriteCont(Unit_addr, Page_addr, Data32) == ERROR)
	{
		__set_PRIMASK(temp);           //�ָ�PRIMASK�Ĵ���״̬
		return  ERROR;
	}

	if(FlashIap_WriteEnd() == ERROR)
	{
		__set_PRIMASK(temp);           //�ָ�PRIMASK�Ĵ���״̬
		return  ERROR;
	}

	__set_PRIMASK(temp);           		//�ָ�PRIMASK�Ĵ���״̬
	return  SUCCESS;		

}

/***************************************************************
  ��������Flash_Read
  ��  ����Flash������
  ����ֵ��Ram_Addr���������ݵĴ�ŵ�ַ �� Flash_Addr��Flash��ַ��0x00000000 ~ 0x0001FFFF���� Len����ȡ���ֳ���
  ���ֵ������������
  ����ֵ���ɹ���ʧ��
 ***************************************************************/
ErrorStatus Flash_Read(uint32_t * Ram_Addr, uint32_t Flash_Addr,  uint8_t Len)
{
	uint8_t  i;
	uint32_t *  ram_addr32;
	const uint32_t  *  flash_addr32;

	ram_addr32 = (uint32_t *)Ram_Addr;
	flash_addr32 = (const uint32_t  * )Flash_Addr;
	if((Len == 0)&(Len>(0x20000-Flash_Addr)/4))  //�ж϶�ȡ�����Ƿ�Ϸ�
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
