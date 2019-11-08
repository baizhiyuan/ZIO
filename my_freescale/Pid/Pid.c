#include "Pid.h"
extern _pid_t pid_left_front;  
extern _pid_t pid_right_front;	
extern _pid_t pid_left_rear;	
extern _pid_t pid_right_rear;		

extern int16 Right_front_speed;
extern int16 Left_front_speed;
extern int16 Right_rear_speed;
extern int16 Left_rear_speed;
extern int Move_Z ; 
extern float KP ; 
extern float KD ;

float P = 90; 
float I = 22;
float D = 30;



void Pid_Init(void)
{
	
	pid_left_front.kp=P;
	pid_left_front.ki=I;
	pid_left_front.kd=D;
	
	pid_right_front.kp=P;
	pid_right_front.ki=I;
	pid_right_front.kd=D;
	
	pid_left_rear.kp=P;
	pid_left_rear.ki=I;
	pid_left_rear.kd=D;
	
	pid_right_rear.kp=P;
	pid_right_rear.ki=I;
	pid_right_rear.kd=D;
	
	pid_left_front.err=0;
	pid_left_front.last_err=0;
	pid_left_front.last_last_err=0;
	pid_left_front.result=0;
	
	pid_right_front.err=0;
	pid_right_front.last_err=0;
	pid_right_front.last_last_err=0;
	pid_right_front.result=0;
	
	
	pid_left_rear.err=0;
	pid_left_rear.last_err=0;
	pid_left_rear.last_last_err=0;
	pid_left_rear.result=0;
	
	pid_right_rear.err=0;
	pid_right_rear.last_err=0;
	pid_right_rear.last_last_err=0;
	pid_right_rear.result=0;
	
}

void PID_Control(_pid_t *pid, int16 actual, int16 set)
{
	float p = 0, i = 0, d = 0;
	pid->err = set - actual;
	p = pid->err - pid->last_err;
	i = pid->err;
	d = pid->err - 2 * pid->last_err + pid->last_last_err;
	pid->result += pid->kp * p +pid->ki * i + pid->kd * d;
	if(pid->result>9500)
		pid->result = 9500;
	else if(pid->result<-9500)
		pid->result = -9500;
	pid->last_last_err = pid->last_err;
	pid->last_err = pid->err;
}



int Dir_Pid(int midpoint,int error_act)
{
	static int midpoint_err =0;  
	static int lastpoint_err =0; 
	midpoint_err = midpoint - error_act;
	Move_Z = KP*midpoint_err + KD*(midpoint_err - lastpoint_err);
	lastpoint_err = midpoint_err ;
	if(Move_Z >= 2000)
		Move_Z = 2000;
	else if(Move_Z <= -2000)
		Move_Z = -2000;
	return Move_Z;


}
