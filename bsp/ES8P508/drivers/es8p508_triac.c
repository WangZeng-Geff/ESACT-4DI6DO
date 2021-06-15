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
#include "dev.h"
#include "es8p508_triac.h"


static struct device triac_dev;

typedef struct
{
    GPIO_TYPE       port;
    GPIO_TYPE_PIN   pin;
} per_triac_t;

static per_triac_t triac_pin[] =
{
  {TRIAC1_PORT, TRIAC1_PIN},
  {TRIAC2_PORT, TRIAC2_PIN},
};

static void setup_triac_peri(per_triac_t *triac)
{
    GPIO_InitStruType x;

	x.GPIO_Signal		= GPIO_Pin_Signal_Digital;
	x.GPIO_Func			= GPIO_Func_0;
	x.GPIO_Direction	= GPIO_Dir_Out;
	x.GPIO_PUEN			= GPIO_PUE_Input_Disable;
	x.GPIO_PDEN			= GPIO_PDE_Input_Disable;
	x.GPIO_OD			= GPIO_ODE_Output_Disable;
	x.GPIO_DS			= GPIO_DS_Output_Normal;

  GPIO_Init(triac->port, triac->pin, &x);
}
static void triac_gpio_init(void)
{ 
	int i;
    for (i = 0; i < array_size(triac_pin); i++)
    {
        setup_triac_peri(&triac_pin[i]);
		GPIO_ResetBit(triac_pin[i].port, triac_pin[i].pin);
    }
}
//---------------------------------------------------------------------------
static void triac_on(uint8_t chn)
{
	if(chn < RESTORE_KEY_NUM)
	{
		GPIO_SetBit(triac_pin[chn].port, triac_pin[chn].pin);
	}
}
static void triac_off(uint8_t chn)
{
	if(chn < RESTORE_KEY_NUM)
	{
		GPIO_ResetBit(triac_pin[chn].port, triac_pin[chn].pin);
	}
}

static void triac_off_on(uint8_t chn)
{
    if (GPIO_ReadBit(triac_pin[chn].port, triac_pin[chn].pin))
    {
        GPIO_ResetBit(triac_pin[chn].port, triac_pin[chn].pin);
    }
    else
    {
        GPIO_SetBit(triac_pin[chn].port, triac_pin[chn].pin);
    }	
}
//---------------------------------------------------------------------------

#ifdef TRIAC_CTRL_KEEP
static err_t triac_ctrl(device_t *dev, uint8_t cmd, void *args)
{
    uint8_t *chn = (uint8_t *)args;
	
	switch (cmd)
    {
    case CTRL_ON:
		triac_on(*chn);
		break;
	case CTRL_OFF:
		triac_off(*chn);
		break;
    case CTRL_OFF_ON:
		triac_off_on(*chn);
		break;
	default:
		break;
    }
    return 0;
}
//---------------------------------------------------------------------------
static struct device_ops triac_ops =
{
	.ctrl = triac_ctrl,
};
int board_setup_triac(void)
{
	triac_gpio_init();

    triac_dev.ops = &triac_ops;
    device_register(&triac_dev, "triac", 0);

    return 0;
}
device_initcall(board_setup_triac);
#endif
//---------------------------------------------------------------------------
