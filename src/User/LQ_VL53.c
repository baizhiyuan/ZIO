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
	OLED_Init();
    OLED_CLS();
	OLED_P8x16Str(5,0,"LQ VL53 Test");
	char txt[16];
	IIC_Init();       

    uint8_t VL53_STAR = 0x40;
    uint8_t VL53_STOP = 0x40;
    uint8_t dis_buff[2];
    float dis;
    while(1)
    {
        /* 开始一次测距 */
        VL53_Write_nByte(VL53ADDR, SYSTEM__MODE_START, 1, &VL53_STAR);
        /* 获取测量数据 */
	    VL53_Read_nByte(VL53ADDR, PHASECAL_CONFIG__TIMEOUT_MACROP, 2, dis_buff);

        VL53_Write_nByte(VL53ADDR, SYSTEM__MODE_START, 1, &VL53_STOP);
        
        /* 转换数据 */
        dis = (dis_buff[0]<<8) | (dis_buff[1]);
        /*if(dis > 8000)
        {
            dis = 0;
        }*/
        float distanceInches = dis * 0.0393701;
        float distanceFeet = distanceInches / 12.0;
		sprintf(txt, "DIS %5f cm",distanceFeet);
		OLED_P8x16Str(0,5,txt);
		delayms(100);
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
