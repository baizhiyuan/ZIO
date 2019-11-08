#include "Fivedir_key.h"

extern uint8 menu;
extern u8 basic_speed;
extern u8 Rotate_speed;
extern u8 coordinate_X ;
extern u8 coordinate_Y ;
extern u8 prama[5];
extern u8 My_sectorNum;
extern u8 beacon_threshold;
extern uint8 min_gray;
extern float KP ; 
extern float KD ;
extern float KP_show ; 
extern float KD_show ;
void key_init(void)
{

	port_init (KEY1,IRQ_FALLING|ALT1|PULLUP );
	port_init (KEY2,IRQ_FALLING|ALT1|PULLUP );
	port_init (KEY3,IRQ_FALLING|ALT1|PULLUP );
	port_init (KEY4,IRQ_FALLING|ALT1|PULLUP );
	port_init (KEY5,IRQ_FALLING|ALT1|PULLUP );
	port_init (key_down,IRQ_FALLING|ALT1|PULLUP );
	gpio_init(KEY1,GPI,1);
	gpio_init(KEY2,GPI,1);
	gpio_init(KEY3,GPI,1);
	gpio_init(KEY4,GPI,1);
	gpio_init(KEY5,GPI,1);
	gpio_init(key_down,GPI,1);

	
	set_irq_priority(PORTD_IRQn,5);						
	enable_irq(PORTD_IRQn);								
	set_irq_priority(PORTB_IRQn,6);						
	enable_irq(PORTB_IRQn);								
	EnableInterrupts;								
	
}

void LED_Init(void)
{
	gpio_init(A28,GPO,0);
	gpio_init(A29,GPO,0);
	gpio_set(A28,1);				
	gpio_set(A29,1);			
}

void BEEP_Init() 
{
	gpio_init(E24,GPO,0);	
	gpio_set(E24,0);				
}

void IF_KEY2_Press()
{

   menu++;
   if(menu==7)menu=1;

}
void IF_KEY1_Press()
{

   if(menu==1)menu=7;
   menu--;

}
void IF_KEY3_Press()
{
	 switch(menu)
   {            
     case 1 :  basic_speed-=1;                    break;     
     case 2 :  KP-=0.1f;                      break;
     case 3 :  KD-=0.1f;                      break;
		 case 4 :  Rotate_speed-=1;								break;
		 case 5 :  min_gray-=1;                   break;
     default :                                break;

   }
	if(KP<0)
		KP = 0;
	if(KD<0)
		 KD = 0;
	KP_show = KP*10;
	KD_show = KD*10;

}
void IF_KEY4_Press()
{
	 switch(menu)
   {            
     case 1 :  basic_speed+=1;                    break;
     case 2 :  KP+=0.1f;                      break;
     case 3 :  KD+=0.1f;                      break;
		 case 4 :  Rotate_speed+=1;								break;
		 case 5 :  min_gray+=1;                   break;
     default :                                break; 
   }
	 
	 if(KP>25)
		 KP = 25;
	 if(KD>25)
		 KD = 25;
	 KP_show = KP*10;
   KD_show = KD*10;


}
void IF_KEY5_Press()
{
	prama[0]=basic_speed;
  prama[1]=KP*10;
  prama[2]=KD*10;
	prama[3]=Rotate_speed;
	prama[4]=min_gray;
	FLASH_EraseSector(My_sectorNum);	
	FLASH_WriteSector(My_sectorNum,(const uint8 *)prama,5,0);	
	
}



