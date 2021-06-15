#ifndef _DEV_H_
#define _DEV_H_

#include <device.h>

enum RELAY_OPT
{
	REVERSE,
	ON_OFF,
};

extern device_t *dev_key, *dev_led, *dev_buz, *dev_relay;

void dev_show(void);

int dev_type_get(void *out, int maxlen);
int dev_type_cmp(const void *dt, size_t len);
int dev_ver_get(void *out, int maxlen);
int dev_ver_cmp(const void *ver, size_t len);

void sys_led( uint8_t index);
void refresh_current_state(void);

//void check_chn_need_sync(uint8_t chn);
void clear_local_linkctrl(uint8_t i ,uint8_t j);
void dev_show_start(void);
uint8_t led_status_save(uint8_t state, uint8_t type);
uint8_t relay_status_save(uint8_t state, uint8_t type,uint8_t source);
#endif
