/***************************************************************
 *Copyright (C), 2017, Shanghai Eastsoft Microelectronics Co., Ltd.
 *�ļ�����  
 *��  ��:  Liut
 *��  ��:  V1.00
 *��  ��:  2017/07/14
 *��  ����  
 *��  ע��  ������ ES8P508оƬ
 ����������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 ***************************************************************/
#include "lib_iap.h"

/*************************************************
 * @brief  IAP ҳ�������.�ص��̻���ROM�еĳ���
 * @param  address: ҳ�׵�ַ
 * @retval None
 ***********************************************/
ErrorStatus IAP_PageErase(uint32_t address)
{
    IAP_PE IAPPageErase = (IAP_PE)(*(uint32_t *)IAP_PageErase_addr);
	ErrorStatus result;
	
	uint32_t cpu_sr = __get_PRIMASK();
	__disable_irq();
    result = (*IAPPageErase)(address);
    __set_PRIMASK(cpu_sr);
    return  result;
}

/*************************************************
 * @brief  IAP �����Ա��.�ص��̻���ROM�еĳ���
 * @param  address: ����׵�ַ
 * @param  data: �������
 * @retval None
 ************************************************/
ErrorStatus IAP_WordProgram(uint32_t address, uint32_t data)
{
    IAP_WP IAPWordProgram = (IAP_WP)(*(uint32_t *)IAP_WordProgram_addr);
	ErrorStatus result;
	
    uint32_t cpu_sr = __get_PRIMASK();
	__disable_irq();
    result = (*IAPWordProgram)( address,  data);
    __set_PRIMASK(cpu_sr);
    return  result;
}

/************************************************
 * @brief  IAP �����Ա��.�ص��̻���ROM�еĳ���
 * @param  address: ����׵�ַ
 * @param  data[]: �������
 * @param  length: ������ݳ��ȣ��ֽڣ�
 * @param  address: ���ҳ�Ƿ��Ȳ���
 *     @arg 0: ������
 *     @arg 1: ����
 * @retval None
 **********************************************/
ErrorStatus IAP_PageProgram(uint32_t address, uint32_t data[], uint32_t length, uint32_t erase)  //@0x100001c5
{
    IAP_PP IAPPageProgram = (IAP_PP)(*(uint32_t *)IAP_PageProgram_addr);
	ErrorStatus result;
	
    uint32_t cpu_sr = __get_PRIMASK();
	__disable_irq();
    result = (*IAPPageProgram)(address,data,length,erase);
    __set_PRIMASK(cpu_sr);
    return  result;
}
/**
 * @brief  flash write
 * @param  address: ����׵�ַ
 * @param  data: �������
 * @param  len: ������ݳ���
 * @retval None
 */
/*************************************
  ��������IARead
  ��  ����Flash��
  ����ֵ���ڴ��ַ--��ȡ���ݵĴ�ŵ�ַ
  Flash��ַ
  ��ȡ���ݵĳ��ȣ���λΪ�֣�4�ֽڣ�
  ����ֵ���ɹ�/ʧ��
 **************************************/
ErrorStatus IAPRead(uint32_t *Ram_Addr, uint32_t Flash_Addr, uint8_t Len)
{
    uint8_t i;
    uint32_t *ram_addr;
    const uint32_t *flash_addr;

    ram_addr = Ram_Addr;
    flash_addr = (const uint32_t *)Flash_Addr;

    if (Len == 0)
        return ERROR;

    for (i = 0; i < Len; ++i)
    {
        *ram_addr = *flash_addr;
        ++ram_addr;
        ++flash_addr;
    }

    return SUCCESS;
}

/************************END OF FILE**************************/
