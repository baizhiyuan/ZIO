//#include "headfile.h"
#include "Show.h"
#include "OLED.h"
#include "common.h"
#include "Variable.h"
#include "math.h"

extern uint8 menu;
extern u8 Rotate_speed;
extern u8 basic_speed;
extern u8 coordinate_X ;
extern u8 coordinate_Y ;
extern uint8 min_gray;
extern FONT_T tFont12,tFont16;
extern u8 beacon_threshold;
extern int16 beacon_x;
extern int16 beacon_y;
extern float KP ; 
extern float KD ;
extern float KP_show ;  
extern float KD_show ;

void Show_variable()   
{
	switch(menu)
	{
		case 1:  //spd
						OLED_DispStr_fan(80,0,"spd :",&tFont12);   
						OLED_Disp_Data_fan(104,0,basic_speed,3);		
            OLED_DispStr(80,12,"KP:",&tFont12);
 						OLED_Disp_Data(104,12,KP_show,3);	      
            OLED_DispStr(80,24,"KD:",&tFont12);
 						OLED_Disp_Data(104,24,KD_show,3);		        
			break;
		
		case 2://x
            OLED_DispStr(80,0,"spd :",&tFont12);   
						OLED_Disp_Data(104,0,basic_speed,3);		 
            OLED_DispStr_fan(80,12,"KP:",&tFont12);
 						OLED_Disp_Data_fan(104,12,KP_show,3);	      
            OLED_DispStr(80,24,"KD:",&tFont12);
 						OLED_Disp_Data(104,24,KD_show,3);		        

			break;
		
		case 3: //y
            OLED_DispStr(80,0,"spd :",&tFont12);   
						OLED_Disp_Data(104,0,basic_speed,3);		
            OLED_DispStr(80,12,"KP:",&tFont12);
 						OLED_Disp_Data(104,12,KP_show,3);	      
            OLED_DispStr_fan(80,24,"KD:",&tFont12);
 						OLED_Disp_Data_fan(104,24,KD_show,3);		        

			break;                    
        
		case 4: 
		        OLED_DispStr(80,0,"KP :",&tFont12);   
						OLED_Disp_Data(104,0,KP_show,3);		 
            OLED_DispStr(80,12,"KD :",&tFont12);
 						OLED_Disp_Data(104,12,KD_show,3);	      
            OLED_DispStr_fan(80,24,"spr",&tFont12);
 						OLED_Disp_Data_fan(104,24,Rotate_speed,3);		        

			break;
		case 5: 
		        OLED_DispStr(80,0,"KD :",&tFont12);   
						OLED_Disp_Data(104,0,KD_show,3);		 
            OLED_DispStr(80,12,"spr:",&tFont12);
 						OLED_Disp_Data(104,12,Rotate_speed,3);	      
            OLED_DispStr_fan(80,24,"yz ",&tFont12);
 						OLED_Disp_Data_fan(104,24,min_gray,3);		        

			break;
		
		default:
			break;
			
	}
	if(beacon_x>=0)
	{
		OLED_DispStr(80,36,"+",&tFont12);
		OLED_Disp_Data(90,36,beacon_x,4);
	}
			
	else
	{
		OLED_DispStr(80,36,"-",&tFont12);
		OLED_Disp_Data(90,36,-beacon_x,4);
	}
	OLED_Disp_Data(80,48,beacon_y,4);	

}
