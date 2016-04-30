#pragma once

#include "stm32f0xx.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
//#include <stdint.h>
//#include <math.h>
//#include <ctype.h>
#include <string.h>


//ESC 테스트를 디펄트 동작  모드로 설정시 활성화 , 비활성화시 표준 서보모드(25ms priod로 800~2200us High puls범위로 동작됨.
//#define ESC_TEST_MODE		//FAST PWM & 800~2200 us PWM mode

#define STANDARD_PWM_FREQ		50		//20ms 주기
#define FAST_PWM_FREQ		400				//2.5ms주기

#define MIN_RC_STD		1000
#define MAX_RC_STD	2000

#define MIN_RC_LIMIT		800		//일반적으로 800~2200us 영역을 사용
#define MAX_RC_LIMIT		2200


#define MID_RC 1500
#define PULSE_1p5MS (MID_RC *2)


#define CPPM_SYNC	2700
#define RC_DEADBAND 200
#define RC_TIMEOUT 100000
#define RC_CENTER_RANGE 3	// +2 us

#define LED_ACTION_TIME 300


#define CC_FACTOR 1


//PUSH BUTTON ACTION
#define LONG_PUSH_TIME_MS		2000



#define UART1_BAURD		115200


//------------------ LED DISPLAY --------------------

#define LED1_GPIO   GPIOA
#define LED1_PIN    GPIO_Pin_2

#define LED2_GPIO   GPIOA
#define LED2_PIN    GPIO_Pin_3

#define LED3_GPIO   GPIOA
#define LED3_PIN    GPIO_Pin_4


//---------------  SWITCH INPUT ----------------

#define PUSH_SW_GPIO   GPIOA
#define PUSH_SW_PIN    GPIO_Pin_5
	
#define MODE_SW_GPIO	 GPIOA
#define MODE_SW_PIN			GPIO_Pin_0


//---------------PPM OUTPUT CHANNEL -------------

#define PWM1_GPIO   GPIOA
#define PWM1_PIN    GPIO_Pin_6

#define PWM2_GPIO   GPIOA
#define PWM2_PIN    GPIO_Pin_7

#define PWM3_GPIO   GPIOB
#define PWM3_PIN    GPIO_Pin_1




//JUMPER PAD SETTING 

//#define J1_GPIO  GPIOB
//#define J1_PIN		GPIO_Pin_1

//#define J2_GPIO  GPIOA
//#define J2_PIN		GPIO_Pin_7

//#define J3_GPIO  GPIOA
//#define J3_PIN		GPIO_Pin_3

//#define J4_GPIO  GPIOA
//#define J4_PIN		GPIO_Pin_5

//#define J5_GPIO  GPIOA
//#define J5_PIN		GPIO_Pin_4


//#define TX1_GPIO  GPIOA
//#define TX1_PIN		GPIO_Pin_9
//#define RX1_GPIO  GPIOA
//#define RX1_PIN		GPIO_Pin_10



//RC INPUT PIN SETTING

//#define RC_IN_GPIO		GPIOA
//#define CH1_PIN		GPIO_Pin_9
//#define CH2_PIN		GPIO_Pin_10




#define ir_data_t_END	0xFFFF


#define LED1_TOGGLE              digitalToggle(LED1_GPIO, LED1_PIN)
#define LED1_ON                 digitalHi(LED1_GPIO, LED1_PIN)
#define LED1_OFF                  digitalLo(LED1_GPIO, LED1_PIN)

#define LED2_TOGGLE              digitalToggle(LED2_GPIO, LED2_PIN)
#define LED2_ON                 digitalHi(LED2_GPIO, LED2_PIN)
#define LED2_OFF                  digitalLo(LED2_GPIO, LED2_PIN)

#define LED3_TOGGLE              digitalToggle(LED3_GPIO, LED3_PIN)
#define LED3_ON                 digitalHi(LED3_GPIO, LED3_PIN)
#define LED3_OFF                  digitalLo(LED3_GPIO, LED3_PIN)

#define LED_MENUAL_ON			LED1_ON;
#define LED_MENUAL_OFF			LED1_OFF;
#define LED_NETURAL_ON			LED2_ON;
#define LED_NETURAL_OFF		LED2_OFF;
#define LED_AUTO_ON					LED3_ON;
#define LED_AUTO_OFF					LED3_OFF;



/*
#define IR_LED_TOGGLE              digitalToggle(IR_LED_GPIO, IR_LED_PIN)
#define IR_LED_ON                 digitalHi(IR_LED_GPIO, IR_LED_PIN)
#define IR_LED_OFF                  digitalLo(IR_LED_GPIO, IR_LED_PIN)
*/
//#define SW_TOGGLE              digitalToggle(SW2_GPIO, SW2_PIN)
//#define SW_ON                 digitalHi(SW2_GPIO, SW2_PIN)
//#define SW_OFF                  digitalLo(SW2_GPIO, SW2_PIN)


#define digitalHi(p, i)     { p->BSRR = i; }
#define digitalLo(p, i)     { p->BRR = i; }
#define digitalToggle(p, i) { p->ODR ^= i; }




 //#include "stm32f0xx_conf.h"
#include "drv_system.h"
#include "drv_timer.h"
//#include "rc_input.h"
//#include "ir_protocol.h"
#include "queue.h"
#include "drv_uart.h"
#include "stm32f0xx_it.h"

#include "drv_adc.h"
#include "drv_pwm.h"
#include "drv_keyInput.h"
//#include "drv_flash.h"
#include "config.h"

