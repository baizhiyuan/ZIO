#include "include.h"
#include "ZIO_Car.h"

float KP = 0; 
float KD = 0;
float P = 50; 
float I = 30;
float D = 30;
float Lspeed_temp_1;
float Rspeed_temp_1;
float Lspeed_temp_2 = 0;
float Rspeed_temp_2 = 0;

int perform = 0;
int lamp[2][12];//前后循迹情况
int lamp_num[2][12];//前后循迹偏差的对应赋值
uint8 arm[] = { 0x55,0x55,0x05,0x06,0x00,0x01,0x00 };//机械臂动作组
uint16_t batv[12];
uint16_t batv_paixu[12];

int8 counter_black=0;    //十字黑线计数
int8 counter_pos=0; //丁字定位点计数
int8 decide=0; //元素判断
uint8 counter_X=0;
uint8 counter_T=0;
uint8 transform=0;
uint8 transform_now=0;//达到先亮后灭的情况，表示经过一条黑线
uint8 lamp_center;

int16 error = 0;
int16 twinkle_delay = 0;
int16 Right_front_speed;
int16 Left_front_speed;
int16 Right_rear_speed;
int16 Left_rear_speed;
int16 Left_front_goalspeed;
int16 Right_front_goalspeed;
int16 Left_rear_goalspeed;
int16 Right_rear_goalspeed;

uint8 kong_flag = 1;
uint8 part_flag = 0;
int16 temp_num = 0;
int16 left_num = 0;
int16 right_num = 0;
short ABGet_left, ABGet_right;

_pid_t pid_left_front;    
_pid_t pid_right_front;		
_pid_t pid_track;				

volatile uint8_t step_flag = 0;
vint8 pos_flag = 0;
vint16 Target_num = 0;
vint16 Target_speed = 0;


void GCXL(void)
{
  while(GPIO_PinRead(PTD13) == 1){
  PART();
  if(perform == 1) Part_Choose();
  track_PID();counter_sum();
  }delayms(100);
}

void Sys_init_all(void)
{
  Motor_Init();
  ADC_Init(ADC0);
  ADC_Init(ADC1);
  STEP_Init();
  Uart_Init();
  LED_Init();
  KEY_Init();   
  FTM_ABInit(FTM1);
  FTM_ABInit(FTM2);
  OLED_Init();
  OLED_CLS();
  Pid_Init();//速度环的四轮pid参数，实际只用了前轮，后轮依照其特性公式改变速度
  Timer_Init(); //定时刷新速度环，PIT0，5ms
}

void Positioning(void)
{
  uint8 a=1;
  Target_num = 200;
  pos_flag = 1;
  while (a)
  {
    track_PID();
    if(pos_flag != 1){
      a=0;
      break;
    }
    //lamp_center = GPIO_PinRead(PTC10);
  }
}

void track_delay(int16 time)
{
  uint8 a=1;
  uint8 b=0;
  while (a)
  {
    b ++;
    track_PID();
    if(b == time){
      a=0;
      break;
    }
  }
}

void Obstacle(void){
  uint8 a=1;
  while(a){
    a=0;
    break;
  }
}

void start(void)
{ 
  uint8 a=1;
  while (a)
  {  
    switch(KEY_Read(0))     
    {
      case 2:LED_Reverse(1); a=0;Target_speed =60;break;  
      case 1:step_flag = 1;break; 
      case 3:step_flag = 3;break; 
      default: break; 
    }delayms(100);
  }
}

void Uart_Init(void)
{
    UART_Init(UART4, 115200); 
    UART_Init(UART3, 9600);   
    //NVIC_SetPriority(UART3_RX_TX_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3,4));
    //NVIC_EnableIRQ(UART3_RX_TX_IRQn);			          //使能UART4_RX_TX_IRQn的中断
    //UART_PutBuff(UART3, arm, 7);
}

void Timer_Init(void)
{
    PIT_Init(PIT0, 5);		//速度环更新
    PIT_Step(PIT1, 300);
    PIT_Init(PIT2, 200);	//显示屏刷新	
    NVIC_SetPriority(PIT0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),2,3));
    NVIC_SetPriority(PIT1_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));					
    NVIC_SetPriority(PIT2_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3,4));
    NVIC_EnableIRQ(PIT0_IRQn);			          //使能PIT0_IRQn的中断	
    NVIC_EnableIRQ(PIT1_IRQn);	                  //使能中断函数在IRQ_Handle内
    NVIC_EnableIRQ(PIT2_IRQn);		          
}


