#ifndef __FOC_PWM_H__
#define __FOC_PWM_H__

#include "foc_type.h"



typedef struct
{
    PhaseCurrents_t     Uabc;
    PhaseCurrents_t     Tabc;
    Clarke_ab_t         Ualpha_beta;
    Park_dq_t           Uqd;
    
    float               angle_el;
    float               bus_Voltage;
    uint32_t            wave_period;
    
}FOC_PWM_t;



typedef struct
{
    float target_freq;
    float current_freq;
    
    
    

}FOC_VF_t;



void FOC_Run_SPWM(FOC_PWM_t *pFOC_PWM, Park_dq_t *pUqd, float angle_el);


#endif

