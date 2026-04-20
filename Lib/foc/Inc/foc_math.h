#ifndef __FOC_MATH_H__
#define __FOC_MATH_H__

#include "arm_math.h"
#include "foc_type.h"



//float类型遵循IEEE754单精度标准：1位符号位+8位指数位+23位尾数位
#define SQRT_3                      1.73205080757f
#define _1_DIV_SQRT_3               0.57735026919f
#define SQRT3_DIV_2                 0.86602540378f
#define PI_DIV_3                    1.04719755120f      /*60° 弧度制*/



Clarke_ab_t FOC_Clarke(PhaseCurrents_t *pParam);

Park_dq_t FOC_Park(Clarke_ab_t *pParam, float angle_el);

PhaseCurrents_t FOC_InvClarke(Clarke_ab_t *pParam);

Clarke_ab_t FOC_InvPark(Park_dq_t *pParam, float angle_el);



#endif




