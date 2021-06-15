/***************************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
 *文件名：  lib_lcd.c
 *作  者：  JiangYz
 *版  本：  V1.00
 *日  期：  2016/02/02
 *描  述：  LCD控制器模块库函数
 *备  注：  适用于 HR8P296芯片
 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
 ***************************************************************/

#include "lib_lcd.h"
#include "system_HR8P296.h"
//#include "Lib_Timer.h"


/***************************************************************
  函数名：LCD_Init
  描  述：初始化LCD模块
  输入值：初始化配置结构体地址,SELx:选择的LCD段
  输出值：无
  返回值：成功、失败
 ***************************************************************/

ErrorStatus LCD_Init(LCD_InitStruType * LCD_InitStruct,LCDSEL_TYPE SELx)
{
	if(LCD_InitStruct->LCD_PRS>63)
	{
		return ERROR;
	}
	else
	{

		LCDC->CON0.PRS   = LCD_InitStruct->LCD_PRS;  // LCD时钟源分频比选择位 0~63
	}

	switch(SELx)
	{
		case Sel_0:                                           //选择SEL0段
			if(LCD_InitStruct->LCD_SEG0>0xFFFFFFFF)
			{
				return ERROR;
			}
			else
			{
				LCDC->SEL0.SEG  = LCD_InitStruct->LCD_SEG0; //SEL0段使能

			}
			break;
		case Sel_1:                                      //选择SEL1段
			if(LCD_InitStruct->LCD_SEG1>0x00FFFFFF)
			{
				return ERROR;
			}
			else
			{
				LCDC->SEL1.SEG  = LCD_InitStruct->LCD_SEG1;  // SEL1段使能


			}
			break;
		case Sel_All:                                  //选择SEL0+SEL1段
			if((LCD_InitStruct->LCD_SEG0>0xFFFFFFFF)||(LCD_InitStruct->LCD_SEG1>0x00FFFFFF))
			{
				return ERROR;
			}
			else
			{
				LCDC->SEL0.SEG  = LCD_InitStruct->LCD_SEG0; 
				LCDC->SEL1.SEG  = LCD_InitStruct->LCD_SEG1; 

			}
		default: break;

	}

	LCDC->CON0.COMS = LCD_InitStruct->LCD_Coms;       // 公共端选择位 

	LCDC->CON0.FLIK  = LCD_InitStruct->LCD_LcdFlik;   // LCD显示闪烁使能位 

	LCDC->CON0.WFS  = LCD_InitStruct->LCD_LcdWFS ;     // LCD驱动波形类型选择位

	LCDC->CON0.BIAS  = LCD_InitStruct->LCD_Bias;      // LCD偏置选择

	LCDC->CON0.RS  = LCD_InitStruct->LCD_RS;          // LCD内部偏置电阻选择

	LCDC->CON0.BVS  = LCD_InitStruct->LCD_Grayscale;  // LCD显示灰度控制 

	LCDC->CON0.BIAS_SEL=LCD_InitStruct->LCD_BIAS_SEL;  //内部偏置电压使能位

	LCDC->CON0.BIAS_PWMSEL=LCD_InitStruct->LCD_BIAS_PWMSEL;//LCD 偏置使能位PWM 输出占空比选择

	return SUCCESS;
}

/***************************************************************
  函数名：LCD_ITConfig
  描  述：LCD中断配置
  输入值：LCD_IT：需要配置的中断 、 NewState：使能或关闭
  输出值：无
  返回值：无
 ***************************************************************/
void LCD_ITConfig(LCD_TYPE_IE LCD_IT,TYPE_FUNCEN NewState)
{
	if (NewState != DISABLE)
	{
		LCDC->IF.Word |= (uint32_t)LCD_IT;
	}
	else
	{
		LCDC->IF.Word &= ~((uint32_t)LCD_IT);
	}
}

/***************************************************************
  函数名：LCD_GetRFITStatus
  描  述：读取LCD像素刷新中断标志
  输入值：无
  输出值：无
  返回值：SET（中断）/RESET（无中断）
 ***************************************************************/
ITStatus LCD_GetRFITStatus(void)
{
	ITStatus bitstatus = RESET;
	/* 检查中断标志位 */
	if ((LCDC->IF.RFIF != (uint32_t)RESET))
	{

		bitstatus = SET;   //转换完成，进中断
	}
	else
	{

		bitstatus = RESET; //无中断
	}
	return  bitstatus;
}

