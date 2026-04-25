#include "foc_pwm.h"
#include "foc_math.h"


static void FOC_Run_SPWM(FOC_PWM_t *pFOC_PWM, Park_dq_t *pUqd, float angle_el)
{
    Clarke_ab_t v_ab;
    PhaseCurrents_t v_abc;
    
    v_ab = FOC_InvPark(pUqd, angle_el);
    pFOC_PWM->Ualpha_beta = v_ab;
    
    v_abc = FOC_InvClarke(&v_ab);
    pFOC_PWM->Uabc = v_abc;
}


























