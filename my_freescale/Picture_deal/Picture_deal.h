#ifndef _Picture_deal_h
#define _Picture_deal_h

#include "Variable.h"
#include "common.h"
#include "Function.h"
#include "SEEKFREE_MT9V032.h"
#include "stdint.h"
#include "OLED.h"
int16 Coordinate_Filter(int16 Channal,int16 Data_In);
void MyScan_Point(void);
void Scan_Point(void);
u8 Judge_distance(int16 x,int16 y);
u8 Judge_Rotating_Direction(void);

#define far 1 
#define near 2
#define very_near 3
#define very_very_near 6
#define roll 4  //未找到灯
#define FOREC_RAMPAGE 5  //强制横行



#define Left_Rotate 1
#define Right_Rotate 0
#define normal_staright 3
#endif

