#include "irqhandler.h"
#include "lib_config.h"
#include "jiffies.h"
#include "printk.h"
#include "dev.h"
#include <device.h>
#include "drivers\\es8p508_zcp.h"

void NMI_IRQHandler(void)
{
	log_d("NMI_IRQHandler!\n");
}

void HardFault_IRQHandler(void)
{
	log_d("HardFault!\n");
    while (1);
}

void SVC_IRQHandler(void)
{
	log_d("SVC_IRQHandler!\n");
}

void PendSV_IRQHandler(void)
{
	log_d("PendSV_IRQHandler!\n");
}

//10ms jiffies
void SysTick_IRQHandler(void)
{
    jiffies++;
}

//void PINT5_IRQHandler(void)
//{
//	if(PINT_GetITStatus(PINT_IT_PINT5) != RESET)
//    {
//        log_d("zcp int!\n");
//		PINT_ClearITPendingBit(PINT_IT_PINT5);      // 清除过零中断标志位
//		device_ctrl(dev_zcp, ZCP_CTRL_CLOSE, NULL);
//    }
//}

//void T16N1_IRQHandler(void)
//{
//	if(T16Nx_GetITStatus(T16N1,TIM_IT_MAT0) != RESET)
//    {
//		log_d("timer int!\n");
//		T16Nx_ClearIFPendingBit(T16N1,TIM_IF_MAT0);
//		device_ctrl(dev_zcp, ZCP_CTRL_RELAY, NULL);
//    }
//}
