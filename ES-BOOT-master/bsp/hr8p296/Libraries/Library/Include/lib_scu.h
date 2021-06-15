
#ifndef __LIBSCU_H
#define __LIBSCU_H

#include "HR8P296.h"
#include "type.h"
/* NMI不可屏蔽中断选择 */
typedef enum
{
	SCU_NMIIRQ_PINT0 = 0, 
	SCU_NMIIRQ_PINT1 = 1, 
	SCU_NMIIRQ_PINT2 = 2, 
	SCU_NMIIRQ_PINT3 = 3, 
	SCU_NMIIRQ_PINT4 = 4, 
	SCU_NMIIRQ_PINT5 = 5, 
	SCU_NMIIRQ_PINT6 = 6, 
	SCU_NMIIRQ_PINT7 = 7, 
	SCU_NMIIRQ_T16N0 = 8, 
	SCU_NMIIRQ_T16N1 = 9, 
	SCU_NMIIRQ_T16N2 = 10, 
	SCU_NMIIRQ_T16N3 = 11, 
	SCU_NMIIRQ_T32N0 = 12, 
	SCU_NMIIRQ_T32N1 = 13, 
	SCU_NMIIRQ_T32N2 = 14, 
	SCU_NMIIRQ_WDTINT = 16, 
	SCU_NMIIRQ_RTCINT = 17, 
	SCU_NMIIRQ_ADCINT = 19, 
	SCU_NMIIRQ_LCDINT = 20, 
	SCU_NMIIRQ_LVD0INT = 21, 
	SCU_NMIIRQ_UART0 = 23, 
	SCU_NMIIRQ_UART1 = 24, 
	SCU_NMIIRQ_UART2 = 25, 
	SCU_NMIIRQ_UART3 = 26, 
	SCU_NMIIRQ_EUART0 = 27,  
	SCU_NMIIRQ_SPI0INT = 29, 
	SCU_NMIIRQ_IIC0INT = 30,  
}SCU_TYPE_NMICS;

/* PWRC复位状态寄存器标志位 */
typedef enum
{
	SCU_PWRC_PORVF= 0X00001,     //5V POR复位标志位
	SCU_PWRC_RRCF = 0X00002,     //1.5V POR复位标志位
	SCU_PWRC_PORF = 0x00004,     //POR总复位标志
	SCU_PWRC_BORF = 0x00008,     //BOR总复位标志
	SCU_PWRC_WDTRSTF = 0x00010,  //WDT复位标志
	SCU_PWRC_MRSTF = 0x00020,    //MRSTn复位标志
	SCU_PWRC_SOFTRSTF = 0x00040, //软件复位标志
	SCU_PWRC_PORLOST = 0x00080,  //POR丢失标志位
}SCU_TYPE_PWRC;

/* LVD寄存器标志位 */
typedef enum
{
	SCU_LVDFlag_IF = 0x0100,  //LVD中断标志
	SCU_LVDFlag_Out = 0x8000, //输出状态位
}SCU_TYPE_LVD0CON;

/* 时钟选择 */
typedef enum
{
	SCU_SysClk_HRC = 0x0 ,  //内部16MHZ RC时钟
	SCU_SysClk_LRC = 0x1 ,  //内部32KHZ RC时钟
	SCU_SysClk_HOSC = 0x2 ,   //外部2-20MHZ主时钟
	SCU_SysClk_PLLCLK = 0x3 ,   // PLL锁相环倍频时钟
	SCU_SysClk_LOSC = 0x4 , //外部RTC时钟
}SCU_TYPE_SYSCLK;

/*PLL时钟源选择*/
typedef enum
{
	PLL_LRC    = 0x0, //内部32KHz
	PLL_LOSC   = 0x1, //外部32KHz
	PLL_HRC_4  = 0x4, //内部4MHz
	PLL_HOSC_4 = 0x5, //外接16MHz晶振
	PLL_HOSC   = 0x6, //外接4MHz晶振
}PLL_TYPE_CLK;

/*PLL失锁处理*/
typedef enum
{
	PLL_UNLOCK_Mode0 = 0x0, //禁止无任何处理
	PLL_UNLOCK_Mode1 = 0x1, //PLL时钟门控禁止，无系统时钟，等重新锁频后，提供时钟
	PLL_UNLOCK_Mode2 = 0X2, //系统时钟切换成HRC
	PLL_UNLOCK_Mode3 = 0X3, //系统时钟切换成HRC，PLL锁频后自动切回
}PLL_TYPE_UNLOCK;
/************SCU模块宏定义***********/

