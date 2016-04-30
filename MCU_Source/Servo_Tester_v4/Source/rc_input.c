#include "board.h"
//#ifdef RC_INPUT
#define MAX_INPUTS 2
//#define MAX_INPUTS 0
#define MAX_SERVOS 0

//static pwmPortData_t pwmPorts[MAX_PORTS];
//static uint16_t captures[MAX_INPUTS];
//static pwmPortData_t *motors[MAX_MOTORS];
//static pwmPortData_t *servos[MAX_SERVOS];
//static uint8_t numMotors = 0;
//static uint8_t numServos = 0;
//static uint8_t  numInputs = 0;
uint16_t failsafeCnt =0;



static struct TIM_Channel {
    TIM_TypeDef *tim;
    uint16_t channel;
    uint16_t cc;
} Channels[] = {
    { TIM1, TIM_Channel_2, TIM_IT_CC2 },
    { TIM1, TIM_Channel_3, TIM_IT_CC3 },
   // { TIM1, GPIOA, GPIO_Pin_9, TIM_Channel_2, GPIO_AF_2, TIM1_CC_IRQn, TIM_IT_CC2, 0, },          // RC_CH1
   // { TIM1, GPIOA, GPIO_Pin_10, TIM_Channel_3, GPIO_AF_2, TIM1_CC_IRQn, TIM_IT_CC3, 0, },          // RC_CH2

};



rcData_t rcData[RC_DATA_SIZE];




//-------------------------------------
// Rc Input Initialize
//-------------------------------------

void initRCPins(void) {
	uint8_t id=0;
	//  uint8_t Channel_count = sizeof(Channels) / sizeof(Channels[0]);
	
  for (id = 0; id < RC_DATA_SIZE; id++)
  {
    rcData[id].rise = 0;
    rcData[id].fall = 0;
    rcData[id].rx               = 1500;
    rcData[id].update           = true;
    rcData[id].valid            = true;
    rcData[id].rcSpeed          = 0.0;
    rcData[id].setpoint         = 0.0;
  }

		pwmInputInit();
}




//------------------------------------------RC INPUT DRIVER --------------------------------------------------


static TIM_ICInitTypeDef  TIM_ICInitStructure = { 0, };

uint8_t rcActive = false;

///////////////////////////////////////////////////////////////////////////////

static void serialPWM_IRQHandler(TIM_TypeDef *tim)
{
    uint16_t diff;
    static uint16_t now;
    static uint16_t last = 0;
    static uint8_t  chan = 0;
//		struct rcData_t *state = &rcData[0];		//rc 짐벌 제어를 위해서 변수 변경

	
    if (TIM_GetITStatus(tim, TIM_IT_CC2) == SET)
    {
        last = now;
        now = TIM_GetCapture1(tim);
        rcActive = true;
    }

    TIM_ClearITPendingBit(tim, TIM_IT_CC2);

    diff = now - last;

    if (diff > CPPM_SYNC )   // Per http://www.rcgroups.com/forums/showpost.php?p=21996147&postcount=3960
    {                      // "So, if you use 2.5ms or higher as being the reset for the PPM stream start,
        chan = 0;          // you will be fine. I use 2.7ms just to be safe."
    }
    else
    {
        if (diff > MIN_RC && diff < MAX_RC  && chan < RC_DATA_SIZE)    // 750 to 2250 ms is our 'valid' channel range
        {
 //           Inputs[chan].capture = diff;
					rcData[chan].rx = diff;
					rcData[chan].valid=true;
					rcData[chan].update=true;
					rcData[chan].microsLastUpdate =micros();	//<<-추가					
					
        }
        chan++;
    }
}

///////////////////////////////////////////////////////////////////////////////

static void parallelPWM_IRQHandler(TIM_TypeDef *tim)
{
	uint8_t i;
	uint16_t val = 0;
	uint16_t pulseInPWMtmp;

    for (i = 0; i < RC_DATA_SIZE; i++)  {
        struct TIM_Channel channel = Channels[i];
//        struct PWM_State *state = &Inputs[i];
		struct rcData_t *state = &rcData[i];		//rc 짐벌 제어를 위해서 변수 변경
		
		
        if (channel.tim == tim && (TIM_GetITStatus(tim, channel.cc) == SET))
        {
            TIM_ClearITPendingBit(channel.tim, channel.cc);
            if ( i == 0 )
                rcActive = true;

            switch (channel.channel)
            {
                case TIM_Channel_1:
                    val = TIM_GetCapture1(channel.tim);
                    break;
                case TIM_Channel_2:
                    val = TIM_GetCapture2(channel.tim);
                    break;
                case TIM_Channel_3:
                    val = TIM_GetCapture3(channel.tim);
                    break;
                case TIM_Channel_4:
                    val = TIM_GetCapture4(channel.tim);
                    break;
            }

            if (state->state == 0)
                state->rise = val;
            else
                state->fall = val;

            if (state->state == 0)
            {
                // switch states
                state->state = 1;

                TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
                TIM_ICInitStructure.TIM_Channel = channel.channel;
                TIM_ICInit(channel.tim, &TIM_ICInitStructure);
            }
            else
            {
                // compute capture
//                state->capture = state->fall - state->rise;
				pulseInPWMtmp =  state->fall - state->rise;
				if ((pulseInPWMtmp >= MIN_RC) && (pulseInPWMtmp <= MAX_RC)) 	{
					state->rx = pulseInPWMtmp;
					state->valid=true;
					state->update=true;
					state->microsLastUpdate =micros();	//<<-추가
//					state->rx = state->fall - state->rise;
				}
				

                // switch state
                state->state = 0;

                TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
                TIM_ICInitStructure.TIM_Channel = channel.channel;
                TIM_ICInit(channel.tim, &TIM_ICInitStructure);
            }
        }
    }	//for
}

