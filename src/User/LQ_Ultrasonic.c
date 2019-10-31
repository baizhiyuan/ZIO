#include "include.h"

#define HC_TRIG   PTE9   
#define HC_ECHO   PTE10  


void Test_HCSR04(void)
{
	GPIO_PinInit(HC_TRIG, GPO, 0);  
  	GPIO_PinInit(HC_ECHO, GPI, 0);

	LED_Init();
    UART_Init(UART4, 115200);
	systime.init();
	printf("\r\nLQ HC-SR04 Test");
	
	char txt[16];
	uint64_t now_time = systime.get_time_us();
	static uint8_t count = 0;
	uint64_t hc_time;
	float dis = 0;
	while(1)
	{
		if(systime.get_time_us() - now_time >= 20000)
		{
			now_time = systime.get_time_us();

			GPIO_PinWrite(HC_TRIG, 1);
			systime.delay_us(10);
			GPIO_PinWrite(HC_TRIG, 0);
			

			while(!GPIO_PinRead(HC_ECHO));

			hc_time = systime.get_time_us();

			while(GPIO_PinRead(HC_ECHO));

			hc_time = systime.get_time_us() - hc_time;

			dis = (float)hc_time * 0.017; 
			
			if(count++ % 5 == 0)
			{
				sprintf(txt, "Dis:%5.2fcm", dis);
				OLED_P8x16Str(0,3,txt);

			}
		}
	}
}