#ifndef _ES8P508_TRIAC_H_
#define _ES8P508_TRIAC_H_

#define RELAY2_ON_PORT       GPIOB
#define RELAY2_ON_PIN        GPIO_Pin_7
#define RELAY2_OFF_PORT      GPIOB
#define RELAY2_OFF_PIN       GPIO_Pin_6

#define RELAY1_ON_PORT       GPIOB
#define RELAY1_ON_PIN        GPIO_Pin_3
#define RELAY1_OFF_PORT      GPIOB
#define RELAY1_OFF_PIN       GPIO_Pin_2

//#define RELAY3_ON_PORT       GPIOB
//#define RELAY3_ON_PIN        GPIO_Pin_1
//#define RELAY3_OFF_PORT      GPIOB
//#define RELAY3_OFF_PIN       GPIO_Pin_0

#define SET_PIN_H(port, pin)          GPIO_SetBit(port, pin)
#define SET_PIN_L(port, pin)          GPIO_ResetBit(port, pin)
/**/
#endif
