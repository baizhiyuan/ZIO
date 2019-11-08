#include "Variable.h"
#include "SEEKFREE_MT9V032.h"
int16 Right_front_speed;
int16 Left_front_speed;
int16 Right_rear_speed;
int16 Left_rear_speed;
int16 Left_front_goalspeed;
int16 Right_front_goalspeed;
int16 Left_rear_goalspeed;
int16 Right_rear_goalspeed;

u8 set_speed_l =0;  
int set_speed_s =0;  
int Move_Z = 0; 
float KP = 0; 
float KD = 0;
float KP_show = 0 ;  
float KD_show = 0 ;
_pid_t pid_left_front;    
_pid_t pid_right_front;		
_pid_t pid_left_rear;			
_pid_t pid_right_rear;			


u8 basic_speed =40;
u8 Rotate_speed = 20;
u8 coordinate_X = 0; 
u8 coordinate_Y = 0;  
uint8 menu =1;   

u8 shield_flag =0;


u8 prama[5] = {0};       
u8 R_prama[5] = {0};		 
u8 My_sectorNum = 20;    

volatile int threshold=0;
uint8 min_gray=70;

int16 use_time = 0;      

uint8 Data_State=0;


uint8 avoid_x;
uint8 avoid_y;
const u8 COL_2 = 98;
#define NO_Barrier_advoidance 0
u8 delay_flag =0;
u8 Barrier_flag = 0;

int16 beacon_x=0;
int16 beacon_y=0;
int16 excursion_x = 0; 
int16 excursion_y = 0;
u8 beacon_threshold =2;
int16 twinkle_delay=0;
u8 No_Barrier_flag =0;
u8 excursion_flag =0; 
u8 state_flag = 0;    

