#ifndef __PID_H
#define __PID_H
#include "system.h"

extern int Output;

void PID_Compute(void);
void PID_Output(void);

#endif