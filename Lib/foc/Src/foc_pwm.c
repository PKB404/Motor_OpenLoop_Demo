#include "foc_pwm.h"
#include "foc_math.h"
#include "foc_hardware.h"


#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
 


void FOC_Run_SPWM(FOC_PWM_t *pFOC_PWM, Park_dq_t *pUqd, float angle_el)
{
    Clarke_ab_t v_ab;
    PhaseCurrents_t v_abc;
    uint32_t center, period = pFOC_PWM->wave_period;
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



//比较法
uint8_t SectorJudgment(Clarke_ab_t *pAlphabeta)
{
    float A = pAlphabeta->beta;
    float B = SQRT3_DIV_2 * pAlphabeta->alpha - 0.5f *pAlphabeta->beta;
    float C = -(SQRT3_DIV_2 * pAlphabeta->alpha) - 0.5f * pAlphabeta->beta;
    static uint8_t sector_map[8] = {0, 2, 6, 1, 4, 3, 5, 0};
    
    uint8_t weight = 0, sector = 0;
    
    if(A > 0)
    {
        weight |= 1;
    }
    
    if(B > 0)
    {
        weight |= 2;
    }
    
    if(C > 0)
    {
        weight |= 4;
    }    
    
    sector = sector_map[weight];
    return sector;
}



//反正切法



//static void VectorActionTime(FOC_PWM_t *pFOC_PWM, uint8_t sector, Clarke_ab_t *pAlphabeta)
//{
//    float T0 = 0, T1 = 0, T2 = 0, sum;            /*0矢量的时间，以及扇区的两个相邻矢量的时间*/
//    float vbus = pFOC_PWM->bus_Voltage;
//    uint32_t Tpwm = pFOC_PWM->wave_period;
//    
//    PhaseCurrents_t Tabc = {0};

//    float tmp = (float)Tpwm * SQRT_3 / vbus;
//    
//    float x = tmp * pAlphabeta->beta;
//    float y = tmp * (SQRT3_DIV_2 * pAlphabeta->alpha + pAlphabeta->beta * 0.5f);
//    float z = tmp * (-SQRT3_DIV_2 * pAlphabeta->alpha + pAlphabeta->beta * 0.5f);
//    
//    switch(sector)
//    {
//        case 1:
//            T1 = z;
//            T2 = y;
//            break;
//    
//        case 2:
//            T1 = y;
//            T2 = -x;
//            break;
//        
//        case 3:
//            T1 = -z;
//            T2 = x;
//            break;
//        
//        case 4:
//            T1 = -x;
//            T2 = z;
//            break;
//    
//        case 5:
//            T1 = x;
//            T2 = -y;
//            break;
//        
//        case 6:
//            T1 = -y;
//            T2 = -z;  
//            break;
//        
//        default:
//            T1 = 0;
//            T1 = 0;
//            break;        
//    }
//    
//    //限幅处理
//    sum = T1 + T2;
//    if(sum > Tpwm)
//    {
//        T1 = T1 / sum * Tpwm;
//        T2 = T2 / sum * Tpwm;
//        T0 = 0;
//    }
//    else
//    {
//        T0 = Tpwm - T1 - T2;
//    }
//   

//    //参考三相波形图来看，谁先到比较值就给最小的值
//    switch(sector)
//    {
//        case 1:
//            Tabc.a = T1 + T2 + T0 * 0.5f;
//            Tabc.b = T2 + T0 * 0.5f;
//            Tabc.c = T0 * 0.5f;
//            break;
//        
//        case 2:
//            Tabc.a = T1 + T0 * 0.5f;
//            Tabc.b = T1 + T2 + T0 * 0.5f;
//            Tabc.c = T0 * 0.5f;
//            break;  
//        

//        case 3:
//            Tabc.a = T0 * 0.5f;
//            Tabc.b = T1 + T2 + T0 * 0.5f;
//            Tabc.c = T2 + T0 * 0.5f;
//            break;         

//        case 4:
//            Tabc.a = T0 * 0.5f;
//            Tabc.b = T1 + T0 * 0.5f;
//            Tabc.c = T1 + T2 + T0 * 0.5f;
//            break;

//        case 5:
//            Tabc.a = T2 + T0 * 0.5f;
//            Tabc.b = T0 * 0.5f;
//            Tabc.c = T1 + T2 + T0 * 0.5f;
//            break;
//        
//        case 6:
//            Tabc.a = T1 + T2 + T0 * 0.5f;
//            Tabc.b = T0 * 0.5f;
//            Tabc.c = T1 + T0 * 0.5f;
//            break;
//        
//        default:
//            Tabc.a = Tabc.b = Tabc.c = T0 * 0.5f;
//            break;
//    }
//    
//    float cmpA = Tabc.a;
//    float cmpB = Tabc.b;
//    float cmpC = Tabc.c;
//    
//    
//    FOC_PWM_SetCompare(cmpA, cmpB, cmpC);

//    pFOC_PWM->Uabc.a = Tabc.a / (float)Tpwm * vbus;
//    pFOC_PWM->Uabc.b = Tabc.b / (float)Tpwm * vbus;
//    pFOC_PWM->Uabc.c = Tabc.c / (float)Tpwm * vbus;
//    
//}

static void VectorActionTime(FOC_PWM_t *pFOC_PWM, uint8_t sector, Clarke_ab_t *pAlphabeta)
{
    float vbus = pFOC_PWM->bus_Voltage;
    float Tpwm = (float)pFOC_PWM->wave_period;
    
    float Ta = 0, Tb = 0, Tc = 0;
    float T1 = 0, T2 = 0, T3 = 0;
    float T4 = 0, T6 = 0, SUM = 0;
    
    float tmp = Tpwm * SQRT_3 / vbus;
    
    float x = tmp * pAlphabeta->beta;
    float y = tmp * (SQRT3_DIV_2 * pAlphabeta->alpha + pAlphabeta->beta * 0.5f);
    float z = tmp * (-SQRT3_DIV_2 * pAlphabeta->alpha + pAlphabeta->beta * 0.5f);
    
    switch(sector)
    {
        case 1:
            T4 = z;
            T6 = y;
            break;
    
        case 2:
            T4 = y;
            T6 = -x;
            break;
        
        case 3:
            T4 = -z;
            T6 = x;
            break;
        
        case 4:
            T4 = -x;
            T6 = z;
            break;
    
        case 5:
            T4 = x;
            T6 = -y;
            break;
        
        case 6:
            T4 = -y;
            T6 = -z;  
            break;
        
        default:
            T4 = 0.0f;
            T6 = 0.0f;
            break;        
    }
    
    //限幅处理
    SUM = T4 + T6;
    if(SUM > Tpwm)
    {
        T4 = (T4 / SUM) * (Tpwm);
        T6 = (T6 / SUM) * (Tpwm);
    }
    
    Ta = (Tpwm - T4 - T6) * 0.25f;
    Tb = Ta + T4 * 0.5f;
    Tc = Tb + T6 * 0.5f;

    //参考三相波形图来看，谁先到比较值就给最小的值
    switch(sector)
    {
        case 1:
            T1 = Tb;
            T2 = Ta;
            T3 = Tc;
            break;
        
        case 2:
            T1 = Ta;
            T2 = Tc;
            T3 = Tb;
            break;
        
        case 3:
            T1 = Ta;
            T2 = Tb;
            T3 = Tc;
            break;
        
        case 4:
            T1 = Tc;
            T2 = Tb;
            T3 = Ta;
            break;
        
        case 5:
            T1 = Tc;
            T2 = Ta;
            T3 = Tb;
            break;
        
        case 6:
            T1 = Tb;
            T2 = Tc;
            T3 = Ta;
            break;
        
        default:
            
            break;
    }
    
    uint16_t cmpA = T1;
    uint16_t cmpB = T2;
    uint16_t cmpC = T3;
    
    
    FOC_PWM_SetCompare(cmpA, cmpB, cmpC);

    pFOC_PWM->Uabc.a = ((Tpwm / 2 - Ta) / Tpwm) * vbus;
    pFOC_PWM->Uabc.b = ((Tpwm / 2 - Tb) / Tpwm) * vbus;
    pFOC_PWM->Uabc.c = ((Tpwm / 2 - Tc) / Tpwm) * vbus;
    
}

void FOC_Run_SVPWM(FOC_PWM_t *pFOC_PWM)
{
    uint8_t sector;
    Clarke_ab_t input = {0};
    
    input = FOC_InvPark(&pFOC_PWM->Uqd, pFOC_PWM->angle_el);
    pFOC_PWM->Ualpha_beta = input;
    
    sector = SectorJudgment(&input);
    VectorActionTime(pFOC_PWM, sector, &input);
    
    
}



//void FOC_Run_VF(FOC_VF_t *pFOC_VF)
//{



//}






















