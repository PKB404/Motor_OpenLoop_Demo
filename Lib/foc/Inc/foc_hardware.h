#ifndef __FOC_HARDWARE_H__
#define __FOC_HARDWARE_H__

#include <stdint.h>



#define FOC_DRIVER_TIM          htim1
#define ADC_BUFFER_SIZE         3

extern volatile uint8_t adc_conversion_flag;
extern volatile float adc_buf[ADC_BUFFER_SIZE];

void FOC_PWM_Init(void);

void FOC_PWM_DeInit(void);

void FOC_PWM_SetCompare(uint32_t ccr1, uint32_t ccr2, uint32_t ccr3);

void FOC_ADC_Init(void);







#endif