/***************************************************************
  函数名：LCD_GetOFFITStatus
  描  述：读取LCD驱动关闭中断标志
  输入值：无
  输出值：无
  返回值：SET（中断）/RESET（无中断）
 ***************************************************************/
ITStatus LCD_GetOFFITStatus(void)
{
	ITStatus bitstatus = RESET;
	/* 检查中断标志位 */
	if ((LCDC->IF.OFFIF != (uint32_t)RESET))
	{
		bitstatus = SET;    //转换完成，进中断
	}
	else
	{
		bitstatus = RESET;  //无中断
	}
	return  bitstatus;
}

/***************************************************************
  函数名：LCD_GrayscaleConfig
  描  述：LCD灰度选择
  输入值：灰度电压值
  输出值：无
  返回值：无
 ***************************************************************/
void LCD_GrayscaleConfig(LCD_TYPE_BVS LCD_Grayscale )
{
	/* LCD显示灰度控制 */   
	LCDC->CON0.BVS  = LCD_Grayscale;
}

/***************************************************************
  函数名：LCD_FlickerTimeConfig
  描  述：闪烁时间设置
  输入值：On_Time：闪烁时间/0.25s 、 Off_Time：熄灭时间/0.25s
  输出值：无
  返回值：成功、失败
 ***************************************************************/
ErrorStatus LCD_FlickerTimeConfig(uint8_t On_Time,uint8_t Off_Time)
{

	if(On_Time>255 || Off_Time>255)
	{
		return ERROR;			
	}

	LCDC->FLKT.TON  = On_Time;    //LCD点亮时间

	LCDC->FLKT.TOFF   = Off_Time;   //LCD熄灭时间	
	return SUCCESS;
}

/***************************************************************
  函数名：LCD_PixelWriteByte
  描  述：LCD像素寄存器 一次写8bit数据
  输入值：LCD_DD:LCD像素寄存器名称 、 nByte：该像素寄存器的第几个字节 、 LCD_data：对应数据
  输出值：无
  返回值：成功、失败
 ***************************************************************/
ErrorStatus  LCD_PixelWriteByte(LCD_TYPE_PIXEL LCD_DD, LCD_DD_BYTE nByte ,uint8_t LCD_data)
{
	if(LCD_DD>13||nByte >3  )
	{
		return ERROR;
	}
	switch (LCD_DD)
	{
		case LCD_Pixel_LCDD0:
			LCDC->D0.LCDD0=(LCDC->D0.LCDD0&(~(0X000000FF<<(nByte*8))))|(((uint32_t)LCD_data)<<(nByte*8));  //把8bit数据写入到LCD像素寄存器的第nByte字节
			break;
		case LCD_Pixel_LCDD1:
			LCDC->D1.LCDD1=(LCDC->D1.LCDD1&(~(0X000000FF<<(nByte*8))))|(((uint32_t)LCD_data)<<(nByte*8));
			break;
		case LCD_Pixel_LCDD2:
			LCDC->D2.LCDD2=(LCDC->D2.LCDD2&(~(0X000000FF<<(nByte*8))))|(((uint32_t)LCD_data)<<(nByte*8));
			break;
		case LCD_Pixel_LCDD3:
			LCDC->D3.LCDD3=(LCDC->D3.LCDD3&(~(0X000000FF<<(nByte*8))))|(((uint32_t)LCD_data)<<(nByte*8));
			break;
		case LCD_Pixel_LCDD4:
			LCDC->D4.LCDD4=(LCDC->D4.LCDD4&(~(0X000000FF<<(nByte*8))))|(((uint32_t)LCD_data)<<(nByte*8));
			break;
		case LCD_Pixel_LCDD5:
			LCDC->D5.LCDD5=(LCDC->D5.LCDD5&(~(0X000000FF<<(nByte*8))))|(((uint32_t)LCD_data)<<(nByte*8));
			break;		
		case LCD_Pixel_LCDD6:
			LCDC->D6.LCDD6=(LCDC->D6.LCDD6&(~(0X000000FF<<(nByte*8))))|(((uint32_t)LCD_data)<<(nByte*8));
			break;
		case LCD_Pixel_LCDD7:
			LCDC->D7.LCDD7=(LCDC->D7.LCDD7&(~(0X000000FF<<(nByte*8))))|(((uint32_t)LCD_data)<<(nByte*8));
			break;
		case LCD_Pixel_LCDD8:
			LCDC->D8.LCDD8=(LCDC->D8.LCDD8&(~(0X000000FF<<(nByte*8))))|(((uint32_t)LCD_data)<<(nByte*8));
			break;
		case LCD_Pixel_LCDD9:
			LCDC->D9.LCDD9=(LCDC->D9.LCDD9&(~(0X000000FF<<(nByte*8))))|(((uint32_t)LCD_data)<<(nByte*8));
			break;
		case LCD_Pixel_LCDD10:
			LCDC->D10.LCDD10=(LCDC->D10.LCDD10&(~(0X000000FF<<(nByte*8))))|(((uint32_t)LCD_data)<<(nByte*8));
			break;
		case LCD_Pixel_LCDD11:
			LCDC->D11.LCDD11=(LCDC->D11.LCDD11&(~(0X000000FF<<(nByte*8))))|(((uint32_t)LCD_data)<<(nByte*8));
			break;		
		case LCD_Pixel_LCDD12:
			LCDC->D12.LCDD12=(LCDC->D12.LCDD12&(~(0X000000FF<<(nByte*8))))|(((uint32_t)LCD_data)<<(nByte*8));
			break;		
		case LCD_Pixel_LCDD13:
			LCDC->D13.LCDD13=(LCDC->D13.LCDD13&(~(0X000000FF<<(nByte*8))))|(((uint32_t)LCD_data)<<(nByte*8));
			break;	
		default:
			return 	ERROR;			
	}
	return SUCCESS;
}


