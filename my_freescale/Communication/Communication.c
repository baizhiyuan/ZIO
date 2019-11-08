#include "Communication.h"
#include "SEEKFREE_MT9V032.h"
extern uint8 Data_State;

extern uint8 avoid_x;
extern uint8 avoid_y;
extern int16 Right_rear_speed;
extern int16 Left_rear_speed;   

extern int16 slave_beacon_x;
extern int16 slave_beacon_y;

void Connunication_init(void)   
{
	uart_init (uart1, 115200);
	set_irq_priority(UART1_RX_TX_IRQn,4);				
	uart_rx_irq_en(uart1);
}


void UART1_RX_ISR(void)              
{
	static uint8_t Temp[8]={0};  									
	static uint8_t state = 0;
	static uint8_t _data_len = 6,_data_cnt = 0;                      
	uint8_t byteReceived;
	uart_getchar(uart1,&byteReceived);
	if(byteReceived == 0xfe && state==0)                              
	{
		state = 1;
		Temp[0] = byteReceived;
	}			
	else if(byteReceived == 0xfa && state==1)													
	{
		state=2;
		Temp[1] = byteReceived;
	}
	else if(state == 2&&_data_len>0)
	{
		_data_len--;
		Temp[2+_data_cnt++] = byteReceived;
		if(_data_len==0)
			state = 3;
	}
	else if(state == 3)
	{
		if(byteReceived == 0xff)  																			
		{
		
			Left_rear_speed = ((int)(Temp[2]<<8)&0xff00)|Temp[3];         
			Right_rear_speed = ((int)(Temp[4]<<8)&0xff00)|Temp[5];	
			avoid_x = Temp[6];
			avoid_y = Temp[7];
#if NO_Barrier_advoidance 
			avoid_x = 0;
			avoid_y = 0;
#endif
			if(Left_rear_speed >30000)    
				Left_rear_speed = Left_rear_speed-65535;	
			if(Right_rear_speed >30000)
				Right_rear_speed = Right_rear_speed-65535;
			
			state = 0;
			_data_len = 6;                                                
			_data_cnt = 0;
			return;
		}
		else    
		{
			_data_cnt = 0;
			_data_len = 6;																							 
			state = 0;
			return;
		}

	}
	else
	{
		state = 0;
		_data_len = 6;                                                 
		_data_cnt = 0;
	}
}

