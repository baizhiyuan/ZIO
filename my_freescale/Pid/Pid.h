#ifndef __PID_H
#define __PID_H

#include "common.h"
#include "Variable.h"
#include "Pwm.h"
#include "Function.h"


void Pid_Init(void);
int Dir_Pid(int midpoint,int error_act);
void PID_Control(_pid_t *pid, int16 actual, int16 set);

#endif

