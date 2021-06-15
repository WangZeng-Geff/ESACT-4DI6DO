#ifndef _ES8P508_LED_H_
#define _ES8P508_LED_H_

#define LED_PERIOD      2000

#define LED1_PORT       GPIOA
#define LED1_PIN        GPIO_Pin_11 
#define LED2_PORT       GPIOA
#define LED2_PIN        GPIO_Pin_6   
#define LED3_PORT       GPIOB
#define LED3_PIN        GPIO_Pin_5   
#define LED4_PORT       GPIOB
#define LED4_PIN        GPIO_Pin_4   

#define LED_RUN_PORT    GPIOA
#define LED_RUN_PIN     GPIO_Pin_1  

int board_setup_led(void);

#endif
