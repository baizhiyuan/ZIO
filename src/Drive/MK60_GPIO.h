/*-----------------------------------------------------------------------------------------------------
【平    台】龙邱K60核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.2
【功    能】GPIO 模块
【注意事项】
-------------------------------------------------------------------------------------------------------*/

#ifndef __GPIO_H__
#define __GPIO_H__

//定义管脚方向
typedef enum GPIO_CFG
{
    //这里的值不能改！！！
    GPI         = 0,                          //定义管脚输入方向      GPIOx_PDDRn里，0表示输入，1表示输出
    GPO         = 1,                          //定义管脚输出方向

    GPI_DOWN    = 0x02,                       //输入下拉              PORTx_PCRn需要PE=1，PS=0
    GPI_UP      = 0x03,                       //输入上拉              PORTx_PCRn需要PE=1，PS=1
    GPI_PF      = 0x10,                       //输入，带无源滤波器,滤波范围：10 MHz ~ 30 MHz 。不支持高速接口（>=2MHz）  0b10000           Passive Filter Enable
    GPI_DOWN_PF = GPI_DOWN | GPI_PF ,         //输入下拉，带无源滤波器
    GPI_UP_PF   = GPI_UP   | GPI_PF ,         //输入上拉，带无源滤波器

    GPO_HDS     = 0x41,                        //输出高驱动能力   0b100 0001    High drive strength
    GPO_SSR     = 0x05,                        //输出慢变化率          0b101     Slow slew rate
    GPO_HDS_SSR = GPO_HDS | GPO_SSR,           //输出高驱动能力、慢变化率
} GPIO_CFG;  //最低位为0，肯定是输入；GPI_UP 和 GPI_UP_PF的最低位为1，其他为输出


typedef enum exti_cfg
{
    zero_down     = 0x08u,     //低电平触发，内部下拉
    rising_down   = 0x09u,     //上升沿触发，内部下拉
    falling_down  = 0x0Au,     //下降沿触发，内部下拉
    either_down   = 0x0Bu,     //跳变沿触发，内部下拉
    one_down      = 0x0Cu,     //高电平触发，内部下拉

    //用最高位标志上拉和下拉
    zero_up       = 0x88u,     //低电平触发，内部上拉
    rising_up     = 0x89u,     //上升沿触发，内部上拉
    falling_up    = 0x8Au,     //下降沿触发，内部上拉
    either_up     = 0x8Bu,     //跳变沿触发，内部上拉
    one_up        = 0x8Cu      //高电平触发，内部上拉
} exti_cfg;

extern GPIO_MemMapPtr GPIOX[5];
extern PORT_MemMapPtr PORTX[5];
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************************************
                                                    GPIO配置函数
**************************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*------------------------------------------------------------------------------------------------------
【函    数】GPIO_Init
【功    能】初始化GPIO 并配置GPIO模式
【参    数】ptx_n ： 要初始化的GPIO， 在common.h中定义
【参    数】dir   ： GPIO方向和配置， 具体在GPIO.h中
【参    数】data  ： GPIO默认状态  1：高电平  0：低电平
【返 回 值】无
【实    例】GPIO_Init(PTA17, GPO, 1); //设置PTA17为输出模式 并设置为高电平
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void GPIO_PinInit(PTXn_e ptx_n, GPIO_CFG dir, uint8_t data);



/*------------------------------------------------------------------------------------------------------
【函    数】GPIO_PinWrite
【功    能】设置IO口输出
【参    数】ptx_n ： 要初始化的GPIO， 在common.h中定义
【参    数】data  ： GPIO输出状态  1：高电平  0：低电平
【返 回 值】无
【实    例】GPIO_PinWrite(PTA17, 1); //设置PTA17输出高电平
【注意事项】注意要使用GPIO初始化函数
--------------------------------------------------------------------------------------------------------*/
void GPIO_PinWrite(PTXn_e ptx_n, uint8_t data);



/*------------------------------------------------------------------------------------------------------
【函    数】GPIO_PinSetDir
【功    能】设置IO口输入还是输出
【参    数】ptx_n ： 要初始化的GPIO， 在common.h中定义
【参    数】dir   ： GPIO方向   1：输出  0：输入
【返 回 值】无
【实    例】GPIO_PinSetDir(PTA17, 1); //设置PTA17输出
【注意事项】注意要使用GPIO初始化函数
--------------------------------------------------------------------------------------------------------*/
void GPIO_PinSetDir(PTXn_e ptx_n, uint8_t dir);


/*------------------------------------------------------------------------------------------------------
【函    数】GPIO_PinReverse
【功    能】GPIO取反
【参    数】ptx_n ： 要初始化的GPIO， 在common.h中定义
【返 回 值】无
【实    例】GPIO_PinReverse(PTA17); //取反PTA17管脚电平
【注意事项】注意要使用GPIO初始化函数
--------------------------------------------------------------------------------------------------------*/
void GPIO_PinReverse(PTXn_e ptx_n);



/*------------------------------------------------------------------------------------------------------
【函    数】GPIO_PinRead
【功    能】读取IO口电平
【参    数】ptx_n ： 要初始化的GPIO， 在common.h中定义
【返 回 值】1： 高电平   0：低电平
【实    例】GPIO_PinRead(PTA17); //读取PTA17管脚电平
【注意事项】注意要使用GPIO初始化函数
--------------------------------------------------------------------------------------------------------*/
uint8_t GPIO_PinRead(PTXn_e ptx_n);


/*------------------------------------------------------------------------------------------------------
【函    数】GPIO_PinReverse
【功    能】GPIO取反
【参    数】ptx_n ： 要初始化的GPIO， 在common.h中定义
【返 回 值】无
【实    例】GPIO_ExtiInit(PTA17, rising_down); //PTA17管脚上升沿触发中断
【注意事项】注意需要使用NVIC_SetPriority来配置PIT中断优先级   NVIC_EnableIRQ来使能中断 
【注意事项】
【注意事项】优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断 
【注意事项】NVIC_SetPriority(PORTA_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
【注意事项】NVIC_EnableIRQ(PORTA_IRQn);			         //使能PORTA_IRQn的中断  
--------------------------------------------------------------------------------------------------------*/
void GPIO_ExtiInit(PTXn_e ptx_n, exti_cfg cfg);
#endif 