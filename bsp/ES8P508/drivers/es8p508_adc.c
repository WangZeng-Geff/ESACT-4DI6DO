#include <config.h>
#include <board.h>
#include <init.h>
#include <os.h>
#include <math.h>
#include <bitmap.h>
#include <bitops.h>
#include <settings.h>
#include "ES8P508x.h"
#include "lib_config.h"
#include "device.h"
#include "printk.h"
#include "es8p508_adc.h"


static struct device ntc_sensor_dev;
static void ntc_sensor_tmr_cb(struct soft_timer *st);



static struct soft_timer ntc_sensor_tmr =
{
    .expires = INITIAL_JIFFIES,
    .cb      = ntc_sensor_tmr_cb,
};

const double Rp  = 10000.0; //10K
const double T2  = (273.15 + 25.0);
const double Bx  = 3950.0;  //3950K
const double Ka  = 273.15;

static float tmpConvert(float cur_adc_resault ,float adc_total)
{
	float temp = 0, Res = 0;
    Res = (float)((float)10*adc_total/cur_adc_resault - 10);
    Res = 1000.0 * Res;
    temp = 1 / ((log(Res / Rp)) / Bx + (1 / T2)) - Ka;
    return temp;
}

static void ntc_sensor_tmr_cb(struct soft_timer *st)
{
	uint8_t i;
    uint16_t max, min, sum, ad_temp;
    float temperature = 0;

    max = 0x00;
    min = 0x0fff;
    sum = 0x00;

    for (i=0; i<18; i++)
    {
        ADC_Start();
        while(ADC_GetFlagStatus(ADC_IF) == RESET);
        ad_temp = ADC_GetConvValue();
        ADC_ClearIFStatus(ADC_IF);

        if (ad_temp > max)
        {
            max = ad_temp;
        }
        if (ad_temp < min)
        {
            min = ad_temp;
        }
        sum += ad_temp;
    }
    sum -= min;
    sum -= max;
    sum >>= 4;
    temperature = tmpConvert(sum ,4095);
    temperature = temperature * 10;
    dev_state.temp_over.temp = temperature;
//    log_d("the temprature is: %d\n", dev_state.temp_over.temp);	
//    log_d("the adc is: %d\n", sum);	
    ntc_sensor_tmr.expires = jiffies + configHZ ;
    soft_timer_add(&ntc_sensor_tmr);
}
/**/
static void ntc_sensor_gpio_init(void);

/**/
static err_t ntc_sensor_ctrl(device_t *dev, uint8_t cmd, void *args)
{
    switch (cmd)
    {
    case CTRL_ON:
    {
		ntc_sensor_gpio_init();
		ntc_sensor_tmr.expires = jiffies + configHZ ;
		soft_timer_add(&ntc_sensor_tmr);
    }
    break;
    }
    return 0;
}

static err_t ntc_sensor_init(device_t *dev)
{
    ntc_sensor_gpio_init();
    return 0;
}


static size_t ntc_sensor_read(device_t *dev, off_t pos, void *buffer, size_t size)
{
    uint8_t i;
    uint16_t max, min, sum, ad_temp;

    max = 0x00;
    min = 0x0fff;
    sum = 0x00;

    for (i=0; i<18; i++)
    {
        ADC_Start();
        while(ADC_GetFlagStatus(ADC_IF) == RESET);
        ad_temp = ADC_GetConvValue();
        ADC_ClearIFStatus(ADC_IF);

        if (ad_temp > max)
        {
            max = ad_temp;
        }
        if (ad_temp < min)
        {
            min = ad_temp;
        }
        sum += ad_temp;
    }
    sum -= min;
    sum -= max;
	log_d("VER: %d\n", sum);
    return (sum >> 4);
}
   


static struct device_ops ntc_sensor_ops =
{
	.init = ntc_sensor_init,
    .ctrl = ntc_sensor_ctrl,
    .read = ntc_sensor_read,
};


static void ntc_sensor_gpio_init(void)
{
    GPIO_InitStruType x;
    ADC_InitStruType y;             
    
    x.GPIO_Signal = GPIO_Pin_Signal_Analog;
    x.GPIO_Func = GPIO_Func_0;
    x.GPIO_Direction = GPIO_Dir_In;
    x.GPIO_PUEN = GPIO_PUE_Input_Disable;
    x.GPIO_PDEN = GPIO_PDE_Input_Disable;
    x.GPIO_OD = GPIO_ODE_Output_Disable;
    x.GPIO_DS = GPIO_DS_Output_Normal;
	
    GPIO_Init(NTC_SENSOR_PORT,NTC_SENSOR_PIN,&x);      

    y.ADC_ChS = ADC_CHS_AIN2;            //通道:AIN2
    y.ADC_ClkS = ADC_ClkS_PCLK;          //时钟：PCLK
    y.ADC_ClkDiv = ADC_ClkDiv_32;        //预分频：1:32  ADC转换时钟源一定要符合数据手册中ADC转化时钟源选择表
    y.ADC_VrefP = ADC_VrefP_Vcc;         //正向参考电压：VDD
    y.ADC_SampS = ADC_SMPS_HARD;         //AD采样模式选择：硬件
    y.ST = 7;
	//y.VREF_SEL = 0;
    y.ADC_VREFN = ENABLE;
    y.ADC_VRBUF_EN = ENABLE;
    ADC_Init(&y);                        //按照结构体的参数配置ADC

    ADC_IE_Disable();                    //不使能中断
    ADC_Enable();                        //使能ADC
	log_d("ntc_sensor_gpio_init success\n");	
}
// the 	T16N3 is used for encode, please check sn and buzz fuc when buz_init location different
int board_setup_ntc_sensor(void)
{
    ntc_sensor_dev.ops = &ntc_sensor_ops;
    device_register(&ntc_sensor_dev, "ntc_sensor", 0);

    return 0;
}
//device_initcall(board_setup_ntc_sensor);
