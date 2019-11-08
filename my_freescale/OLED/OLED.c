#include "MK60_gpio.h"
#include "OLED.h"
#include "font.h"
#include "fonts.h"
#include "common.h"
#include "math.h"
FONT_T tFont12,tFont16,tFont12_fan;


  uint8_t s_ucGRAM[8][128];//此处略去了STATIC以便让主函数调用s_ucGRAM数组
static uint8_t s_ucUpdateEn = 1;

void OLED_WrDat(unsigned char dat)
{
    unsigned char i=8;
    
    OLED_DC=1;
    OLED_SCL=0;
    
    while(i--)
    {
        if(dat&0x80)
					OLED_SDA=1;
		    else 
					OLED_SDA=0;
		OLED_SCL=1;        
		OLED_SCL=0;   
		dat<<=1; 
    }
}


//****************************************************************************
//  函数名：Oled_Init_n(void)
//  功能：对OLED屏幕进行初始化
//  说明：无
//*****************************************************************************/ 
//void Oled_Init_n(void)
//{
//	OLED_Init();                                
//	OLED_StartDraw();                            
//	OLED_ClrScr(0);
//	OLED_EndDraw();
//}

void OLED_WrCmd(unsigned char cmd)
{
    unsigned char i=8;
    
    OLED_DC=0;
    OLED_SCL=0;
    
    while(i--)
    {
        if(cmd&0x80)
					OLED_SDA=1;
		    else 
					OLED_SDA=0;
		OLED_SCL=1;        
		OLED_SCL=0;   
		cmd<<=1; 
    }
}

void OLED_Set_Pos(unsigned char x,unsigned char y )
{ 
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01); 
}

void OLED_Fill(unsigned char bmp_dat)
{
    unsigned char x,y;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			OLED_WrDat(bmp_dat);
	}
}

//________________________________________________________________

void OLED_CLS(void)
{
    unsigned char x,y;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10); 
		for(x=0;x<X_WIDTH;x++)
			OLED_WrDat(0);
	}
}

//________________________________________________________________

void OLED_DLY_ms(unsigned int ms)
{
    unsigned int a;
    while(ms)
	{
		a=1335;
		while(a--);
		ms--;
	}
}

void OLED_Init(void)
{
    //初始化相应IO口
	gpio_init (A15, GPO,1);
	gpio_init (A16, GPO,1);  
	gpio_init (E26, GPO,1);
	gpio_init (E28 , GPO,1);
	gpio_init (E27 , GPO,0);
	
	port_init_NoAlt (E26, PULLUP );
	port_init_NoAlt (E28, PULLUP );
	port_init_NoAlt (A15, PULLUP );
	port_init_NoAlt (A16 , PULLUP );
  port_init_NoAlt (E27 , PULLUP );
	
	
	OLED_SCL=1;
	OLED_CS=0;	//预制SLK和SS为高电平  	
	OLED_RST=0;
	OLED_DLY_ms(50);
  OLED_RST=1;
	
	//从上电到下面开始初始化要有足够的时间，即等待RC复位完毕   

	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)

	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(0xcf); // Set SEG Output Current Brightness

	OLED_WrCmd(0xa0);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常

	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00);  //初始清屏

