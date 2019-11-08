#include "Flash_Adjust_Param.h"
#include "MK60_flash.h"

extern u8 R_prama[5];		 
extern u8 basic_speed;
extern u8 Rotate_speed;
extern u8 coordinate_X ;
extern u8 coordinate_Y ;
extern u8 My_sectorNum;
extern u8 beacon_threshold;
extern u8 min_gray;
extern float KP ;  
extern float KD ;
extern float KP_show ;  
extern float KD_show ;
void Flash_Operate()
{
		FLASH_Init();
		R_prama[0] = flash_read(My_sectorNum,0,uint8);
		R_prama[1] = flash_read(My_sectorNum,1,uint8);
		R_prama[2] = flash_read(My_sectorNum,2,uint8);
		R_prama[3] = flash_read(My_sectorNum,3,uint8);
		R_prama[4] = flash_read(My_sectorNum,4,uint8);	
		basic_speed  = R_prama[0];
		KP_show = R_prama[1];
		KD_show = R_prama[2];
		Rotate_speed = R_prama[3];
		min_gray     = R_prama[4];
		KP = KP_show/10;
		KD = KD_show/10;
		
}

