/*-----------------------------------------------------------------------------------------------------
【平    台】龙邱K60核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.2
【功    能】PIT 模块
【注意事项】
-------------------------------------------------------------------------------------------------------*/
#ifndef _PIT_H_
#define _PIT_H_

#define PIT_Flag_Clear(PITn)   PIT_TFLG(PITn)|=PIT_TFLG_TIF_MASK      //清中断标志

//模块定义
typedef enum PITn
{
    PIT0,
    PIT1,
    PIT2,
    PIT3
} PITn;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************************************
                                                    PIT模块函数
**************************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*------------------------------------------------------------------------------------------------------
【函    数】PIT_Init
【功    能】PIT定时器定时中断初始化
【参    数】pitn:模块名PIT0或PIT1或PIT2或PIT3
【参    数】ms 中断时间，单位ms
【返 回 值】无
【实    例】pit_init(PIT0,1000); // PIT0中断，1000ms，即1s进入PIT0_interrupt()一次 
【注意事项】注意需要使用NVIC_SetPriority来配置PIT中断优先级   NVIC_EnableIRQ来使能中断 
【注意事项】
【注意事项】优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断 
【注意事项】NVIC_SetPriority(PIT0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
【注意事项】NVIC_EnableIRQ(PIT0_IRQn);			          //使能PIT0_IRQn的中断
--------------------------------------------------------------------------------------------------------*/
void PIT_Init(PITn pitn, uint32_t ms);
void PIT_Step(PITn pitn, uint32_t ns);


/*------------------------------------------------------------------------------------------------------
【函    数】PIT_delayms
【功    能】PIT定时器定时延时
【参    数】pitn:模块名PIT0或PIT1或PIT2或PIT3
【参    数】ms  延时时间，单位ms
【返 回 值】无
【实    例】PIT_delayms(PIT0,1000); // PIT0延时，1000ms 
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void PIT_Delayms(PITn pitn, uint32_t ms);


/*------------------------------------------------------------------------------------------------------
【函    数】PIT_delayus
【功    能】PIT定时器定时延时
【参    数】pitn:模块名PIT0或PIT1或PIT2或PIT3
【参    数】us  延时时间，单位us
【返 回 值】无
【实    例】PIT_delayms(PIT0,1000); // PIT0延时，1000us 
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void PIT_Delayus(PITn pitn, uint32_t us);


/*------------------------------------------------------------------------------------------------------
【函    数】PIT_TimeStart
【功    能】PIT定时器定时计时开始
【参    数】pitn:模块名PIT0或PIT1或PIT2或PIT3
【返 回 值】无
【实    例】PIT_TimeStart(PIT0); // PIT0计时 当作程序计时器用 
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void PIT_TimeStart(PITn pitn);


/*------------------------------------------------------------------------------------------------------
【函    数】PIT_TimeGet
【功    能】PIT定时器得到计时时间
【参    数】pitn:模块名PIT0或PIT1或PIT2或PIT3
【返 回 值】计时时间 单位us
【实    例】PIT_TimeGet(PIT0);  // PIT0计时 当作程序计时器用  
【注意事项】注意要先用PIT_TimeStart(PIT0);开启计时器
【注意事项】注意PIT为32为定时器  最大计时 (0xFFFFFFFF / Bus Clock 总线频率) s
--------------------------------------------------------------------------------------------------------*/
uint32_t PIT_TimeGet(PITn pitn);


/*------------------------------------------------------------------------------------------------------
【函    数】PIT_TimeGet
【功    能】PIT定时器得到计时时间
【参    数】pitn:模块名PIT0或PIT1或PIT2或PIT3
【返 回 值】计时时间 单位us
【实    例】PIT_TimeGet(PIT0);  // PIT0计时 当作程序计时器用  
【注意事项】注意要先用PIT_TimeStart(PIT0);开启计时器
【注意事项】注意PIT为32为定时器  最大计时 (0xFFFFFFFF / Bus Clock 总线频率) s
--------------------------------------------------------------------------------------------------------*/
void PIT_Close(PITn pitn);

#endif  