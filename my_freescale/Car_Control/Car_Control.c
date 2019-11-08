#include "Car_Control.h"
#include "SEEKFREE_MT9V032.h"
extern int16 beacon_x;
extern int16 beacon_y;
extern uint8 avoid_x;
extern uint8 avoid_y;
extern u8 basic_speed;
extern u8 set_speed_l;
extern int set_speed_s;
extern u8 Rotate_speed;
extern int16 excursion_x;  
extern int16 excursion_y;
extern u8 No_Barrier_flag ;
extern const u8 COL_2;
extern u8 excursion_flag;
extern 	u8 state_flag;
extern u8 delay_flag;
extern u8 Barrier_flag ;
u8 force_left =0;
u8 force_right = 0;

void Car_Control()
{
	int16 Barrier_X=0;
	int16 Barrier_Y =0;
	delay_flag = 0;
	Barrier_flag = 0;
	force_left =0;
	force_right = 0;
	Barrier_X = avoid_x-COL_2/2;  
	Barrier_Y = avoid_y;
	state_flag = Judge_distance(beacon_x,beacon_y);
	switch(state_flag)
	{
		case far :
			{
				if(Barrier_X ==-COL_2/2 && Barrier_Y ==0 && ((beacon_x !=0)||(beacon_y!=0))) 
				{
					LED2_ON;
					if(beacon_x>=0)
					{
						set_speed_l=basic_speed;
						set_speed_s = 0;
						excursion_x = beacon_x-4;   
						excursion_y = beacon_y;			
					}		
					else
					{
						set_speed_l=basic_speed;
						set_speed_s = 0;
						excursion_x = beacon_x+4;   
						excursion_y = beacon_y;		
					}					
				}
				else if((Barrier_X !=-COL_2/2 || Barrier_Y !=0)&& ((beacon_x !=0)||(beacon_y!=0)))  
				{
					LED2_OFF;
					delay_flag = 8;    
					if(Barrier_X>16)
					{
						Barrier_flag =1;
						set_speed_l=basic_speed;
						set_speed_s = 0;
						excursion_x = -10;
						excursion_y = 58;
					}
					else if(Barrier_X<-16)
					{
						Barrier_flag =2;
						set_speed_l=basic_speed;
						set_speed_s = 0;
						excursion_x = 10;
						excursion_y = 58;					
					}
					else if(Barrier_X >= 0 &&Barrier_X <= 16 )   
					{
						Barrier_flag =3;
						set_speed_l=basic_speed*0.85;
						set_speed_s = 0;
						excursion_x = -10;
						excursion_y = 59	;		
					}
					else
					{
						Barrier_flag =4;
						set_speed_l=basic_speed*0.85;
						set_speed_s = 0;
						excursion_x = 10;
						excursion_y = 59;	
					}	

				}
			}
			break;			
		case near :
			{
				if(beacon_x>=5 && beacon_x<=38)   
				{
					set_speed_l=basic_speed;
					set_speed_s = 0;
					excursion_x = beacon_x-13;
					excursion_y = beacon_y ;
				}
				else if(beacon_x<=-5 &&  beacon_x>=-38)  
				{
					set_speed_l=basic_speed;
					set_speed_s = 0;
					excursion_x = beacon_x+16;
					excursion_y = beacon_y ;
				}
				else if(beacon_x<=5 &&  beacon_x>=-5)   
				{
					set_speed_l=basic_speed;
					set_speed_s = 0;
					excursion_x = beacon_x+18;
					excursion_y = beacon_y ;
				}
				else if(beacon_x> 38 )   //         
				{
					set_speed_l=basic_speed;
					set_speed_s = 0;
					excursion_x = beacon_x-5;  
					excursion_y = beacon_y ;
					force_right = 1;
				}
				else                                  
				{
					set_speed_l=basic_speed;
					set_speed_s = 0;
					excursion_x = beacon_x+5;
					excursion_y = beacon_y ;
					force_left = 1;
				}
			}
			break;
					
		case very_near:
			{
				if(beacon_x>=0)
				{
					set_speed_l=basic_speed;    
					set_speed_s = 0;
					excursion_x = beacon_x-24	 ;   
					excursion_y = beacon_y;
				}
				else
				{
					set_speed_l=basic_speed;
					set_speed_s = 0;
					excursion_x = beacon_x+26;
					excursion_y = beacon_y;
				}
			}
			break;
			case very_very_near:
			{
				if(beacon_x>=0)
				{
					set_speed_l=basic_speed;   
					set_speed_s = 0;
					excursion_x = beacon_x-24	 ;  
					excursion_y = beacon_y;
				}
				else
				{
					set_speed_l=basic_speed;
					set_speed_s = 0;
					excursion_x =beacon_x+26;
					excursion_y = beacon_y;
				}
			}
			break;
		case roll:
			{
				set_speed_l=0;
				excursion_x = 0;   
				excursion_y = 0;
				set_speed_s = -Rotate_speed;
			}
			break;
		case FOREC_RAMPAGE:
			{
				set_speed_l=basic_speed;
				set_speed_s = 0;
				excursion_x = 1;
				excursion_y = 1;	
			}
			break;
			
		default:
			break;
	}
	Speed_Conversion(set_speed_l,set_speed_s,excursion_x,60-excursion_y);
	
}



