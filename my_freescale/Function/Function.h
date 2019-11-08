#ifndef __FUNCTION_H
#define __FUNCTION_H
#include "common.h"

#define Error(a,b)   (a > b) ? (a - b) : (b - a)
#define Isimage_Black(i,j)  (image[i][j]>threshold)? 1:0

int16 limit_s16(int16 Data,int16 Min,int16 Max) ;
float limit_float(float Data,float Min,float Max);
uint8 limit_u8(uint8 Data,uint8 Min,uint8 Max);
int16 PaiXu(int16 Data0,int16 Data1,int16 Data2);
uint16_t Distance_Filter(uint16_t distance);
#endif

