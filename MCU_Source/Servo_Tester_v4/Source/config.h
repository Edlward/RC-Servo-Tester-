#pragma once


typedef enum {
					PPM,
					CPPM,
					UPWM,
	
} _output_mode;


typedef enum {
					INIT,
					MENUAL,
					NETURAL,
					AUTO,
					REMOTE,

} _runningmode;

typedef enum  {
	DIR_UP,
	DIR_DOWN,

} _count_direction;


typedef enum  {
	POTENTIO,
	SERIAL,

} _control_mode;


typedef struct  {
	
		uint16_t 	vrPosion;
		uint16_t 	PwmMode;
		uint16_t	PwmRate;		//pwm output Frequency
		uint16_t	servoPwmRate;
		uint16_t 	cycleTime;
		uint16_t  runningMode;
		uint16_t 	autoCycleTime;
		uint16_t VR_ADC_average;
		uint16_t max_rc;
		uint16_t min_rc;
		bool PPM_STDmode;
		uint16_t PPM_CH_vlaue[3];
//		bool rxd_Flag;
//		bool uPWM_Mode;
			uint32_t push_time;
	

} system_t;


typedef enum {
    VAR_UINT8,
    VAR_INT8,
    VAR_UINT16,
    VAR_INT16,
    VAR_UINT32,
    VAR_FLOAT
} vartype_e;

typedef struct {
    const char *name;
    const uint8_t type; // vartype_e
    void *ptr;
    const int32_t min;
    const int32_t max;
	
	void (* function) (void);
	
} clivalue_t;


typedef struct config_t {
	
		uint8_t ctrl_mode[3];
		uint8_t PPM_Mode[3];
//		uint16_t Ch1_Value;
//		uint16_t Ch2_Value;
	
		uint16_t Ch_Value[3];
} config;


extern system_t 	sys;
extern config cfg;

void mode_Change(void);
void configInit (void);

 void cliSet(char *cmdline);
static float _atof(const char *p);

static char *ftoa(float x, char *floatString);
char *itoa(int i, char *a, int r);
static char *i2a(unsigned i, char *a, unsigned r);
static void cliPrintVar(const clivalue_t *var, uint32_t full);
void cliSet(char *cmdline);
bool read_uart_string(void);
void pwm_mode_set(void) ;		//ultra pwm mode or std ppm mode 설정 
void  push_putton_long_push_check(uint32_t * push_time);
void led_street_action (void);	//LED를 20ms 주기로 순차적으로 3회 점멸함.
void led_wink_action (void) ;	//LED를 20ms 주기로  3회  동시 점멸함.
void led_boom_action (void);	//LED를 중심에서 외과으로 점멸함.