/***************************************************************
  函数名：LCD_PixelWriteHalfWord
  描  述：LCD 一次写16bit数据
  输入值：LCD_DD 	像素寄存器选择
  nHalfWord 	高/低6bit选择
  LCD_data		写入的16bit数据
  输出值：无
  返回值：SUCCESS 成功
  ERROR 失败
 ***************************************************************/
ErrorStatus  LCD_PixelWriteHalfWord(LCD_TYPE_PIXEL LCD_DD, LCD_DD_HALFWORD nHalfWord , uint16_t LCD_data)
{
	if( LCD_DD>13 || nHalfWord >1)
	{
		return ERROR;
	}
	switch (LCD_DD)
	{
		case LCD_Pixel_LCDD0:
			LCDC->D0.LCDD0=(LCDC->D0.LCDD0&(~(0X0000FFFF<<(nHalfWord*16))))|(((uint32_t)LCD_data)<<(nHalfWord*16)); //把16bit数据写入到LCD像素寄存器的高16位/低16位
			break;
		case LCD_Pixel_LCDD1:
			LCDC->D1.LCDD1=(LCDC->D1.LCDD1&(~(0X0000FFFF<<(nHalfWord*16))))|(((uint32_t)LCD_data)<<(nHalfWord*16));
			break;
		case LCD_Pixel_LCDD2:
			LCDC->D2.LCDD2=(LCDC->D2.LCDD2&(~(0X0000FFFF<<(nHalfWord*16))))|(((uint32_t)LCD_data)<<(nHalfWord*16));
			break;
		case LCD_Pixel_LCDD3:
			LCDC->D3.LCDD3=(LCDC->D3.LCDD3&(~(0X0000FFFF<<(nHalfWord*16))))|(((uint32_t)LCD_data)<<(nHalfWord*16));
			break;
		case LCD_Pixel_LCDD4:
			LCDC->D4.LCDD4=(LCDC->D4.LCDD4&(~(0X0000FFFF<<(nHalfWord*16))))|(((uint32_t)LCD_data)<<(nHalfWord*16));
			break;
		case LCD_Pixel_LCDD5:
			LCDC->D5.LCDD5=(LCDC->D5.LCDD5&(~(0X0000FFFF<<(nHalfWord*16))))|(((uint32_t)LCD_data)<<(nHalfWord*16));
			break;		
		case LCD_Pixel_LCDD6:
			LCDC->D6.LCDD6=(LCDC->D6.LCDD6&(~(0X0000FFFF<<(nHalfWord*16))))|(((uint32_t)LCD_data)<<(nHalfWord*16));
			break;
		case LCD_Pixel_LCDD7:
			LCDC->D7.LCDD7=(LCDC->D7.LCDD7&(~(0X0000FFFF<<(nHalfWord*16))))|(((uint32_t)LCD_data)<<(nHalfWord*16));
			break;
		case LCD_Pixel_LCDD8:
			LCDC->D8.LCDD8=(LCDC->D8.LCDD8&(~(0X0000FFFF<<(nHalfWord*16))))|(((uint32_t)LCD_data)<<(nHalfWord*16));
			break;
		case LCD_Pixel_LCDD9:
			LCDC->D9.LCDD9=(LCDC->D9.LCDD9&(~(0X0000FFFF<<(nHalfWord*16))))|(((uint32_t)LCD_data)<<(nHalfWord*16));
			break;
		case LCD_Pixel_LCDD10:
			LCDC->D10.LCDD10=(LCDC->D10.LCDD10&(~(0X0000FFFF<<(nHalfWord*16))))|(((uint32_t)LCD_data)<<(nHalfWord*16));
			break;
		case LCD_Pixel_LCDD11:
			LCDC->D11.LCDD11=(LCDC->D11.LCDD11&(~(0X0000FFFF<<(nHalfWord*16))))|(((uint32_t)LCD_data)<<(nHalfWord*16));
			break;	
		case LCD_Pixel_LCDD12:
			LCDC->D12.LCDD12=(LCDC->D12.LCDD12&(~(0X0000FFFF<<(nHalfWord*16))))|(((uint32_t)LCD_data)<<(nHalfWord*16));
			break;	
		case LCD_Pixel_LCDD13:
			LCDC->D13.LCDD13=(LCDC->D13.LCDD13&(~(0X0000FFFF<<(nHalfWord*16))))|(((uint32_t)LCD_data)<<(nHalfWord*16));
			break;				
		default: 
			return 	ERROR;
	}
	return SUCCESS;
}

