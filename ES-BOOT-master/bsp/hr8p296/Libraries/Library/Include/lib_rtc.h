/*********************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd.
 *�ļ���:  lib_rtc.h
 *��  ��:  WangMX
 *��  ��:  V1.00
 *��  ��:  2014/11/11
 *��  ��:  RTCģ�����ͷ�ļ�
 *��  ע:  ������HRSDK-GDB-HR8P296 V1.1
 ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 **********************************************************/
#ifndef __LIBRTC_H__
#define __LIBRTC_H__

#include "lib_config.h"

/*RTCʱ��Դѡ��*/
typedef enum
{
    RTC_LOSC = 0x0,  //�ⲿ32768HZʱ��
    RTC_LRC  = 0x1,  //�ڲ�LRCʱ��
}RTC_TYPE_CLKS;

/*RTCʱ��12/24Сʱ��ѡ��*/
typedef enum
{
    RTC_HOUR12 = 0X0, //12Сʱ��
    RTC_HOUR24 = 0X1, //24Сʱ��
}RTC_TYPE_TIME;

/* RTC�ж�Դѡ�� */
typedef enum 
{
    RTC_Interrupt_Source_Second = 0,    // ���ж�
    RTC_Interrupt_Source_Minute = 1,    // ���ж�
    RTC_Interrupt_Source_Hour = 2,      // Сʱ�ж�
    RTC_Interrupt_Source_Day = 3,       // ���ж�
    RTC_Interrupt_Source_Month = 4,     // ���ж�
    RTC_Interrupt_Source_DayALE = 5,    // �������ж�
    RTC_Interrupt_Source_WeekALE = 6,   // �������ж�
    RTC_Interrupt_Source_OSC = 7,   // ����ͣ���ж�
} RTC_Interrupt_Source;

/********************* RTCģ�麯������ *********************/
void RTC_Init(RTC_TYPE_CLKS CLKx,RTC_TYPE_TIME HOURx);
uint32_t RTC_ReadHourmode(void);
uint32_t RTC_ReadSecond(void);
uint32_t RTC_ReadMinute(void);
uint32_t RTC_ReadHour(uint32_t *meridiem);
uint32_t RTC_ReadDay(void);
uint32_t RTC_ReadMonth(void);
uint32_t RTC_ReadYear(void);
uint32_t RTC_ReadWeek(void);

ErrorStatus RTC_WriteSecond(uint32_t second);
ErrorStatus RTC_WriteMinute(uint32_t minute);
ErrorStatus RTC_WriteHour(uint32_t hour, uint32_t meridiem);
ErrorStatus RTC_WriteDay(uint32_t day);
ErrorStatus RTC_WriteMonth(uint32_t month);
ErrorStatus RTC_WriteYear(uint32_t year);
ErrorStatus RTC_WriteWeek(uint32_t week);

uint32_t RTC_ReadWeekAlarmMinute(void);
uint32_t RTC_ReadWeekAlarmHour(uint32_t *meridiem);
uint32_t RTC_ReadWeekAlarmWeek(void);
uint32_t RTC_ReadDayAlarmMinute(void);
uint32_t RTC_ReadDayAlarmHour(uint32_t *meridiem);

ErrorStatus RTC_WriteWeekAlarmMinute(uint32_t minute);
ErrorStatus RTC_WriteWeekAlarmHour(uint32_t hour, uint32_t meridiem);
ErrorStatus RTC_WriteWeekAlarmWeek(uint32_t week);
ErrorStatus RTC_WriteDayAlarmMinute(uint32_t minute);
ErrorStatus RTC_WriteDayAlarmHour(uint32_t hour, uint32_t meridiem);

void RTC_InterruptEnable(RTC_Interrupt_Source src);
void RTC_InterruptDisable(RTC_Interrupt_Source src);
ITStatus RTC_GetITStatus(RTC_Interrupt_Source src);
FlagStatus RTC_GetFlagStatus(RTC_Interrupt_Source src);
void RTC_ClearAllITFlag(void);

#endif
