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
#include "es8p508_led.h"

static struct device led_dev;
static struct soft_timer led_tmr;


typedef struct
{
    GPIO_TYPE       port;
    GPIO_TYPE_PIN   pin;
} per_led_t;

#define LED_RUN (4)
static per_led_t led_pin[] =
{
    {LED1_PORT, LED1_PIN},
    {LED2_PORT, LED2_PIN},
    {LED3_PORT, LED3_PIN},
    {LED4_PORT, LED4_PIN},
    {LED_RUN_PORT, LED_RUN_PIN}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
static void led_on(uint8_t chn)
{
	if(chn <= RESTORE_KEY_NUM)
	{
		GPIO_SetBit(led_pin[chn].port, led_pin[chn].pin);
	}
}
static void led_off(uint8_t chn)
{
	if(chn <= RESTORE_KEY_NUM)
	{
		GPIO_ResetBit(led_pin[chn].port, led_pin[chn].pin);
	}
}

static void led_on_off(uint8_t chn)
{
    if (GPIO_ReadBit(led_pin[chn].port, led_pin[chn].pin))
    {
        GPIO_ResetBit(led_pin[chn].port, led_pin[chn].pin);
    }
    else
    {
        GPIO_SetBit(led_pin[chn].port, led_pin[chn].pin);
    }	
}
//static void led_tmr_cb2(struct soft_timer *st)
//{
//	st->expires = jiffies + configHZ/10 ;  //10ms
//	soft_timer_add(st);
//    led_tmr.expires = jiffies + configHZ/5 ; 

//}
static void led_tmr_cb(struct soft_timer *st)
{
//	st->expires = jiffies + configHZ/5 ;  //10ms
//	soft_timer_add(st);
	GPIO_SetBit(led_pin[LED_RUN].port, led_pin[LED_RUN].pin);	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static err_t led_ctrl(device_t *dev, uint8_t cmd, void *args)
{
    uint8_t *chn = (uint8_t *)args;
	switch (cmd)
	{
	case CTRL_ON:
		led_on(*chn);
		break;
	case CTRL_OFF:
		led_off(*chn);
		break;
    case CTRL_ON_OFF:
		led_on_off(*chn);
		break;
	case CTRL_OFF_ON:
        led_off(LED_RUN);
        led_tmr.expires = jiffies + configHZ/5 ;  //200ms
        soft_timer_add(&led_tmr);
//        led_tmr2.expires = jiffies + configHZ/10 ;  //100ms
//        soft_timer_add(&led_tmr2);
		break;
	default:
		break;
	}
    return 0;
}

static struct device_ops led_ops =
{
    .ctrl = led_ctrl,
};

static void led_gpio_init(void)
{
    GPIO_InitStruType y;

	y.GPIO_Signal		= GPIO_Pin_Signal_Digital;
	y.GPIO_Func			= GPIO_Func_0;
	y.GPIO_Direction	= GPIO_Dir_Out;
	y.GPIO_PUEN			= GPIO_PUE_Input_Disable;
	y.GPIO_PDEN			= GPIO_PDE_Input_Disable;
	y.GPIO_OD			= GPIO_ODE_Output_Disable;
	y.GPIO_DS			= GPIO_DS_Output_Normal;
	
    GPIO_Init(LED1_PORT, LED1_PIN, &y); 
	GPIO_Init(LED2_PORT, LED2_PIN, &y); 
	GPIO_Init(LED3_PORT, LED3_PIN, &y); 
	GPIO_Init(LED4_PORT, LED4_PIN, &y); 
	GPIO_Init(LED_RUN_PORT, LED_RUN_PIN, &y); 
	
	GPIO_SetBit(LED_RUN_PORT,LED_RUN_PIN);
	GPIO_ResetBit(LED1_PORT,LED1_PIN);
	GPIO_ResetBit(LED2_PORT,LED2_PIN);
	GPIO_ResetBit(LED3_PORT,LED3_PIN);
	GPIO_ResetBit(LED4_PORT,LED4_PIN);
}

static struct soft_timer led_tmr =
{
    .expires = INITIAL_JIFFIES + configHZ / 5,
    .cb      = led_tmr_cb,
};

//static struct soft_timer led_tmr2 =
//{
//    .expires = INITIAL_JIFFIES + configHZ / 5,
//    .cb      = led_tmr_cb2,
//};



int board_setup_led(void)
{
    led_gpio_init();
    led_dev.ops = &led_ops;
    device_register(&led_dev, "led", 0);
    soft_timer_add(&led_tmr);
    return 0;
}
device_initcall(board_setup_led);