//    OLED_WrCmd(0xae);//--turn off oled panel
//    OLED_WrCmd(0x00);//--set low column address
//    OLED_WrCmd(0x10);//--set high column address
//    OLED_WrCmd(0x40);//--set start line address: Set Mapping RAM Display Start Line (0x00~0x3F)
//    OLED_WrCmd(0x81);//--set contrast control register
//    OLED_WrCmd(0xcf); // set SEG Output Current Brightness
//    OLED_WrCmd(0xa1);//--set SEG/Column Mapping: 0xa0???? 0xa1??
//    OLED_WrCmd(0xc8);//--set COM/Row Scan Direction: 0xc0???? 0xc8??
//    OLED_WrCmd(0xa6);//--set normal display
//    OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
//    OLED_WrCmd(0x3f);//--1/64 duty
//    OLED_WrCmd(0xd3);//--set display offset: Shift Mapping RAM Counter (0x00~0x3F)
//    OLED_WrCmd(0x00);//--not offset
//    OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
//    OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
//    OLED_WrCmd(0xd9);//--set pre-charge period
//    OLED_WrCmd(0xf1);//--set Pre-Charge as 15 Clocks & Discharge as 1 Clock
//    OLED_WrCmd(0xda);//--set com pins hardware configuration
//    OLED_WrCmd(0x12);
//    OLED_WrCmd(0xdb);//--set vcomh
//    OLED_WrCmd(0x40);//--set VCOM Deselect Level
//    OLED_WrCmd(0x20);//--set Page Addressing Mode (0x00/0x01/0x02)
//    OLED_WrCmd(0x02);//
//    OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
//    OLED_WrCmd(0x14);//--set(0x10) disable
//    OLED_WrCmd(0xa4);//--disable Entire Display On (0xa4/0xa5)
//    OLED_WrCmd(0xa6);//--disable Inverse Display On (0xa6/a7)
//    OLED_WrCmd(0xaf);//--turn on oled panel
//    OLED_Fill(0xff);
	OLED_Set_Pos(0,0);
	
	OLED_StartDraw();                            
	OLED_ClrScr(0);
	OLED_EndDraw();

	//字体参数设置结构体

	tFont16.usFontCode = FC_ST_16;	/* 字体代码 16点阵 */
	tFont16.usTextColor = 1;		/* 字体颜色 0 或 1 */
	tFont16.usBackColor = 0;		/* 文字背景颜色 0 或 1 */
	tFont16.usSpace = 0;			/* 文字间距，单位 = 像素 */


    tFont12.usFontCode = FC_ST_12;	/* 字体代码 12点阵 */
	  tFont12.usTextColor = 1;		/* 字体颜色 0 或 1 */
	  tFont12.usBackColor = 0;		/* 文字背景颜色 0 或 1 */
	  tFont12.usSpace =0;			/* 文字间距，单位 = 像素 */
}


void OLED_StartDraw(void)
{
	s_ucUpdateEn = 0;
}


