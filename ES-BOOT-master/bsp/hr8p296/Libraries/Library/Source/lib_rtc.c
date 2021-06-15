/*********************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd.
 *�ļ���:  lib_rtc.c
 *��  ��:  WangMX
 *��  ��:  V1.00
 *��  ��:  2014/11/11
 *��  ��:  SysTickģ�����
 *��  ע:  ������HRSDK-GDB-HR8P296 V1.1
 ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 **********************************************************/
#include "lib_rtc.h"


/***************************************************************
  ��������RTC_Init
  ��  ����ʵʱʱ�ӳ�ʼ��
  ����ֵ��CLKx:RTCʱ��Դѡ��HOURx��12Сʱ/24Сʱ��ѡ��
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void RTC_Init(RTC_TYPE_CLKS CLKx,RTC_TYPE_TIME HOURx)
{
    uint32_t flag;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;
    RTC->CON.PON = 0;           // ���㣬ʹ��RTC
    RTC->CON.HSWI = HOURx;  // 24Сʱģʽ
    RTC->CON.CLKS = CLKx;   // ʱ��Դѡ��32768Hz

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;
}
/***************************************************************
  ��������RTC_StartRead
  ��  ��������ʵʱʱ�Ӷ�����
  ����ֵ����
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void RTC_StartRead(void)
{
    RTC->CON.TMWR = 0;  // ���������Ƕ�
    RTC->CON.TMUP = 1;  // ����������
    while (RTC->CON.TMUP == 1); //�ȴ������
}

/***************************************************************
  ��������RTC_ReadHourmode
  ��  ������ȡСʱģʽ
  ����ֵ����
  ���ֵ����
  ����ֵ����ǰ��Сʱģʽ
 ***************************************************************/
uint32_t RTC_ReadHourmode(void)
{
    uint32_t flag;
    uint32_t result;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �������������� */
    RTC_StartRead();

    result = RTC->CON.HSWI;

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;

    return result;
}
/***************************************************************
  ��������RTC_ReadSecond
  ��  ������ȡ��
  ����ֵ����
  ���ֵ����
  ����ֵ����ǰ����
 ***************************************************************/
uint32_t RTC_ReadSecond(void)
{
    uint32_t flag;
    uint32_t result;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �������������� */
    RTC_StartRead();

    /* ����ʱ�� */
    result = (RTC->SEC.S01*1);
    result += (RTC->SEC.S02*2);
    result += (RTC->SEC.S04*4);
    result += (RTC->SEC.S08*8);
    result += (RTC->SEC.S10*10);
    result += (RTC->SEC.S20*20);
    result += (RTC->SEC.S40*40);

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;

    return result;
}

/***************************************************************
  ��������RTC_ReadMinute
  ��  ������ȡ��
  ����ֵ����
  ���ֵ����
  ����ֵ����ǰ�ķ�
 ***************************************************************/
uint32_t RTC_ReadMinute(void)
{
    uint32_t flag;
    uint32_t result;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �������������� */
    RTC_StartRead();

    /* ����ʱ�� */
    result = (RTC->MIN.M01*1);
    result += (RTC->MIN.M02*2);
    result += (RTC->MIN.M04*4);
    result += (RTC->MIN.M08*8);
    result += (RTC->MIN.M10*10);
    result += (RTC->MIN.M20*20);
    result += (RTC->MIN.M40*40);

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;

    return result;
}

/***************************************************************
  ��������RTC_ReadHour
  ��  ������ȡСʱ
  ����ֵ����
  ���ֵ����
  ����ֵ����ǰ��Сʱ
 ***************************************************************/
uint32_t RTC_ReadHour(uint32_t *meridiem)
{
    uint32_t flag;
    uint32_t result;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �������������� */
    RTC_StartRead();

    /* ����ʱ�� */
    result = ((RTC->HOUR.Word & (uint32_t)0x01) * 1);
    result += (((RTC->HOUR.Word >> 1) & (uint32_t)0x01) * 2);
    result += (((RTC->HOUR.Word >> 2) & (uint32_t)0x01) * 4);
    result += (((RTC->HOUR.Word >> 3) & (uint32_t)0x01) * 8);
    result += (((RTC->HOUR.Word >> 4) & (uint32_t)0x01) * 10);

    if(RTC_ReadHourmode() == RTC_HOUR24)                        //24Сʱģʽ��20Сʱ
    {
        result += (((RTC->HOUR.Word >> 5) & (uint32_t)0x01) * 20);
    }
    else                                                        //12Сʱģʽ��1 PM,0 AM
    {
        *meridiem = (((RTC->HOUR.Word >> 5) & (uint32_t)0x01));
    }

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;

    return result;
}

/***************************************************************
  ��������RTC_ReadDay
  ��  ������ȡ��
  ����ֵ����
  ���ֵ����
  ����ֵ����ǰ����
 ***************************************************************/
uint32_t RTC_ReadDay(void)
{
    uint32_t flag;
    uint32_t result;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;
    /* �������������� */
    RTC_StartRead();

    /* ����ʱ�� */
    result = (RTC->DAY.D01 * 1);
    result += (RTC->DAY.D02 * 2);
    result += (RTC->DAY.D04 * 4);
    result += (RTC->DAY.D08 * 8);
    result += (RTC->DAY.D10 * 10);
    result += (RTC->DAY.D20 * 20);


    /* ����RTCд���� */
    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;
    return result;
}

