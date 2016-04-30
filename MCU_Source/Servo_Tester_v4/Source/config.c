#include "board.h"

system_t 	sys;
config cfg;

void pwm_mode_set(void) {

		pwmOutputInit(&pwmOutputConfig);

}

const clivalue_t valueTable[] = {
    { "ChMode1", VAR_UINT8, &cfg.ctrl_mode[0], 0, 1,  },
    { "ChMode2", VAR_UINT8, &cfg.ctrl_mode[1], 0, 1,  },
    { "ChMode3", VAR_UINT8, &cfg.ctrl_mode[2], 0, 1,  },
//    { "PwmMode", VAR_UINT8, &cfg.PPM_Mode[0], 0, 1,  },
    { "PwmMode", VAR_UINT8, &pwmOutputConfig.useServos, 0, 1,pwm_mode_set  },
    { "ChValue1", VAR_UINT16, &cfg.Ch_Value[0], 700, 2300,},
    { "ChValue2", VAR_UINT16, &cfg.Ch_Value[1], 700, 2300,},
    { "ChValue3", VAR_UINT16, &cfg.Ch_Value[2], 700, 2300,},
		 { "CycleTime", VAR_UINT16, &sys.cycleTime, 3, 50,},
		 
		
	};
#define VALUE_COUNT (sizeof(valueTable) / sizeof(valueTable[0]))
	

void configInit (void) {
		sys.autoCycleTime = 500;			//ms
		sys.cycleTime = 20;
		sys.PwmRate	= 50;		//Frequency
		sys.vrPosion	= MID_RC;;
		sys.runningMode	= MENUAL;
		sys.VR_ADC_average = ADC_Value[VR];


}

void led_boom_action (void) {	//LED를 중심에서 외과으로 점멸함.
		 uint8_t j=0;
				LED_NETURAL_OFF;
				LED_AUTO_OFF;
				LED_MENUAL_OFF;
			for(j=0;j<3;j++) {
				LED_NETURAL_ON;
				delay(200);
				LED_NETURAL_OFF;
				LED_MENUAL_ON;
				LED_AUTO_ON;
				delay(200);
				LED_AUTO_OFF;
				LED_MENUAL_OFF;
				delay(200);
		}
}

void led_street_action (void) {	//LED를 주기로 순차적으로 3회 점멸함.
		 uint8_t j=0;
				LED_NETURAL_OFF;
				LED_AUTO_OFF;
				LED_MENUAL_OFF;
			for(j=0;j<3;j++) {
				LED_MENUAL_ON;
				delay(100);
				LED_MENUAL_OFF;
				LED_NETURAL_ON;
				delay(100);
				LED_NETURAL_OFF;
				LED_AUTO_ON;
				delay(100);
				LED_AUTO_OFF;
				LED_NETURAL_ON;
				delay(100);
				LED_NETURAL_OFF;
				LED_MENUAL_ON;
				delay(100);
		}
}


void led_wink_action (void) {	//LED를 주기로  3회  동시 점멸함.
		 uint8_t j=0;
			for(j=0;j<4;j++) {
				LED_MENUAL_ON;
				LED_AUTO_ON;
				LED_NETURAL_ON;
				delay(200);
				LED_MENUAL_OFF;
				LED_AUTO_OFF;
				LED_NETURAL_OFF;
				delay(200);
		}
}


void  push_putton_long_push_check(uint32_t * push_time) {
	static uint32_t befor_time =0;
	static uint32_t push_sum_time =0;
	uint32_t timeNow;
	timeNow = millis();

	if (!GPIO_ReadInputDataBit(PUSH_SW_GPIO, PUSH_SW_PIN)){		//Push button 을 연속해고 누루고 있는 시간을 측정하여 돌려줌
			push_sum_time += timeNow - befor_time;
	}
	else {
			push_sum_time =0;
	}
		befor_time= timeNow;		
	
		* push_time = push_sum_time;
}





#define KEY_BOUNSING_TIME 50

void mode_Change(void) {
	static uint32_t readDeadline =0;
	uint32_t timeNow;
	timeNow = millis();
	
	if ( (int64_t)timeNow - (readDeadline + KEY_BOUNSING_TIME) < 0 ) {	//이전 키가 눌러진후 디바운싱 시간이 경과되지 않을경우
			return ;
	}
			readDeadline= timeNow;	
	
		switch(sys.runningMode) {
			
			case MENUAL:
					sys.runningMode = NETURAL;
					break;

			case NETURAL:
					sys.runningMode = AUTO;
					break;

			case AUTO:
					sys.runningMode = MENUAL;
					break;

			default :
					break;
		}
		LED_MENUAL_OFF;
		LED_NETURAL_OFF;
		LED_AUTO_OFF;

}






