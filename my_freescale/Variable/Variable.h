#ifndef __VARIABLE_H
#define __VARIABLE_H

#include "common.h"
//#include "SEEKFREE_MT9V032.h"
/****µç»ú¿ØÖÆPID*****/
typedef struct
{
	float err;
	float last_err;
	float last_last_err;
	float kp,ki,kd;
	float result;
}_pid_t;


#endif