/***************************************************************
  ��������RTC_ReadMonth
  ��  ������ȡ��
  ����ֵ����
  ���ֵ����
  ����ֵ����ǰ����
 ***************************************************************/
uint32_t RTC_ReadMonth(void)
{
    uint32_t flag;
    uint32_t result;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �������������� */
    RTC_StartRead();

    /* ����ʱ�� */
    result = (RTC->MON.MN01 * 1);
    result += (RTC->MON.MN02 * 2);
    result += (RTC->MON.MN04 * 4);
    result += (RTC->MON.MN08 * 8);
    result += (RTC->MON.MN10 * 10);

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;

    return result;
}

/***************************************************************
  ��������RTC_ReadYear
  ��  ������ȡ��
  ����ֵ����
  ���ֵ����
  ����ֵ����ǰ����
 ***************************************************************/
uint32_t RTC_ReadYear(void)
{
    uint32_t flag;
    uint32_t result;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �������������� */
    RTC_StartRead();

    /* ����ʱ�� */
    result = (RTC->YEAR.Y01 * 1);
    result += (RTC->YEAR.Y02 * 2);
    result += (RTC->YEAR.Y04 * 4);
    result += (RTC->YEAR.Y08 * 8);
    result += (RTC->YEAR.Y10 * 10);
    result += (RTC->YEAR.Y20 * 20);
    result += (RTC->YEAR.Y40 * 40);
    result += (RTC->YEAR.Y80 * 80);
    result += 2000;

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;

    return result;
}
/***************************************************************
  ��������RTC_ReadWeek
  ��  ������ȡ����
  ����ֵ����
  ���ֵ����
  ����ֵ����ǰ������
 ***************************************************************/
uint32_t RTC_ReadWeek(void)
{
    uint32_t flag;
    uint32_t result;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �������������� */
    RTC_StartRead();


    /* ����ʱ�� */
    switch(RTC->WEEK.WW)
    {
        case 0x00: result = 0;break;
        case 0x01: result = 1;break;
        case 0x02: result = 2;break;
        case 0x03: result = 3;break;
        case 0x04: result = 4;break;
        case 0x05: result = 5;break;
        case 0x06: result = 6;break;
        default:   result = 0;break;
    }

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;

    return result;
}

/***************************************************************
  ��������RTC_StartRead
  ��  ��������ʵʱʱ��д����
  ����ֵ����
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void RTC_StartWrite(void)
{
    /* ����д�������� */
    RTC->CON.TMWR = 1;      // ����������д
    RTC->CON.TMUP = 1;      // ����д����
    while (RTC->CON.TMUP == 1);//�ȴ�д���
}

/***************************************************************
  ��������RTC_WriteSecond
  ��  �����޸���
  ����ֵ��second: ��
  ���ֵ����
  ����ֵ����
 ***************************************************************/
ErrorStatus RTC_WriteSecond(uint32_t second)
{
    uint32_t flag;

    /* ���������� */
    if (second >= 60)
        return ERROR;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �����µ�ֵ */
    if (second >= 40) 
    {
        RTC->SEC.S40 = 1;
        second -= 40;
    } 
    else 
    {
        RTC->SEC.S40 = 0;
    }

    if (second >= 20) 
    {
        RTC->SEC.S20 = 1;
        second -= 20;
    } 
    else 
    {
        RTC->SEC.S20 = 0;
    }

    if (second >= 10)
    {
        RTC->SEC.S10 = 1;
        second -= 10;
    } 
    else 
    {
        RTC->SEC.S10 = 0;
    }

    if (second >= 8) 
    {
        RTC->SEC.S08 = 1;
        second -= 8;
    } 
    else 
    {
        RTC->SEC.S08 = 0;
    }

    if (second >= 4) 
    {
        RTC->SEC.S04 = 1;
        second -= 4;
    } 
    else 
    {
        RTC->SEC.S04 = 0;
    }

    if (second >= 2) 
    {
        RTC->SEC.S02 = 1;
        second -= 2;
    } 
    else 
    {
        RTC->SEC.S02 = 0;
    }

    if (second >= 1) 
    {
        RTC->SEC.S01 = 1;
        second -= 1;
    } 
    else 
    {
        RTC->SEC.S01 = 0;
    }
    /* ����д�������� */
    RTC_StartWrite();

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;
    return SUCCESS;
}

/***************************************************************
  ��������RTC_WriteMinute
  ��  �����޸���
  ����ֵ��minute: ��
  ���ֵ����
  ����ֵ����
 ***************************************************************/
