#ifndef _FIVEDIRKEY_h
#define _FIVEDIRKEY_h

#include "common.h"
#include "MK60_port.h"
#include "MK60_flash.h"
#include "MK60_gpio.h"
#include "Variable.h"

void LED_Init(void);
void BEEP_Init(void);

#define  LED1  A28
#define  LED2  A29

#define LED1_ON 	gpio_set(A29,0);	
#define LED2_ON 	gpio_set(A28,0);	
#define LED1_OFF 	gpio_set(A29,1);	
#define LED2_OFF 	gpio_set(A28,1);	

void key_init(void);
void IF_KEY1_Press(void);
void IF_KEY2_Press(void);
void IF_KEY3_Press(void);
void IF_KEY4_Press(void);
void IF_KEY5_Press(void);

#define  KEY1  D15   //up
#define  KEY2  D13   //down
#define  KEY3  D14   //sub
#define  KEY4  D11   //add
#define  KEY5  D12   //OK 

#define  key_down  B23   //add   B23  a8

#define BEEP_ON 	gpio_set(E24,1);			//设置B0引脚为低电平	
#define BEEP_OFF 	gpio_set(E24,0);			//设置B0引脚为低电平	

#endif
