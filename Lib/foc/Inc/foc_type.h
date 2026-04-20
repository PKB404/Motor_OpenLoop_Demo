#ifndef __FOC_TYPE_H__
#define __FOC_TYPE_H__

#include <stdint.h>
#include <stdbool.h>



/* 三相电流/电压瞬时值结构体 */
typedef struct {
    float a;
    float b;
    float c;
} PhaseCurrents_t;

/* 静止两相坐标系 α-β */
typedef struct {
    float alpha;
    float beta;
} Clarke_ab_t;

/* 旋转坐标系 d-q */
typedef struct {
    float d;
    float q;
} Park_dq_t;















#endif

