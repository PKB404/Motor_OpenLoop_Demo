#include "foc_hardware.h"

void FOC_PWM_Init(void)
{
    HAL_TIM_Base_Start_IT(&FOC_DRIVER_TIM);
    
    FOC_DRIVER_TIM.Instance->CCR1 = 0;
    FOC_DRIVER_TIM.Instance->CCR2 = 0;
    FOC_DRIVER_TIM.Instance->CCR3 = 0;
    
    HAL_TIM_PWM_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_3);
}



void FOC_PWM_DeInit(void)
{
    HAL_TIM_Base_Stop_IT(&FOC_DRIVER_TIM);
    
    HAL_TIM_PWM_Stop(&FOC_DRIVER_TIM, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&FOC_DRIVER_TIM, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&FOC_DRIVER_TIM, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Stop(&FOC_DRIVER_TIM, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&FOC_DRIVER_TIM, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Stop(&FOC_DRIVER_TIM, TIM_CHANNEL_3);    
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    //16Khz进一次(RCR = 1)
    if(htim->Instance == FOC_DRIVER_TIM.Instance)
    {
        
    }

}