void Timer_IQR_handle()
{
  ABGet_left = FTM_ABGet(FTM1);
  ABGet_right = FTM_ABGet(FTM2);
	Lspeed_temp_1 = ABGet_left * 12.6 ;//12.6根据减速比和脉冲数计算所得
	Rspeed_temp_1 = ABGet_right * 12.6 ;
	Left_front_speed = Lspeed_temp_1 + Lspeed_temp_2;//因为定时周期为5ms，增加了上次的脉冲数以得到10ms内的脉冲数
	Right_front_speed = Rspeed_temp_1 + Rspeed_temp_2;//获得的速度单位为  mm/100ms
	Lspeed_temp_2 = Lspeed_temp_1; Rspeed_temp_2 = Rspeed_temp_1;

  if(pos_flag == 1){
    left_num +=ABGet_left;  
    right_num +=ABGet_right;
    temp_num = left_num + right_num;
    if(Target_num < temp_num){
      Target_speed = 0;pos_flag = 0;left_num = 0;right_num = 0;}
  }

	PID_Control(&pid_left_front, Left_front_speed, Left_front_goalspeed);//增量式PID速度环
	PID_Control(&pid_right_front, Right_front_speed, Right_front_goalspeed);
  Left_rear_speed = 45.715 * Left_rear_goalspeed + 1346.4;//根据实验测试电机特性曲线算得
	Right_rear_speed = 41.826 * Right_rear_goalspeed + 1015;

	if(Target_speed == 0){Left_rear_speed = 0;Right_rear_speed = 0;pid_left_front.result=0;pid_right_front.result=0;}//使其为0时能保证停止
  
	Left_front_pwm_set(pid_left_front.result);     Right_front_pwm_set(pid_right_front.result);
  Left_rear_pwm_set(Left_rear_speed);            Right_rear_pwm_set(Right_rear_speed); 
}


/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】PID循迹程序
-------------------------------------------------------------------------------------------------------*/


void paixu(unsigned char *p,unsigned char len)
{
  unsigned char i, j,buf;
  for(i=0; i<len-1; i++){       
    for(j=0; j<len-i-1; j++){
      if(p[j]<p[j+1]){
        buf = p[j];
        p[j] = p[j+1];
        p[j+1] = buf;}
    } 
 }
}

void ADC_turn(void)
{        /* 获取 ADC通道值 */
    batv[0] = (uint16_t)(ADC_Get(0)*0.806);
    batv[1] = (uint16_t)(ADC_Get(1)*0.806);
    batv[2] = (uint16_t)(ADC_Get(2)*0.806);
    batv[3] = (uint16_t)(ADC_Get(3)*0.806);

    batv[4] = (uint16_t)(ADC_Get(4)*0.806);
    batv[5] = (uint16_t)(ADC_Get(5)*0.806);
    batv[6] = (uint16_t)(ADC_Get(6)*0.806);
    batv[7] = (uint16_t)(ADC_Get(7)*0.806);

    batv[8] = (uint16_t)(ADC_Get(8)*0.806);
    batv[9] = (uint16_t)(ADC_Get(9)*0.806);
    batv[10] = (uint16_t)(ADC_Get(10)*0.806);
    batv[11] = (uint16_t)(ADC_Get(11)*0.806);
    for(int t=0;t<12;t++)
    {
      if( batv[t]< 2500 ) lamp[0][t] = 1;
      else lamp[0][t] = 0;
    }
    
    int temp;
    for(int i=1;i<11;i++){
      if(i != 5 || i != 6){
      temp = 2*lamp[0][i] - lamp[0][i-1] - lamp[0][i+1];
      if(temp == 2) lamp[0][i]=0;
      else if(temp == -2) lamp[0][i]=1;}
    }
}

