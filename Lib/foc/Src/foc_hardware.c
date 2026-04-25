#include "foc_hardware.h"
#include "tim.h"
#include "adc.h"


volatile uint8_t adc_conversion_flag;
volatile float adc_buf[ADC_BUFFER_SIZE];

void FOC_PWM_Init(void)
{
//    HAL_TIM_Base_Start_IT(&FOC_DRIVER_TIM);

    FOC_DRIVER_TIM.Instance->CCR1 = 2625;
    FOC_DRIVER_TIM.Instance->CCR2 = 2625;
    FOC_DRIVER_TIM.Instance->CCR3 = 2625;

    HAL_TIM_PWM_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_3);

}



void FOC_PWM_DeInit(void)
{
//    HAL_TIM_Base_Stop_IT(&FOC_DRIVER_TIM);

    HAL_TIM_PWM_Stop(&FOC_DRIVER_TIM, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&FOC_DRIVER_TIM, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&FOC_DRIVER_TIM, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Stop(&FOC_DRIVER_TIM, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&FOC_DRIVER_TIM, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Stop(&FOC_DRIVER_TIM, TIM_CHANNEL_3);
}



void FOC_ADC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //ADC采样相关
    HAL_TIM_PWM_Start(&FOC_DRIVER_TIM, TIM_CHANNEL_4);
    __HAL_TIM_SetCompare(&FOC_DRIVER_TIM, TIM_CHANNEL_4, 5240);
    HAL_ADCEx_InjectedStart(&hadc3);
    __HAL_ADC_ENABLE_IT(&hadc3, ADC_IT_JEOC);

}



void FOC_ADC_DeInit(void)
{
    HAL_TIM_PWM_Stop(&FOC_DRIVER_TIM, TIM_CHANNEL_4);
    HAL_ADCEx_InjectedStop(&hadc3);

}




void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if(htim->Instance == FOC_DRIVER_TIM.Instance)
    {
        
    }

}



void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    // 确认是ADC3触发的中断
    if(hadc->Instance == ADC3)
    {
        adc_conversion_flag = 1;
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_11);
        // 读取注入组转换结果 (Rank 1, 2, 3)
        adc_buf[0] = (hadc->Instance->JDR1 * 3.3f) / 4096.0f;
        adc_buf[1] = (hadc->Instance->JDR2 * 3.3f) / 4096.0f;
        adc_buf[2] = (hadc->Instance->JDR3 * 3.3f) / 4096.0f;


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



