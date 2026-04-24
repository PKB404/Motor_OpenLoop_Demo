#include "foc_math.h"



Clarke_ab_t FOC_Clarke(PhaseCurrents_t *pParam)
{
    Clarke_ab_t Output;
    
    Output.alpha = pParam->a;
    Output.beta = (pParam->a + 2.0f * pParam->b) * _1_DIV_SQRT_3;
    
    return Output;
}



Park_dq_t FOC_Park(Clarke_ab_t *pParam, float angle_el)
{
    Park_dq_t Output;
    
    float sin = arm_sin_f32(angle_el);
    float cos = arm_cos_f32(angle_el);
    
    Output.d = pParam->alpha * cos + pParam->beta * sin;
    Output.q = pParam->beta * cos - pParam->alpha * sin;
    
    return Output;
}



PhaseCurrents_t FOC_InvClarke(Clarke_ab_t *pParam)
{
    PhaseCurrents_t Output;
    
    Output.a = pParam->alpha;
    Output.b = -0.5f * pParam->alpha + SQRT3_DIV_2 * pParam->beta;
    Output.c = -0.5f * pParam->alpha - SQRT3_DIV_2 * pParam->beta;
    
    return Output;
}



Clarke_ab_t FOC_InvPark(Park_dq_t *pParam, float angle_el)
{
    Clarke_ab_t Output;
    
    float sin = arm_sin_f32(angle_el);
    float cos = arm_cos_f32(angle_el);
    
    Output.alpha = pParam->d * cos - pParam->q * sin;
    Output.beta = pParam->d * sin + pParam->q *cos;

    return Output;
}




