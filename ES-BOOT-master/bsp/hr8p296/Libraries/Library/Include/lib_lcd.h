/*Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd.
 *�ļ����� lib_lcd.h
 *�� �ߣ� JiangYz
 *�� ���� V1.00
 *�� �ڣ� 2016/02/02
 *�� ���� LCD������ģ��⺯��ͷ�ļ�
 *�� ע�� ������ HR8P296оƬ
 ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 ***************************************************************/

#ifndef __LIBLCDC_H__
#define __LIBLCDC_H__


#include "HR8P296.h"
#include "type.h"



/*SEG�˿ڶ���*/
typedef enum
{
	Sel_0 = 0x0 ,  //ѡ��LCDSEL0��
	Sel_1 = 0x1 ,  //ѡ��LCDSEL1��
	Sel_All=0x2,  //ѡ��LCDSEL0+LCDSEL1
}LCDSEL_TYPE;

/* �ж�ʹ�� */
typedef enum
{
	LCD_IE_OFF = 0x1 , //LCD�����ر��ж�ʹ��λ
	LCD_IE_RF = 0x2 ,  //LCD���ؼĴ���ˢ���ж�ʹ��λ 

}LCD_TYPE_IE;

/* LCDCOMS������ѡ�� */
typedef enum
{
	LCD_Coms_1 = 0,	/*  */
	LCD_Coms_2 = 4,	/* COM1~COM0*/
	LCD_Coms_3 = 6,	/* COM2~COM0*/
	LCD_Coms_4 = 1,	/* COM3~COM0*/
	LCD_Coms_6 = 2,	/* COM5~COM0*/
	LCD_Coms_8 = 3,	/* COM7~COM0*/
}LCD_TYPE_COMS;

/* LCD������������ѡ��λ */ 
typedef enum
{
	LCD_WaveMode_A = 0x0 , // A�Ͳ���
	LCD_WaveMode_B = 0x1 , // B�Ͳ���
}LCD_TYPE_WFS;

/* LCDƫ��ѡ�� */ 
typedef enum
{
	LCD_Bias_3 = 0x0 ,      // 1/3ƫ��
	LCD_Bias_4 = 0x3 ,      // 1/4ƫ��
}LCD_TYPE_BIAS;

/* LCD�ڲ�ƫ�õ���ѡ�� */
typedef enum
{
	LCD_Res_15k = 0x0 ,  // ƫѹ����15k
	LCD_Res_50k = 0x1 ,  // ƫѹ����50k
	LCD_Res_100k = 0x2 , // ƫѹ����100k
	LCD_Res_200k = 0x3 , // ƫѹ����200k
}LCD_TYPE_LCDRS; 

/* LCD��ʾ�Ҷȿ��� */ 
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
/*LCD �ڲ�ƫ��ѡ��λ*/
typedef enum
{
	LCD_BiasSel_0 =0x0,   //ƫ��ʹ��λ����Ϊ0 
	LCD_BiasSel_1 =0x1,   //ƫ��ʹ��λ����Ϊ1
	LCD_BiasSel_2 =0x2,  //ƫ��ʹ��λ����λPWM����ź�
	LCD_BiasSel_3 =0x3,  //ƫ��ʹ��λ����λPWM����ź�

}LCD_TYPE_BIAS_SEL;
/*LCD ƫ��ʹ��λPWM ���ռ�ձ�ѡ�� ����BIAS_SEL<1:0>=1x ʱ��Ч��*/
typedef enum
{
	LCD_Bias_PwmSel_0 = 0x0,  //PWMռ�ձ�1:31
	LCD_Bias_PwmSel_1 = 0x1,  //PWMռ�ձ�1:15
	LCD_Bias_PwmSel_2 = 0x2,  //PWMռ�ձ�1:7
	LCD_Bias_PwmSel_3 = 0x3,  //PWMռ�ձ�1:3
	LCD_Bias_PwmSel_4 = 0x4,  //PWMռ�ձ�1:1
	LCD_Bias_PwmSel_5 = 0x5,  //PWMռ�ձ�3:1
	LCD_Bias_PwmSel_6 = 0x6,  //PWMռ�ձ�7:1
	LCD_Bias_PwmSel_7 = 0x7,  //PWMռ�ձ�15:1
	LCD_Bias_PwmSel_8 = 0x8,  //PWMռ�ձ�31:1
}LCD_TYPE_BIAS_PWMSEL;

/* LCD���ؼĴ����ֽ�ѡ�� */ 
typedef enum
{
	LCD_Byte_0 = 0x0 , 
	LCD_Byte_1 = 0x1 , 
	LCD_Byte_2 = 0x2 , 
	LCD_Byte_3 = 0x3 , 
}LCD_DD_BYTE; 