static void cliSetVar(const clivalue_t *var, const int32_t value)
{
    switch (var->type) {
        case VAR_UINT8:
        case VAR_INT8:
            *(char *)var->ptr = (char)value;
            break;

        case VAR_UINT16:
        case VAR_INT16:
            *(short *)var->ptr = (short)value;
            break;

        case VAR_UINT32:
            *(int *)var->ptr = (int)value;
            break;

        case VAR_FLOAT:
            *(float *)var->ptr = *(float *)&value;
            break;
    }
}


 void cliSet(char *cmdline)
{
    uint32_t i;
    uint32_t len;
    const clivalue_t *val;
    char *eqptr = NULL;
    int32_t value = 0;
    float valuef = 0;

    len = strlen(cmdline);

	    if (len == 3 && cmdline[0] == '*') {
//        printf("Current settings: \r\n");
			for (i = 0; i < VALUE_COUNT; i++) {
				val = &valueTable[i];
				printf("%s=", valueTable[i].name);
//				cliPrintVar(val, len); // when len is 1 (when * is passed as argument), it will print min/max values as well, for gui
				cliPrintVar(val, 0);
				printf("\r");
			}
//			f.IMU_GRAPH_OUT=1;
	}


			
 	 
	 else if (strstr(cmdline, "=")) {
			void (*Func) (void);
			eqptr = strstr(cmdline, "=");
			// has equal, set var
			eqptr++;
			len--;
			value= atoi(eqptr);
			valuef = _atof(eqptr);
			for (i = 0; i < VALUE_COUNT; i++) {
				val = &valueTable[i];
				if (strncasecmp(cmdline, valueTable[i].name, strlen(valueTable[i].name)) == 0) {
					if (valuef >= valueTable[i].min && valuef <= valueTable[i].max) { // here we compare the float value since... it should work, RIGHT?
						cliSetVar(val, valueTable[i].type == VAR_FLOAT ? *(uint32_t *)&valuef : value); // this is a silly dirty hack. please fix me later.
						if(valueTable[i].function!=NULL) {
							Func = valueTable[i].function;		//관련된 함수 호출
							Func();
						}
						printf("%s=", valueTable[i].name);
		//                printf("%d  ", value);
						cliPrintVar(val, 0);
						printf("\r");
	//					printf(" %d",cfg.gyroPitchKi);
					} else {
						printf("ERR: Value assignment out of range\r\n");
					}
					return;
				}
			}
			printf("ERR: Unknown variable name\r\n");
		}
}


static void cliPrintVar(const clivalue_t *var, uint32_t full)
{
    int32_t value = 0;
    char buf[8];

    switch (var->type) {
        case VAR_UINT8:
            value = *(uint8_t *)var->ptr;
            break;

        case VAR_INT8:
            value = *(int8_t *)var->ptr;
            break;

        case VAR_UINT16:
            value = *(uint16_t *)var->ptr;
            break;

        case VAR_INT16:
            value = *(int16_t *)var->ptr;
            break;

        case VAR_UINT32:
            value = *(uint32_t *)var->ptr;
            break;

        case VAR_FLOAT:
            printf("%s", ftoa(*(float *)var->ptr, buf));
            if (full) {
                printf(" %s", ftoa((float)var->min, buf));
                printf(" %s", ftoa((float)var->max, buf));
            }
            return; // return from case for float only
    }
    printf("%d", value);
    if (full)
        printf(" %d %d", var->min, var->max);
}

static char *i2a(unsigned i, char *a, unsigned r)
{
    if (i / r > 0) 
        a = i2a(i / r, a, r);
    *a = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[i % r];
    return a + 1;
}

char *itoa(int i, char *a, int r)
{
    if ((r < 2) || (r > 36))
        r = 10;
    if (i < 0) {
        *a = '-';
        *i2a(-(unsigned)i, a + 1, r) = 0;
    } else 
        *i2a(i, a, r) = 0;
    return a;
} 


