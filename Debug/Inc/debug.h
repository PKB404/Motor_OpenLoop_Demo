#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "foc_type.h"



#define VOFA_CH_COUNT                   7
#define VOFA_RECV_CH_COUNT              4



typedef struct{
    float fdata[VOFA_CH_COUNT];
    const unsigned char tail[4];          /*尾帧 0x00, 0x00, 0x80, 0x7f*/ 
}VOFA_Send_Handle_t;

typedef struct{
    float fdata[VOFA_RECV_CH_COUNT];
}VOFA_Recv_Handle_t;


extern PhaseCurrents_t Iabc;
extern Clarke_ab_t Ialphabeta;
extern Park_dq_t Iqd;

void Debug_Send(void);





#endif