///////////////////////////////////////////////////////////////////////////////

// void TIM1_IRQHandler(void)
 void TIM1_CC_IRQHandler(void)
 {
	 if (RCINPUT ==CPPM)
	         serialPWM_IRQHandler(TIM1);
     else
         parallelPWM_IRQHandler(TIM1);
 }


///////////////////////////////////////////////////////////////////////////////



static void pwmInputInit(void)
{
  uint8_t Channel_count = sizeof(Channels) / sizeof(Channels[0]);

	
    GPIO_InitTypeDef GPIO_InitStructure            = { 0, };
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = { 0, };
    NVIC_InitTypeDef NVIC_InitStructure            = { 0, };
    uint8_t i;

    if (RCINPUT == CPPM)	//CPPM INPUT
    {
//		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
		
        // PPM CH1 input
        // Configure TIM1_CH2, TIM1_CH3 for PPM input
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;					//Alternate function
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

        GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);			//TIME1_CH2
//		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2);			//TIME1_CH2

        // Input timer on TIM4 only for PPM
        NVIC_InitStructure.NVIC_IRQChannel                   = TIM1_CC_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;

        NVIC_Init(&NVIC_InitStructure);

        // TIM4 timebase
        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

        TIM_TimeBaseStructure.TIM_Prescaler   = (SystemCoreClock / 1000000 ) - 1;;
        TIM_TimeBaseStructure.TIM_Period      = 0xFFFF;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

        TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

        // Input capture on TIM4_CH2 for PPM
        TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
        TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
        TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
        TIM_ICInitStructure.TIM_ICFilter    = 0x0;
        TIM_ICInitStructure.TIM_Channel     = TIM_Channel_2;			

        TIM_ICInit(TIM1, &TIM_ICInitStructure);

        // TIM3_CH2 capture compare interrupt enable
        TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);				
        TIM_Cmd(TIM1, ENABLE);
    }


    else	//PPM INPUT
    {
       // Configure TIM1,  CH2, CH3 channels
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//직결저항이 크므로  pull-up시 시그널이 전달되지 않음.
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);			//TIME1_CH2
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2);		////TIME1_CH3


        // Input timers on TIM2 and TIM3 for PWM
        NVIC_InitStructure.NVIC_IRQChannel                   = TIM1_CC_IRQn ;
        NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
        NVIC_Init(&NVIC_InitStructure);


        // TIM1 timebase
        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

        TIM_TimeBaseStructure.TIM_Prescaler   = (48 - 1);		
        TIM_TimeBaseStructure.TIM_Period      = 0xffff;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;


        TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

        // PWM Input capture
        TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
        TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
        TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
        TIM_ICInitStructure.TIM_ICFilter    = 0x0;


        for (i = 0; i < Channel_count; i++)
        {
            TIM_ICInitStructure.TIM_Channel = Channels[i].channel;
            TIM_ICInit(Channels[i].tim, &TIM_ICInitStructure);
        }
        TIM_ICInit(TIM1, &TIM_ICInitStructure);

        TIM_ITConfig(TIM1, TIM_IT_CC2  | TIM_IT_CC3, ENABLE);
        TIM_Cmd(TIM1, ENABLE);
    }
}

///////////////////////////////////////////////////////////////////////////////

//uint16_t pwmRead(uint8_t channel)
//{
//    return rcData[channel].rx;
//}

// check for RC timout

void checkRcTimeouts(void) {
	int32_t microsNow = micros();
	int32_t microsLastUpdate;
	int8_t id=0;
	
	for (id = 0; id < RC_DATA_SIZE; id++) {
    microsLastUpdate = rcData[id].microsLastUpdate;			// 시간 측정 루틴 추가 필요.
    if (rcData[id].valid && ((microsNow - microsLastUpdate)/CC_FACTOR) > RC_TIMEOUT) 
    {
      rcData[id].rx     = MID_RC;
      rcData[id].valid  = false;
      rcData[id].update = true;
    }
  }
}

//#endif


