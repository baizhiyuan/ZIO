#ifndef __OLED_H
#define __OLED_H
#include "stdint.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
//引脚定义
#define OLED_SCL_PIN 15       //11
#define OLED_SCL  PAout(OLED_SCL_PIN)
#define OLED_SDA_PIN 16       //10
#define OLED_SDA  PAout(OLED_SDA_PIN)
#define OLED_RST_PIN 26       //9
#define OLED_RST  PEout(OLED_RST_PIN)
#define OLED_DC_PIN 28         //8
#define OLED_DC   PEout(OLED_DC_PIN)
#define OLED_CS_PIN 27         //8
#define OLED_CS   PEout(OLED_CS_PIN)


//显示范围
#define X_WIDTH 128
#define Y_WIDTH 64

//#define byte  unsigned char
//#define word  unsigned int
//#define dword unsigned long 

#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xCF 



	
enum
{
	FC_ST_16 = 0,		/* 宋体15x16点阵 （宽x高） */
	FC_ST_12 = 1		/* 宋体12x12点阵 （宽x高） */
};

typedef struct
{
	uint16_t usFontCode;	/* 字体代码 0 表示16点阵 */
	uint16_t usTextColor;	/* 字体颜色 0 或 1 */
	uint16_t usBackColor;	/* 文字背景颜色 0 或 1 */
	uint16_t usSpace;		/* 文字间距，单位 = 像素 */
}FONT_T;

//FONT_T tFont12,tFont16;

void OLED_Init(void);  
void Oled_Init_n(void);
void OLED_WrDat(unsigned char dat);
void OLED_WrCmd(unsigned char cmd);
void OLED_Set_Pos(unsigned char x,unsigned char y);
void OLED_Fill(unsigned char bmp_dat);
void OLED_CLS(void);
void OLED_DLY_ms(unsigned int ms);

void OLED_StartDraw(void);
void OLED_EndDraw(void);
void OLED_ClrScr(uint8_t _ucMode);

void OLED_DispStr_fan(uint16_t _usX, uint16_t _usY,  const char  *_ptr,FONT_T *_tFont);//取反函数
void OLED_Disp_Data_fan(int x,int y,int dat,int num) ;
void OLED_DispStr(uint16_t _usX, uint16_t _usY,  const char  *_ptr,FONT_T *_tFont);
void OLED_PutPixel(uint16_t _usX, uint16_t _usY, uint8_t _ucColor);
 void OLED_BufToPanel(void);//将此处函数声明挪到oled.h以便主函数直接调用
uint8_t OLED_GetPixel(uint16_t _usX, uint16_t _usY);
void OLED_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint8_t _ucColor);
void OLED_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint8_t _ucColor);
void OLED_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor);
void OLED_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint8_t _ucColor);
void OLED_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t *_ptr);
void OLED_DrawWave(uint16_t _ptr, uint16_t Y);
void OLED_Disp_Data(int x,int y,int dat,int num);
void OLED_P6x8Num(unsigned char x,unsigned char y,uint16_t num);
void OLED_f6x8Str(uint8_t x, uint8_t y, uint8_t ch[]) ;
#endif



