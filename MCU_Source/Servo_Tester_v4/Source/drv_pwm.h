
#pragma once


typedef struct drv_pwm_output_config_t {
    uint8_t useServos;
    uint16_t escPwmRate;
    uint16_t servoPwmRate;
} drv_pwm_output_config_t;

extern drv_pwm_output_config_t pwmOutputConfig;

///////////////////////////////////////////////////////////////////////////////

void pwmWrite(uint8_t channel, uint16_t value);

///////////////////////////////////////////////////////////////////////////////

void pwmOutputInit(drv_pwm_output_config_t * init);



