#pragma once


typedef enum { VR, MCU_TEMP
	
} _adc_channel;

extern __IO uint16_t ADC_Value[2];


void adc_GPIO_Init(void);
void ADC_CH_DMA_Config(void);
