/*-----------------------------------------------------------------------------------------------------
【平    台】龙邱K60核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.2
【功    能】
【注意事项】
------------------------------------------------------------------------------------------------    
测试VL53模块   在屏幕上显示距离   接线SCL->PTE7  SDA->PTE8  VIN-> 3.3V  GND->GND
------------------------------------------------------------------------------------------------    
-------------------------------------------------------------*/

#include "include.h"
/**
  * @brief    测试VL53
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @example  
  *
  * @date     2019/4/17 星期三
  */
void Test_Vl53(void)
{
    LED_Init();
    UART_Init(UART4, 115200);
	systime.init();
	printf("\r\nLQ VL53 Test");
	
#ifdef LQ_OLED
	OLED_Init();
    OLED_CLS();
	OLED_P8x16Str(5,0,"LQ VL53 Test");
#else
	TFTSPI_Init(1);                //TFT1.8初始化  0：横屏显示  1：竖屏显示  
    TFTSPI_CLS(u16BLUE);           //清屏
	TFTSPI_P8X16Str(0,0,"LQ VL53 Test",u16RED,u16BLUE);
#endif
	char txt[16];
	
	IIC_Init();       

    uint8_t VL53_STAR = 0x01;
    uint8_t dis_buff[2];
    uint16_t dis;
    while(1)
    {
        /* 开始一次测距 */
        VL53_Write_nByte(VL53ADDR, VL53L0X_REG_SYSRANGE_START, 1, &VL53_STAR);
        
        /* 获取测量数据 */
		VL53_Read_nByte(VL53ADDR, VL53_REG_DIS, 2, dis_buff);
        
        /* 转换数据 */
        dis = (dis_buff[0]<<8) | (dis_buff[1]);
        if(dis > 8000)
        {
            dis = 0;
        }
		sprintf(txt, "DIS %5d mm",dis);
#ifdef LQ_OLED
		OLED_P8x16Str(0,5,txt);
#else
		TFTSPI_P8X16Str(2,5,txt,u16RED,u16BLUE);
#endif
		delayms(50);
		LED_Reverse(1);
    
    }
    
}


/**
  * @brief    VL53 写n个寄存器
  *
  * @param    dev：    地址
  * @param    reg：    寄存器
  * @param    length;  长度
  * @param    data：   指向写入数据
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/4/29 星期一
  */
void VL53_Write_nByte(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data)
{
    IIC_WriteMultByteToSlave( dev,  reg,  length,  data);
}

/**
  * @brief    VL53 读n个寄存器
  *
  * @param    dev：    地址
  * @param    reg：    寄存器
  * @param    length;  长度
  * @param    data：   指向存放读数据
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/4/29 星期一
  */
void VL53_Read_nByte(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data)
{
    IIC_ReadMultByteFromSlave(dev, reg, length, data);
}