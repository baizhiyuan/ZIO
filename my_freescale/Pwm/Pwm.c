#include "Pwm.h"



void Pwm_Init()
{
	ftm_pwm_init(ftm3,ftm_ch0,10000,0);			
	ftm_pwm_init(ftm3,ftm_ch1,10000,0);	
	
	ftm_pwm_init(ftm3,ftm_ch2,10000,0 );		
	ftm_pwm_init(ftm3,ftm_ch3,10000,0);		
	
	ftm_pwm_init(ftm3,ftm_ch4,10000,0);			
	ftm_pwm_init(ftm3,ftm_ch5,10000,0);		
	
	ftm_pwm_init(ftm3,ftm_ch6,10000,0 );			
	ftm_pwm_init(ftm3,ftm_ch7,10000,0);		
	
	
}




void Left_front_pwm_set(int16 Left_frontvalue)
{
	if(Left_frontvalue>=0)
	{
		ftm_pwm_duty(ftm3,ftm_ch4,0);
		ftm_pwm_duty(ftm3,ftm_ch5,Left_frontvalue);
	}
	else
	{
		ftm_pwm_duty(ftm3,ftm_ch4,-Left_frontvalue);
		ftm_pwm_duty(ftm3,ftm_ch5,0);
	}
}


void Right_front_pwm_set(int16 Right_frontvalue)
{
	if(Right_frontvalue>=0)
	{
		ftm_pwm_duty(ftm3,ftm_ch6,0);
		ftm_pwm_duty(ftm3,ftm_ch7,Right_frontvalue);
	}
	else
	{
		ftm_pwm_duty(ftm3,ftm_ch6,-Right_frontvalue);
		ftm_pwm_duty(ftm3,ftm_ch7,0);
	}
}


void Left_rear_pwm_set(int16 Left_rearvalue)
{
	if(Left_rearvalue>=0)
	{
		ftm_pwm_duty(ftm3,ftm_ch0,Left_rearvalue);
		ftm_pwm_duty(ftm3,ftm_ch1,0);
	}
	else
	{
		ftm_pwm_duty(ftm3,ftm_ch0,0);
		ftm_pwm_duty(ftm3,ftm_ch1,-Left_rearvalue);
	}
}


void Right_rear_pwm_set(int16 Right_rearvalue)
{
	if(Right_rearvalue>=0)
	{
		ftm_pwm_duty(ftm3,ftm_ch2,0);
		ftm_pwm_duty(ftm3,ftm_ch3,Right_rearvalue);
	}
	else
	{
		ftm_pwm_duty(ftm3,ftm_ch2,-Right_rearvalue);
		ftm_pwm_duty(ftm3,ftm_ch3,0);
	}
}

