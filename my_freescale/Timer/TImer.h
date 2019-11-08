#ifndef __TIMER_H
#define __TIMER_H
#include "MK60_pit.h"
#include "Encoder.h"
#include "Pid.h"
#include "MK60_wdog.h"

void Timer_Init(void);
void Timer_IQR_handle(void);

#endif

