#include <utils.h>
#include <types.h>
#include <device.h>
#include <printk.h>
#include "board.h"

#define _a      0x80
#define _b      0x04
#define _c      0x01
#define _d      0x10
#define _e      0x20
#define _f      0x40
#define _g      0x02

#ifdef USING_LCD

const uint8_t digit_table[10] =
{
    _a + _b + _c + _d + _e + _f      ,  //0
    _b + _c                          ,  //1
    _a + _b + _g + _e + _d           ,  //2
    _a + _b + _c + _d + _g           ,  //3
    _b + _c + _g + _f                ,  //4
    _a + _c + _d + _g + _f           ,  //5
    _a + _c + _d + _e + _f + _g      ,  //6
    _a + _b + _c                     ,  //7
    _a + _b + _c + _d + _e + _f + _g ,  //8
    _a + _b + _c + _f + _g + _d      ,  //9
};

uint8_t lcd_buf[LCD_BUF_SIZE];

struct bu9796_data
{
    struct i2c_bus_device *i2c_bus;
};

static struct bu9796_data bu9796_data;

static int bu9796_write_data(struct i2c_bus_device *i2c_bus, uint8_t *values, size_t length)
{
    struct i2c_msg msgs[1];

    msgs[0].addr  = BU9796_ADDR;
    msgs[0].flags = I2C_WR;
    msgs[0].buf   = (uint8_t *)values;
    msgs[0].len   = length;

    if (i2c_transfer(i2c_bus, msgs, 1) == 1)
        return 0;

    return -EIO;
}

static void update_lcd_data(uint8_t *buffer, size_t size)
{
    int temp = get_le_val(buffer, 2);
    int humi = get_le_val(&buffer[2], 2);
    lcd_buf[0] = digit_table[humi % 1000 / 100];
    set_bit(lcd_buf[0], 3); //humidity logo
    lcd_buf[1] = digit_table[humi % 100 / 10];
    set_bit(lcd_buf[1], 3); //%

    set_bit(lcd_buf[2], 1); //temp logo
    temp < 0 ? set_bit(lcd_buf[2], 0) : clr_bit(lcd_buf[2], 0); // -
    if (buffer[5])
        tst_bit(lcd_buf[2], 2) ? clr_bit(lcd_buf[2], 2) : set_bit(lcd_buf[2], 2); // ~
    else
        clr_bit(lcd_buf[2], 2);
//    if (buffer[6])
//        tst_bit(lcd_buf[2], 3) ? clr_bit(lcd_buf[2], 3) : set_bit(lcd_buf[2], 3); // !
//    else
//        clr_bit(lcd_buf[2], 3)

    lcd_buf[3] = digit_table[temp % 1000 / 100];
    lcd_buf[4] = digit_table[temp % 100 / 10];
    set_bit(lcd_buf[4], 3); //point
    lcd_buf[5] = digit_table[temp % 10];
    set_bit(lcd_buf[5], 3); //¡æ
    lcd_buf[6] = 0xE0; //special

    if (temp > 180 && temp < 280
            && humi > 300 && humi < 600)
    {
        clr_bit(lcd_buf[2], 4);
        set_bit(lcd_buf[2], 7);
    }
    else if (temp < 110 || temp > 320
             || humi > 700 || humi < 200)
    {
        clr_bit(lcd_buf[2], 7);
        clr_bit(lcd_buf[2], 4);
        set_bit(lcd_buf[6], 4);
    }
    else
    {
        clr_bit(lcd_buf[2], 7);
        set_bit(lcd_buf[2], 4);
    }
}

static size_t bu9796_write(device_t dev, off_t pos, const void *buffer, size_t size)
{
    size_t len = 0;
    struct bu9796_data *bu9796 = (struct bu9796_data *)dev->user_data;
    uint8_t data[5 + LCD_BUF_SIZE] = { SET_BLINK_OFF, SET_IC, SET_AP_NORMAL, SET_SHOW_ON, SET_LOAD_DATA_ADDR };

    update_lcd_data((uint8_t *)buffer, size);
    memcpy(&data[5], lcd_buf, LCD_BUF_SIZE);
    len = bu9796_write_data(bu9796->i2c_bus, data, sizeof(data));
    return len;
}

static const struct device_ops bu9796_dev_ops =
{
    .write = bu9796_write,
};

static struct device bu9796_dev;
err_t lcd_init(const char *i2c_bus_name)
{
    struct i2c_bus_device *i2c_bus = (struct i2c_bus_device *)device_find(i2c_bus_name);
    if (i2c_bus == NULL)
    {
        log_e(MODULE_APP, "\ni2c_bus %s for bu9796 not found!\n", i2c_bus_name);
        return -ENOSYS;
    }

    if (device_open(&i2c_bus->parent, NULL) != 0)
    {
        log_e(MODULE_APP, "\ni2c_bus %s for bu9796 opened failed!\n", i2c_bus_name);
        return -EIO;
    }

    bu9796_data.i2c_bus  = i2c_bus;
    bu9796_dev.ops = &bu9796_dev_ops;
    bu9796_dev.user_data = &bu9796_data;

    device_register(&bu9796_dev, "b9796", 0);
    return 0;
}

#endif
