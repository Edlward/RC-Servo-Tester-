#pragma once


//#ifdef RC_INPUT

//enum {ppm,cppm} _rc_input;
typedef enum {PPM, CPPM} _rc_input;
#define RCINPUT		PPM		//RC INPUT종류에 따라서 PPM 또는 CPPM설정 !!!!!

#if RCINPUT == PPM
		#define RC_DATA_SIZE 2
	//	#define MAX_PORTS 2

#else
		#define RC_DATA_SIZE 18
#endif

// LP filter coefficient
//#define LOWPASS_K_FLOAT(TAU) (DT_FLOAT/(TAU+DT_FLOAT))

typedef enum  {RC_CH1, RC_CH2, RC_CH3, RC_CH4, RC_CH5, RC_CH6, RC_CH7, RC_CH8} _rc_input_ch;

typedef enum  {RC_LOW_POSION, RC_MID_POSION, RC_HIGH_POSION} _rc_posion_ch;



typedef struct {
    TIM_TypeDef *tim;
    GPIO_TypeDef *gpio;
    uint32_t pin;
    uint8_t channel;
//		uint8_t mode;
		uint8_t AF;
    uint8_t irq;
		uint8_t tim_it;
    uint8_t outputEnable;
} pwmHardware_t;


//void rc_input_Init(void) ;



typedef struct rcData_t
{
	uint8_t state;
//	 uint32_t microsRisingEdge;
	 uint32_t microsLastUpdate;		//마지막 업데이트 시간
	 uint32_t rise;
	 uint32_t fall;		
	 uint16_t rx;				//PWM 입력 값
	 bool     update;		//갱신유무
	 bool     valid;			//유효성
	 float    rcSpeed;		
	 float    setpoint;
} rcData_t;


///////////////////////////////////////////////////////////////////////////////

extern uint8_t	 rcActive;
extern rcData_t rcData[];
extern float rcLPF_tc;

extern	float PitchPhiSet;;
extern	float RollPhiSet;
extern 	float YawPhiSet;

///////////////////////////////////////////////////////////////////////////////

void TIM1_CC_IRQHandler(void);

void initRCPins(void);
uint16_t pwmRead(uint8_t channel);
///////////////////////////////////////////////////////////////////////////////

static void pwmInputInit(void);
 void utilLP_float(float * q, float i, float coeff);
void checkRcTimeouts(void);
///////////////////////////////////////////////////////////////////////////////

//#endif


