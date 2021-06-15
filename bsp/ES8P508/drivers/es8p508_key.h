#ifndef _ES8P508_KEY_H_
#define _ES8P508_KEY_H_

//hardware pin define
#define KEY1_PORT       GPIOA
#define KEY1_PIN        GPIO_Pin_10
#define KEY2_PORT       GPIOA
#define KEY2_PIN        GPIO_Pin_25
#define KEY3_PORT       GPIOA
#define KEY3_PIN        GPIO_Pin_15
#define KEY4_PORT       GPIOA
#define KEY4_PIN        GPIO_Pin_16

////////////////////////////////////////////////////////////////////////////////////////////////////
#define KEY5_PORT       GPIOB //(back up)
#define KEY5_PIN        GPIO_Pin_0

#define KEY_NUM  (4)

int board_key_init(void);

#endif