/* SCU写保护控制 */
#define SCU_RegUnLock() (SCU->PROT.Word = 0x55AA6996)
#define SCU_RegLock()   (SCU->PROT.Word = 0x00000000)

/* NMI使能控制 */
#define SCU_NMI_Enable()  (SCU->NMIC.NMIEN = 0x1)
#define SCU_NMI_Disable() (SCU->NMIC.NMIEN = 0x0)

/*-------LVD模块-------*/

/* LVD使能控制 */
#define SCU_LVD_Enable()  (SCU->LVDC.LVDEN = 0x1)
#define SCU_LVD_Disable() (SCU->LVDC.LVDEN = 0x0)

/* LVD滤波使能控制 */
#define SCU_LVDFLT_Enable()  (SCU->LVDC.LVD_FLTEN = 0x1) 
#define SCU_LVDFLT_Disable() (SCU->LVDN.LVD_FLTEN = 0x0) 

/* LVD触发电压选择 */
#define SCU_LVDVS_2V4() (SCU->LVDC.LVDVS = 0x0)
#define SCU_LVDVS_2V6() (SCU->LVDC.LVDVS = 0x1)
#define SCU_LVDVS_2V8() (SCU->LVDC.LVDVS = 0x2)
#define SCU_LVDVS_3V()  (SCU->LVDC.LVDVS = 0x3)
#define SCU_LVDVS_3V2() (SCU->LVDC.LVDVS = 0x4)
#define SCU_LVDVS_3V4() (SCU->LVDC.LVDVS = 0x5)
#define SCU_LVDVS_3V6() (SCU->LVDC.LVDVS = 0x6)
#define SCU_LVDVS_3V8() (SCU->LVDC.LVDVS = 0x7)
#define SCU_LVDVS_4V()  (SCU->LVDC.LVDVS = 0x8)
#define SCU_LVDVS_4V2() (SCU->LVDC.LVDVS = 0x9)
#define SCU_LVDVS_4V4() (SCU->LVDC.LVDVS = 0xA)
#define SCU_LVDVS_4V6() (SCU->LVDC.LVDVS = 0xB)
#define SCU_LVDVS_4V8() (SCU->LVDC.LVDVS = 0xC)
#define SCU_LVDVS_5V() (SCU->LVDC.LVDVS = 0xD)
#define SCU_LVDVS_LVDIN() (SCU->LVDC.LVDVS = 0xE)

/* LVD中断使能控制 */
#define SCU_LVDIT_Enable()  (SCU->LVDC.LVDIE = 0x1)
#define SCU_LVDIT_Disable() (SCU->LVDC.LVDIE = 0x0)

/* LVD中断标志位清除 */
#define SCU_LVDClearIFBit() (SCU->LVDC.LVDIF = 1)

/* LVD中断产生模式选择 */
#define SCU_LVDIFS_Rise() (SCU->LVDC.LVDIFS = 0x0) //LVDO上升沿产生中断
#define SCU_LVDIFS_Fall() (SCU->LVDC.LVDIFS = 0x1) //LVDO下降沿产生中断
#define SCU_LVDIFS_High() (SCU->LVDC.LVDIFS = 0x2) //LVDO高电平产生中断
#define SCU_LVDIFS_Low()  (SCU->LVDC.LVDIFS = 0x3)   //LVDO低电平产生中断 
#define SCU_LVDIFS_Change() (SCU->LVDC.LVDIFS = 0x4) //LVDO电平变化产生中断

