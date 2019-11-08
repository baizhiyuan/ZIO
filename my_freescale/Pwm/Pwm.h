#ifndef __PWM_H
#define __PWM_H
#include "common.h"
#include "MK60_ftm.h"

void Pwm_Init(void);
void Left_front_pwm_set(int16 Left_frontvalue);
void Right_front_pwm_set(int16 Right_frontvalue);
void Left_rear_pwm_set(int16 Left_rearvalue);
void Right_rear_pwm_set(int16 Right_rearvalue);

#endif

