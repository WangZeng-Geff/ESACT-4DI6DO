#include <config.h>
#include <board.h>
#include <init.h>
#include <os.h>
#include <bitmap.h>
#include <bitops.h>
#include <settings.h>
#include "ES8P508x.h"
#include "lib_config.h"
#include "device.h"
#include "printk.h"
#include "es8p508_relay.h"
#include "proto_smart.h"

static struct device relay_dev;
//static struct soft_timer relay_normal_tmr;

typedef struct
{
    GPIO_TYPE       port;
    GPIO_TYPE_PIN   pin;
} per_relay_t;

static per_relay_t relay_pin[] =
{
    {RELAY1_ON_PORT, 	RELAY1_ON_PIN},
	{RELAY1_OFF_PORT, 	RELAY1_OFF_PIN},
	{RELAY2_ON_PORT, 	RELAY2_ON_PIN},
	{RELAY2_OFF_PORT, 	RELAY2_OFF_PIN},
};


static void setup_relay_peri(per_relay_t *relay)
{
    GPIO_InitStruType y;
	
	y.GPIO_Signal		= GPIO_Pin_Signal_Digital;
	y.GPIO_Func			= GPIO_Func_0;
	y.GPIO_Direction	= GPIO_Dir_Out;
	y.GPIO_PUEN			= GPIO_PUE_Input_Enable;
	y.GPIO_PDEN			= GPIO_PDE_Input_Disable;
	y.GPIO_OD			= GPIO_ODE_Output_Disable;
	y.GPIO_DS			= GPIO_DS_Output_Normal;
	
    GPIO_Init(relay->port, relay->pin, &y);
}

static void relay_gpio_init(void)
{ 
	int i;
    for (i = 0; i < array_size(relay_pin); i++)
    {
        setup_relay_peri(&relay_pin[i]);
		GPIO_ResetBit(relay_pin[i].port, relay_pin[i].pin);
    }
}

static void relay_on(uint8_t chn)
{
    log_d("relay_on %d\r\n",chn);
	int i;
	for(i = 0; i < RESTORE_RELAY_NUM; i++)
	{
		if(tst_bit(chn,i))
		{
            if(setting.dev_infor.relay_para[i].normal_state == RELAY_NORMAL_OPEN)
            {
                SET_PIN_H(relay_pin[2*i].port, relay_pin[2*i].pin);//ON_PIN
                SET_PIN_L(relay_pin[2*i + 1].port, relay_pin[2*i + 1].pin);//OFF_PIN
            }
            else if(setting.dev_infor.relay_para[i].normal_state == RELAY_NORMAL_CLOSED)
            {
                SET_PIN_L(relay_pin[2*i].port, relay_pin[2*i].pin);
                SET_PIN_H(relay_pin[2*i + 1].port, relay_pin[2*i + 1].pin);
            }
			setting.dev_infor.opt_cnt[i]++;
			dev_state.param_save = 0x01;
		}

	}
}
static void relay_off(uint8_t chn)
{
    log_d("relay_off %d\r\n",chn);
	int i;
	for(i = 0; i < RESTORE_RELAY_NUM; i++)
	{
		if(tst_bit(chn,i))
		{
			if(setting.dev_infor.relay_para[i].normal_state == RELAY_NORMAL_CLOSED)
            {
                SET_PIN_H(relay_pin[2*i].port, relay_pin[2*i].pin);//ON_PIN
                SET_PIN_L(relay_pin[2*i + 1].port, relay_pin[2*i + 1].pin);//OFF_PIN
            }
            else if(setting.dev_infor.relay_para[i].normal_state == RELAY_NORMAL_OPEN)
            {
                SET_PIN_L(relay_pin[2*i].port, relay_pin[2*i].pin);
                SET_PIN_H(relay_pin[2*i + 1].port, relay_pin[2*i + 1].pin);
            }
			setting.dev_infor.opt_cnt[i]++;
			dev_state.param_save = 0x01;
		}
	}
}
static void relay_normal(uint8_t chn)
{
	int i;
	for(i = 0; i < RESTORE_RELAY_NUM; i++)
	{
		if(tst_bit(chn,i))
		{
			SET_PIN_L(relay_pin[2*i].port, relay_pin[2*i].pin);
			SET_PIN_L(relay_pin[2*i + 1].port, relay_pin[2*i + 1].pin);
		}
	}
}



static err_t relay_ctrl(device_t *dev, uint8_t cmd, void *args)
{
    uint8_t *chn = (uint8_t *)args;
	
	switch (cmd)
    {
    case CTRL_RELAY_ON:
		relay_on(*chn);
		break;
	case CTRL_RELAY_OFF:
		relay_off(*chn);
		break;
	case CTRL_RELAY_NORMAL:
		relay_normal(*chn);
		break;
	default:
		break;
    }
    return 0;
}
static struct device_ops relay_ops =
{
	.ctrl = relay_ctrl,
};

//static void relay_normal_tmr_cb(struct soft_timer *st)
//{
//    uint8_t i = 0, chn = 0;
//	for(i = 0; i < RESTORE_RELAY_NUM  ; i++)
//	{
//		chn = 0;
//		if(tst_bit(dev_state.relay_to_normal, i))
//		{
//			if(dev_state.relay_to_normal_cnt[i]-- <= 0)
//			{
//				clr_bit(dev_state.relay_to_normal, i);
//				set_bit(chn, i);
//				relay_normal(chn);
//			}
//		}
//	}
////	st->expires = jiffies + configHZ/5 ;  //10ms
////	soft_timer_add(st);
//    
//}
//static struct soft_timer relay_normal_tmr =
//{
//    .expires = INITIAL_JIFFIES + configHZ / 5,
//    .cb      = relay_normal_tmr_cb,
//};

int board_setup_relay(void)
{
	relay_gpio_init();

    relay_dev.ops = &relay_ops;
    device_register(&relay_dev, "relay", 0);

    return 0;
}
device_initcall(board_setup_relay);

