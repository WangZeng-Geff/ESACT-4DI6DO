#ifndef _LCD_H
#define _LCD_H

#include <types.h>


#define BU9796_ADDR         (0x7C >> 1)

#define SET_LOAD_DATA_ADDR  0x00            //0x00
#define SET_SOFT_RESET      0xEA            //ICSET
#define SET_IC              0xE8
#define SET_BLINK_OFF       0xF0            //BLINK
#define SET_BLINK_ON        0xF1            //BLINK 0.5Hz
#define SET_AP_OFF          0xFD            //ALL PIX
#define SET_AP_NORMAL       0xFC
#define SET_SHOW_ON         0xC8            //MODE SET
#define SET_SHOW_OFF        0xC0            //MODE SET
#define SET_DISPLAY         0xA2            //DISCTRL

#define LCD_BUF_SIZE        7

err_t lcd_init(const char *i2c_bus_name);

#endif
