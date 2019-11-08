#include "Function.h"
int16 limit_s16(int16 Data,int16 Min,int16 Max)      
{
	if(Data<Min)Data=Min;
	else if(Data>Max)Data=Max;
	return Data;
}

float limit_float(float Data,float Min,float Max)    
{
	if(Data<Min)Data=Min;
	else if(Data>Max)Data=Max;
	return Data;
}

uint8 limit_u8(uint8 Data,uint8 Min,uint8 Max)     
{
	if(Data<Min)Data=Min;
	else if(Data>Max)Data=Max;
	return Data;
}

int16 PaiXu(int16 Data0,int16 Data1,int16 Data2)//3 2 1
{
	int16 Temp=0;
	if(Data0>Data1)//2 3 1
	{
		Temp=Data0;
		Data0=Data1;
		Data1=Temp;
	}
	if(Data0>Data2)//1 3 2
	{
		Temp=Data0;
		Data0=Data2;
		Data2=Temp;
	}
	if(Data1>Data2)//1 2 3
	{
		Temp=Data1;
		Data1=Data2;
		Data2=Temp;
	}
	return Data1;
}

