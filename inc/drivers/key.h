#ifndef _KEY_H_
#define _KEY_H_

#include <stdint.h>
#include <stdbool.h>
#include "device.h"

#define KEY_POOL_MAX        8
#define KEY_TYPE            5//evt_pressed,evt_lpressed,evt_keyup,evt_keydown,evt_double_pressed

#define KEY_SCAN_PERIOD     (20)    /* 20ms */
#define KEY_LONG_PRESS_TM   (1000)  /* 1s */
#define KEY_DOUBLE_PRESS_TM (400)   
#define KEY_LONG_PRESS_CNT  (KEY_LONG_PRESS_TM / KEY_SCAN_PERIOD)  /* 500 count */
//#define USE_KEY_FIRST_STATE  

struct key_ops
{
    bool (*is_pressed)(uint8_t keyno);
};

typedef struct
{
    uint8_t  pressed;
    uint8_t  lpressed;
    uint8_t  keyup;
    uint8_t  keydown;
    uint8_t  double_pressed;
    uint8_t  single_pressed;
}key_event_t;


struct key_device
{
    struct device          parent;
    const struct key_ops  *ops;

    uint16_t  used;
    uint16_t  press_cnt[KEY_POOL_MAX];
    uint16_t  press_map[KEY_POOL_MAX];
	uint16_t  single_double_press[KEY_POOL_MAX];
    uint16_t  trg, con, last_con;    //trg 
	key_event_t key_event;
    uint8_t key_state_init_count;
};

err_t key_device_register(struct key_device *key, const char *name, uint32_t flag, void *data);
#endif