/* LCD���ؼĴ�������ѡ�� */ 
typedef enum
{
	LCD_HalfWord_0 = 0x0 ,   //�Ͱ���
	LCD_HalfWord_1 = 0x1 ,   //�߰���
}LCD_DD_HALFWORD; 

/* LCD ���ؼĴ���ѡ��*/
typedef enum
{
	LCD_Pixel_LCDD0 = 0x0 , /* ���ؼĴ���0 */
	LCD_Pixel_LCDD1 = 0x1 , /* ���ؼĴ���1 */
	LCD_Pixel_LCDD2 = 0x2 , /* ���ؼĴ���2 */
	LCD_Pixel_LCDD3 = 0x3 , /* ���ؼĴ���3 */
	LCD_Pixel_LCDD4 = 0x4 , /* ���ؼĴ���4 */
	LCD_Pixel_LCDD5 = 0x5 , /* ���ؼĴ���5 */
	LCD_Pixel_LCDD6 = 0x6 , /* ���ؼĴ���6 */
	LCD_Pixel_LCDD7 = 0x7 , /* ���ؼĴ���7 */
	LCD_Pixel_LCDD8 = 0x8 , /* ���ؼĴ���8 */
	LCD_Pixel_LCDD9 = 0x9 , /* ���ؼĴ���9 */
	LCD_Pixel_LCDD10 = 0xA , /* ���ؼĴ���10 */
	LCD_Pixel_LCDD11 = 0xB , /* ���ؼĴ���11 */
	LCD_Pixel_LCDD12 = 0xC , /* ���ؼĴ���10 */
	LCD_Pixel_LCDD13 = 0xD , /* ���ؼĴ���11 */
}LCD_TYPE_PIXEL;

typedef struct
{
	LCD_TYPE_COMS  LCD_Coms;           // LCDCON0:bit2~0 ������ѡ��λ 
	TYPE_FUNCEN  LCD_LcdFlik;         // LCDCON0:bit5 LCD��ʾ��˸ʹ��λ  
	LCD_TYPE_WFS  LCD_LcdWFS;        // LCDCON0:bit6 LCD������������ѡ��λ
	LCD_TYPE_BIAS  LCD_Bias;        // LCDCON0:bit9~8 LCDƫ��ѡ��
	LCD_TYPE_LCDRS  LCD_RS;        // LCDCON0:bit11~10 LCD�ڲ�ƫ�õ���ѡ��
	LCD_TYPE_BVS  LCD_Grayscale;  // LCDCON0:bit14~12 LCD��ʾ�Ҷȿ���  
	LCD_TYPE_BIAS_SEL LCD_BIAS_SEL;      // LCDCON0:bit3 �ڲ�ƫ�õ�ѹʹ��λ
	LCD_TYPE_BIAS_PWMSEL LCD_BIAS_PWMSEL;//LCD ƫ��ʹ��λPWM ���ռ�ձ�ѡ��
	uint8_t  LCD_PRS;           // LCDʱ��Դ��Ƶ��ѡ��λ 0~63
	uint32_t  LCD_SEG0;          // LCDSEL0�bbit31~0 LCD��ʹ��
	uint32_t  LCD_SEG1:24;       // LCDSEL1�bbit23~0 LCD��ʹ��
}LCD_InitStruType;

/************LCDģ��궨��************/

/* LCD����ʹ�ܿ��� */
#define LCD_Enable()  (LCDC->CON1.EN= 0x1)
#define LCD_Disable() (LCDC->CON1.EN= 0x0)

/* LCD��ʾ��˸ʹ�ܿ��� */
#define LCD_FLIK_Enable()  (LCDC->CON0.FLIK = 0x1)
#define LCD_FLIK_Disable() (LCDC->CON0.FLIK = 0x0)

/* ���ؼĴ���ˢ������ */
#define LCD_SetRFREQ()   (LCDC->CON1.RFREQ = 0x1)
#define LCD_ClearRFREQ() (LCDC->CON1.RFREQ = 0x0)

/* ��LCD�����ر��жϱ�־ */
#define LCD_ClearOFFIF() ( LCDC->IF.OFFIF = 0x1)

/* ��LCD���ؼĴ���ˢ���жϱ�־ */
#define LCD_ClearRFIF() (LCDC->IF.RFIF = 0x1)

/* LCD�����λ */
#define LCD_Rest() (LCDC->CON1.RST = 0x01)

/************LCDģ�麯������***********/
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
