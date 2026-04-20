#include "debug.h"
#include "usart.h"
#include "foc_type.h"


VOFA_Send_Handle_t VOFA_Handle = {
    .tail = {0x00, 0x00, 0x80, 0x7f},
};

PhaseCurrents_t Iabc;
Clarke_ab_t Ialphabeta;
Park_dq_t Iqd;



void Debug_Send(void)
{
    VOFA_Handle.fdata[0] = Iabc.a;
    VOFA_Handle.fdata[1] = Iabc.b;
    VOFA_Handle.fdata[2] = Iabc.c;

    VOFA_Handle.fdata[3] = Ialphabeta.alpha + 3;
    VOFA_Handle.fdata[4] = Ialphabeta.beta + 3;
    
    VOFA_Handle.fdata[5] = Iqd.q + 5; 
    VOFA_Handle.fdata[6] = Iqd.d + 5;  

    HAL_UART_Transmit(&huart1, (uint8_t *)&VOFA_Handle, sizeof(VOFA_Handle), 100);    
}
    

