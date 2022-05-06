#ifndef _ADC_H
#define __ADC_H
#include "stm32f10x.h" 
#include "system.h"
#define T50  770   	// 50℃时ADC
#define T100 1153  // 100℃时ADC
#define T150 1528  // 150℃时ADC
#define T200 1970  // 200℃时ADC
#define T250 2394  // 250℃时ADC
#define T300 2848  // 300℃时ADC
#define T350 3388  // 350℃时ADC
#define T400 3718  // 400℃时ADC
#define T420 3928  // 420℃时ADC

extern u16 temp_t12
u16 get_t12_temp(void);

#endif