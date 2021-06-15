/***************************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
 *�ļ�����  lib_lcd.c
 *��  �ߣ�  JiangYz
 *��  ����  V1.00
 *��  �ڣ�  2016/02/02
 *��  ����  LCD������ģ��⺯��
 *��  ע��  ������ HR8P296оƬ
 ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 ***************************************************************/

#include "lib_lcd.h"
#include "system_HR8P296.h"
//#include "Lib_Timer.h"


/***************************************************************
  ��������LCD_Init
  ��  ������ʼ��LCDģ��
  ����ֵ����ʼ�����ýṹ���ַ,SELx:ѡ���LCD��
  ���ֵ����
  ����ֵ���ɹ���ʧ��
 ***************************************************************/

ErrorStatus LCD_Init(LCD_InitStruType * LCD_InitStruct,LCDSEL_TYPE SELx)
{
	if(LCD_InitStruct->LCD_PRS>63)
	{
		return ERROR;
	}
	else
	{

		LCDC->CON0.PRS   = LCD_InitStruct->LCD_PRS;  // LCDʱ��Դ��Ƶ��ѡ��λ 0~63
	}

	switch(SELx)
	{
		case Sel_0:                                           //ѡ��SEL0��
			if(LCD_InitStruct->LCD_SEG0>0xFFFFFFFF)
			{
				return ERROR;
			}
			else
			{
				LCDC->SEL0.SEG  = LCD_InitStruct->LCD_SEG0; //SEL0��ʹ��

			}
			break;
		case Sel_1:                                      //ѡ��SEL1��
			if(LCD_InitStruct->LCD_SEG1>0x00FFFFFF)
			{
				return ERROR;
			}
			else
			{
				LCDC->SEL1.SEG  = LCD_InitStruct->LCD_SEG1;  // SEL1��ʹ��


			}
			break;
		case Sel_All:                                  //ѡ��SEL0+SEL1��
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

	LCDC->CON0.COMS = LCD_InitStruct->LCD_Coms;       // ������ѡ��λ 

	LCDC->CON0.FLIK  = LCD_InitStruct->LCD_LcdFlik;   // LCD��ʾ��˸ʹ��λ 

	LCDC->CON0.WFS  = LCD_InitStruct->LCD_LcdWFS ;     // LCD������������ѡ��λ

	LCDC->CON0.BIAS  = LCD_InitStruct->LCD_Bias;      // LCDƫ��ѡ��

	LCDC->CON0.RS  = LCD_InitStruct->LCD_RS;          // LCD�ڲ�ƫ�õ���ѡ��

	LCDC->CON0.BVS  = LCD_InitStruct->LCD_Grayscale;  // LCD��ʾ�Ҷȿ��� 

	LCDC->CON0.BIAS_SEL=LCD_InitStruct->LCD_BIAS_SEL;  //�ڲ�ƫ�õ�ѹʹ��λ

	LCDC->CON0.BIAS_PWMSEL=LCD_InitStruct->LCD_BIAS_PWMSEL;//LCD ƫ��ʹ��λPWM ���ռ�ձ�ѡ��

	return SUCCESS;
}

/***************************************************************
  ��������LCD_ITConfig
  ��  ����LCD�ж�����
  ����ֵ��LCD_IT����Ҫ���õ��ж� �� NewState��ʹ�ܻ�ر�
  ���ֵ����
  ����ֵ����
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
  ��������LCD_GetRFITStatus
  ��  ������ȡLCD����ˢ���жϱ�־
  ����ֵ����
  ���ֵ����
  ����ֵ��SET���жϣ�/RESET�����жϣ�
 ***************************************************************/
ITStatus LCD_GetRFITStatus(void)
{
	ITStatus bitstatus = RESET;
	/* ����жϱ�־λ */
	if ((LCDC->IF.RFIF != (uint32_t)RESET))
	{

		bitstatus = SET;   //ת����ɣ����ж�
	}
	else
	{

		bitstatus = RESET; //���ж�
	}
	return  bitstatus;
}

/***************************************************************
  ��������LCD_GetOFFITStatus
  ��  ������ȡLCD�����ر��жϱ�־
  ����ֵ����
  ���ֵ����
  ����ֵ��SET���жϣ�/RESET�����жϣ�
 ***************************************************************/
ITStatus LCD_GetOFFITStatus(void)
{
	ITStatus bitstatus = RESET;
	/* ����жϱ�־λ */
	if ((LCDC->IF.OFFIF != (uint32_t)RESET))
	{
		bitstatus = SET;    //ת����ɣ����ж�
	}
	else
	{
		bitstatus = RESET;  //���ж�
	}
	return  bitstatus;
}

/***************************************************************
  ��������LCD_GrayscaleConfig
  ��  ����LCD�Ҷ�ѡ��
  ����ֵ���Ҷȵ�ѹֵ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void LCD_GrayscaleConfig(LCD_TYPE_BVS LCD_Grayscale )
{
	/* LCD��ʾ�Ҷȿ��� */   
	LCDC->CON0.BVS  = LCD_Grayscale;
}

/***************************************************************
  ��������LCD_FlickerTimeConfig
  ��  ������˸ʱ������
  ����ֵ��On_Time����˸ʱ��/0.25s �� Off_Time��Ϩ��ʱ��/0.25s
  ���ֵ����
  ����ֵ���ɹ���ʧ��
 ***************************************************************/
ErrorStatus LCD_FlickerTimeConfig(uint8_t On_Time,uint8_t Off_Time)
{

	if(On_Time>255 || Off_Time>255)
	{
		return ERROR;			
	}

	LCDC->FLKT.TON  = On_Time;    //LCD����ʱ��

	LCDC->FLKT.TOFF   = Off_Time;   //LCDϨ��ʱ��	
	return SUCCESS;
}

/***************************************************************
  ��������LCD_PixelWriteByte
  ��  ����LCD���ؼĴ��� һ��д8bit����
  ����ֵ��LCD_DD:LCD���ؼĴ������� �� nByte�������ؼĴ����ĵڼ����ֽ� �� LCD_data����Ӧ����
  ���ֵ����
  ����ֵ���ɹ���ʧ��
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
			LCDC->D0.LCDD0=(LCDC->D0.LCDD0&(~(0X000000FF<<(nByte*8))))|(((uint32_t)LCD_data)<<(nByte*8));  //��8bit����д�뵽LCD���ؼĴ����ĵ�nByte�ֽ�
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
  ��������LCD_PixelWriteHalfWord
  ��  ����LCD һ��д16bit����
  ����ֵ��LCD_DD 	���ؼĴ���ѡ��
  nHalfWord 	��/��6bitѡ��
  LCD_data		д���16bit����
  ���ֵ����
  ����ֵ��SUCCESS �ɹ�
  ERROR ʧ��
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
			LCDC->D0.LCDD0=(LCDC->D0.LCDD0&(~(0X0000FFFF<<(nHalfWord*16))))|(((uint32_t)LCD_data)<<(nHalfWord*16)); //��16bit����д�뵽LCD���ؼĴ����ĸ�16λ/��16λ
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
  ��������LCD_PixelWriteWord
  ��  ����LCD һ��д32bit����
  ����ֵ��LCD_DD 	���ؼĴ���ѡ��
  LCD_data		д���32bit����
  ���ֵ����
  ����ֵ��SUCCESS �ɹ�
  ERROR ʧ��
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
			LCDC->D0.LCDD0=(uint32_t)LCD_data;  //��32bit����д�뵽LCD���ؼĴ���
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
