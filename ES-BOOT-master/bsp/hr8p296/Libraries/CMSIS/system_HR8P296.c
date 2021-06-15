/***************************************************************
*Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
*�ļ�����  system_HR8P296.h
*��  �ߣ�  Zoux
*��  ����  V1.00
*��  �ڣ�  
*��  ����  �⺯�������ļ�
*��  ע��  ������ HR8P296оƬ
           ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
***************************************************************/

#ifndef __SYSTEMHR8P296_H__
#define __SYSTEMHR8P296_H__

 

#include "HR8P296.h"



uint32_t SystemCoreClock   = 16000000;

/***************************************************************
��������SystemCoreClockUpdate
��  ����ϵͳʱ������Ϊ�ڲ�ʱ��16MHZ��ͬʱ����SystemCoreClock
����ֵ����
���ֵ����
����ֵ����
***************************************************************/
void SystemCoreClockUpdate (void)
{
    uint32_t Prot_Temp;
    uint16_t Count=0;
	  
    Prot_Temp = SCU->PROT.PROT;
    if(Prot_Temp != 0)   //д������
        {SCU->PROT.Word = 0x55AA6996;}  //����*/

    SCU->SCLKEN0.HRC_EN = 1;                      //ʹ���ڲ�16MHZ
    do
    {
        Count++;
    }
    while((SCU->SCLKEN0.HRC_ON == 0)&&(Count != 0xFFF));//�ȴ�ʱ�ӿ���
    
    if(Count == 0xFFF)
    {
        return ;
    }
    
    SCU->SCLKEN0.SYSCLK_SSE = 0;    //ѡ���ڲ�16MHZΪϵͳʱ��
    Count = 0;
    do
    {
        Count++;
    }
    while((SCU->SCLKEN0.SYSCLK_CHG != 0)&&(Count != 0xFFF));//�ȴ��л����
    
    if(Count == 0xFFF)
    {
        return ;
    }
    
    SCU->SCLKEN0.SYSCLK_DIV = 0;          //ϵͳʱ�Ӻ��Ƶ1:1
    //SCU->SCLKEN0.SYSCLKOE = 1;            //ϵͳʱ��128��Ƶ���ʹ�ܿ�����ͨ��SYSCLK0���

    
    switch(SCU->SCLKEN0.SYSCLK_SST)
    {
        case 0:
            SystemCoreClock = 16000000;   //HRC
            break;
        case 1:
            SystemCoreClock = 32000;      //LRC
            break;
        default:
            break;
    }
    if(Prot_Temp != 0)   //д������
        {SCU->PROT.Word = 0x00000000;}   //��д����  
}



/***************************************************************
��������SystemInit
��  ����ϵͳ��ʼ��������ʱ��
����ֵ����
���ֵ����
����ֵ����
***************************************************************/
void SystemInit (void)
{
	SystemCoreClockUpdate();   
}


#endif

/*************************END OF FILE**********************/
