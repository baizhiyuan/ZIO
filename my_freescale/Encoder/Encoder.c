#include "Encoder.h"


extern int16 Left_rear_speed;/*右前轮速度*/
extern int16 Left_front_speed;/*左前轮速度*/
extern int16 Right_front_speed;/*右前轮速度*/
extern int16 Right_rear_speed;/*右后轮速度*/

void Encoder_Init()
{
	ftm_quad_init(ftm1);	//A12 A13
	ftm_quad_init(ftm2);	//B18 B19
}


/*速度采集函数，定时器内运行*/
void speed_get(void)
{
	Left_front_speed=ftm_quad_get(ftm1);
	Right_front_speed=-ftm_quad_get(ftm2);
	
	ftm_quad_clean(ftm1);
	ftm_quad_clean(ftm2);

}


/*二通道速度滤波函数*///这是滤波部分，在程序里可加可不加，我没加，加了也可以。但是看不出明显效果
int16 Speed_Filter(int16 Channal,int16 Data_In)
{
	static int16	Data[2][3]={0};
	int16 Temp=0;
	Data[Channal][0]=Data[Channal][1];
	Data[Channal][1]=Data[Channal][2];
	Data[Channal][2]=Data_In;
	Temp=PaiXu(Data[Channal][0],Data[Channal][1],Data[Channal][2]);
	
	return Temp;
}
