#include "foc_pwm.h"
#include "foc_math.h"
#include "foc_hardware.h"


#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
 


void FOC_Run_SPWM(FOC_PWM_t *pFOC_PWM, Park_dq_t *pUqd, float angle_el)
{
    Clarke_ab_t v_ab;
    PhaseCurrents_t v_abc;
    uint32_t center, period = pFOC_PWM->period;
    uint32_t ccr1, ccr2, ccr3;
    
    
    v_ab = FOC_InvPark(pUqd, angle_el);
    pFOC_PWM->Ualpha_beta = v_ab;
    
    v_abc = FOC_InvClarke(&v_ab);
    pFOC_PWM->Uabc = v_abc;
    
    center = period / 2;
    ccr1 = CLAMP(center + v_abc.a * center, 0, period);
    ccr2 = CLAMP(center + v_abc.b * center, 0, period);
    ccr3 = CLAMP(center + v_abc.c * center, 0, period);
    
    FOC_PWM_SetCompare(ccr1, ccr2, ccr3);
    
}



void FOC_Run_VF(FOC_VF_t *pFOC_VF)
{



}






