/*
*********************************************************************************************************
*	函 数 名: OLED_EndDraw
*	功能说明: 结束绘图。缓冲区的数据刷新到面板显存。 OLED_StartDraw() 和 OLED_EndDraw() 必须成对使用
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_EndDraw(void)
{
	s_ucUpdateEn = 1;
	OLED_BufToPanel();
}


/*
*********************************************************************************************************
*	函 数 名: OLED_ClrScr
*	功能说明: 清屏
*	形    参：_ucMode : 0 表示全黑； 0xFF表示全亮
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_ClrScr(uint8_t _ucMode)
{
	uint8_t i,j;

	for (i = 0 ; i < 8; i++)
	{
		for (j = 0 ; j < 128; j++)
		{
			s_ucGRAM[i][j] = _ucMode;
		}
	}

	if (s_ucUpdateEn == 1)
	{
		OLED_BufToPanel();
	}
}

/*********************************************************************************************************
*	函 数 名: OLED_BufToPanel
*	功能说明: 将缓冲区中的点阵数据写入面板
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
 void OLED_BufToPanel(void)//此处略去STATIC以便主函数直接调用
{
	uint8_t i,j;

	for (i = 0 ; i< 8; i++)
	{
		OLED_WrCmd (0xB0 + i);	/* 设置页地址（0~7） */
		OLED_WrCmd (0x00);		/* 设置列地址的低地址 */
		OLED_WrCmd (0x10);		/* 设置列地址的高地址 */

		for (j = 0 ; j < 128; j++)
		{
			 OLED_WrDat(s_ucGRAM[i][j]);
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: OLED_DispStr
*	功能说明: 在屏幕指定坐标（左上角为0，0）显示一个字符串
*	形    参：
*		_usX : X坐标，对于12864屏，范围为【0 - 127】
*		_usY : Y坐标，对于12864屏，范围为【0 - 63】
*		_ptr  : 字符串指针
*		_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_DispStr(uint16_t _usX, uint16_t _usY,  const char  *_ptr,FONT_T *_tFont)
{
	uint32_t i;
	uint8_t code1;
	uint8_t code2;
	uint32_t address;
	uint8_t buf[24 * 24 / 8];	/* 最大支持24点阵汉字 */
	uint8_t m, width;
	uint8_t font_width,font_height, font_bytes;
	uint16_t x, y;
	const uint8_t *pAscDot;
	const uint8_t *pHzDot;
	
	/* 如果字体结构为空指针，则缺省按16点阵 */	
	if (_tFont->usFontCode == FC_ST_12)
	{
		font_height = 12;
		font_width = 12;
		font_bytes = 24;
		pAscDot = g_Ascii12;
		//pHzDot = g_Hz12;
	}
	else
	{
		/* 缺省是16点阵 */
		font_height = 16;
		font_width = 16;
		font_bytes = 32;
		pAscDot = g_Ascii16;
		//pHzDot = g_Hz16;
	}

	/* 开始循环处理字符 */
	while (*_ptr !=0)
	{
		code1 = *_ptr;	/* 读取字符串数据， 该数据可能是ascii代码，也可能汉字代码的高字节 */
		if (code1 < 0x80)
		{
			/* 将ascii字符点阵复制到buf */
			memcpy(buf, &pAscDot[code1 * (font_bytes / 2)], (font_bytes / 2));
			width = font_width / 2;
		}
		else
		{
			code2 = *++_ptr;
			if (code2 == 0)
			{
				break;
			}

			/* 计算16点阵汉字点阵地址
				ADDRESS = [(code1-0xa1) * 94 + (code2-0xa1)] * 32
				;
			*/
			#ifdef USE_SMALL_FONT
				m = 0;
				while(1)
				{
					address = m * (font_bytes + 2);
					m++;
					if ((code1 == pHzDot[address + 0]) && (code2 == pHzDot[address + 1]))
					{
						address += 2;
						memcpy(buf, &pHzDot[address], font_bytes);
						break;
					}
					else if ((pHzDot[address + 0] == 0xFF) && (pHzDot[address + 1] == 0xFF))
					{
						/* 字库搜索完毕，未找到，则填充全FF */
						memset(buf, 0xFF, font_bytes);
						break;
					}
				}
			#else	/* 用全字库 */
				/* 此处需要根据字库文件存放位置进行修改 */
				address = ((code1-0xa1) * 94 + (code2-0xa1)) * font_bytes + HZK16_ADDR;
				memcpy(buf, (const uint8_t *)address, font_bytes);
			#endif

				width = font_width;
		}

		y = _usY;
		/* 开始刷LCD */
		for (m = 0; m < font_height; m++)	/* 字符高度 */
		{
			x = _usX;															 
			for (i = 0; i < width; i++)	/* 字符宽度 */
			{
				if ((buf[m * ((2 * width) / font_width) + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
				{
					OLED_PutPixel(x, y, _tFont->usTextColor);	/* 设置像素颜色为文字色 */
				}
				else
				{
					OLED_PutPixel(x, y, _tFont->usBackColor);	/* 设置像素颜色为文字背景色 */
				}

				x++;
			}
			y++;
		}

		if (_tFont->usSpace > 0)
		{
			/* 如果文字底色按_tFont->usBackColor，并且字间距大于点阵的宽度，那么需要在文字之间填充(暂时未实现) */
		}
		_usX += width + _tFont->usSpace;	/* 列地址递增 */
		_ptr++;			/* 指向下一个字符 */
	}
}






void OLED_DispStr_fan(uint16_t _usX, uint16_t _usY,  const char  *_ptr,FONT_T *_tFont)
{
	uint32_t i;
	uint8_t code1;
	uint8_t code2;
	uint32_t address;
	uint8_t buf[24 * 24 / 8];	/* 最大支持24点阵汉字 */
	uint8_t m, width;
	uint8_t font_width,font_height, font_bytes;
	uint16_t x, y;
	const uint8_t *pAscDot;
	const uint8_t *pHzDot;
	
	/* 如果字体结构为空指针，则缺省按16点阵 */	
	if (_tFont->usFontCode == FC_ST_12)
	{
		font_height = 12;
		font_width = 12;
		font_bytes = 24;
		pAscDot = g_Ascii12;
		//pHzDot = g_Hz12;
	}
	else
	{
		/* 缺省是16点阵 */
		font_height = 16;
		font_width = 16;
		font_bytes = 32;
		pAscDot = g_Ascii16;
		//pHzDot = g_Hz16;
	}

	/* 开始循环处理字符 */
	while (*_ptr !=0)
	{
		code1 = *_ptr;	/* 读取字符串数据， 该数据可能是ascii代码，也可能汉字代码的高字节 */
		if (code1 < 0x80)
		{
			/* 将ascii字符点阵复制到buf */
			memcpy(buf, &pAscDot[code1 * (font_bytes / 2)], (font_bytes / 2));
			width = font_width / 2;
		}
		else
		{
			code2 = *++_ptr;
			if (code2 == 0)
			{
				break;
			}

			/* 计算16点阵汉字点阵地址
				ADDRESS = [(code1-0xa1) * 94 + (code2-0xa1)] * 32
				;
			*/
			#ifdef USE_SMALL_FONT
				m = 0;
				while(1)
				{
					address = m * (font_bytes + 2);
					m++;
					if ((code1 == pHzDot[address + 0]) && (code2 == pHzDot[address + 1]))
					{
						address += 2;
						memcpy(buf, &pHzDot[address], font_bytes);
						break;
					}
					else if ((pHzDot[address + 0] == 0xFF) && (pHzDot[address + 1] == 0xFF))
					{
						/* 字库搜索完毕，未找到，则填充全FF */
						memset(buf, 0xFF, font_bytes);
						break;
					}
				}
			#else	/* 用全字库 */
				/* 此处需要根据字库文件存放位置进行修改 */
				address = ((code1-0xa1) * 94 + (code2-0xa1)) * font_bytes + HZK16_ADDR;
				memcpy(buf, (const uint8_t *)address, font_bytes);
			#endif

				width = font_width;
		}

		y = _usY;
		/* 开始刷LCD */
		for (m = 0; m < font_height; m++)	/* 字符高度 */
		{
			x = _usX;															 
			for (i = 0; i < width; i++)	/* 字符宽度 */
			{
				if ((buf[m * ((2 * width) / font_width) + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
				{
					OLED_PutPixel(x, y, _tFont->usBackColor);	/* 设置像素颜色为文字色 */
				}
				else
				{
					OLED_PutPixel(x, y, _tFont->usTextColor);	/* 设置像素颜色为文字背景色 */
				}

				x++;
			}
			y++;
		}

		if (_tFont->usSpace > 0)
		{
			/* 如果文字底色按_tFont->usBackColor，并且字间距大于点阵的宽度，那么需要在文字之间填充(暂时未实现) */
		}
		_usX += width + _tFont->usSpace;	/* 列地址递增 */
		_ptr++;			/* 指向下一个字符 */
	}
}
/*
*********************************************************************************************************
*	函 数 名: OLED_PutPixel
*	功能说明: 画1个像素
*	形    参：
*			_usX,_usY : 像素坐标
*			_ucColor  ：像素颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_PutPixel(uint16_t _usX, uint16_t _usY, uint8_t _ucColor)
{
	uint8_t ucValue;
	uint8_t ucPageAddr;
	uint8_t ucColAddr;
//#ifdef TRUN_180_EN
	const uint8_t aOrTab[8]  = {0x01, 0x02, 0x04, 0x08,0x10,0x20,0x40,0x80};
	const uint8_t aAndTab[8] = {0xFE, 0xFD, 0xFB, 0xF7,0xEF,0xDF,0xBF,0x7F};

	ucPageAddr = _usY / 8;
	ucColAddr = 127 - _usX;
// #else
// 	const uint8_t aOrTab[8]  = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
// 	const uint8_t aAndTab[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};

// 	ucPageAddr = 7 - _usY / 8;
// 	ucColAddr = _usX;
  //#endif

	ucValue = s_ucGRAM[ucPageAddr][ucColAddr];
	if (_ucColor == 0)
	{
		ucValue &= aAndTab[_usY % 8];
	}
	else
	{
		ucValue |= aOrTab[_usY % 8];
	}
	s_ucGRAM[ucPageAddr][ucColAddr] = ucValue;

	if (s_ucUpdateEn == 1)
	{
		OLED_WrCmd (0xB0 + ucPageAddr);					/* 设置页地址（0~7） */
		OLED_WrCmd (0x00 + (ucColAddr & 0x0F));			/* 设置列地址的低地址 */
		OLED_WrCmd (0x10 + ((ucColAddr >> 4) & 0x0F));	/* 设置列地址的高地址 */
		OLED_WrDat(ucValue);
	}
}


/*
*********************************************************************************************************
*	函 数 名: OLED_GetPixel
*	功能说明: 读取1个像素
*	形    参：
*			_usX,_usY : 像素坐标
*	返 回 值: 颜色值 (0, 1)
*********************************************************************************************************
*/
uint8_t OLED_GetPixel(uint16_t _usX, uint16_t _usY)
{
	uint8_t ucValue;
	uint8_t ucPageAddr;
	uint8_t ucColAddr;
#ifdef TRUN_180_EN
	ucPageAddr = _usY / 8;
	ucColAddr = 127 - _usX;
#else
	ucPageAddr = 7 - _usY / 8;
	ucColAddr = _usX;
#endif

	ucValue = s_ucGRAM[ucPageAddr][ucColAddr];
	if (ucValue & (_usY % 8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: OLED_DrawLine
*	功能说明: 采用 Bresenham 算法，在2点间画一条直线。
*	形    参：
*			_usX1, _usY1 ：起始点坐标
*			_usX2, _usY2 ：终止点Y坐标
*			_ucColor     ：颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint8_t _ucColor)
{
	int32_t dx , dy ;
	int32_t tx , ty ;
	int32_t inc1 , inc2 ;
	int32_t d , iTag ;
	int32_t x , y ;

	/* 采用 Bresenham 算法，在2点间画一条直线 */

	OLED_PutPixel(_usX1 , _usY1 , _ucColor);

	/* 如果两点重合，结束后面的动作。*/
	if ( _usX1 == _usX2 && _usY1 == _usY2 )
	{
		return;
	}

	iTag = 0 ;
	/* dx = abs ( _usX2 - _usX1 ); */
	if (_usX2 >= _usX1)
	{
		dx = _usX2 - _usX1;
	}
	else
	{
		dx = _usX1 - _usX2;
	}

	/* dy = abs ( _usY2 - _usY1 ); */
	if (_usY2 >= _usY1)
	{
		dy = _usY2 - _usY1;
	}
	else
	{
		dy = _usY1 - _usY2;
	}

	if ( dx < dy )   /*如果dy为计长方向，则交换纵横坐标。*/
	{
		uint16_t temp;

		iTag = 1 ;
		temp = _usX1; _usX1 = _usY1; _usY1 = temp;
		temp = _usX2; _usX2 = _usY2; _usY2 = temp;
		temp = dx; dx = dy; dy = temp;
	}
	tx = _usX2 > _usX1 ? 1 : -1 ;    /* 确定是增1还是减1 */
	ty = _usY2 > _usY1 ? 1 : -1 ;
	x = _usX1 ;
	y = _usY1 ;
	inc1 = 2 * dy ;
	inc2 = 2 * ( dy - dx );
	d = inc1 - dx ;
	while ( x != _usX2 )     /* 循环画点 */
	{
		if ( d < 0 )
		{
			d += inc1 ;
		}
		else
		{
			y += ty ;
			d += inc2 ;
		}
		if ( iTag )
		{
			OLED_PutPixel ( y , x , _ucColor) ;
		}
		else
		{
			OLED_PutPixel ( x , y , _ucColor) ;
		}
		x += tx ;
	}
}

/*
*********************************************************************************************************
*	函 数 名: OLED_DrawPoints
*	功能说明: 采用 Bresenham 算法，绘制一组点，并将这些点连接起来。可用于波形显示。
*	形    参：
*			x, y     ：坐标数组
*			_ucColor ：颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint8_t _ucColor)
{
	uint16_t i;

	for (i = 0 ; i < _usSize - 1; i++)
	{
		OLED_DrawLine(x[i], y[i], x[i + 1], y[i + 1], _ucColor);
	}
}

/*
*********************************************************************************************************
*	函 数 名: OLED_DrawRect
*	功能说明: 绘制矩形。
*	形    参：
*			_usX,_usY：矩形左上角的坐标
*			_usHeight ：矩形的高度
*			_usWidth  ：矩形的宽度
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor)
{
	/*
	 ---------------->---
	|(_usX，_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/

	OLED_DrawLine(_usX, _usY, _usX + _usWidth - 1, _usY, _ucColor);	/* 顶 */
	OLED_DrawLine(_usX, _usY + _usHeight - 1, _usX + _usWidth - 1, _usY + _usHeight - 1, _ucColor);	/* 底 */

	OLED_DrawLine(_usX, _usY, _usX, _usY + _usHeight - 1, _ucColor);	/* 左 */
	OLED_DrawLine(_usX + _usWidth - 1, _usY, _usX + _usWidth - 1, _usY + _usHeight, _ucColor);	/* 右 */
}

/*
*********************************************************************************************************
*	函 数 名: OLED_DrawCircle
*	功能说明: 绘制一个圆，笔宽为1个像素
*	形    参：
*			_usX,_usY  ：圆心的坐标
*			_usRadius  ：圆的半径
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint8_t _ucColor)
{
	int32_t  D;			/* Decision Variable */
	uint32_t  CurX;		/* 当前 X 值 */
	uint32_t  CurY;		/* 当前 Y 值 */

	D = 3 - (_usRadius << 1);
	CurX = 0;
	CurY = _usRadius;

	while (CurX <= CurY)
	{
		OLED_PutPixel(_usX + CurX, _usY + CurY, _ucColor);
		OLED_PutPixel(_usX + CurX, _usY - CurY, _ucColor);
		OLED_PutPixel(_usX - CurX, _usY + CurY, _ucColor);
		OLED_PutPixel(_usX - CurX, _usY - CurY, _ucColor);
		OLED_PutPixel(_usX + CurY, _usY + CurX, _ucColor);
		OLED_PutPixel(_usX + CurY, _usY - CurX, _ucColor);
		OLED_PutPixel(_usX - CurY, _usY + CurX, _ucColor);
		OLED_PutPixel(_usX - CurY, _usY - CurX, _ucColor);

		if (D < 0)
		{
			D += (CurX << 2) + 6;
		}
		else
		{
			D += ((CurX - CurY) << 2) + 10;
			CurY--;
		}
		CurX++;
	}
}

/*
*********************************************************************************************************
*	函 数 名: OLED_DrawBMP
*	功能说明: 在LCD上显示一个BMP位图，位图点阵扫描次序：从左到右，从上到下
*	形    参：
*			_usX, _usY : 图片的坐标
*			_usHeight  ：图片高度
*			_usWidth   ：图片宽度
*			_ptr       ：单色图片点阵指针，每个像素占用1个字节
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t *_ptr)
{
	uint16_t x, y;

	for (x = 0; x < _usWidth; x++)
	{
		for (y = 0; y < _usHeight; y++)
		{

			OLED_PutPixel(_usX + x, _usY + y, *_ptr);
 			_ptr++;
		}
	}
}
/*
*********************************************************************************************************
*	函 数 名: OLED_DrawWave
*	功能说明: 在LCD上显示CCD采集的图像
*	形    参：
*			_ptr       : CCD采集数据数组
*			Y          ：阈值的Y轴坐标
*	返 回 值: 无
*********************************************************************************************************
*/
//void OLED_DrawWave(uint16_t _ptr, uint16_t Y)
//{
//	uint16_t x=0;
//	OLED_DrawLine(0,Y,127,Y,1);
//	x++;
//	if(x == 127 )
//		x = 0;
//	OLED_PutPixel(x,_ptr, 1);
//// 	for(x = 0; x < 128; x++)
//// 	{
//// 		OLED_PutPixel(x,_ptr, 1);
//// 		//DelayMs(100);
//// // 		if(x == 127)
//// // 				{
//// // 					x = 0;
//// // 				}
////   }		
//	
//}


void OLED_Disp_Data(int x,int y,int dat,int num) 
{	
	int n=1,i;
	uint8_t Disp_Buf[6];//Disp_Buf_1[10];
	for(i=1;i<num;i++){
	  n*=10;
	}
	for(i=0;i<num;i++)
	{
		Disp_Buf[i] = dat / n ;
		dat = dat - Disp_Buf[i] * n;
		Disp_Buf[i]+='0';
		n = n/10;
	}
	OLED_DispStr(x,y,(const char *)Disp_Buf,&tFont12);
}


void OLED_Disp_Data_fan(int x,int y,int dat,int num) 
{	
	int n=1,i;
	uint8_t Disp_Buf[6];//Disp_Buf_1[10];
	for(i=1;i<num;i++){
	  n*=10;
	}
	for(i=0;i<num;i++)
	{
		Disp_Buf[i] = dat / n ;
		dat = dat - Disp_Buf[i] * n;
		Disp_Buf[i]+='0';
		n = n/10;
	}
	OLED_DispStr_fan(x,y,(const char *)Disp_Buf,&tFont12);
}


    
 //==============================================================
//???:LCD_P6x8Num(unsigned char x,unsigned char y,unsigned int *p)
//??:????
//??:????(x,y),y????0~7,num???
//??:?
//==============================================================  
void OLED_P6x8Num(unsigned char x,unsigned char y,uint16_t num)
{
	unsigned char ch[3]={0};
	unsigned char c=0,i=0,j=0;   
	ch[0]=num%10+48;
	ch[1]=num/10%10+48;
	ch[2]=num/100%10+48;
//	ch[3]=num/1000%10+48;
//	ch[4]=num/10000%10+48;
	while (ch[j]!='\0')
	{    
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);    
		for(i=0;i<6;i++)     
		OLED_WrDat(F6x8[c][i]);  
		x+=6;
		j++;
	}
}


void OLED_f6x8Num(unsigned char x,unsigned char y,uint16_t num)
{
	unsigned char ch[5]={0};
	unsigned char c=0,i=0,j=0;   
	ch[0]=num%10+48;
	ch[1]=num/10%10+48;
	ch[2]=num/100%10+48;
	ch[3]=num/1000%10+48;
	ch[4]=num/10000%10+48;
	while (ch[j]!='\0')
	{    
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);    
		for(i=0;i<6;i++)     
		OLED_WrDat(0xff-F6x8[c][i]);  
		x+=6;
		j++;
	}
}
   

//反显
void OLED_f6x8Str(uint8_t x, uint8_t y, uint8_t ch[]) 
{
    uint8_t c = 0, i = 0, j = 0;

    while(ch[j]) 
        {
        c = ch[j] - 32;
        if(x > 126) {
            x = 0;
            ++y;
        }
        OLED_Set_Pos(x, y);
        for(i = 0; i < 6; ++i)
            OLED_WrDat(F6x8_old[c][i]);
        x+=6;
        ++j;
    }
}