ErrorStatus RTC_WriteMinute(uint32_t minute)
{
    uint32_t flag;

    /* ���������� */
    if (minute >= 60)
        return ERROR;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �����µ�ֵ */
    if (minute >= 40) 
    {
        RTC->MIN.M40 = 1;
        minute -= 40;
    } 
    else 
    {
        RTC->MIN.M40 = 0;
    }

    if (minute >= 20) 
    {
        RTC->MIN.M20 = 1;
        minute -= 20;
    } 
    else 
    {
        RTC->MIN.M20 = 0;
    }

    if (minute >= 10)
    {
        RTC->MIN.M10 = 1;
        minute -= 10;
    } 
    else 
    {
        RTC->MIN.M10 = 0;
    }

    if (minute >= 8) 
    {
        RTC->MIN.M08 = 1;
        minute -= 8;
    } 
    else 
    {
        RTC->MIN.M08 = 0;
    }

    if (minute >= 4) 
    {
        RTC->MIN.M04 = 1;
        minute -= 4;
    } 
    else 
    {
        RTC->MIN.M04 = 0;
    }

    if (minute >= 2) 
    {
        RTC->MIN.M02 = 1;
        minute -= 2;
    } 
    else 
    {
        RTC->MIN.M02 = 0;
    }

    if (minute >= 1) 
    {
        RTC->MIN.M01 = 1;
        minute -= 1;
    } 
    else 
    {
        RTC->MIN.M01 = 0;
    }
    /* ����д�������� */
    RTC_StartWrite();

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;
    return SUCCESS;
}


/***************************************************************
  ��������RTC_WriteHour
  ��  �����޸�ʱ
  ����ֵ��hour: ʱ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
ErrorStatus RTC_WriteHour(uint32_t hour, uint32_t meridiem)
{
    uint32_t flag,hour_buf,mode;

    /* ���������� */
    if (hour >= 24)
        return ERROR;

    mode = RTC_ReadHourmode();

    if(mode == RTC_HOUR12 && hour > 12)     //12Сʱģʽ���ɴ���12Сʱ
        return ERROR;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �����µ�ֵ */
    if(mode == RTC_HOUR24)
    {
        if (hour >= 20)
        {
            hour_buf |= (uint32_t)1<<5;
            hour -= 20;
        }
        else
        {
            hour_buf &= ~((uint32_t)1<<5);
        }
    }
    else
    {
        if(meridiem == 1)
            hour_buf |= (uint32_t)1<<5;
        else
            hour_buf &= ~((uint32_t)1<<5);
    }

    if (hour >= 10)
    {
        hour_buf |= (uint32_t)1<<4;
        hour -= 10;
    } 
    else 
    {
        hour_buf &= ~((uint32_t)1<<4);
    }

    if (hour >= 8) 
    {
        hour_buf |= (uint32_t)1<<3;
        hour -= 8;
    } 
    else 
    {
        hour_buf &= ~((uint32_t)1<<3);
    }

    if (hour >= 4) 
    {
        hour_buf |= (uint32_t)1<<2;
        hour -= 4;
    } 
    else 
    {
        hour_buf &= ~((uint32_t)1<<2);
    }

    if (hour >= 2) 
    {
        hour_buf |= (uint32_t)1<<1;
        hour -= 2;
    } 
    else 
    {
        hour_buf &= ~((uint32_t)1<<1);
    }

    if (hour >= 1) 
    {
        hour_buf |= (uint32_t)1;
        hour -= 1;
    } 
    else 
    {
        hour_buf &= ~(uint32_t)1;
    }
    /* ����д�������� */
    RTC->HOUR.Word = hour_buf;
    RTC_StartWrite();

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;
    return SUCCESS;
}

/***************************************************************
  ��������RTC_WriteDay
  ��  �����޸���
  ����ֵ��day: ��
  ���ֵ����
  ����ֵ����
 ***************************************************************/
ErrorStatus RTC_WriteDay(uint32_t day)
{
    uint32_t flag;

    /* ���������� */
    if ((day == 0) || (day >= 32))
        return ERROR;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �����µ�ֵ */    
    if (day >= 20) 
    {
        RTC->DAY.D20 = 1;
        day -= 20;
    } 
    else 
    {
        RTC->DAY.D20 = 0;
    }

    if (day >= 10)
    {
        RTC->DAY.D10 = 1;
        day -= 10;
    } 
    else 
    {
        RTC->DAY.D10 = 0;
    }

    if (day >= 8) 
    {
        RTC->DAY.D08 = 1;
        day -= 8;
    } 
    else 
    {
        RTC->DAY.D08 = 0;
    }

    if (day >= 4) 
    {
        RTC->DAY.D04 = 1;
        day -= 4;
    } 
    else 
    {
        RTC->DAY.D04 = 0;
    }

    if (day >= 2) 
    {
        RTC->DAY.D02 = 1;
        day -= 2;
    } 
    else 
    {
        RTC->DAY.D02 = 0;
    }

    if (day >= 1) 
    {
        RTC->DAY.D01 = 1;
        day -= 1;
    } 
    else 
    {
        RTC->DAY.D01 = 0;
    }
    /* ����д�������� */
    RTC_StartWrite();

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;
    return SUCCESS;
}


/***************************************************************
  ��������RTC_WriteMonth
  ��  �����޸���
  ����ֵ��month: ��
  ���ֵ����
  ����ֵ����
 ***************************************************************/
