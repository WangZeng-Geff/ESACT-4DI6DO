
#ifndef __IRQHANDLER_H__
#define __IRQHANDLER_H__

#include "HR8P296.h"

/************�жϺ�������***********/
void NMI_IRQHandler(void);
void HardFault_IRQHandler(void);
void SVC_IRQHandler(void);
void PendSV_IRQHandler(void);
void SysTick_IRQHandler(void);


#endif

