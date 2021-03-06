/*---------------------------------------------------------------
【平    台】龙邱K60核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.2
【功    能】中断服务函数
【注意事项】中断服务函数的名称在startup_MK60D10.s 中
----------------------------------------------------------------*/
#include "include.h"

//------------------------------------------------------------------------------------------------    
//全局变量  用于测试按键外部中断
//------------------------------------------------------------------------------------------------
extern volatile uint8_t key_exti_flag;

/////////////////////////////////////////////////////////////////
///////////////GPIO中断服务函数//////////////////////////////////
/////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------
【函    数】PORTB_Interrupt
【功    能】PORTB端口的中断服务函数
【参    数】无
【返 回 值】无
【注意事项】注意进入后要清除中断标志位
----------------------------------------------------------------*/
void PORTB_IRQHandler()
{
    /* 中断使用的引脚号，这里只写了管脚20和管脚21和22，使用其他管脚可以自行添加 */
    int n;  
    n=20;
    if((PORTB_ISFR & (1<<n)))
    {
        /* 用户自行添加中断内程序 */
        key_exti_flag = 0;
    } 
    n=21;
    if((PORTB_ISFR & (1<<n)))
    {
        /* 用户自行添加中断内程序 */
        key_exti_flag = 1;
    } 
    n=22;
    if((PORTB_ISFR & (1<<n)))
    {
        /* 用户自行添加中断内程序 */
        key_exti_flag = 2;
    }
    //清除中断标志
    PORTB_ISFR = 0xffffffff;
}

/////////////////////////////////////////////////////////////////
///////////////PIT中断服务函数///////////////////////////////////
/////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------    
//全局变量  用于测试PIT定时器
//------------------------------------------------------------------------------------------------ 
extern volatile uint8_t pit0_test_flag;
extern volatile uint8_t pit1_test_flag;
extern volatile uint8_t pit2_test_flag;
extern volatile uint8_t pit3_test_flag;
extern volatile uint8 step_flag;
uint16 step_num;
int16 step_target;
uint8 step_last;
extern volatile uint8_t pit0_flag;
extern volatile uint8_t pit2_flag;
/*---------------------------------------------------------------
【函    数】PIT0_Interrupt
【功    能】PIT0的中断服务函数
【参    数】无
【返 回 值】无
【注意事项】注意进入后要清除中断标志位
----------------------------------------------------------------*/
void PIT0_IRQHandler()
{
    Timer_IQR_handle();
    PIT_Flag_Clear(PIT0);       //清中断标志位
    
    //pit0_flag = 1;
    /*用户添加所需代码*/  
}
/*---------------------------------------------------------------
【函    数】PIT1_Interrupt
【功    能】PIT1的中断服务函数
【参    数】无
【返 回 值】无
【注意事项】注意进入后要清除中断标志位
----------------------------------------------------------------*/
void PIT1_IRQHandler()//步进电机中断
{
	if (step_flag == 0){
		step_target = 2780;//为了初始化步进电机的定位
		GPIO_PinWrite(PTE28, 0);
		step_flag = 2;
		step_last = step_flag;
	}
	if(step_flag != step_last){
		step_target = (step_flag - step_last) * 2720;
		if (step_target > 0){
			GPIO_PinWrite(PTE28, 0);
		}else if (step_target < 0){
			GPIO_PinWrite(PTE28, 1);
			step_target= - step_target;
		}
		step_last = step_flag;
	}
	if (step_num < step_target){
		GPIO_PinWrite(PTE27, 1);
		LPTMR_delayus(5);
		GPIO_PinWrite(PTE27, 0);
		step_num++;
	}else {
		step_num = 0;
		step_target = 0;
	}
	PIT_Flag_Clear(PIT1);       //清中断标志位
   // pit2_flag = 1;
}
/*---------------------------------------------------------------
【函    数】PIT2_Interrupt
【功    能】PIT2的中断服务函数
【参    数】无
【返 回 值】无
【注意事项】注意进入后要清除中断标志位
----------------------------------------------------------------*/
void PIT2_IRQHandler()//显示屏中断
{
    //Show_IQR_handle();
    PIT_Flag_Clear(PIT2);       //清中断标志位
   // pit2_flag = 1;
    /*用户添加所需代码*/
}
/*---------------------------------------------------------------
【函    数】PIT3_Interrupt
【功    能】PIT3的中断服务函数
【参    数】无
【返 回 值】无
【注意事项】注意进入后要清除中断标志位
----------------------------------------------------------------*/
void PIT3_IRQHandler()
{
    PIT_Flag_Clear(PIT3);       //清中断标志位
    /*用户添加所需代码*/
    pit3_test_flag = 1;//一般中断就用一个flag
}

/////////////////////////////////////////////////////////////////
///////////////串口中断服务函数/////////////////////////////////
/////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------
【函    数】DMA_CH4_Handler
【功    能】DMA通道4的中断服务函数
【参    数】无
【返 回 值】无
【注意事项】注意进入后要清除中断标志位
----------------------------------------------------------------*/
/*******************************/
uint8_t pi_receive_tmp;
uint8_t pi_dat[6] = {0, 0, 0, 0, 0};
uint8_t pi_getbuf[3] = {0,0,0};

void UART2_RX_TX_IRQHandler(void)
{ 
    if(UART2_S1 & UART_S1_RDRF_MASK)                                     //接收数据寄存器满
    {
      pi_receive_tmp = UART_GetChar(UART2);
      for(uint8 i = 0; i < 6 - 1; i++)
      {
          pi_dat[i] = pi_dat[i + 1];
      }
      pi_dat[5 - 1] = pi_receive_tmp;
      if(pi_dat[0] == 0x55 && pi_dat[1] == 0x55 && pi_dat[6 - 1] == 0x50)
      {
          pi_getbuf[0] = pi_dat[2];
          pi_getbuf[1] = pi_dat[3];
          pi_getbuf[2] = pi_dat[4];
      }
     }
}
/*******************************/
void UART4_RX_TX_IRQHandler(void)
{
    if(UART4_S1 & UART_S1_RDRF_MASK)                                     //接收数据寄存器满
    {
        //用户需要处理接收数据
        ANO_DT_Data_Receive_Prepare(UART_GetChar(UART4));
    }
    
}