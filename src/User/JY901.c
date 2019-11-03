#include "include.h"
#include "JY901.h"

uint8_t ucDevAddr;

void StartIIC()
{
	ucDevAddr = 0x50;
}
/*
void CopeSerialData(uint8_t ucData)
{
	static uint8_t ucRxBuffer[250];
	static uint8_t ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxBuffer[0]!=0x55) 
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}
	else
	{
		switch(ucRxBuffer[1])
		{
			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;
			case 0x54:	memcpy(&stcMag,&ucRxBuffer[2],8);break;
			case 0x55:	memcpy(&stcDStatus,&ucRxBuffer[2],8);break;
			case 0x59:	memcpy(&stcQuater,&ucRxBuffer[2],8);break;
			case 0x5a:	memcpy(&stcSN,&ucRxBuffer[2],8);break;
		}
		ucRxCnt=0;
	}
}*/

short ReadWord(uint8_t ucAddr)
{
	short sResult;
	IIC_WriteMultByteToSlave(ucDevAddr, ucAddr, 2, (uint8_t *)&sResult);
	return sResult;
}
void WriteWord(uint8_t ucAddr,short sData)
{	
	IIC_WriteMultByteToSlave(ucDevAddr, ucAddr, 2, (uint8_t *)&sData);
}
void ReadData(uint8_t ucAddr,uint8_t ucLength,uint8_t chrData[])
{
	IIC_ReadMultByteFromSlave(ucDevAddr, ucAddr, ucLength, chrData);
}

void GetTime()
{
	IIC_ReadMultByteFromSlave(ucDevAddr, 0x30, 8, (uint8_t*)&stcTime);
}
void GetAcc()
{
	IIC_ReadMultByteFromSlave(ucDevAddr, AX, 6, (uint8_t *)&stcAcc);
}
void GetGyro()
{
	IIC_ReadMultByteFromSlave(ucDevAddr,GX, 6, (uint8_t *)&stcGyro);
}

void GetAngle()
{
	IIC_ReadMultByteFromSlave(ucDevAddr, Roll, 6, (uint8_t *)&stcAngle);
}
void GetMag()
{
	IIC_ReadMultByteFromSlave(ucDevAddr, HX, 6, (uint8_t *)&stcMag);
}
void GetDStatus()
{
	IIC_ReadMultByteFromSlave(ucDevAddr,D0Status, 8, (uint8_t *)&stcDStatus);
}