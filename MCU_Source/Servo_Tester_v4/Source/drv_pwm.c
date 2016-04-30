#include "board.h"


drv_pwm_output_config_t pwmOutputConfig;

///////////////////////////////////////////////////////////////////////////////

static volatile uint32_t *OutputChannels[] = {
    &(TIM3->CCR1),
    &(TIM3->CCR2),
    &(TIM3->CCR4),
};


void pwmWrite(uint8_t channel, uint16_t value)
{
    if (channel < 3)
        *OutputChannels[channel] = value;
}


///////////////////////////////////////////////////////////////////////////////

void pwmOutputInit(drv_pwm_output_config_t * init)
{
    GPIO_InitTypeDef GPIO_InitStructure = { 0, };
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = { 0, };
    TIM_OCInitTypeDef TIM_OCInitStructure = { 0, };

    // Outputs
    // PWM1 TIM3_CH1 PA6
    // PWM2 TIM3_CH2 PA7
    // PWM3 TIM3_CH4 PB1

	//  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	//  /* Time Base configuration */
//  TIM_TimeBaseStructure.TIM_Prescaler = 0;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//  TIM_TimeBaseStructure.TIM_Period = (SystemCoreClock / 10000 ) - 1;
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

//  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
    // Output pins
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_1);
	
    // Output timers
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 1000000 ) - 1;

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;

    if (init->useServos == true) {

        TIM_TimeBaseStructure.TIM_Period = (1000000 / init->servoPwmRate) - 1;
        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

//        TIM_OCInitStructure.TIM_Pulse = PULSE_1MS;
        TIM_OCInitStructure.TIM_Pulse = sys.min_rc*2;
        TIM_OC1Init(TIM3, &TIM_OCInitStructure);
        TIM_OC2Init(TIM3, &TIM_OCInitStructure);
        TIM_OC4Init(TIM3, &TIM_OCInitStructure);

    } else {
        TIM_TimeBaseStructure.TIM_Period = (1000000 / init->escPwmRate) - 1;
        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  //      TIM_OCInitStructure.TIM_Pulse = PULSE_1MS;
        TIM_OCInitStructure.TIM_Pulse = sys.min_rc*2;
        TIM_OC1Init(TIM3, &TIM_OCInitStructure);
        TIM_OC2Init(TIM3, &TIM_OCInitStructure);
        TIM_OC4Init(TIM3, &TIM_OCInitStructure);
    }

    TIM_Cmd(TIM3, ENABLE);
    TIM_CtrlPWMOutputs(TIM3, ENABLE);
};


//void PWM_Init(void) {
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;


//	/* GPIOA Clocks enable */
//  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
//  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB, ENABLE);


////--------- pwm ch1, ch2 ------------
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);

////------ pwm ch3 -------------
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);

//	
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);
//  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_1);

///* Compute the value to be set in ARR regiter to generate signal frequency at 37.9 Khz */
//  TimerPeriod = (SystemCoreClock / 37900 ) - 1;
////  TimerPeriod = (SystemCoreClock / 10000 ) - 1;
//  /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 */
//  IR_Duty = (uint16_t) (((uint32_t) 4 * (TimerPeriod - 1)) / 10);
//  

//  /* TIM3 clock enable */
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
//  
//  /* Time Base configuration */
//  TIM_TimeBaseStructure.TIM_Prescaler = 0;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

//  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

//  /* Channel 1 Configuration in PWM mode */
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1 ;
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;

////	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
//  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
//  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
//  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

//  TIM_OCInitStructure.TIM_Pulse = IR_Duty;
//  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

//  /* TIM3 counter enable */
////  TIM_Cmd(TIM3, ENABLE);

//  /* TIM3 Main Output Enable */
//  TIM_CtrlPWMOutputs(TIM3, ENABLE);
//  
//TIM3->CNT = 0;
//TIM_Cmd(TIM3, ENABLE);
//  
//}
//  