/* FLASH访问等待时间选择 */
#define SCU_FlashWait_2Tclk()  (SCU->FLASHW.ACCT = 0x0)
#define SCU_FlashWait_3Tclk()  (SCU->FLASHW.ACCT = 0x1)
#define SCU_FlashWait_4Tclk()  (SCU->FLASHW.ACCT = 0x2)
#define SCU_FlashWait_5Tclk()  (SCU->FLASHW.ACCT = 0x3)
#define SCU_FlashWait_6Tclk()  (SCU->FLASHW.ACCT = 0x4)
#define SCU_FlashWait_7Tclk()  (SCU->FLASHW.ACCT = 0x5)
#define SCU_FlashWait_8Tclk()  (SCU->FLASHW.ACCT = 0x6)
#define SCU_FlashWait_9Tclk()  (SCU->FLASHW.ACCT = 0x7)
#define SCU_FlashWait_10Tclk() (SCU->FLASHW.ACCT = 0x8)
#define SCU_FlashWait_11Tclk() (SCU->FLASHW.ACCT = 0x9)
#define SCU_FlashWait_12Tclk() (SCU->FLASHW.ACCT = 0xA)
#define SCU_FlashWait_13Tclk() (SCU->FLASHW.ACCT = 0xB)
#define SCU_FlashWait_14Tclk() (SCU->FLASHW.ACCT = 0xC)
#define SCU_FlashWait_15Tclk() (SCU->FLASHW.ACCT = 0xD)
#define SCU_FlashWait_16Tclk() (SCU->FLASHW.ACCT = 0xE)
#define SCU_FlashWait_17Tclk() (SCU->FLASHW.ACCT = 0xF)

/* 系统时钟后分频选择 */
#define SCU_SysClk_Div1()   (SCU->SCLKEN0.SYSCLK_DIV = 0)
#define SCU_SysClk_Div2()   (SCU->SCLKEN0.SYSCLK_DIV = 1)
#define SCU_SysClk_Div4()   (SCU->SCLKEN0.SYSCLK_DIV = 2)
#define SCU_SysClk_Div8()  (SCU->SCLKEN0.SYSCLK_DIV = 3)
#define SCU_SysClk_Div16()  (SCU->SCLKEN0.SYSCLK_DIV = 4)
#define SCU_SysClk_Div32()  (SCU->SCLKEN0.SYSCLK_DIV = 5)
#define SCU_SysClk_Div64()  (SCU->SCLKEN0.SYSCLK_DIV = 6)
#define SCU_SysClk_Div128() (SCU->SCLKEN0.SYSCLK_DIV = 7)


/* HRC使能控制 (内部16Mhz) */
#define SCU_HRC_Enable()  (SCU->SCLKEN0.HRC_EN = 1)
#define SCU_HRC_Disable() (SCU->SCLKEN0.HRC_EN = 0)

/* HRC调校时间间隔选择 */
#define SCU_HRC1Wait_8()  (SCU->SCLKEN0.HRC_WAIT_SEL= 0) //8个RTC时钟周期
#define SCU_HRC1Wait_32() (SCU->SCLKEN0.HRC_WAIT_SEL = 1)
#define SCU_HRC1Wait_128() (SCU->SCLKEN0.HRC_WAIT_SEL = 2)
#define SCU_HRC1Wait_512() (SCU->SCLKEN0.HRC_WAIT_SEL = 3)

/* LRC使能控制 (内部32KHz) */
#define SCU_LRC_Enable()  (SCU->SCLKEN0.LRC_EN = 1)
#define SCU_LRC_Disable() (SCU->SCLKEN0.LRC_EN = 0)


/* HOSC使能控制 */                                                          
#define SCU_HOSC_Enable()  (SCU->SCLKEN1.HOSC_EN = 1)                         
#define SCU_HOSC_Disable() (SCU->SCLKEN1.HOSC_EN = 0)                         

/* HOSC振荡器稳定时间选择 */                                                
#define SCU_HOSCTX_512()  (SCU->SCLKEN1.HOSC_TS = 0) 

#define SCU_HOSCTX_1024() (SCU->SCLKEN1.HOSC_TS = 1)                          
#define SCU_HOSCTX_2048() (SCU->SCLKEN1.HOSC_TS = 2)                          

/* LOSC低功耗模式使能控制 */                                                
#define SCU_LOSCLP_Enable()  (SCU->SCLKEN1.LOSC_LPEN = 1)                     
#define SCU_LOSCLP_Disable() (SCU->SCLKEN1.LOSC_LPEN = 0)    

/* PLL模式使能控制 */ 
#define SCU_PLL_Enable()  (SCU->SCLKEN1.PLL_EN = 1)
#define SCU_PLL_Disable() (SCU->SCLKEN1.PLL_EN = 0)    