ErrorStatus RTC_WriteMonth(uint32_t month)
{
    uint32_t flag;

    /* ���������� */
    if ((month == 0) || (month >= 13))
        return ERROR;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �����µ�ֵ */    

    if (month >= 10)
    {
        RTC->MON.MN10 = 1;
        month -= 10;
    } 
    else 
    {
        RTC->MON.MN10 = 0;
    }

    if (month >= 8) 
    {
        RTC->MON.MN08 = 1;
        month -= 8;
    } 
    else 
    {
        RTC->MON.MN08 = 0;
    }

    if (month >= 4) 
    {
        RTC->MON.MN04 = 1;
        month -= 4;
    } 
    else 
    {
        RTC->MON.MN04 = 0;
    }

    if (month >= 2) 
    {
        RTC->MON.MN02 = 1;
        month -= 2;
    } 
    else 
    {
        RTC->MON.MN02 = 0;
    }

    if (month >= 1) 
    {
        RTC->MON.MN01 = 1;
        month -= 1;
    } 
    else 
    {
        RTC->MON.MN01 = 0;
    }
    /* ����д�������� */
    RTC_StartWrite();

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;
    return SUCCESS;
}

/***************************************************************
  ��������RTC_WriteYear
  ��  �����޸���
  ����ֵ��year: ��
  ���ֵ����
  ����ֵ����
 ***************************************************************/
ErrorStatus RTC_WriteYear(uint32_t year)
{
    uint32_t flag;

    /* ���������� */
    if ((year < 2000) || (year > 2099))
        return ERROR;

    /* ��ת�� */
    year -= 2000;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �����µ�ֵ */
    if (year >= 80) {
        RTC->YEAR.Y80 = 1;
        year -= 80;
    } else {
        RTC->YEAR.Y80 = 0;
    }

    if (year >= 40) {
        RTC->YEAR.Y40 = 1;
        year -= 40;
    } else {
        RTC->YEAR.Y40 = 0;
    }

    if (year >= 20) {
        RTC->YEAR.Y20 = 1;
        year -= 20;
    } else {
        RTC->YEAR.Y20 = 0;
    }

    if (year >= 10) {
        RTC->YEAR.Y10 = 1;
        year -= 10;
    } else {
        RTC->YEAR.Y10 = 0;
    }

    if (year >= 8) {
        RTC->YEAR.Y08 = 1;
        year -= 8;
    } else {
        RTC->YEAR.Y08 = 0;
    }

    if (year >= 4) {
        RTC->YEAR.Y04 = 1;
        year -= 4;
    } else {
        RTC->YEAR.Y04 = 0;
    }

    if (year >= 2) {
        RTC->YEAR.Y02 = 1;
        year -= 2;
    } else {
        RTC->YEAR.Y02 = 0;
    }

    if (year >= 1) {
        RTC->YEAR.Y01 = 1;
        year -= 1;
    } else {
        RTC->YEAR.Y01 = 0;
    }
    /* ����д�������� */
    RTC_StartWrite();

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;
    return SUCCESS;
}

/***************************************************************
  ��������RTC_WriteWeek
  ��  �����޸�����
  ����ֵ��week: ���ڣ�0-6��
  ���ֵ����
  ����ֵ����
 ***************************************************************/
ErrorStatus RTC_WriteWeek(uint32_t week)
{
    uint32_t flag;

    /* ���������� */
    if (week > 6)
        return ERROR;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    RTC->WEEK.WW = week;

    /* ����д�������� */
    RTC->CON.TMWR = 1;      // ����������д
    RTC->CON.TMUP = 1;      // ����д����
    while (RTC->CON.TMUP == 1);

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;
    return SUCCESS;
}



/***************************************************************
  ��������RTC_ReadWeekAlarmMinute
  ��  ������ȡ��
  ����ֵ����
  ���ֵ����
  ����ֵ����ǰ�ķ�
 ***************************************************************/
uint32_t RTC_ReadWeekAlarmMinute(void)
{
    uint32_t flag;
    uint32_t result;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �������������� */
    RTC_StartRead();

    /* ����ʱ�� */
    result = ((RTC->WA.WM & (uint32_t)0x01) * 1);
    result += (((RTC->WA.WM >> 1) & (uint32_t)0x01) * 2);
    result += (((RTC->WA.WM >> 2) & (uint32_t)0x01) * 4);
    result += (((RTC->WA.WM >> 3) & (uint32_t)0x01) * 8);
    result += (((RTC->WA.WM >> 4) & (uint32_t)0x01) * 10);
    result += (((RTC->WA.WM >> 5) & (uint32_t)0x01) * 20);
    result += (((RTC->WA.WM >> 6) & (uint32_t)0x01) * 40);

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;

    return result;
}

/***************************************************************
  ��������RTC_ReadWeekAlarmHour
  ��  ������ȡСʱ
  ����ֵ����
  ���ֵ����
  ����ֵ����ǰ��Сʱ
 ***************************************************************/
uint32_t RTC_ReadWeekAlarmHour(uint32_t *meridiem)
{
    uint32_t flag;
    uint32_t result;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �������������� */
    RTC_StartRead();

    /* ����ʱ�� */
    result = ((RTC->WA.WH & (uint32_t)0x01) * 1);
    result += (((RTC->WA.WH >> 1) & (uint32_t)0x01) * 2);
    result += (((RTC->WA.WH >> 2) & (uint32_t)0x01) * 4);
    result += (((RTC->WA.WH >> 3) & (uint32_t)0x01) * 8);
    result += (((RTC->WA.WH >> 4) & (uint32_t)0x01) * 10);

    if(RTC_ReadHourmode() == RTC_HOUR24)                        //24Сʱģʽ��20Сʱ
    {
        result += (((RTC->WA.WH >> 5) & (uint32_t)0x01) * 20);
    }
    else                                                        //12Сʱģʽ��1 PM,0 AM
    {
        *meridiem = (((RTC->WA.WH >> 5) & (uint32_t)0x01));
    }

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;

    return result;
}