void read_lamp_values(void)
{
  ADC_turn();
  int error_current = 0;
  uint8 lamp_Lsum = 0;
  uint8 lamp_Rsum = 0;
  for(int j = 0; j < 12; j ++){
    if(lamp[0][j] == 1){
      lamp_num[0][j] = 2*j - 11;            //-11 -9 -7 -5 -3 -1  1  3  5  7  9  11
      if(j < 6)lamp_Lsum ++;
      else lamp_Rsum ++;
    }else lamp_num[0][j] = 0;
    error_current += lamp_num[0][j];//前循迹偏差值
  }if (lamp_Rsum > 3 ) {
    if(transform == 1) decide = 1;//十字路口  1 1 1 1 1 1     直行
    error = 0;
  /*}else if ((lamp_Lsum < 3)&&(lamp_Rsum > 3)) {
    decide = 2;//定位点
    error = 0;*/
  }else error = error_current;
  /*if ((lamp_Lsum == 0)&&(lamp_Rsum == 0)) {
    if(kong_flag == 0){
      if ((error > 0)&&(error < 30)) error = 35;//  0 0 0 0 0前面循迹迷失，以上次偏差情况进行反向补偿 
      else if((error < 0)&&(error > -30)) error = -35; 
    }else Target_speed = 80;}*/

  if (lamp_Lsum+lamp_Rsum == 2) transform = 1;//这一次不在交叉线，在普通白线
}

void counter_sum(void){

    if(counter_X < 2){
      if(decide == 1){
        counter_X++;
        transform = 0;
        decide = 0;
        perform = 1;}
    }else if(counter_X == 2){
      if(counter_T < 3){
        if(decide == 2){
          counter_T++; 
          transform = 0;
          decide = 0;
          perform = 1;}}
      else if(counter_T==3){
        if(decide == 1){
          counter_X++;
          transform = 0;
          decide = 0;
          perform = 1;
          counter_T=0;}}
    }else if(counter_X == 3){
      if(counter_T < 3){
        if(decide == 2){
          counter_T++; 
          transform = 0;
          decide = 0;
          perform = 1;}
      }else if(counter_T==3){
        if(decide == 1){
          counter_X++;
          transform = 0;
          decide = 0;
          perform = 1;}
      }
    }
}
/*
void counter_sum(void){

 // if (transform == 1){
    if(decide == 1){
      counter_X++;
      transform = 0;
      decide = 0;
    }
 // }
}*/


void PART(void)
{  
    if(counter_X == 0 && counter_T == 0) part_flag=1;
    else if(counter_X == 1 && counter_T == 0) part_flag=2;
    else if(counter_X == 2 && counter_T == 0) part_flag=2;
    else if(counter_X == 2 && counter_T == 1) part_flag=11;
    else if(counter_X == 2 && counter_T == 2) part_flag=12;
    else if(counter_X == 2 && counter_T == 3) part_flag=13;
    else if(counter_X == 3 && counter_T == 0) part_flag=3;
    else if(counter_X == 3 && counter_T == 1) part_flag=21;
    else if(counter_X == 3 && counter_T == 2) part_flag=22;
    else if(counter_X == 3 && counter_T == 3) part_flag=23;
   /* else if(counter_X == 3 && counter_T == 4) part_flag=24;
    else if(counter_X == 3 && counter_T == 5) part_flag=25;
    else if(counter_X == 3 && counter_T == 6) part_flag=26;*/
    else if(counter_X == 4) {counter_X = 0;counter_T = 0;}
    else {part_flag=0;}
    perform = 1;
}
/*void PART_all(void)
{  
    if(counter_X == 0) part_flag=1;
    else if(counter_X == 1) part_flag=2;
    else if(counter_X == 2) part_flag=2;
    else if(counter_X == 3) part_flag=11;
    else if(counter_X == 4) part_flag=12;
    else if(counter_X == 5) part_flag=13;
    else if(counter_X == 6) part_flag=3;
    else if(counter_X == 7) part_flag=21;
    else if(counter_X == 8) part_flag=22;
    else if(counter_X == 9) part_flag=23;
    else if(counter_X == 3 && counter_T == 4) part_flag=24;
    else if(counter_X == 3 && counter_T == 5) part_flag=25;
    else if(counter_X == 3 && counter_T == 6) part_flag=26;
    else if(counter_X == 10) {counter_X = 0;Target_speed = 0;}
    else {part_flag=0;}
    perform = 1;
}*/

