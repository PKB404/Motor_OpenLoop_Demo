#include "foc_hardware.h"
#include "tim.h"
#include "adc.h"



#define ADC_BUFFER_SIZE 3
volatile uint16_t adc_buf[ADC_BUFFER_SIZE];

void FOC_PWM_Init(void)
{
//    HAL_TIM_Base_Start_IT(&FOC_DRIVER_TIM);
    
    FOC_DRIVER_TIM.Instance->CCR1 = 0;
    FOC_DRIVER_TIM.Instance->CCR2 = 0;
    FOC_DRIVER_TIM.Instance->CCR3 = 0;
    
    HAL_TIM_PWM_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_3);
    
    //ADC采样相关
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, 5250 - 1);
    HAL_ADCEx_InjectedStart(&hadc3);
    __HAL_ADC_ENABLE_IT(&hadc3, ADC_IT_JEOC); // 使能注入组转换完成中断HAL_ADCEx_InjectedStart(&hadc3);
    
    HAL_ADC_Start_DMA(&hadc3, (uint32_t*)adc_buf, ADC_BUFFER_SIZE);
    
    
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



void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    // 确认是ADC3触发的中断
    if(hadc->Instance == ADC3)
    {
        // 读取注入组转换结果 (Rank 1, 2, 3)
        uint16_t adcValue1 = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1);
        uint16_t adcValue2 = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_2);
        uint16_t adcValue3 = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_3);

        // 如果使用了规则组+DMA，数据已在 `adc_buf` 中，无需手动读取

        // ========== 在此处执行你的FOC计算 ==========
        // 1. 将ADC值转换为实际电流值
        // 2. 执行Clarke/Park变换
        // 3. 执行PID计算
        // 4. 执行反Park变换，计算SVPWM占空比
        // 5. 更新TIM1的CCR1, CCR2, CCR3寄存器
        // =======================================
    }
}



