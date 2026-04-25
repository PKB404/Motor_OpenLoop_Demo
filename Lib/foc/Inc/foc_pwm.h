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
    uint16_t            period;
    
}FOC_PWM_t;






#endif