void Part_Choose(void)
{
    switch(part_flag){
        case 1:Target_speed = 60;break;
        case 2:Target_speed = 90;break;
        case 11:Target_speed = 30; Positioning();UART_PutBuff(UART3, arm, 7);step_flag = 1;delayms(5000);Target_speed = 50;break;
        case 12:Target_speed = 30; Positioning();UART_PutBuff(UART3, arm, 7);step_flag = 2;delayms(2000);break;
        case 13:Target_speed = 30; Positioning();UART_PutBuff(UART3, arm, 7);step_flag = 3;delayms(5000);Target_speed = 80;step_flag = 2;break;
        case 3:break;
              //Obstacle();break;//避障区域
               // Target_speed = 100;track_PID ();counter_sum();break;    
        case 21:Target_speed = 30; Positioning();UART_PutBuff(UART3, arm, 7);step_flag = 1;delayms(5000);Target_speed = 50;break;
        case 22:Target_speed = 30; Positioning();UART_PutBuff(UART3, arm, 7);step_flag = 2;delayms(2000);Target_speed = 50;break;
        case 23:Target_speed = 30; Positioning();UART_PutBuff(UART3, arm, 7);step_flag = 3;delayms(2000);Target_speed = 50;break;
      /*  case 24:Target_speed = 50; Positioning();UART_PutBuff(UART3, arm, 7);delayms(2000);Target_speed = 50;break;
        case 25:Target_speed = 50; Positioning();UART_PutBuff(UART3, arm, 7);delayms(2000);Target_speed = 50;break;
        case 26:Target_speed = 50; Positioning();UART_PutBuff(UART3, arm, 7);delayms(2000);Target_speed = 50;break;*/
        default: Target_speed = 0;break;
    }
    perform = 0;
}

void Pid_Init(void)
{
	pid_left_front.kp=90;
	pid_left_front.ki=5;
	pid_left_front.kd=17;
	
	pid_right_front.kp=90;
	pid_right_front.ki=5;
	pid_right_front.kd=17;
	
	pid_left_front.err=0;
	pid_left_front.last_err=0;
	pid_left_front.last_last_err=0;
	pid_left_front.result=0;
	
	pid_right_front.err=0;
	pid_right_front.last_err=0;
	pid_right_front.last_last_err=0;
	pid_right_front.result=0;

  pid_track.kp=100;
	pid_track.ki=80;
	pid_track.kd=80;
	
	pid_track.err=0;
	pid_track.last_err=0;
	pid_track.last_last_err=0;
	pid_track.result=0;
}

void PID_Control(_pid_t *pid, int16 actual, int16 set)
{
	float p = 0, i = 0, d = 0;
	pid->err = set - actual;
	p = pid->err - pid->last_err;
	i = pid->err;
	d = pid->err - 2 * pid->last_err + pid->last_last_err;
	pid->result += pid->kp * p +pid->ki * i + pid->kd * d;
	if(pid->result>9000)
		pid->result = 9000;
	else if(pid->result<-9000)
		pid->result = -9000;
	pid->last_last_err = pid->last_err;
	pid->last_err = pid->err;
}

void calc_pid(_pid_t *pid)//得到速差
{
	float p = 0, i = 0, d = 0;
	pid->err = error;
	p = pid->err - pid->last_err;
	i = pid->err;
	d = pid->err - 2 * pid->last_err + pid->last_last_err;
	pid->result = pid->kp * p +pid->ki * i + pid->kd * d;
	if(pid->result>2000)
		pid->result = 2000;
	else if(pid->result<-2000)
		pid->result = -2000;
	pid->last_last_err = pid->last_err;
	pid->last_err = pid->err;
}

void diff_speed(void){
  Left_front_goalspeed = Target_speed * (1 +0.001 * pid_track.result); Right_front_goalspeed = Target_speed * (1 - 0.001 * pid_track.result);
  Left_rear_goalspeed = Target_speed * (1 + 0.001 * pid_track.result); Right_rear_goalspeed = Target_speed * (1 - 0.001 * pid_track.result);
}

void track_PID (void)
{
  read_lamp_values();
  calc_pid(&pid_track);
  diff_speed();
}