/***************************************************************
  ��������RTC_ReadWeekAlarmWeek
  ��  ������ȡ����
  ����ֵ����
  ���ֵ����
  ����ֵ����ǰ������
 ***************************************************************/
uint32_t RTC_ReadWeekAlarmWeek(void)
{
    uint32_t flag;
    uint32_t result;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �������������� */
    RTC_StartRead();

    /* ����ʱ�� */
    switch(RTC->WA.WW)
    {
        case 0x01: result = 0;break;
        case 0x02: result = 1;break;
        case 0x04: result = 2;break;
        case 0x08: result = 3;break;
        case 0x10: result = 4;break;
        case 0x20: result = 5;break;
        case 0x40: result = 6;break;
        default:   result = 0;break;
    }

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;

    return result;
}


/***************************************************************
  ��������RTC_ReadDayAlarmMinute
  ��  ������ȡ����
  ����ֵ����
  ���ֵ����
  ����ֵ����ǰ�ķ���
 ***************************************************************/
uint32_t RTC_ReadDayAlarmMinute(void)
{
    uint32_t flag;
    uint32_t result;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �������������� */
    RTC_StartRead();

    /* ����ʱ�� */
    result = ((RTC->DA.DM & (uint32_t)0x01) * 1);
    result += (((RTC->DA.DM >> 1) & (uint32_t)0x01) * 2);
    result += (((RTC->DA.DM >> 2) & (uint32_t)0x01) * 4);
    result += (((RTC->DA.DM >> 3) & (uint32_t)0x01) * 8);
    result += (((RTC->DA.DM >> 4) & (uint32_t)0x01) * 10);
    result += (((RTC->DA.DM >> 5) & (uint32_t)0x01) * 20);
    result += (((RTC->DA.DM >> 6) & (uint32_t)0x01) * 40);

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;

    return result;
}

/***************************************************************
  ��������RTC_ReadDaykAlarmHour
  ��  ������ȡСʱ
  ����ֵ����
  ���ֵ����
  ����ֵ����ǰ��Сʱ
 ***************************************************************/
uint32_t RTC_ReadDayAlarmHour(uint32_t *meridiem)
{
    uint32_t flag;
    uint32_t result;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �������������� */
    RTC_StartRead();

    /* ����ʱ�� */
    result = ((RTC->DA.DH & (uint32_t)0x01) * 1);
    result += (((RTC->DA.DH >> 1) & (uint32_t)0x01) * 2);
    result += (((RTC->DA.DH >> 2) & (uint32_t)0x01) * 4);
    result += (((RTC->DA.DH >> 3) & (uint32_t)0x01) * 8);
    result += (((RTC->DA.DH >> 4) & (uint32_t)0x01) * 10);

    if(RTC_ReadHourmode() == RTC_HOUR24)                        //24Сʱģʽ��20Сʱ
    {
        result += (((RTC->DA.DH >> 5) & (uint32_t)0x01) * 20);
    }
    else                                                        //12Сʱģʽ��1 PM,0 AM
    {
        *meridiem = (((RTC->DA.DH >> 5) & (uint32_t)0x01));
    }

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;

    return result;
}

/***************************************************************
  ��������RTC_WriteWeekAlarmMinute
  ��  �����޸ķ���
  ����ֵ��minute: ����
  ���ֵ����
  ����ֵ����
 ***************************************************************/
ErrorStatus RTC_WriteWeekAlarmMinute(uint32_t minute)
{
    uint32_t flag,minute_buf;

    /* ���������� */
    if (minute >= 60)
        return ERROR;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �����µ�ֵ */
    if (minute >= 40)
    {
        minute_buf |= (uint32_t)1<<6;
        minute -= 40;
    }
    else
    {
        minute_buf &= ~(uint32_t)1<<6;
    }

    if (minute >= 20)
    {
        minute_buf |= (uint32_t)1<<5;
        minute -= 20;
    }
    else
    {
        minute_buf &= ~(uint32_t)1<<5;
    }

    if (minute >= 10)
    {
        minute_buf |= (uint32_t)1<<4;
        minute -= 10;
    }
    else
    {
        minute_buf &= ~(uint32_t)1<<4;
    }

    if (minute >= 8)
    {
        minute_buf |= (uint32_t)1<<3;
        minute -= 8;
    }
    else
    {
        minute_buf &= ~(uint32_t)1<<3;
    }

    if (minute >= 4)
    {
        minute_buf |= (uint32_t)1<<2;
        minute -= 4;
    }
    else
    {
        minute_buf &= ~(uint32_t)1<<2;
    }

    if (minute >= 2)
    {
        minute_buf |= (uint32_t)1<<1;
        minute -= 2;
    }
    else
    {
        minute_buf &= ~(uint32_t)1<<1;
    }

    if (minute >= 1)
    {
        minute_buf |= (uint32_t)1;
        minute -= 1;
    }
    else
    {
        minute_buf &= ~(uint32_t)1;
    }

    RTC->WA.WM = minute_buf;

    /* ����д�������� */
    RTC->CON.TMWR = 1;      // ����������д
    RTC->CON.TMUP = 1;      // ����д����
    while (RTC->CON.TMUP == 1);

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;
    return SUCCESS;
}