/* 系统时钟128分频输出使能控制 */ 
#define SYSCLK_OUT_Enable()  {SCU_RegUnLock();SCU->SCLKEN0.SYSCLKOE=1;SCU_RegLock();}
#define SYSCLK_OUT_Disable() {SCU_RegUnLock();SCU->SCLKEN0.SYSCLKOE=0;SCU_RegLock();}

/* ADC 32k时钟选择 */                                                       
#define SCU_ADC32K_WdtClk() (SCU->SCLKEN1.ADC32K_SEL = 0)                     
#define SCU_ADC32K_RtcOsc() (SCU->SCLKEN1.ADC32K_SEL = 1)                     

/* LCD 32k时钟选择 */                                                       
#define SCU_LCD32K_WdtClk() (SCU->SCLKEN1.LCD32K_SEL = 0)                     
#define SCU_LCD32K_RtcOsc() (SCU->SCLKEN1.LCD32K_SEL = 1)                     

/* WDT 32k时钟选择 */                                                       
#define SCU_WDT32K_WdtClk() (SCU->SCLKEN1.WDT32K_SEL = 0)                     
#define SCU_WDT32K_RtcOsc() (SCU->SCLKEN1.WDT32K_SEL = 1)                     


/*-------外设时钟控制-------*/                                              

/*-------外设时钟控制-------*/                                              

/* GPIO时钟使能控制 */                                                      
#define SCU_GPIOCLK_Enable()  (SCU->PCLKEN0.GPIO_EN = 1)                         
#define SCU_GPIOCLK_Disable() (SCU->PCLKEN0.GPIO_EN = 0)                         

/* FLASH IAP时钟使能控制 */                                                 
#define SCU_IAPCLK_Enable()  (SCU->PCLKEN0.IAP_EN = 1)                           
#define SCU_IAPCLK_Disable() (SCU->PCLKEN0.IAP_EN = 0)                           

/* ADC时钟使能控制 */                                                       
#define SCU_ADCCLK_Enable()  (SCU->PCLKEN0.ADC_EN = 1)                           
#define SCU_ADCCLK_Disable() (SCU->PCLKEN0.ADC_EN = 0)                           

/* RTC时钟使能控制 */                                                       
#define SCU_RTCCLK_Enable()  (SCU->PCLKEN0.RTC_EN = 1)                           
#define SCU_RTCCLK_Disable() (SCU->PCLKEN0.RTC_EN = 0)                           

/* LCDC时钟使能控制 */                                                      
#define SCU_LCDCCLK_Enable()  (SCU->PCLKEN0.LCDC_EN = 1)                         
#define SCU_LCDCCLK_Disable() (SCU->PCLKEN0.LCDC_EN = 0)                         

/* WDT时钟使能控制 */                                                       
#define SCU_WDTCLK_Enable()  (SCU->PCLKEN0.WDT_EN = 1)                           
#define SCU_WDTCLK_Disable() (SCU->PCLKEN0.WDT_EN = 0)                           

/* T16N0时钟使能控制 */                                                     
#define SCU_T16N0CLK_Enable()  (SCU->PCLKEN1.T16N0_EN = 1)                       
#define SCU_T16N0CLK_Disable() (SCU->PCLKEN1.T16N0_EN = 0)                       

/* T16N1时钟使能控制 */                                                     
#define SCU_T16N1CLK_Enable()  (SCU->PCLKEN1.T16N1_EN = 1)                       
#define SCU_T16N1CLK_Disable() (SCU->PCLKEN1.T16N1_EN = 0)                       

/* T16N2时钟使能控制 */                                                     
#define SCU_T16N2CLK_Enable()  (SCU->PCLKEN1.T16N2_EN = 1)                       
#define SCU_T16N2CLK_Disable() (SCU->PCLKEN1.T16N2_EN = 0)                       

/* T16N3时钟使能控制 */                                                     
#define SCU_T16N3CLK_Enable()  (SCU->PCLKEN1.T16N3_EN = 1)                       
#define SCU_T16N3CLK_Disable() (SCU->PCLKEN1.T16N3_EN = 0)                       

/* T32N0时钟使能控制 */                                                     
#define SCU_T32N0CLK_Enable()  (SCU->PCLKEN1.T32N0_EN = 1)                       
#define SCU_T32N0CLK_Disable() (SCU->PCLKEN1.T32N0_EN = 0)                       