/*------------------------------------------------------------------------------------------------------
【函    数】Test_Motor
【功    能】FTM0输出PWM信号控制电机例子
【参    数】num  :  电机标号  用PTC1 和 PTC2控制电机1  用PTC3 和 PTC4控制电机2
【返 回 值】duty ： 占空比    范围 -FTM_PRECISON  到 +FTM_PRECISON
【实    例】Test_Motor(); //测试电机
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void Left_front_pwm_set(int16 Left_frontvalue)//左前轮速度
{
	if(Left_frontvalue > 0){
    GPIO_PinWrite(PTD0, 1);
		GPIO_PinWrite(PTD1, 0);		
    FTM_PwmDuty(FTM0, FTM_CH0, Left_frontvalue);
	}else if(Left_frontvalue < 0){
		GPIO_PinWrite(PTD0, 0);
		GPIO_PinWrite(PTD1, 1);
    FTM_PwmDuty(FTM0, FTM_CH0, -Left_frontvalue);
	}else{
    GPIO_PinWrite(PTD0, 0);
		GPIO_PinWrite(PTD1, 0);
    FTM_PwmDuty(FTM0, FTM_CH0, 0);
  }
}


void Right_front_pwm_set(int16 Right_frontvalue)
{
	if(Right_frontvalue > 0){
    GPIO_PinWrite(PTD4, 1);
		GPIO_PinWrite(PTD5, 0);
		FTM_PwmDuty(FTM0, FTM_CH2, Right_frontvalue);
	}else if(Right_frontvalue < 0){
    GPIO_PinWrite(PTD4, 0);
		GPIO_PinWrite(PTD5, 1);
		FTM_PwmDuty(FTM0, FTM_CH2, -Right_frontvalue);
	}else{
    GPIO_PinWrite(PTD4, 0);
		GPIO_PinWrite(PTD5, 0);
    FTM_PwmDuty(FTM0, FTM_CH2, 0);
  }
}


void Left_rear_pwm_set(int16 Left_rearvalue)
{
	if(Left_rearvalue > 0){
		GPIO_PinWrite(PTD2, 0);
		GPIO_PinWrite(PTD3, 1);
    FTM_PwmDuty(FTM0, FTM_CH1, Left_rearvalue);
	}else if(Left_rearvalue < 0){
		GPIO_PinWrite(PTD2, 1);
		GPIO_PinWrite(PTD3, 0);
    FTM_PwmDuty(FTM0, FTM_CH1, -Left_rearvalue);
	}else{
    GPIO_PinWrite(PTD2, 0);
		GPIO_PinWrite(PTD3, 0);
    FTM_PwmDuty(FTM0, FTM_CH1, 0);
  }
}


void Right_rear_pwm_set(int16 Right_rearvalue)
{
	if(Right_rearvalue > 0){
    GPIO_PinWrite(PTD6, 0);
		GPIO_PinWrite(PTD7, 1);
		FTM_PwmDuty(FTM0, FTM_CH3, Right_rearvalue);
	}else if(Right_rearvalue < 0){
    GPIO_PinWrite(PTD6, 1);
		GPIO_PinWrite(PTD7, 0);
		FTM_PwmDuty(FTM0, FTM_CH3, -Right_rearvalue);
	}else{
    GPIO_PinWrite(PTD6, 0);
		GPIO_PinWrite(PTD7, 0);
    FTM_PwmDuty(FTM0, FTM_CH3, 0);
    }
}

void All_pwm_set(int16 Left_frontvalue, int16 Right_frontvalue, int16 Left_rearvalue, int16 Right_rearvalue)
{
    Left_front_pwm_set(Left_frontvalue);
    Right_front_pwm_set(Right_frontvalue);
    Left_rear_pwm_set(Left_rearvalue);
    Right_rear_pwm_set(Right_rearvalue);
}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】GPIO输入输出初始化
-------------------------------------------------------------------------------------------------------*/
void Infrared_Init(void)
{
    ADC_Init(ADC0);
    ADC_Init(ADC1);
    
    batv[0] = (uint16_t)(ADC_Get(0)*0.806);
    batv[1] = (uint16_t)(ADC_Get(1)*0.806);
    batv[2] = (uint16_t)(ADC_Get(2)*0.806);
    batv[3] = (uint16_t)(ADC_Get(3)*0.806);

    batv[4] = (uint16_t)(ADC_Get(4)*0.806);
    batv[5] = (uint16_t)(ADC_Get(5)*0.806);
    batv[6] = (uint16_t)(ADC_Get(6)*0.806);
    batv[7] = (uint16_t)(ADC_Get(7)*0.806);

    batv[8] = (uint16_t)(ADC_Get(8)*0.806);
    batv[9] = (uint16_t)(ADC_Get(9)*0.806);
    batv[10] = (uint16_t)(ADC_Get(10)*0.806);
    batv[11] = (uint16_t)(ADC_Get(11)*0.806);
}