/***************************************************************
  ��������RTC_WriteWeekAlarmHour
  ��  �����޸�Сʱ
  ����ֵ��hour: Сʱ
          meridiem��(����12Сʱģʽʱ��Ч)
                    0 AM
                    1 PM
  ���ֵ����
  ����ֵ����
 ***************************************************************/
ErrorStatus RTC_WriteWeekAlarmHour(uint32_t hour, uint32_t meridiem)
{
    uint32_t flag,hour_buf,mode;

    /* ���������� */
    if (hour >= 24)
        return ERROR;

    mode = RTC_ReadHourmode();

    if(mode == RTC_HOUR12 && hour > 12)     //12Сʱģʽ���ɴ���12Сʱ
        return ERROR;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �����µ�ֵ */
    if(mode == RTC_HOUR24)
    {
        if (hour >= 20)
        {
            hour_buf |= (uint32_t)1<<5;
            hour -= 20;
        }
        else
        {
            hour_buf &= ~((uint32_t)1<<5);
        }
    }
    else
    {
        if(meridiem == 1)
            hour_buf |= (uint32_t)1<<5;
        else
            hour_buf &= ~((uint32_t)1<<5);
    }

    if (hour >= 10)
    {
        hour_buf |= (uint32_t)1<<4;
        hour -= 10;
    } 
    else 
    {
        hour_buf &= ~((uint32_t)1<<4);
    }

    if (hour >= 8) 
    {
        hour_buf |= (uint32_t)1<<3;
        hour -= 8;
    } 
    else 
    {
        hour_buf &= ~((uint32_t)1<<3);
    }

    if (hour >= 4) 
    {
        hour_buf |= (uint32_t)1<<2;
        hour -= 4;
    } 
    else 
    {
        hour_buf &= ~((uint32_t)1<<2);
    }

    if (hour >= 2) 
    {
        hour_buf |= (uint32_t)1<<1;
        hour -= 2;
    } 
    else 
    {
        hour_buf &= ~((uint32_t)1<<1);
    }

    if (hour >= 1) 
    {
        hour_buf |= (uint32_t)1;
        hour -= 1;
    } 
    else 
    {
        hour_buf &= ~(uint32_t)1;
    }

    RTC->WA.WH = hour_buf;

    /* ����д�������� */
    RTC->CON.TMWR = 1;      // ����������д
    RTC->CON.TMUP = 1;      // ����д����
    while (RTC->CON.TMUP == 1);

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;
    return SUCCESS;
}
/***************************************************************
  ��������RTC_WriteWeekAlarmWeek
  ��  �����޸�����
  ����ֵ��week: ���ڣ�0-6��
  ���ֵ����
  ����ֵ����
 ***************************************************************/
ErrorStatus RTC_WriteWeekAlarmWeek(uint32_t week)
{
    uint32_t flag,week_buf;

    /* ���������� */
    if (week > 6)
        return ERROR;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �����µ�ֵ */
    if (week >= 6)
    {
        week_buf |= (uint32_t)1<<6;
        week -= 6;
    }
    else
    {
        week_buf &= ~(uint32_t)1<<6;
    }

    if (week >= 5)
    {
        week_buf |= (uint32_t)1<<5;
        week -= 5;
    }
    else
    {
        week_buf &= ~(uint32_t)1<<5;
    }

    if (week >= 4)
    {
        week_buf |= (uint32_t)1<<4;
        week -= 4;
    }
    else
    {
        week_buf &= ~(uint32_t)1<<4;
    }

    if (week >= 3)
    {
        week_buf |= (uint32_t)1<<3;
        week -= 3;
    }
    else
    {
        week_buf &= ~(uint32_t)1<<3;
    }

    if (week >= 2)
    {
        week_buf |= (uint32_t)1<<2;
        week -= 2;
    }
    else
    {
        week_buf &= ~(uint32_t)1<<2;
    }

    if (week >= 1)
    {
        week_buf |= (uint32_t)1<<1;
        week -= 1;
    }
    else
    {
        week_buf &= ~(uint32_t)1<<1;
    }

    RTC->WA.WW = week_buf;

    /* ����д�������� */
    RTC->CON.TMWR = 1;      // ����������д
    RTC->CON.TMUP = 1;      // ����д����
    while (RTC->CON.TMUP == 1);

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;
    return SUCCESS;
}

/***************************************************************
  ��������RTC_WriteDayAlarmMinute
  ��  �����޸ķ���
  ����ֵ��minute: ����
  ���ֵ����
  ����ֵ����
 ***************************************************************/
