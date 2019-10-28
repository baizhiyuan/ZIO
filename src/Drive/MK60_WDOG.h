/******************** LQ_K60_函数库 v1.0 ********************
 * 文件名           ：wdog.h
 * 功能             ：看门狗
 * 备注             ：官方例程上修改
 * 日期             ：2015-10-16
 * 实验平台         ：龙丘 k60DN512Z核心板 
 * 作者             ：龙丘技术 006
 * 淘宝店           ：https://longqiu.taobao.com
 * 龙丘智能车讨论群 ：202949437
*************************************************************/

#ifndef __WDOG_H__
#define __WDOG_H__

//-------------------------------------------------------------------------*
//函数名: WDOG_SetTime                                                     *
//功  能: 设置喂狗时间                                                     * 
//参  数: 无			  					                               *	
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void WDOG_SetTime(uint32 ms);


//-------------------------------------------------------------------------*
//函数名: WDOG_Feed                                                        *
//功  能: 喂狗                                                             * 
//参  数: 无			  					                               *	
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void WDOG_Feed(void);


//-------------------------------------------------------------------------*
//函数名: WDOG_Unlock                                                      *
//功  能: 看门狗解锁                                                       * 
//参  数: 无							                                   *	
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void WDOG_Unlock(void);


//-------------------------------------------------------------------------*
//函数名: WDOG_Enable                                                      *
//功  能: 使能看门狗模块                                                   * 
//参  数: 无			  					                               *	
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void WDOG_Enable(void);


//-------------------------------------------------------------------------*
//函数名: WDOG_Disable                                                     *
//功  能: 关闭看门狗模块                                                   * 
//参  数: 无			  					                               *	
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void WDOG_Disable(void);

#endif 