void Motor_Init(void)
{
	GPIO_PinInit(PTD0, GPO, 0);
	GPIO_PinInit(PTD1, GPO, 0);
	GPIO_PinInit(PTD2, GPO, 0);
	GPIO_PinInit(PTD3, GPO, 0);
	GPIO_PinInit(PTD4, GPO, 0);
	GPIO_PinInit(PTD5, GPO, 0);
	GPIO_PinInit(PTD6, GPO, 0);
	GPIO_PinInit(PTD7, GPO, 0);
	FTM_PwmInit(FTM0, FTM_CH0, 10000, 0);
	FTM_PwmInit(FTM0, FTM_CH1, 10000, 0);
	FTM_PwmInit(FTM0, FTM_CH2, 10000, 0);
	FTM_PwmInit(FTM0, FTM_CH3, 10000, 0);
}

void STEP_Init(void)
{
  GPIO_PinInit(PTD13, GPI, 0); 
	GPIO_PinInit(PTE27, GPO, 0);          
	GPIO_PinInit(PTE28, GPO, 0);
}

void Show_IQR_handle()
{
  char txt[16]; 
  int16 speed1, speed2;
 // speed = Target_speed;
  speed1 = Left_front_speed;
  speed2 = Right_front_speed;
  //step = step_flag;
  sprintf(txt,"BZY_Flag:%3d ",part_flag);
    OLED_P8x16Str(5,0,(uint8_t*)txt);  

  sprintf(txt,"%1d ",lamp_num[0][0]);
    OLED_P6x8Str(10,2,(uint8_t*)txt);
  sprintf(txt,"%1d ",lamp_num[0][1]);
    OLED_P6x8Str(20,2,(uint8_t*)txt);
  sprintf(txt,"%1d ",lamp_num[0][2]);
    OLED_P6x8Str(30,2,(uint8_t*)txt);
  sprintf(txt,"%1d ",lamp_num[0][3]);
    OLED_P6x8Str(40,2,(uint8_t*)txt);
  sprintf(txt,"%1d ",lamp_num[0][4]);
    OLED_P6x8Str(50,2,(uint8_t*)txt);
  sprintf(txt,"%1d ",lamp_num[0][5]);
    OLED_P6x8Str(60,2,(uint8_t*)txt);
  sprintf(txt,"%1d ",lamp_num[0][6]);
    OLED_P6x8Str(70,2,(uint8_t*)txt);
  sprintf(txt,"%1d ",lamp_num[0][7]);
    OLED_P6x8Str(80,2,(uint8_t*)txt);
  sprintf(txt,"%1d ",lamp_num[0][8]);
    OLED_P6x8Str(90,2,(uint8_t*)txt);
  sprintf(txt,"%1d ",lamp_num[0][9]);
    OLED_P6x8Str(100,2,(uint8_t*)txt);
  sprintf(txt,"%1d ",lamp_num[0][10]);
    OLED_P6x8Str(110,2,(uint8_t*)txt);
  sprintf(txt,"%1d ",lamp_num[0][11]);
    OLED_P6x8Str(120,2,(uint8_t*)txt);

  sprintf(txt,"pos_flag:%3d mm/0.1s",pos_flag);
    OLED_P6x8Str(5,3,(uint8_t*)txt);

  sprintf(txt,"Error:%1d ",error);
    OLED_P6x8Str(5,4,(uint8_t*)txt);
  /*sprintf(txt,"STEP:%1d ",step);
    OLED_P6x8Str(70,4,(uint8_t*)txt);*/
  sprintf(txt,"ENC1:%3d ",speed1);
    OLED_P6x8Str(5,5,(uint8_t*)txt);    
  sprintf(txt,"ENC2:%3d ",speed2);
    OLED_P6x8Str(70,5,(uint8_t*)txt);
  sprintf(txt,"X:%1d ",counter_X);
    OLED_P6x8Str(10,6,(uint8_t*)txt);
  sprintf(txt,"T:%1d ",counter_T);
    OLED_P6x8Str(70,6,(uint8_t*)txt);
 /* sprintf(txt,"Diffe:%1f ",pid_track.result);
    OLED_P6x8Str(50,3,(uint8_t*)txt);*/

/*  sprintf(txt,"Vl_f:%2d ",(int16)pid_left_front.result);
    OLED_P6x8Str(5,4,(uint8_t*)txt);
  sprintf(txt,"Vr_f:%2d ",(int16)pid_right_front.result);
    OLED_P6x8Str(70,4,(uint8_t*)txt);
  sprintf(txt,"Vl_r:%2d ",(int16)Left_rear_speed);
    OLED_P6x8Str(5,5,(uint8_t*)txt);
  sprintf(txt,"Vr_r%2d ",(int16)Right_rear_speed);
    OLED_P6x8Str(70,5,(uint8_t*)txt);*/

}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】位置PID
-------------------------------------------------------------------------------------------------------*/
//函数功能： PID 积分置零
//函数参数： PID参数结构体 
//函数返回值： 无
/*
void reset_I(_pid_param_t * pid_param)
{
    pid_param->_integrator = 0;
	// we use NAN (Not A Number) to indicate that the last 
	// derivative value is not valid
    pid_param->_last_derivative = NAN;
}
//函数功能： PID全部参数置零
//函数参数： PID参数结构体 
//函数返回值： 无
void reset(_pid_param_t * pid_param) 
{
    memset(&pid_param, 0, sizeof(pid_param));
}

//函数功能： 得到PID输出结果  位置式PID
//函数参数： PID参数结构体   误差
//函数返回值： PID输出值
float get_pid(_pid_param_t * pid_param, float error)
{
    uint32_t tnow = systime.get_time_ms();
    uint32_t dt = tnow - pid_param->_last_t;
    float output            = 0;
    float delta_time;
    
//    if (pid_param->_last_t == 0 || dt > 6) {       //限制超时时间6ms 自行修改 
//        dt = 0;
//        
//		// if this PID hasn't been used for a full second then zero
//		// the intergator term. This prevents I buildup from a
//		// previous fight mode from causing a massive return before
//		// the integrator gets a chance to correct itself
//        reset_I(pid_param);
//    }
    pid_param->_last_t = tnow;
    
    delta_time = (float)dt / 1000.0f;
    
    // Compute proportional component
    pid_param->_pid_out_p = error * pid_param->_kp;
    output += pid_param->_pid_out_p;
    
    // Compute derivative component if time has elapsed
    if ((fabsf(pid_param->_kd) > 0) && (dt > 0)) {
        float derivative;
        
		if (isnan(pid_param->_last_derivative)) {
			// we've just done a reset, suppress the first derivative
			// term as we don't want a sudden change in input to cause
			// a large D output change			
			derivative = 0;
			pid_param->_last_derivative = 0;
		} else {
			derivative = (error - pid_param->_last_error) / delta_time;
		}
        
        // discrete low pass filter, cuts out the
        // high frequency noise that can drive the controller crazy
        derivative = pid_param->_last_derivative + ((delta_time / (RC + delta_time)) *(derivative - pid_param->_last_derivative));
        
        // update state
        pid_param->_last_error             = error;
        pid_param->_last_derivative        = derivative;
        
        // add in derivative component
        pid_param->_pid_out_d = pid_param->_kd * derivative;
        output += pid_param->_pid_out_d;
    }
    
    // scale the P and D components
    output *= pid_param->scaler;
    pid_param->_pid_out_d *= pid_param->scaler;
    pid_param->_pid_out_p *= pid_param->scaler;
    
    // Compute integral component if time has elapsed
    if ((fabsf(pid_param->_ki) > 0) && (dt > 0)) {
        pid_param->_integrator             += (error * pid_param->_ki) * pid_param->scaler * delta_time;
        if (pid_param->_integrator < -pid_param->_imax) 
        {
            pid_param->_integrator = -pid_param->_imax;
        } 
        else if (pid_param->_integrator > pid_param->_imax) 
        {
            pid_param->_integrator = pid_param->_imax;
        }
        pid_param->_pid_out_i = pid_param->_integrator;
        output += pid_param->_integrator;
    }
    pid_param->_pid_out = output;
    return output;
}

_pid_s pid =
{
    get_pid,
    reset,
    reset_I,
};
*/