ErrorStatus RTC_WriteDayAlarmMinute(uint32_t minute)
{
    uint32_t flag,minute_buf;

    /* ���������� */
    if (minute >= 60)
        return ERROR;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �����µ�ֵ */
    if (minute >= 40)
    {
        minute_buf |= (uint32_t)1<<6;
        minute -= 40;
    }
    else
    {
        minute_buf &= ~(uint32_t)1<<6;
    }

    if (minute >= 20)
    {
        minute_buf |= (uint32_t)1<<5;
        minute -= 20;
    }
    else
    {
        minute_buf &= ~(uint32_t)1<<5;
    }

    if (minute >= 10)
    {
        minute_buf |= (uint32_t)1<<4;
        minute -= 10;
    }
    else
    {
        minute_buf &= ~(uint32_t)1<<4;
    }

    if (minute >= 8)
    {
        minute_buf |= (uint32_t)1<<3;
        minute -= 8;
    }
    else
    {
        minute_buf &= ~(uint32_t)1<<3;
    }

    if (minute >= 4)
    {
        minute_buf |= (uint32_t)1<<2;
        minute -= 4;
    }
    else
    {
        minute_buf &= ~(uint32_t)1<<2;
    }

    if (minute >= 2)
    {
        minute_buf |= (uint32_t)1<<1;
        minute -= 2;
    }
    else
    {
        minute_buf &= ~(uint32_t)1<<1;
    }

    if (minute >= 1)
    {
        minute_buf |= (uint32_t)1;
        minute -= 1;
    }
    else
    {
        minute_buf &= ~(uint32_t)1;
    }

    RTC->DA.DM = minute_buf;

    /* ����д�������� */
    RTC->CON.TMWR = 1;      // ����������д
    RTC->CON.TMUP = 1;      // ����д����
    while (RTC->CON.TMUP == 1);

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;
    return SUCCESS;
}

/***************************************************************
  ��������RTC_WriteDayAlarmHour
  ��  �����޸�Сʱ
  ����ֵ��hour: Сʱ
          meridiem��(����12Сʱģʽʱ��Ч)
                    0 AM
                    1 PM
  ���ֵ����
  ����ֵ����
 ***************************************************************/
ErrorStatus RTC_WriteDayAlarmHour(uint32_t hour, uint32_t meridiem)
{
    uint32_t flag,hour_buf,mode;

    /* ���������� */
    if (hour >= 24)
        return ERROR;

    mode = RTC_ReadHourmode();

    if(mode == RTC_HOUR12 && hour > 12)     //12Сʱģʽ���ɴ���12Сʱ
        return ERROR;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �����µ�ֵ */
    if(mode == RTC_HOUR24)
    {
        if (hour >= 20)
        {
            hour_buf |= (uint32_t)1<<5;
            hour -= 20;
        }
        else
        {
            hour_buf &= ~((uint32_t)1<<5);
        }
    }
    else
    {
        if(meridiem == 1)
            hour_buf |= (uint32_t)1<<5;
        else
            hour_buf &= ~((uint32_t)1<<5);
    }

    if (hour >= 10)
    {
        hour_buf |= (uint32_t)1<<4;
        hour -= 10;
    } 
    else 
    {
        hour_buf &= ~((uint32_t)1<<4);
    }

    if (hour >= 8) 
    {
        hour_buf |= (uint32_t)1<<3;
        hour -= 8;
    } 
    else 
    {
        hour_buf &= ~((uint32_t)1<<3);
    }

    if (hour >= 4) 
    {
        hour_buf |= (uint32_t)1<<2;
        hour -= 4;
    } 
    else 
    {
        hour_buf &= ~((uint32_t)1<<2);
    }

    if (hour >= 2) 
    {
        hour_buf |= (uint32_t)1<<1;
        hour -= 2;
    } 
    else 
    {
        hour_buf &= ~((uint32_t)1<<1);
    }

    if (hour >= 1) 
    {
        hour_buf |= (uint32_t)1;
        hour -= 1;
    } 
    else 
    {
        hour_buf &= ~(uint32_t)1;
    }

    RTC->DA.DH = hour_buf;

    /* ����д�������� */
    RTC->CON.TMWR = 1;      // ����������д
    RTC->CON.TMUP = 1;      // ����д����
    while (RTC->CON.TMUP == 1);

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;
    return SUCCESS;
}


/***************************************************************
  ��������RTC_InterruptEnable
  ��  ����ʹ��ʵʱʱ�ӵ�ĳЩ�ж�
  ����ֵ��src: ʵʱʱ�ӵ��ж�Դ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void RTC_InterruptEnable(RTC_Interrupt_Source src)
{
    uint32_t flag;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �����µ�ֵ */
    switch (src)
    {
        case RTC_Interrupt_Source_Second:
            RTC->IE.SCDIE = 1;
            break;
        case RTC_Interrupt_Source_Minute:
            RTC->IE.MINIE = 1;
            break;
        case RTC_Interrupt_Source_Hour:
            RTC->IE.HORIE = 1;
            break;
        case RTC_Interrupt_Source_Day:
            RTC->IE.DAYIE = 1;
            break;
        case RTC_Interrupt_Source_Month:
            RTC->IE.MONIE = 1;
            break;
        case RTC_Interrupt_Source_DayALE:
            RTC->IE.DALE = 1;
            break;
        case RTC_Interrupt_Source_WeekALE:
            RTC->IE.WALE = 1;
            break;
        case RTC_Interrupt_Source_OSC:
            RTC->IE.OSCDIE = 1;
            break;
        default:
            break;
    }

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;

    NVIC->ISER[0] |= (1 << 17); // ʹ��NVIC�ж�
    return;
}

