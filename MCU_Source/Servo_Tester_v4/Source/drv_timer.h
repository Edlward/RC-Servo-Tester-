#pragma once

//#define   IR_PWM_ON			TIM_CtrlPWMOutputs(TIM3, ENABLE)
//#define   IR_PWM_OFF			TIM_CtrlPWMOutputs(TIM3, DISABLE)

void TIM14_IRQHandler(void);
void TIM16_IRQHandler(void);
void loop_Timer_Init(void);
void loop_Timer_set(uint16_t ms);

 
// void IR_PWM_StartStop(bool onoff);
// void IR_PWM_Config2(void) ;
//void IR_Timer_Set(uint16_t microtime);


