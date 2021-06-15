/*Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd.
 *文件名： lib_lcd.h
 *作 者： JiangYz
 *版 本： V1.00
 *日 期： 2016/02/02
 *描 述： LCD控制器模块库函数头文件
 *备 注： 适用于 HR8P296芯片
 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
 ***************************************************************/

#ifndef __LIBLCDC_H__
#define __LIBLCDC_H__


#include "HR8P296.h"
#include "type.h"



/*SEG端口定义*/
typedef enum
{
	Sel_0 = 0x0 ,  //选择LCDSEL0段
	Sel_1 = 0x1 ,  //选择LCDSEL1段
	Sel_All=0x2,  //选择LCDSEL0+LCDSEL1
}LCDSEL_TYPE;

/* 中断使能 */
typedef enum
{
	LCD_IE_OFF = 0x1 , //LCD驱动关闭中断使能位
	LCD_IE_RF = 0x2 ,  //LCD像素寄存器刷新中断使能位 

}LCD_TYPE_IE;

/* LCDCOMS公共端选择 */
typedef enum
{
	LCD_Coms_1 = 0,	/*  */
	LCD_Coms_2 = 4,	/* COM1~COM0*/
	LCD_Coms_3 = 6,	/* COM2~COM0*/
	LCD_Coms_4 = 1,	/* COM3~COM0*/
	LCD_Coms_6 = 2,	/* COM5~COM0*/
	LCD_Coms_8 = 3,	/* COM7~COM0*/
}LCD_TYPE_COMS;

/* LCD驱动波形类型选择位 */ 
typedef enum
{
	LCD_WaveMode_A = 0x0 , // A型波形
	LCD_WaveMode_B = 0x1 , // B型波形
}LCD_TYPE_WFS;

/* LCD偏置选择 */ 
typedef enum
{
	LCD_Bias_3 = 0x0 ,      // 1/3偏置
	LCD_Bias_4 = 0x3 ,      // 1/4偏置
}LCD_TYPE_BIAS;

/* LCD内部偏置电阻选择 */
typedef enum
{
	LCD_Res_15k = 0x0 ,  // 偏压电阻15k
	LCD_Res_50k = 0x1 ,  // 偏压电阻50k
	LCD_Res_100k = 0x2 , // 偏压电阻100k
	LCD_Res_200k = 0x3 , // 偏压电阻200k
}LCD_TYPE_LCDRS; 

/* LCD显示灰度控制 */ 
typedef enum
{
	LCD_Grayscale_3v6 = 0x0 , // Vbias = 3.6V
	LCD_Grayscale_3v5 = 0x1 , // Vbias = 3.5V
	LCD_Grayscale_3v4 = 0x2 , // Vbias = 3.4V
	LCD_Grayscale_3v2 = 0x3 , // Vbias = 3.2V
	LCD_Grayscale_3v1 = 0x4 , // Vbias = 3.1V
	LCD_Grayscale_3v0 = 0x5 , // Vbias = 3.0V
	LCD_Grayscale_2v9 = 0x6 , // Vbias = 2.9V
	LCD_Grayscale_2v8 = 0x7 , // Vbias = 2.8V
}LCD_TYPE_BVS;
/*LCD 内部偏置选择位*/
typedef enum
{
	LCD_BiasSel_0 =0x0,   //偏置使能位设置为0 
	LCD_BiasSel_1 =0x1,   //偏置使能位设置为1
	LCD_BiasSel_2 =0x2,  //偏置使能位设置位PWM输出信号
	LCD_BiasSel_3 =0x3,  //偏置使能位设置位PWM输出信号

}LCD_TYPE_BIAS_SEL;
/*LCD 偏置使能位PWM 输出占空比选择（ 仅当BIAS_SEL<1:0>=1x 时有效）*/
typedef enum
{
	LCD_Bias_PwmSel_0 = 0x0,  //PWM占空比1:31
	LCD_Bias_PwmSel_1 = 0x1,  //PWM占空比1:15
	LCD_Bias_PwmSel_2 = 0x2,  //PWM占空比1:7
	LCD_Bias_PwmSel_3 = 0x3,  //PWM占空比1:3
	LCD_Bias_PwmSel_4 = 0x4,  //PWM占空比1:1
	LCD_Bias_PwmSel_5 = 0x5,  //PWM占空比3:1
	LCD_Bias_PwmSel_6 = 0x6,  //PWM占空比7:1
	LCD_Bias_PwmSel_7 = 0x7,  //PWM占空比15:1
	LCD_Bias_PwmSel_8 = 0x8,  //PWM占空比31:1
}LCD_TYPE_BIAS_PWMSEL;

/* LCD像素寄存器字节选择 */ 
typedef enum
{
	LCD_Byte_0 = 0x0 , 
	LCD_Byte_1 = 0x1 , 
	LCD_Byte_2 = 0x2 , 
	LCD_Byte_3 = 0x3 , 
}LCD_DD_BYTE; 

/* LCD像素寄存器半字选择 */ 
typedef enum
{
	LCD_HalfWord_0 = 0x0 ,   //低半字
	LCD_HalfWord_1 = 0x1 ,   //高半字
}LCD_DD_HALFWORD; 