/***************************************************************
  ��������RTC_InterruptDisable
  ��  ��������ʵʱʱ�ӵ�ĳЩ�ж�
  ����ֵ��src: ʵʱʱ�ӵ��ж�Դ
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void RTC_InterruptDisable(RTC_Interrupt_Source src)
{
    uint32_t flag;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �����µ�ֵ */
    switch (src)
    {
        case RTC_Interrupt_Source_Second:
            RTC->IE.SCDIE = 0;
            break;
        case RTC_Interrupt_Source_Minute:
            RTC->IE.MINIE = 0;
            break;
        case RTC_Interrupt_Source_Hour:
            RTC->IE.HORIE = 0;
            break;
        case RTC_Interrupt_Source_Day:
            RTC->IE.DAYIE = 0;
            break;
        case RTC_Interrupt_Source_Month:
            RTC->IE.MONIE = 0;
            break;
        case RTC_Interrupt_Source_DayALE:
            RTC->IE.DALE = 0;
            break;
        case RTC_Interrupt_Source_WeekALE:
            RTC->IE.WALE = 0;
            break;
        case RTC_Interrupt_Source_OSC:
            RTC->IE.OSCDIE = 0;
            break;
        default:
            break;
    }
    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;
    return;
}

/***************************************************************
  ��������RTC_GetITStatus
  ��  ������ȡʵʱʱ�ӵ�ĳЩ�ж�״̬
  ����ֵ��src: ʵʱʱ�ӵ��ж�Դ
  ���ֵ����
  ����ֵ���жϱ�־
 ***************************************************************/
ITStatus RTC_GetITStatus(RTC_Interrupt_Source src)
{
    ITStatus result = RESET;

    switch (src) 
    {
        case RTC_Interrupt_Source_Second:
            if(RTC->IE.SCDIE)
            result = SET;
            break;
        case RTC_Interrupt_Source_Minute:
            if(RTC->IE.MINIE)
                result = SET;
            break;
        case RTC_Interrupt_Source_Hour:
            if(RTC->IE.HORIE)
                result = SET;
            break;
        case RTC_Interrupt_Source_Day:
            if(RTC->IE.DAYIE)
                result = SET;
            break;
        case RTC_Interrupt_Source_Month:
            if(RTC->IE.MONIE)
                result = SET;
            break;
        case RTC_Interrupt_Source_DayALE:
            if(RTC->IE.DALE)
                result = SET;
            break;
        case RTC_Interrupt_Source_WeekALE:
            if(RTC->IE.WALE)
                result = SET;
            break;
        case RTC_Interrupt_Source_OSC:
            if(RTC->IE.OSCDIE)
                result = SET;
        default:
            result = RESET;
            break;
    }

    return result;
}

/***************************************************************
  ��������RTC_GetFlagStatus
  ��  ������ȡʵʱʱ�ӵ�ĳЩ�жϱ�־
  ����ֵ��src: ʵʱʱ�ӵ��ж�Դ
  ���ֵ����
  ����ֵ���жϱ�־
 ***************************************************************/
FlagStatus RTC_GetFlagStatus(RTC_Interrupt_Source src)
{
    FlagStatus result = RESET;

    switch (src) 
    {
        case RTC_Interrupt_Source_Second:
            if(RTC->IF.SCDIF)
                result = SET;
            break;
        case RTC_Interrupt_Source_Minute:
            if(RTC->IF.MINIF)
                result = SET;
            break;
        case RTC_Interrupt_Source_Hour:
            if(RTC->IF.HORIF)
                result = SET;
            break;
        case RTC_Interrupt_Source_Day:
            if(RTC->IF.DAYIF)
                result = SET;
            break;
        case RTC_Interrupt_Source_Month:
            if(RTC->IF.MONIF)
                result = SET;
            break;
        case RTC_Interrupt_Source_DayALE:
            if(RTC->IF.DAFG)
                result = SET;
            break;
        case RTC_Interrupt_Source_WeekALE:
            if(RTC->IF.WAFG)
                result = SET;
            break;
        case RTC_Interrupt_Source_OSC:
            if(RTC->IF.OSCDIF)
                result = SET;
        default:
            result = RESET;
            break;
    }

    return result;
}

/***************************************************************
  ��������RTC_ClearAllITFlag
  ��  �������ʵʱʱ�ӵ������жϱ�־
  ����ֵ����
  ���ֵ����
  ����ֵ����
 ***************************************************************/
void RTC_ClearAllITFlag(void)
{
    uint32_t flag;

    /* ���RTCд���� */
    flag = RTC->WP.WP;
    if (flag == 0x00000000)
        RTC->WP.WP = 0x55AAAA55;

    /* �����µ�ֵ */
    RTC->IF.SCDIF = 1;      // д1����жϱ�־λ
    RTC->IF.MINIF = 1;      // д1����жϱ�־λ
    RTC->IF.HORIF = 1;      // д1����жϱ�־λ
    RTC->IF.DAYIF = 1;      // д1����жϱ�־λ
    RTC->IF.MONIF = 1;      // д1����жϱ�־λ
    RTC->IF.DAFG = 1;       // д1����жϱ�־λ
    RTC->IF.WAFG = 1;       // д1����жϱ�־λ
    RTC->IF.OSCDIF = 1;     // д1����жϱ�־λ

    /* ����RTCд���� */
    if (flag == 0x00000000)
        RTC->WP.WP = 0x00000000;
    return;
}

