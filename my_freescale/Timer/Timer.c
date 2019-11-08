#include "Timer.h"
#include "Variable.h"

extern int16 Right_front_speed;
extern int16 Left_front_speed;
extern int16 Left_front_goalspeed;
extern int16 Right_front_goalspeed;

extern int16 Right_rear_speed;
extern int16 Left_rear_speed;
extern int16 Left_rear_goalspeed;
extern int16 Right_rear_goalspeed;

extern _pid_t pid_left_front;   
extern _pid_t pid_right_front;	
extern _pid_t pid_left_rear;		
extern _pid_t pid_right_rear;		
extern int16 twinkle_delay;

void Timer_Init()
{
	pit_init_ms(pit0,5);									
	set_irq_priority(PIT0_IRQn,3);						
	enable_irq(PIT0_IRQn);						
	EnableInterrupts;						

}

void Timer_IQR_handle()
{
	speed_get();	
	
	if(twinkle_delay>0)twinkle_delay--;
	
	PID_Control(&pid_left_front, Left_front_speed, Left_front_goalspeed);
	PID_Control(&pid_right_front, Right_front_speed, Right_front_goalspeed);
	PID_Control(&pid_left_rear, Left_rear_speed, Left_rear_goalspeed);
	PID_Control(&pid_right_rear, Right_rear_speed, Right_rear_goalspeed);

	Left_front_pwm_set(pid_left_front.result);
	Right_front_pwm_set(pid_right_front.result);
	Left_rear_pwm_set(pid_left_rear.result);
	Right_rear_pwm_set(pid_right_rear.result);
	
}