/* LCD 像素寄存器选择*/
typedef enum
{
	LCD_Pixel_LCDD0 = 0x0 , /* 像素寄存器0 */
	LCD_Pixel_LCDD1 = 0x1 , /* 像素寄存器1 */
	LCD_Pixel_LCDD2 = 0x2 , /* 像素寄存器2 */
	LCD_Pixel_LCDD3 = 0x3 , /* 像素寄存器3 */
	LCD_Pixel_LCDD4 = 0x4 , /* 像素寄存器4 */
	LCD_Pixel_LCDD5 = 0x5 , /* 像素寄存器5 */
	LCD_Pixel_LCDD6 = 0x6 , /* 像素寄存器6 */
	LCD_Pixel_LCDD7 = 0x7 , /* 像素寄存器7 */
	LCD_Pixel_LCDD8 = 0x8 , /* 像素寄存器8 */
	LCD_Pixel_LCDD9 = 0x9 , /* 像素寄存器9 */
	LCD_Pixel_LCDD10 = 0xA , /* 像素寄存器10 */
	LCD_Pixel_LCDD11 = 0xB , /* 像素寄存器11 */
	LCD_Pixel_LCDD12 = 0xC , /* 像素寄存器10 */
	LCD_Pixel_LCDD13 = 0xD , /* 像素寄存器11 */
}LCD_TYPE_PIXEL;

typedef struct
{
	LCD_TYPE_COMS  LCD_Coms;           // LCDCON0:bit2~0 公共端选择位 
	TYPE_FUNCEN  LCD_LcdFlik;         // LCDCON0:bit5 LCD显示闪烁使能位  
	LCD_TYPE_WFS  LCD_LcdWFS;        // LCDCON0:bit6 LCD驱动波形类型选择位
	LCD_TYPE_BIAS  LCD_Bias;        // LCDCON0:bit9~8 LCD偏置选择
	LCD_TYPE_LCDRS  LCD_RS;        // LCDCON0:bit11~10 LCD内部偏置电阻选择
	LCD_TYPE_BVS  LCD_Grayscale;  // LCDCON0:bit14~12 LCD显示灰度控制  
	LCD_TYPE_BIAS_SEL LCD_BIAS_SEL;      // LCDCON0:bit3 内部偏置电压使能位
	LCD_TYPE_BIAS_PWMSEL LCD_BIAS_PWMSEL;//LCD 偏置使能位PWM 输出占空比选择
	uint8_t  LCD_PRS;           // LCD时钟源分频比选择位 0~63
	uint32_t  LCD_SEG0;          // LCDSEL0bit31~0 LCD段使能
	uint32_t  LCD_SEG1:24;       // LCDSEL1bit23~0 LCD段使能
}LCD_InitStruType;

/************LCD模块宏定义************/

/* LCD驱动使能控制 */
#define LCD_Enable()  (LCDC->CON1.EN= 0x1)
#define LCD_Disable() (LCDC->CON1.EN= 0x0)

/* LCD显示闪烁使能控制 */
#define LCD_FLIK_Enable()  (LCDC->CON0.FLIK = 0x1)
#define LCD_FLIK_Disable() (LCDC->CON0.FLIK = 0x0)

/* 像素寄存器刷新请求 */
#define LCD_SetRFREQ()   (LCDC->CON1.RFREQ = 0x1)
#define LCD_ClearRFREQ() (LCDC->CON1.RFREQ = 0x0)

/* 清LCD驱动关闭中断标志 */
#define LCD_ClearOFFIF() ( LCDC->IF.OFFIF = 0x1)

/* 清LCD像素寄存器刷新中断标志 */
#define LCD_ClearRFIF() (LCDC->IF.RFIF = 0x1)

/* LCD软件复位 */
#define LCD_Rest() (LCDC->CON1.RST = 0x01)

/************LCD模块函数声明***********/
ErrorStatus LCD_Init(LCD_InitStruType * LCD_InitStruct,LCDSEL_TYPE SELx);
void LCD_ITConfig(LCD_TYPE_IE LCD_IT,TYPE_FUNCEN NewState);
ITStatus LCD_GetRFITStatus(void);
ITStatus LCD_GetOFFITStatus(void);
void LCD_GrayscaleConfig(LCD_TYPE_BVS LCD_Grayscale );
ErrorStatus LCD_FlickerTimeConfig(uint8_t On_Time,uint8_t Off_Time);
ErrorStatus  LCD_PixelWriteByte(LCD_TYPE_PIXEL LCD_DD, LCD_DD_BYTE nByte ,uint8_t LCD_data);
ErrorStatus  LCD_PixelWriteHalfWord(LCD_TYPE_PIXEL LCD_DD, LCD_DD_HALFWORD nHalfWord , uint16_t LCD_data);
ErrorStatus  LCD_PixelWriteWord(LCD_TYPE_PIXEL LCD_DD,uint32_t LCD_data);


#endif



/*************************END OF FILE**********************/
