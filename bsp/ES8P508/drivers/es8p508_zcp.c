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
#include "es8p508_zcp.h"
#include "jiffies.h"
#include "dev.h"

extern volatile unsigned long jiffies;
static struct device zcp_dev;

////-------------------------------------------------------------------------------
static int8_t get_chn_from_bit(uint8_t chn)
{
	uint8_t i=0;
	for(i=0; i<8; i++)
	{
		if(tst_bit(chn, i))
			return i;
	}
	return -1;
}
static void relay_to_normal(uint8_t chn)
{
	dev_state.relay_to_normal |= chn;
	if(get_chn_from_bit(chn) >= 0)
		dev_state.relay_to_normal_cnt[get_chn_from_bit(chn)] = 8; //8*10ms 80ms keep level
}
//-------------------------------------------------------------------------------
static void zcp_init(void)
{
	GPIO_InitStruType x;

	x.GPIO_Signal		= GPIO_Pin_Signal_Digital;
	x.GPIO_Func			= GPIO_Func_0;
	x.GPIO_Direction	= GPIO_Dir_In;
	x.GPIO_PUEN			= GPIO_PUE_Input_Disable;
	x.GPIO_PDEN			= GPIO_PDE_Input_Enable;
	x.GPIO_OD			= GPIO_ODE_Output_Disable;
	x.GPIO_DS			= GPIO_DS_Output_Normal;
	
    GPIO_Init(ZCP_PORT,ZCP_PIN,&x);    

    PINT_Config(PINT5, PINT_SEL0, PINT_Trig_Rise);
    PINT_ClearITPendingBit(PINT_IT_PINT5);
	NVIC_Init(NVIC_PINT5_IRQn,NVIC_Priority_0,DISABLE);
	PINT5_MaskEnable();
    PINT5_Disable();                       //关闭KINT中断
	
	TIM_BaseInitStruType y;
    y.TIM_ClkS = TIM_ClkS_PCLK;
	y.TIM_SYNC = DISABLE;
    y.TIM_Mode = TIM_Mode_TC0 ;
	y.TIM_EDGE = TIM_EDGE_Rise;
    T16Nx_BaseInit(T16N1, &y);

    T16Nx_SetPREMAT(T16N1,configSYS_CLOCK / 1000000 - 1);
    T16Nx_SetMAT0(T16N1,0xffff);
    T16Nx_MAT0ITConfig(T16N1,TIM_Go_Int);

	T16Nx_ITConfig(T16N1,TIM_IT_MAT0,DISABLE);
	T16Nx_SetCNT(T16N1,0);
	NVIC_Init(NVIC_T16N1_IRQn,NVIC_Priority_1,DISABLE);	
    T16N1_Disable();	
}

static void start_zcp_timer(uint16_t time)
{	
    T16Nx_SetMAT0(T16N1,time);
    T16Nx_MAT0ITConfig(T16N1,TIM_Go_Int);
	
	T16Nx_ITConfig(T16N1,TIM_IT_MAT0,ENABLE);
	T16Nx_SetCNT(T16N1,0);
	NVIC_Init(NVIC_T16N1_IRQn,NVIC_Priority_1,ENABLE);	
    T16N1_Enable();
}

static void stop_zcp_timer(void)
{
	NVIC_Init(NVIC_T16N1_IRQn,NVIC_Priority_1,DISABLE);
	T16Nx_SetCNT(T16N1,0);
	T16N1_Disable();
}

static void zcp_time_init(uint8_t chn, uint8_t opt)
{
	switch(opt)
	{
		case CTRL_ON:
			start_zcp_timer(100 * setting.dev_infor.relay_tm[2*chn + 1]);
			break;
		case CTRL_OFF:
			start_zcp_timer(100 * setting.dev_infor.relay_tm[2*chn]);
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------
static void zcp_relay_ctrl(void)
{
	stop_zcp_timer();
	if(dev_state.state_relay & dev_state.act_chn)
	{
        log_d("zcp_relay_ctrl CTRL_ON%d\r\n",dev_state.act_chn);
		device_ctrl(dev_relay, CTRL_ON, &dev_state.act_chn);
	}
	else
	{
        log_d("zcp_relay_ctrl CTRL_OFF%d\r\n",dev_state.act_chn);
		device_ctrl(dev_relay, CTRL_OFF, &dev_state.act_chn);
	}
	relay_to_normal(dev_state.act_chn);
	dev_state.need_act_relay &= ~dev_state.act_chn;
	dev_state.act_chn = 0;
	
//	if(dev_state.need_act_relay)
//		device_ctrl(dev_zcp, ZCP_CTRL_OPEN, NULL);
}

static void cal_relay_delay_time(void)
{
	int i;
	for(i=0; i<RESTORE_RELAY_NUM; i++)
	{
		if(tst_bit(dev_state.need_act_relay, i))
		{
            log_d("dev_state.need_act_relay = %d\r\n",dev_state.need_act_relay);
			if(tst_bit(dev_state.act_chn, i)) return;//the chn is acting
			set_bit(dev_state.act_chn, i);
            //clr_bit(dev_state.need_act_relay, i);
			if(tst_bit(dev_state.state_relay ,i))
				zcp_time_init(i,CTRL_ON);
			else
				zcp_time_init(i,CTRL_OFF);
			return;
		}
	}
}

void zcp_interrup_disable(void)
{
	NVIC_Init(NVIC_PINT5_IRQn,NVIC_Priority_0,DISABLE);
	PINT5_MaskEnable();
    PINT5_Disable(); 
	
	cal_relay_delay_time();
}

static void zcp_interrupt_enable(void)
{
	PINT_Config(PINT5, PINT_SEL0, PINT_Trig_Rise);
    PINT_ClearITPendingBit(PINT_IT_PINT5);
	NVIC_Init(NVIC_PINT5_IRQn,NVIC_Priority_0,ENABLE);
	PINT5_MaskDisable();
    PINT5_Enable();                       //开启KINT中断
}
//---------------------------------------------------------------------------
static err_t zcp_ctrl(device_t *dev, uint8_t cmd, void *args)
{	
	switch (cmd)
    {
    case ZCP_CTRL_OPEN:
        log_d("zcp_interrupt_enable\r\n");
		zcp_interrupt_enable();
		break;
	case ZCP_CTRL_CLOSE:
        log_d("zcp_interrup_disable\r\n");
		zcp_interrup_disable();
		break;
	case ZCP_CTRL_RELAY:
		zcp_relay_ctrl();
		break;
	default:
		break;
    }
    return 0;
}

static struct device_ops zcp_ops =
{
    .ctrl = zcp_ctrl,
};

int board_setup_zcp(void)
{
	zcp_init();
	zcp_dev.ops = &zcp_ops;
    device_register(&zcp_dev, "zcp", 0);

    return 0;
}
//device_initcall(board_setup_zcp);
