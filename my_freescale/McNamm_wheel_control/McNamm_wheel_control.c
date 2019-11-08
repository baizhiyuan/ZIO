#include "McNamm_wheel_control.h"
#include "math.h"
extern u8 basic_speed ;
extern u8 Rotate_speed;

extern int16 Left_front_goalspeed ;
extern int16 Right_front_goalspeed ;
extern int16 Left_rear_goalspeed;
extern int16 Right_rear_goalspeed;

extern int16 beacon_x;
extern int16 beacon_y;
extern int16 excursion_x;
extern int16 excursion_y;
extern u8 coordinate_X ;
extern u8 coordinate_Y ;

extern 	u8 state_flag; 
extern u8 Barrier_flag ; 
extern u8 force_left ;
extern u8 force_right;
#define a_PARAMETER          (0.09f)               
#define b_PARAMETER          (0.09f) 

void McNamm_wheel_control(float Vx,float Vy,float Vz)
{
	  Left_rear_goalspeed    = -Vx+Vy-Vz*(a_PARAMETER+b_PARAMETER);
    Left_front_goalspeed   = +Vx+Vy-Vz*(a_PARAMETER+b_PARAMETER);
	  Right_front_goalspeed  = -Vx+Vy+Vz*(a_PARAMETER+b_PARAMETER);
		Right_rear_goalspeed   = +Vx+Vy+Vz*(a_PARAMETER+b_PARAMETER);

}

void Speed_Conversion2()
{
	float coefficient = 0;
	float vx=0,vy=0,vz = 0;
	coefficient = (float)(basic_speed*basic_speed)/(coordinate_X*coordinate_X+coordinate_Y*coordinate_Y);
	coefficient = (float)sqrt(coefficient);
	
	vx = coefficient*coordinate_X;
	vy = coefficient*coordinate_Y;
	vz = 1;
	McNamm_wheel_control(vx,vy,vz);
}

void Speed_Conversion(u8 setspeedl,int setspeeds,int16 x,int16 y)
{

	float L =0;
	float vx=0,vy=0;
	int vz = 0;
	if(y == 60)
		y =0;
	L = sqrt(x*x+y*y);
	if(L != 0)
	{
		vx = (float)setspeedl*x/L;
		vy = (float)setspeedl*y/L;
		if(x>=3 || x<=-3)    
		{
			if(Barrier_flag == 0 )                       
			{
				vz = Dir_Pid(0,x);
			}
			else if(Barrier_flag == 1)
				vz = Dir_Pid(0,-45);  
			else if(Barrier_flag == 2)
				vz = Dir_Pid(0,45);
			else if(Barrier_flag == 3)
				vz = Dir_Pid(0,-70);
			else 
				vz = Dir_Pid(0,70);
			if(force_right == 1)
				vz = Dir_Pid(0,60);
			else if(force_left == 1)
				vz = Dir_Pid(0,-60);

		}
	}
	else
	{
		vz = setspeeds;
	}
	McNamm_wheel_control(vx,vy,vz);
}


