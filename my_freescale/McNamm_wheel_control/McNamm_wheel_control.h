#ifndef _McNamm_h
#define _McNamm_h

#include "Variable.h"
#include "common.h"
#include "Pid.h"
#include "Picture_deal.h"
void McNamm_wheel_control(float Vx,float Vy,float Vz);
void Speed_Conversion(u8 setspeedl,int setspeeds,int16 x,int16 y);
void Speed_Conversion2(void);
#endif