/* T32N1时钟使能控制 */                                                     
#define SCU_T32N1CLK_Enable()  (SCU->PCLKEN1.T32N1_EN = 1)                       
#define SCU_T32N1CLK_Disable() (SCU->PCLKEN1.T32N1_EN = 0)                       

/* T32N2时钟使能控制 */                                                     
#define SCU_T32N2CLK_Enable()  (SCU->PCLKEN1.T32N2_EN = 1)                       
#define SCU_T32N2CLK_Disable() (SCU->PCLKEN1.T32N2_EN = 0)  

/* UART0时钟使能控制 */                                                     
#define SCU_UART0CLK_Enable()  (SCU->PCLKEN1.UART0_EN = 1)                       
#define SCU_UART0CLK_Disable() (SCU->PCLKEN1.UART0_EN = 0)                       

/* UART1时钟使能控制 */                                                     
#define SCU_UART1CLK_Enable()  (SCU->PCLKEN1.UART1_EN = 1)                       
#define SCU_UART1CLK_Disable() (SCU->PCLKEN1.UART1_EN = 0)                       

/* UART2时钟使能控制 */                                                     
#define SCU_UART2CLK_Enable()  (SCU->PCLKEN1.UART2_EN = 1)                       
#define SCU_UART2CLK_Disable() (SCU->PCLKEN1.UART2_EN = 0)                       

/* UART3时钟使能控制 */                                                     
#define SCU_UART3CLK_Enable()  (SCU->PCLKEN1.UART3_EN = 1)                       
#define SCU_UART3CLK_Disable() (SCU->PCLKEN1.UART3_EN = 0)                       

/* EUART0时钟使能控制 */                                                    
#define SCU_EUART0CLK_Enable()  (SCU->PCLKEN1.EUART0_EN = 1)                     
#define SCU_EUART0CLK_Disable() (SCU->PCLKEN1.EUART0_EN = 0)                     


/* SPI0时钟使能控制 */                                                      
#define SCU_SPI0CLK_Enable()  (SCU->PCLKEN1.SPI_EN = 1)                          
#define SCU_SPI0CLK_Disable() (SCU->PCLKEN1.SPI_EN = 0)                          

/* IIC0时钟使能控制 */                                                      
#define SCU_IIC0CLK_Enable()  (SCU->PCLKEN1.I2C_EN = 1)                          
#define SCU_IIC0CLK_Disable() (SCU->PCLKEN1.I2C_EN = 0)                          



/* 中断向量表重映射使能控制 */                                              
#define SCU_TBLRemap_Enable()  (SCU->TBLRMEN.TBLREMAPEN= 1)                   
#define SCU_TBLRemap_Disable() (SCU->TBLRMEN.TBLREMAPEN= 0)                   

/* 中断向量表偏移寄存器 x最大为2^24=16777216 */                             
#define SCU_TBL_Offset(x) (SCU->TBLOFFS.TBLOFF = (uint32_t)x)             

/************SCU模块函数声明***********/
void SCU_NMISelect(SCU_TYPE_NMICS NMI_Type);
FlagStatus SCU_GetPWRCFlagStatus(SCU_TYPE_PWRC PWRC_Flag);
void SCU_ClearPWRCFlagBit(SCU_TYPE_PWRC PWRC_Flag);
FlagStatus SCU_GetLVDFlagStatus(SCU_TYPE_LVD0CON LVD_Flag);
uint32_t SCU_GetCFGWord(void);
void SCU_SysClkSelect(SCU_TYPE_SYSCLK Sysclk);
SCU_TYPE_SYSCLK SCU_GetSysClk(void);
FlagStatus SCU_SysClkChangeBusy(void);
FlagStatus SCU_GetHRC1Flag(void);
FlagStatus SCU_GetLRCFlag(void);
FlagStatus SCU_GetHRC2Flag(void);
FlagStatus SCU_HOSCReadyFlag(void);
FlagStatus SCU_LOSCReadyFlag(void);
void SystemClockConfig(void);
void DeviceClockAllEnable(void);
void DeviceClockAllDisable(void);
void SysclkPLL(PLL_TYPE_CLK CLKx,PLL_TYPE_UNLOCK mode);
void SystemClockSelect(SCU_TYPE_SYSCLK CLKx);

#endif

/* ************************END OF FILE********************* */