static char *ftoa(float x, char *floatString)
{
    int32_t value;
    char intString1[12];
    char intString2[12] = { 0, };
    char *decimalPoint = ".";
    uint8_t dpLocation;

    if (x > 0)                  // Rounding for x.xxx display format
        x += 0.0005f;
    else
        x -= 0.0005f;

    value = (int32_t) (x * 1000.0f);    // Convert float * 1000 to an integer

    itoa(abs(value), intString1, 10);   // Create string from abs of integer value

    if (value >= 0)
        intString2[0] = ' ';    // Positive number, add a pad space
    else
        intString2[0] = '-';    // Negative number, add a negative sign

    if (strlen(intString1) == 1) {
        intString2[1] = '0';
        intString2[2] = '0';
        intString2[3] = '0';
        strcat(intString2, intString1);
    } else if (strlen(intString1) == 2) {
        intString2[1] = '0';
        intString2[2] = '0';
        strcat(intString2, intString1);
    } else if (strlen(intString1) == 3) {
        intString2[1] = '0';
        strcat(intString2, intString1);
    } else {
        strcat(intString2, intString1);
    }

    dpLocation = strlen(intString2) - 3;

    strncpy(floatString, intString2, dpLocation);
    floatString[dpLocation] = '\0';
    strcat(floatString, decimalPoint);
    strcat(floatString, intString2 + dpLocation);

    return floatString;
}


#define white_space(c) ((c) == ' ' || (c) == '\t')
#define valid_digit(c) ((c) >= '0' && (c) <= '9')
static float _atof(const char *p)
{
    int frac = 0;
    double sign, value, scale;

    // Skip leading white space, if any.
    while (white_space(*p) ) {
        p += 1;
    }

    // Get sign, if any.
    sign = 1.0;
    if (*p == '-') {
        sign = -1.0;
        p += 1;

    } else if (*p == '+') {
        p += 1;
    }

    // Get digits before decimal point or exponent, if any.
    value = 0.0;
    while (valid_digit(*p)) {
        value = value * 10.0 + (*p - '0');
        p += 1;
    }

    // Get digits after decimal point, if any.
    if (*p == '.') {
        double pow10 = 10.0;
        p += 1;

        while (valid_digit(*p)) {
            value += (*p - '0') / pow10;
            pow10 *= 10.0;
            p += 1;
        }
    }

    // Handle exponent, if any.
    scale = 1.0;
    if ((*p == 'e') || (*p == 'E')) {
        unsigned int expon;
        p += 1;

        // Get sign of exponent, if any.
        frac = 0;
        if (*p == '-') {
            frac = 1;
            p += 1;

        } else if (*p == '+') {
            p += 1;
        }

        // Get digits of exponent, if any.
        expon = 0;
        while (valid_digit(*p)) {
            expon = expon * 10 + (*p - '0');
            p += 1;
        }
        if (expon > 308) expon = 308;

        // Calculate scaling factor.
        while (expon >= 50) { scale *= 1E50; expon -= 50; }
        while (expon >=  8) { scale *= 1E8;  expon -=  8; }
        while (expon >   0) { scale *= 10.0; expon -=  1; }
    }

    // Return signed and scaled floating point result.
    return sign * (frac ? (value / scale) : (value * scale));
}


bool read_uart_string(void) {
		static char uart_rxd_buffer[MAX_QUEUE_SIZE];
		static uint16_t i=0;

		static uint32_t termination_delay_time=0;
		static uint32_t time_now =0;
//		msg_send_t send_message;

//		send_message.busType = UART_BUS;


		time_now = millis();

		if(0==dequeue(&Uart1RxQueue,(uint8_t *)&uart_rxd_buffer[i])) {																//queue에서 수신값이 있을 경우 수신 값을 복사
					termination_delay_time = time_now;
					if(uart_rxd_buffer[i-1] == '\r' && uart_rxd_buffer[i] == '\n') {		//CR + LF 수신시

//							rxd_string_decoder( uart_rxd_buffer,  i+1, UART_BUS, NULL);		//길이 이므로 +1함.
							cliSet(uart_rxd_buffer);
						i=0;
						return 1;

					}
						i++;
						if(i >= MAX_QUEUE_SIZE) {						//rxd buffer over flow error
//								send_message.index =BUFFER_OVER_FLOW;
//								response_msg_send( &send_message);
								memset(uart_rxd_buffer,'\0',MAX_QUEUE_SIZE);								//수신 버퍼 클리어
								i=0;
						}
						return 0;
			}
					//------------------------------- 종료문자값인 \r\n  수신 지연시 time out 처리 -------------------------	
			 if(i >0 ) {		//버퍼에 값이 있고 종료문자 수신 전일 경우
						if(((int64_t)time_now - termination_delay_time-2000) > 0) {		//  타임아웃 초과시 
							memset(uart_rxd_buffer,'\0',MAX_QUEUE_SIZE);								//수신 버퍼 클리어
							i=0;
//							send_message.index =TIME_OUT;
//							response_msg_send( &send_message);
						}
						return 0;
			}
				termination_delay_time = time_now;
			return 0;
}



