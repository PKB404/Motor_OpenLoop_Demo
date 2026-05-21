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
    float B = SQRT_3 * pAlphabeta->alpha - pAlphabeta->beta;
    float C = -SQRT_3 * pAlphabeta->alpha - pAlphabeta->beta;
    
    uint8_t weight = 0, sector = 0;
    
    if(A > 0)
    {
        weight += 1;
    }
    
    if(B > 0)
    {
        weight += 2;
    }
    
    if(C > 0)
    {
        weight += 4;
    }    
    
    switch(weight)
    {
        case 3:
            sector = 1;
            break;
        
        case 1:
            sector = 2;
            break;
        
        case 5:
            sector = 3;
            break;
        
        case 4:
            sector = 4;
            break;
        
        case 6:
            sector = 5;
            break;
        
        case 2:
            sector = 6;
            break;
        
        default:
            break;
    }
    return sector;
}



//反正切法



static void VectorActionTime(FOC_PWM_t *pFOC_PWM, uint8_t sector, Clarke_ab_t *pAlphabeta)
{
    float T0 = 0, T1 = 0, T2 = 0, sum;            /*0矢量的时间，以及扇区的两个相邻矢量的时间*/
    float vbus = pFOC_PWM->bus_Voltage;
    uint32_t Tpwm = pFOC_PWM->wave_period;
    
    PhaseCurrents_t Tabc_0 = {0};
    PhaseCurrents_t Tabc_1 = {0};

    float tmp = (float) Tpwm*SQRT_3 / vbus;
    
    float x = tmp * pAlphabeta->beta;
    float y = tmp * (SQRT3_DIV_2 * pAlphabeta->alpha + pAlphabeta->beta / 2.0f);
    float z = tmp * (-SQRT3_DIV_2 * pAlphabeta->alpha + pAlphabeta->beta / 2.0f);
    
    switch(sector)
    {
        case 1:
            T1 = -z;
            T2 = x;
            break;
    
        case 2:
            T1 = z;
            T2 = y;
            break;
        
        case 3:
            T1 = x;
            T2 = -y;
            break;
        
        case 4:
            T1 = -x;
            T2 = z;
            break;
    
        case 5:
            T1 = -y;
            T2 = -z;
            break;
        
        case 6:
            T1 = y;
            T2 = -x;        
    }
    
    //限幅处理
    if(T1 + T2 > Tpwm)
    {
        sum = T1 + T2;
        T1 = T1/sum * Tpwm;
        T2 = T2/sum * Tpwm;
    }
    T0 = Tpwm - T1 - T2;
    
    //相邻矢量和零矢量
    //T0、T1、T2是各个矢量作用总时间，每个不同的时段，输出的矢量是不同的，到时间点就切换
    Tabc_0.a = T0/4.0f;
    Tabc_0.b = Tabc_0.a + T1/2.0f;
    Tabc_0.c = Tabc_0.b + T2/2.0f; 

    //参考三相波形图来看，谁先到比较值就给最小的值
    switch(sector)
    {
        case 1:
            Tabc_1.a = Tabc_0.a;
            Tabc_1.b = Tabc_0.b;
            Tabc_1.c = Tabc_0.c;
            break;
        
        case 2:
            Tabc_1.a = Tabc_0.b;
            Tabc_1.b = Tabc_0.a;
            Tabc_1.c = Tabc_0.c;
            break;  
        

        case 3:
            Tabc_1.a = Tabc_0.a;
            Tabc_1.b = Tabc_0.c;
            Tabc_1.c = Tabc_0.b;
            break;         

        case 4:
            Tabc_1.a = Tabc_0.c;
            Tabc_1.b = Tabc_0.b;
            Tabc_1.c = Tabc_0.a;
            break;

        case 5:
            Tabc_1.a = Tabc_0.b;
            Tabc_1.b = Tabc_0.c;
            Tabc_1.c = Tabc_0.a;
            break;
        
        case 6:
            Tabc_1.a = Tabc_0.a;
            Tabc_1.b = Tabc_0.c;
            Tabc_1.c = Tabc_0.b;
            break;

        default:
            break;
    }
    FOC_PWM_SetCompare(Tabc_1.a, Tabc_1.b, Tabc_1.c);

    pFOC_PWM->Uabc.a = Tabc_1.a / (Tpwm / 2.0f) * vbus;
    pFOC_PWM->Uabc.b = Tabc_1.b / (Tpwm / 2.0f) * vbus;
    pFOC_PWM->Uabc.c = Tabc_1.c / (Tpwm / 2.0f) * vbus;
    
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






















