/***************************************************************
*Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
*文件名：  system_HR8P296.h
*作  者：  Zoux
*版  本：  V1.00
*日  期：  
*描  述：  库函数配置文件
*备  注：  适用于 HR8P296芯片
           本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
***************************************************************/

#ifndef __SYSTEMHR8P296_H__
#define __SYSTEMHR8P296_H__

 

#include "HR8P296.h"



uint32_t SystemCoreClock   = 16000000;

/***************************************************************
函数名：SystemCoreClockUpdate
描  述：系统时钟配置为内部时钟16MHZ，同时设置SystemCoreClock
输入值：无
输出值：无
返回值：无
***************************************************************/
void SystemCoreClockUpdate (void)
{
    uint32_t Prot_Temp;
    uint16_t Count=0;
	  
    Prot_Temp = SCU->PROT.PROT;
    if(Prot_Temp != 0)   //写保护了
        {SCU->PROT.Word = 0x55AA6996;}  //解锁*/

    SCU->SCLKEN0.HRC_EN = 1;                      //使能内部16MHZ
    do
    {
        Count++;
    }
    while((SCU->SCLKEN0.HRC_ON == 0)&&(Count != 0xFFF));//等待时钟开启
    
    if(Count == 0xFFF)
    {
        return ;
    }
    
    SCU->SCLKEN0.SYSCLK_SSE = 0;    //选择内部16MHZ为系统时钟
    Count = 0;
    do
    {
        Count++;
    }
    while((SCU->SCLKEN0.SYSCLK_CHG != 0)&&(Count != 0xFFF));//等待切换完成
    
    if(Count == 0xFFF)
    {
        return ;
    }
    
    SCU->SCLKEN0.SYSCLK_DIV = 0;          //系统时钟后分频1:1
    //SCU->SCLKEN0.SYSCLKOE = 1;            //系统时钟128分频输出使能开启，通过SYSCLK0输出

    
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
    if(Prot_Temp != 0)   //写保护了
        {SCU->PROT.Word = 0x00000000;}   //打开写保护  
}



/***************************************************************
函数名：SystemInit
描  述：系统初始化，设置时钟
输入值：无
输出值：无
返回值：无
***************************************************************/
void SystemInit (void)
{
	SystemCoreClockUpdate();   
}


#endif

/*************************END OF FILE**********************/
