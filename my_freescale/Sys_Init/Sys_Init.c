#include "Sys_Init.h"

extern volatile int threshold;
extern uint8 min_gray;
extern int16 use_time;
extern FONT_T tFont12,tFont16;

extern u8 shield_flag; 

extern int16 Right_front_speed;
extern int16 Left_front_speed;
extern int16 Right_rear_speed;
extern int16 Left_rear_speed;

extern int16 Left_front_goalspeed ;
extern int16 Right_front_goalspeed ;
extern int16 Left_rear_goalspeed ;
extern int16 Right_rear_goalspeed ;

extern uint8 avoid_x;
extern uint8 avoid_y;
extern int16 beacon_x;
extern int16 beacon_y;
extern u8 prama[5];
extern u8 basic_speed;
extern u8 Rotate_speed;
extern u8 coordinate_X ;
extern u8 coordinate_Y ;
extern u8 My_sectorNum;
void Sys_init_all()
{
	camera_init();
	Flash_Operate();
	LED_Init();
	BEEP_Init();
	OLED_Init();		
	Encoder_Init();
	Pid_Init();
	Pwm_Init();   
	Connunication_init();   
	Timer_Init();
	key_init();

}



