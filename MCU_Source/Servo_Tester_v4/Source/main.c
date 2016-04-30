

/* Includes ------------------------------------------------------------------*/
#include "board.h"

/** @addtogroup STM32F0308_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup IO_Toggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define BSRR_VAL        0x0300

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//GPIO_InitTypeDef        GPIO_InitStructure;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

 void RCC_init(void){
   ErrorStatus HSEStartUpStatus;
   RCC_DeInit(); //RCC system reset(for debug purpose)


   RCC_HSEConfig(RCC_HSE_ON); //Enable HSE
   HSEStartUpStatus = RCC_WaitForHSEStartUp(); //Wait till HSE is ready

 

   if(HSEStartUpStatus == SUCCESS) {
      FLASH_PrefetchBufferCmd(ENABLE); //Enable Prefetch Buffer
      FLASH_SetLatency(FLASH_Latency_1); //Flash 1 wait state
      RCC_HCLKConfig(RCC_SYSCLK_Div1); //HCLK = SYSCLK
	   RCC_PCLKConfig(RCC_HCLK_Div1);
//      RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_6); //PLLCLK = 8MHz * 6 = 48 MHz
      RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_6); //PLLCLK = 8MHz * 6 = 48 MHz
      RCC_PLLCmd(ENABLE); //Enable PLL

 

      while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //Wait till PLL is ready
      RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //Select PLL as system clock source
      while (RCC_GetSYSCLKSource() != 0x08); //Wait till PLL is used as system clock source
   }

}	

int main(void) {


	RCC_init();			//Clock Setting
	systemInit();			//GPIO Init
		init_queue(&Uart1RxQueue);
		init_queue(&Uart1TxQueue);
		USART1_Init();
	
		adc_GPIO_Init();
	  ADC_CH_DMA_Config();



	configInit();
	loop_Timer_Init();

	loop_Timer_set(1200);
	
	#ifndef ESC_TEST_MODE
			sys.max_rc = MAX_RC_LIMIT;
			sys.min_rc =MIN_RC_LIMIT;
			pwmOutputConfig.useServos=true;			//fast ppm mode로 동작
			sys.PPM_STDmode=false;
	#else
			sys.max_rc = MAX_RC_LIMIT;
			sys.min_rc =MIN_RC_LIMIT;
			sys.PPM_STDmode =false;
			pwmOutputConfig.useServos=false;			//servo mode로 동작
	#endif

			
		pwmOutputConfig.escPwmRate = FAST_PWM_FREQ;		//Frequency 200Hz
		pwmOutputConfig.servoPwmRate =STANDARD_PWM_FREQ;	//Frequency 50Hz
		pwmWrite(0, (uint16_t)MID_RC);
		pwmWrite(1, (uint16_t)MID_RC);
		pwmWrite(2, (uint16_t)MID_RC);
		pwmOutputInit(&pwmOutputConfig);
	
	EXTI_Config();
	
	
	cfg.ctrl_mode[0] =POTENTIO;
	cfg.ctrl_mode[1] =POTENTIO;
	cfg.ctrl_mode[2] =POTENTIO;





	while (1) {
			uint32_t msNow;
			static uint32_t  actionTime, elapsedTime, oldTime;
			uint8_t i=0;
			msNow = millis();
			elapsedTime = elapsedTime + (msNow - oldTime);
			oldTime = msNow;  

		
				if(actionTime + 5 <= elapsedTime) {		//5ms loop running
						actionTime = elapsedTime;
					
			
					
						switch(sys.runningMode) {
							case MENUAL:
										for(i = 0; i<3;i++) {
												if(	cfg.ctrl_mode[i] == POTENTIO) {
														sys.PPM_CH_vlaue[i] = sys.min_rc + (uint16_t) ((sys.VR_ADC_average*1000)/(4095*1000/((uint32_t)sys.max_rc - sys.min_rc)));
												}
												else {		//uart ppm control 
														sys.PPM_CH_vlaue[i] = cfg.Ch_Value[i];
												}
														pwmWrite(i,sys.PPM_CH_vlaue[i]); 		//VR 위치에 따른 PPM 출력 DUTY 출력

										}

									if(TIM3->CCR1 <= MID_RC+RC_CENTER_RANGE && TIM3->CCR1 >= MID_RC-RC_CENTER_RANGE) {		//중립점시 LED 점멸 
											LED_AUTO_ON;
											LED_MENUAL_ON;
											LED_NETURAL_ON;
	
									}
									else if(TIM3->CCR1 < MIN_RC_STD || TIM3->CCR1 >MAX_RC_STD) {		//표준 범위를 벗어날경우 LED 밝기를 약하게 변경
												static uint8_t k=0;
													switch(k) {
															case 1:
																LED1_ON;
																break;
								
															case 2:
															case 3:
															case 4:
																LED1_OFF;
																break;

															default:
																k=0;
																break;
													}
													k++;

									}
									else {
											LED_MENUAL_ON;
											LED_NETURAL_OFF;
											LED_AUTO_OFF;
									}
									break;

							case NETURAL:
									LED_MENUAL_OFF;
									LED_NETURAL_ON;
									LED_AUTO_OFF;
									pwmWrite(0, (uint16_t)MID_RC);
									pwmWrite(1, (uint16_t)MID_RC);
									pwmWrite(2, (uint16_t)MID_RC);
									break;

							case AUTO:
							//	loop_Timer_set(500+ ADC_Value[VR]/2);
				//					LED_AUTO_ON;
										
									if(TIM3->CCR1 <= MID_RC+2 && TIM3->CCR1 >= MID_RC-2) {		//중립점시 LED 점멸 
											LED_AUTO_ON;
											LED_MENUAL_ON;
											LED_NETURAL_ON;
							//			failureMode(0);
									}
									else {
											LED_AUTO_ON;
											LED_MENUAL_OFF;
											LED_NETURAL_OFF;

										}
									break;

							default :
									break;
						}		//switch 
			
					}
					push_putton_long_push_check(&sys.push_time);		//연속으로 버튼을 누른 시간을 측정
					read_uart_string();
					
					if(sys.VR_ADC_average < 50 && sys.push_time > LONG_PUSH_TIME_MS) {		//최소 상태에서 오래 누를경우
							led_wink_action();
						pwmOutputConfig.useServos=false;		//FAST PWM MODE로 동작	(400Hz = 2.5ms 주기)
						pwm_mode_set();
						sys.runningMode = MENUAL;
					}
					else if(sys.VR_ADC_average > 4045 && sys.push_time > LONG_PUSH_TIME_MS) {		//최대 상태에서 오래 누를경우
						led_street_action();
						pwmOutputConfig.useServos=true;		//STANDARD SERVO MODE로 동작 (40Hz = 25ms 주기)
						pwm_mode_set();
						sys.runningMode = MENUAL;
					}
					else if(sys.VR_ADC_average > 2023 && sys.VR_ADC_average < 2073  && sys.push_time > LONG_PUSH_TIME_MS) {		//중심에 근점하여 길게 누를경우 
						led_boom_action();
						if(sys.PPM_STDmode ==true) {	//wide ppm pulse generation  800 ~ 2200us high width pulse
								sys.max_rc = MAX_RC_LIMIT;
								sys.min_rc =MIN_RC_LIMIT;
								sys.PPM_STDmode =false;
							
						}
						else {		//standart ppm pulse generation  1000 ~ 2000us high width pulse
								sys.max_rc = MAX_RC_STD;
								sys.min_rc =MIN_RC_STD;
								sys.PPM_STDmode =true;
						}
						sys.runningMode = MENUAL;
					}

	}//while(1)
}//main

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