/***************************************************************
  函数名：LCD_PixelWriteWord
  描  述：LCD 一次写32bit数据
  输入值：LCD_DD 	像素寄存器选择
  LCD_data		写入的32bit数据
  输出值：无
  返回值：SUCCESS 成功
  ERROR 失败
 ***************************************************************/
ErrorStatus  LCD_PixelWriteWord(LCD_TYPE_PIXEL LCD_DD,uint32_t LCD_data)
{
	if( LCD_DD>13 )
	{
		return ERROR;
	}
	switch (LCD_DD)
	{
		case LCD_Pixel_LCDD0:
			LCDC->D0.LCDD0=(uint32_t)LCD_data;  //把32bit数据写入到LCD像素寄存器
			break;
		case LCD_Pixel_LCDD1:
			LCDC->D1.LCDD1=(uint32_t)LCD_data;
			break;
		case LCD_Pixel_LCDD2:
			LCDC->D2.LCDD2=(uint32_t)LCD_data;
			break;
		case LCD_Pixel_LCDD3:
			LCDC->D3.LCDD3=(uint32_t)LCD_data;
			break;
		case LCD_Pixel_LCDD4:
			LCDC->D4.LCDD4=(uint32_t)LCD_data;
			break;
		case LCD_Pixel_LCDD5:
			LCDC->D5.LCDD5=(uint32_t)LCD_data;
			break;		
		case LCD_Pixel_LCDD6:
			LCDC->D6.LCDD6=(uint32_t)LCD_data;
			break;
		case LCD_Pixel_LCDD7:
			LCDC->D7.LCDD7=(uint32_t)LCD_data;
			break;
		case LCD_Pixel_LCDD8:
			LCDC->D8.LCDD8=(uint32_t)LCD_data;
			break;
		case LCD_Pixel_LCDD9:
			LCDC->D9.LCDD9=(uint32_t)LCD_data;
			break;
		case LCD_Pixel_LCDD10:
			LCDC->D10.LCDD10=(uint32_t)LCD_data;
			break;
		case LCD_Pixel_LCDD11:
			LCDC->D11.LCDD11=(uint32_t)LCD_data;
			break;		
		case LCD_Pixel_LCDD12:
			LCDC->D12.LCDD12=(uint32_t)LCD_data;
			break;		
		case LCD_Pixel_LCDD13:
			LCDC->D13.LCDD13=(uint32_t)LCD_data;
			break;
		default:
			return 	ERROR;
	}
	return SUCCESS;
}



/*************************END OF FILE**********************/
