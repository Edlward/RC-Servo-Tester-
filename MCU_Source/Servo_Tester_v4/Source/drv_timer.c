#include "board.h"

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

//uint16_t ir_data_t[32]={0,};


	 uint32_t posion = MID_RC*10000;

void TIM16_IRQHandler(void) {
//	static uint32_t posion = MID_RC;
	static uint32_t move_speed=1;
	static uint8_t direction = DIR_UP;
	
//	static uint16_t adc_sum;

	if (TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET) {
		// Clear TIM16 Capture Compare1 interrupt pending bit
		TIM_ClearITPendingBit(TIM16, TIM_IT_Update);

			sys.VR_ADC_average = (sys.VR_ADC_average*15 + ADC_Value[VR])/16; 			//VR ADC 이동평균처리 
		
		
		if(sys.runningMode == AUTO) {
			
				sys.VR_ADC_average = (sys.VR_ADC_average ==0)?1: sys.VR_ADC_average;

//					move_speed = ((float)sys.VR_ADC_average /2047 ) * ((float)sys.VR_ADC_average /2047 );		//지수함수 속도제어 
					move_speed = ((uint32_t)sys.VR_ADC_average*100 /2047 ) * ((uint32_t)sys.VR_ADC_average*100 /2047 );		//지수함수 속도제어 

//					move_speed =  (move_speed >0.002)? move_speed:0.002;	
					move_speed =  (move_speed >10)? move_speed:10;	
			switch(direction) {
				case DIR_UP:
						posion += move_speed;
						break;
				
				case DIR_DOWN:
						posion -= move_speed;
						break;
				
				default:
						break;
				
			}
			if((posion/10000) >=(sys.max_rc )) {
//					posion = (uint32_t)sys.max_rc*10000;
					direction = DIR_DOWN;
			}
			else if((posion/10000) <= (sys.min_rc )) {
//					posion = (uint32_t)sys.min_rc*10000;
					direction = DIR_UP;
			}			
			
					pwmWrite(0, (uint16_t) (posion/10000)); 
					pwmWrite(1,(uint16_t) (posion/10000)); 
					pwmWrite(2,(uint16_t) (posion/10000)); 
			
		}
		else {
				posion = MID_RC*10000;
				direction =DIR_UP;
				TIM16->CNT =0;
			
		}
		
		//		cfg_mode.loopFlag=true;
//			TIM16->CNT =0;
	}
}


void loop_Timer_set(uint16_t ms) {
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Period =1000;// + ms;
	TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 1000000 ) - 1;	//47
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);
}

void loop_Timer_Init(void){
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM16, ENABLE);
	TIM16->CNT =0;
	
	  /* TIM16 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16 , ENABLE);

	//TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_TimeBaseStructure.TIM_Period =1000;
	TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 1000000 ) - 1;	//47
//	TIM_TimeBaseStructure.TIM_Prescaler = 47;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	/* Enable the TIM3 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);

  /* TIM IT enable */
  TIM_ITConfig(TIM16, TIM_IT_Update ,ENABLE);// | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);

  /* TIM17 enable counter */
  TIM_Cmd(TIM16, ENABLE);

}


void AUTO_Timer_Init(void){
	uint16_t 	Period_Value = 0;
//	#define 	UPDATE_PRESCALER 47
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	  /* TIM14 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14 , ENABLE);

	//TIM_OCInitTypeDef  TIM_OCInitStructure;
	Period_Value = 1000 ;
	TIM_TimeBaseStructure.TIM_Period =Period_Value;
//	TIM_TimeBaseStructure.TIM_Prescaler = UPDATE_PRESCALER;
	TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 1000000 ) - 1;	//47
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	/* Enable the TIM3 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


//	TIM_TimeBaseStructure.TIM_Prescaler = UPDATE_PRESCALER;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

  /* TIM IT enable */
 /// TIM_ITConfig(TIM14, TIM_IT_Update ,ENABLE);// | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);


  /* TIM2 enable counter */
 // TIM_Cmd(TIM14, ENABLE);

}


void ir_Generator_start(void) {
//	TIM14->ARR = ir_data_t.Time[0];
//	TIM14->CNT =0;

//	TIM3->CNT = 0;
//	TIM_CtrlPWMOutputs(TIM3, ENABLE);
//	TIM_Cmd(TIM3, ENABLE);

	TIM14->CNT =0;
	TIM14->ARR = 10;
	

	TIM_ITConfig(TIM14, TIM_IT_Update ,ENABLE);
	TIM_Cmd(TIM14, ENABLE);

}

