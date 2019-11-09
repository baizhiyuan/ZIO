#include "include.h"
#include "ZIO_Car.h"

float KP = 0; 
float KD = 0;
float P = 100; 
float I = 80;
float D = 80;
float Lspeed_temp_1;
float Rspeed_temp_1;
float Lspeed_temp_2 = 0;
float Rspeed_temp_2 = 0;

int side=1;

int perform=0;
uint8 counter_temp=0;//十字交叉口计数
uint8 counter_X=0;//十字交叉口计数
uint8 counter_T=0;//T字交叉口计数
int lamp[2][10];//前后循迹情况
int lamp_num[2][8];//前后循迹偏差的对应赋值

uint8 key[] =                   { 0x55,0x55,0x05,0x06,0x00,0x01,0x00 };//开机动作
uint8 openbox[] =               { 0x55,0x55,0x05,0x06,0x01,0x01,0x00 };//打开盒子
uint8 smooth[] =                { 0x55,0x55,0x05,0x06,0x02,0x01,0x00 };//平稳运行时
uint8 scan_up[] =               { 0x55,0x55,0x05,0x06,0x03,0x01,0x00 };//扫描上面的
uint8 scan_down[] =             { 0x55,0x55,0x05,0x06,0x04,0x01,0x00 };//扫描下面的
uint8 grab_up_to_front[] =      { 0x55,0x55,0x05,0x06,0x05,0x01,0x00 };//抓上面的到外面的盒子
uint8 grab_up_to_rear[] =       { 0x55,0x55,0x05,0x06,0x06,0x01,0x00 };//抓上面的到里面的盒子
uint8 grab_down_to_front[] =    { 0x55,0x55,0x05,0x06,0x07,0x01,0x00 };//抓下面的到外面的盒子
uint8 grab_down_to_rear[] =     { 0x55,0x55,0x05,0x06,0x08,0x01,0x00 };//抓下面的到里面的盒子
uint8 put_front[] =             { 0x55,0x55,0x05,0x06,0x09,0x01,0x00 };//放外面的物料
uint8 put_rear[] =              { 0x55,0x55,0x05,0x06,0x10,0x01,0x00 };//放里面的物料
uint8 prepare[] =               { 0x55,0x55,0x05,0x06,0x11,0x01,0x00 };//准备扫描——节省时间
uint8 arm[] =                   { 0x55,0x55,0x05,0x06,0x11,0x01,0x00 };//测试


int8 decide=0; //元素判断
int8 pos_flag = 0;//定位标志位
int8 double_flag = 0;//定位标志位
uint8 kong_flag = 1;//循迹出去时的盲走开启
uint8 part_flag = 0;//功能标志位
int transform=0;
int transform_now=0;//达到先亮后灭的情况，表示经过一条黑线
int16 error = 0;//循迹偏差

int16 Right_front_speed;//四轮速度
int16 Left_front_speed;
int16 Right_rear_speed;
int16 Left_rear_speed;


int16 Left_front_goalspeed;//四轮目标速度
int16 Right_front_goalspeed;
int16 Left_rear_goalspeed;
int16 Right_rear_goalspeed;
int16 temp_num = 0;//脉冲数计数
int16 Target_left = 0;//目标脉冲数
int16 Target_right = 0;//目标脉冲数
int16 temp_right=0;
int16 temp_left=0;
int16 Target_num = 0;//目标脉冲数
int16 Target_speed = 0;//目标速度

_pid_t pid_left_front;    //左轮pid结构体
_pid_t pid_right_front;		//右轮pid结构体
_pid_t pid_track;				  //循迹pid结构体
_pid_t pid_rotate;				  //循迹pid结构体

volatile uint8_t step_flag = 0;   //步进电机标志位
volatile short ABGet_left, ABGet_right;   //脉冲数记录

void GCXL(void)
{
  Sys_init_all();
  UART_PutBuff(UART3, key, 7);
  start();
  UART_PutBuff(UART3, openbox, 7);
  UART_PutStr (UART1, "#000P1510T0000!");
  while(1)
  {
    PART_all();
    if(perform == 1) Part_Choose();
    track_PID();counter_sum();
    //Obstacle();
    //rotating();
  }
}

void Sys_init_all(void)
{
  Motor_Init();
  Infrared_Init();
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

void start(void)
{ 
  uint8 z=1;
  while (z)
  {  
    switch(KEY_Read(0))     
    {
      case 1:LED_Reverse(1);Target_speed =60;z = 0;break;  
      case 2:step_flag = 1;break; 
      case 3:step_flag = 3;break; 
      case 4:LED_Reverse(1);Target_speed =60;z = 0;break;  
      default: break; 
    }delayms(100);
  }
}

void Uart_Init(void)
{
    UART_Init(UART4, 115200); 
    UART_Init(UART3, 9600);
    UART_Init(UART1, 115200);      
    NVIC_SetPriority(UART3_RX_TX_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    NVIC_SetPriority(UART1_RX_TX_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    //NVIC_EnableIRQ(UART3_RX_TX_IRQn);			          //使能UART4_RX_TX_IRQn的中断
    //UART_PutBuff(UART3, arm, 7);
}

void Timer_Init(void)
{
    PIT_Init(PIT0, 5);		//速度环更新
    PIT_Step(PIT1, 300);  //300us，2200*0.3=660ms
   // PIT_Init(PIT2, 200);	//显示屏刷新	
    NVIC_SetPriority(PIT0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),2,3));
    NVIC_SetPriority(PIT1_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));					
    //NVIC_SetPriority(PIT2_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3,4));
    NVIC_EnableIRQ(PIT0_IRQn);			          //使能PIT0_IRQn的中断	
    NVIC_EnableIRQ(PIT1_IRQn);	                  //使能中断函数在IRQ_Handle内
   // NVIC_EnableIRQ(PIT2_IRQn);		 
    EnableInterrupts;         //开总中断
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
    temp_num = temp_num + ABGet_left;
    temp_num = temp_num + ABGet_right;
    if(Target_num < temp_num){
      temp_num = 0;pos_flag = 0;}
  }else if(pos_flag == 2){
    temp_num = temp_num + ABGet_left;
    temp_num = temp_num + ABGet_right;
    if(Target_num > temp_num){
      temp_num = 0;pos_flag = 0;}
  }
  if(double_flag == 1){
    temp_left = temp_left + ABGet_left;
    temp_right = temp_right + ABGet_right;
    if(temp_left > Target_left || temp_right > Target_right){
      temp_left=0;temp_right=0;double_flag=0;
    }
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

void read_lamp_values(void)  //根据传感器计算出偏移量error
{
  lamp[0][0] = GPIO_PinRead(PTB0);
	lamp[0][1] = GPIO_PinRead(PTB1);
	lamp[0][2] = GPIO_PinRead(PTB2);
	lamp[0][3] = GPIO_PinRead(PTB3);
	lamp[0][4] = GPIO_PinRead(PTB4);
	lamp[0][5] = GPIO_PinRead(PTB5);
	lamp[0][6] = GPIO_PinRead(PTB6);
	lamp[0][7] = GPIO_PinRead(PTB7);

  int error_current = 0;   //当前偏差
  uint8 lamp_Lsum = 0;    //左半边灯循迹到黑线的数量
  uint8 lamp_Rsum = 0;
  for(int j = 0; j < 8; j ++){     //对不同的灯判断到黑线进行相应的赋值
    if(lamp[0][j] == 1){
      //lamp_num[0][j] = 2*j - 7;            //  -7 -5 -3 -1  1  3  5  7   
      if(j==0) lamp_num[0][j] = -13;
      else if(j==1) lamp_num[0][j] = - 8;
      else if(j==2) lamp_num[0][j] = - 3;
      else if(j==3) lamp_num[0][j] = - 1;
      else if(j==4) lamp_num[0][j] = + 1;
      else if(j==5) lamp_num[0][j] = + 3;
      else if(j==6) lamp_num[0][j] = + 8;
      else if(j==7) lamp_num[0][j] = + 13;

      if(j < 4)lamp_Lsum ++;
      else lamp_Rsum ++;
    }else lamp_num[0][j] = 0;
    error_current += lamp_num[0][j];//前循迹偏差值
  }
  if (lamp_Rsum > 2){
    decide = 1;
    if(lamp[0][5]==1 && lamp[0][6] != 1) error = -12;
    else if (lamp[0][4]==1 && lamp[0][5] != 1) error = 0;
    else error = error/2;
  }else if ((lamp_Lsum == 0)&&(lamp_Rsum == 0)) {
    if(part_flag == 26) part_flag =4;
    error = 0;
  }else error = error_current;
}




void PART(void)
{  
    if(counter_X == 0 && counter_T == 0) part_flag=1;
    else if(counter_X == 1 && counter_T == 0) part_flag=1;
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
    else if(counter_X == 4) {PIT_Delayms(PIT3, 500);;Target_speed = 0;}
    else {part_flag=0;}
    //perform = 1;
}

void PART_all(void)
{  
    if(counter_temp == 0) part_flag=1;
    else if(counter_temp == 1) part_flag=1;
    else if(counter_temp == 2) part_flag=2;
    else if(counter_temp == 3) part_flag=11;
    else if(counter_temp == 4) part_flag=12;
    else if(counter_temp == 5) part_flag=13;
    else if(counter_temp == 6) part_flag=3;
    else if(counter_temp == 7) part_flag=21;
    else if(counter_temp == 8) part_flag=22;
    else if(counter_temp == 9) part_flag=23;
    else if(counter_temp == 10) part_flag=24;
    else if(counter_temp == 11) part_flag=25;
    else if(counter_temp == 12) part_flag=26;
    else if(counter_temp == 13) {part_flag = 4;Target_speed = 0;}
    else part_flag=0;
}

void Part_Choose(void)
{
    switch(part_flag){
        case  1:Target_speed = 70;break; 
        case  2:Target_speed = 120;UART_PutStr(UART1, "#000P1501T0000!");Pos_flag();break;
        case 11:Target_speed = 30; UART_PutStr(UART1, "#000P1503T0000!");Positioning();UART_PutBuff(UART3, grab_up_to_rear, 7);step_flag = 2;PIT_Delayms(PIT3, 5000);Target_speed = 20;break;
        case 12:Target_speed = 20; UART_PutStr(UART1, "#000P1503T0000!");Positioning();UART_PutBuff(UART3, grab_up_to_front, 7);step_flag = 1;PIT_Delayms(PIT3, 5000);Target_speed = 20;break;
        case 13:Target_speed = 20; UART_PutStr(UART1, "#000P1503T0000!");Positioning();UART_PutBuff(UART3, grab_down_to_front, 7);step_flag = 3;PIT_Delayms(PIT3, 5000);Target_speed = 50;Pos_flag_2();UART_PutBuff(UART3, smooth, 7);step_flag = 2;break;
        case  3:Target_speed = 60; UART_PutStr(UART1, "#000P1502T0000!");Obstacle();rotating();Target_speed = 30;break;
        //Target_speed = 60;Obstacle();rotating();Target_speed = 50;break;
               //Obstacle();break;//避障区域
               // Target_speed = 100;track_PID ();counter_sum();break;    
        case 21:Target_speed = 30; UART_PutStr(UART1, "#000P1507T0000!");Positioning();UART_PutBuff(UART3, put_front, 7);step_flag = 1;PIT_Delayms(PIT3, 5000);Target_speed = 30;break;
        case 22:Target_speed = 30; UART_PutStr(UART1, "#000P1507T0000!");Positioning();UART_PutBuff(UART3, put_rear, 7);step_flag = 2;PIT_Delayms(PIT3, 5000);Target_speed = 30;break;
        case 23:Target_speed = 30; UART_PutStr(UART1, "#000P1507T0000!");Positioning();PIT_Delayms(PIT3, 2000);Target_speed = 30;break;
        case 24:Target_speed = 30; UART_PutStr(UART1, "#000P1507T0000!");Positioning();PIT_Delayms(PIT3, 2000);Target_speed = 30;break;
        case 25:Target_speed = 30; UART_PutStr(UART1, "#000P1507T0000!");Positioning();PIT_Delayms(PIT3, 2000);Target_speed = 30;break;
        case 26:Target_speed = 30; UART_PutStr(UART1, "#000P1507T0000!");Positioning();UART_PutBuff(UART3, put_front, 7);step_flag = 3;PIT_Delayms(PIT3, 5000);Target_speed = 50;UART_PutBuff(UART3, smooth, 7);break;
        case  4:Target_speed = 50; UART_PutStr(UART1, "#000P1509T0000!");PIT_Delayms(PIT3, 500);Target_speed = 0;break;
        default: Target_speed = 0;break;
    }
    perform = 0;
/*
    if(Target_speed < 40) {
      P = 80;
      I = 60;
      D = 60;
    }else {
      P =100;
      I = 80;
      D = 80 ;
    }*/
}

void Positioning(void)
{
  uint8 a=1;
  Target_num = 10;
  pos_flag = 1;
  while (a)
  {
    track_PID();
    if(pos_flag != 1){
      a=0;
      Target_speed = 0;
      break;
    }
    //lamp_center = GPIO_PinRead(PTC10);
  }
}

void Pos_flag(void)
{
  uint8 a=1;
  Target_num = 800;
  pos_flag = 1;
  while (a)
  {
    track_PID();
    if(pos_flag != 1){
      a=0;
      Target_speed = 30;
      break;
    }
    //lamp_center = GPIO_PinRead(PTC10);
  }
}

void Pos_flag_2(void)
{
  uint8 a=1;
  Target_num = 700;
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

void Pos_flag_3(void)
{
  uint8 a=1;
  uint8 b=0;
  uint8 c=0;
  Target_num = 800;
  pos_flag = 1;
  while (a)
  {
    read_lamp_values();
    if(b==2) error = -error;
    calc_pid(&pid_track);
    diff_speed();
    if(pos_flag == 0 && b == 0){
      Target_speed = 50;
      b=1;
    }else if(pos_flag == 0 && (b == 2 || b ==1)){
      lamp[0][8] = GPIO_PinRead(PTD13);
      if(lamp[0][8] == 0){
        c=0;
      }else if(lamp[0][8] == 1 && c == 0){
        b++;
        c=1;
        if(b==3){
          Target_speed = -30;
          Target_num = -400;
          pos_flag = 2;
        }    
      }
    }else if(pos_flag == 0 && b == 3){
      Target_speed = 30;
      a=0;
      break;
    }
  }
}

void counter_sum(void){

  lamp[0][8] = GPIO_PinRead(PTD13);
  lamp[0][9] = GPIO_PinRead(PTE9);
  if(lamp[0][8] == 1 && lamp[0][9] == 1){
    if(side==0){
      counter_temp++;
      perform = 1;
    }side =1;
  }else if(lamp[0][8] == 0 && lamp[0][9] == 0) side =0;
}

void Obstacle(void){

  uint8 a=1;
  IIC_Init();                    //初始化IIC的IO口 
  uint16_t dis = 0;
  uint16_t Distance[10]  = {0};
  uint16_t Distance1 = 0;
  u8 i =0;
  u8 j =0;
  u8  measure_num = 8; //需要测量的次数
  uint16_t MinDistance = 3000;
  //===注意，浮点计算稍微费时间，因此处理都已化为整型数处理（单位MM）===//
  //采集8个点
  while(a)
  {
    ADC_Init(ADC1);
    for(i = 0 ; i < measure_num ; i++)
    {
      dis = (uint16_t)(ADC_Get(10)*0.806);          //计算距离dis
      Distance[i]=dis;
      PIT_Delayms(PIT3, 1);
    }
    dis = 0;
    //MAG_Z = 0;
    for(i = 0 ; i < measure_num-1 ; i++)
    {
      for(j = 0 ; j < measure_num-1-i; j++)       
      {
        if(Distance[j] > Distance[j+1] )
        {
          Distance1 = Distance[j];
          Distance[j] =  Distance[j+1];
          Distance[j+1] = Distance1; 
        }
      }
    }
    for(i = 1 ; i < measure_num ; i++)
    {
      dis += Distance[i];
    }
    dis /= (measure_num -2);//取平均
    MinDistance = dis;//最终值

    if(MinDistance < 2200){
    a=0;
    break;
    }else track_PID();
  }
}

void turn_left(void){
  error = -50;
  calc_pid(&pid_track);
  diff_speed();
}
void turn_right(void){
  error = 50;
  calc_pid(&pid_track);
  diff_speed();
}
void run_straight(void){
  error = 0;
  calc_pid(&pid_track);
  diff_speed();
}


void rotating(void){

  uint8 a=1;
  uint8 c=1;
  Target_left=100;
  Target_right=200;
  double_flag = 1;
  while (a){

    if(c==1){   //左转
      if(double_flag != 1)
      {
        Target_left=250;
        Target_right=250;
        double_flag = 1;
        c = 6;
      }else turn_left();
    }else if(c==6){//右转后直行
      if(double_flag != 1)
      {
        Target_left=200;
        Target_right=100;
        double_flag = 1;      
        c = 2;
      }else run_straight();
    }else if(c==2){//右转后直行
      if(double_flag != 1)
      {
        Target_left=300;
        Target_right=300;
        double_flag = 1;
        c = 3;
      }else turn_right();
    }else if(c==3){//右转
      if(double_flag != 1)
      {
        Target_left=200;
        Target_right=100;
        double_flag = 1;
        c = 4;
      }else run_straight();
    }else if(c== 4){
      if(double_flag != 1)
      {
        //Target_left=300;
        //Target_right=300;
        //double_flag = 1;
        c = 5;
      }else turn_right();
    }else if(c== 5){

      lamp[0][8] = GPIO_PinRead(PTD13);
      if(lamp[0][8] == 1)
      {
      //if(double_flag != 1)
     // {
        Target_left=100;
        Target_right=200;
        double_flag = 1;
        c = 7;
      }else run_straight(); 
    }else if(c==7){//右转后直行
      if(double_flag != 1)
      {
        a=0;
        //Target_speed = 0;
        break;
      }else turn_left(); 
    }
  }
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

  pid_track.kp=P;
	pid_track.ki=I;
	pid_track.kd=D;
	
	pid_track.err=0;
	pid_track.last_err=0;
	pid_track.last_last_err=0;
	pid_track.result=0;

  pid_rotate.kp=40;
	pid_rotate.ki=30;
	pid_rotate.kd=30;
	
	pid_rotate.err=0;
	pid_rotate.last_err=0;
	pid_rotate.last_last_err=0;
	pid_rotate.result=0;
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

void rotate_pid(_pid_t *pid, int16 actual, int16 set)//得到速差
{
  int16 error=(actual - set);
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

void rotate_PID(int16_t MAG_Tar)
{
  //rotate_pid(&pid_rotate, MAG_Z, MAG_Tar);
  Left_front_goalspeed = Target_speed * (1 +0.001 * pid_rotate.result); Right_front_goalspeed = Target_speed * (1 - 0.001 * pid_rotate.result);
  Left_rear_goalspeed = Target_speed * (1 + 0.001 * pid_rotate.result); Right_rear_goalspeed = Target_speed * (1 - 0.001 * pid_rotate.result);
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
  GPIO_PinInit(PTB0, GPI, 0);
  GPIO_PinInit(PTB1, GPI, 0);
  GPIO_PinInit(PTB2, GPI, 0);
  GPIO_PinInit(PTB3, GPI, 0);
  GPIO_PinInit(PTB4, GPI, 0);
  GPIO_PinInit(PTB5, GPI, 0);
  GPIO_PinInit(PTB6, GPI, 0);
  GPIO_PinInit(PTB7, GPI, 0);
    
  GPIO_PinInit(PTD13, GPI, 0);
  GPIO_PinInit(PTE9, GPI, 0);
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
  GPIO_PinInit(PTC10, GPO, 0);
	FTM_PwmInit(FTM0, FTM_CH0, 10000, 0);
	FTM_PwmInit(FTM0, FTM_CH1, 10000, 0);
	FTM_PwmInit(FTM0, FTM_CH2, 10000, 0);
	FTM_PwmInit(FTM0, FTM_CH3, 10000, 0);
}

void STEP_Init(void)
{
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

  sprintf(txt,"Error:%1d ",error);
    OLED_P6x8Str(5,4,(uint8_t*)txt);
  /*sprintf(txt,"STEP:%1d ",step);
    OLED_P6x8Str(70,4,(uint8_t*)txt);*/
  sprintf(txt,"ENC1:%3d ",speed1);
    OLED_P6x8Str(5,5,(uint8_t*)txt);    
  sprintf(txt,"ENC2:%3d ",speed2);
    OLED_P6x8Str(70,5,(uint8_t*)txt);

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
/*
void rotating(void){

  uint8 a=1;
  uint8 b=1;
  uint8 c=1;
  while (a){
    rota_filter();
    //rotate_PID();
    if(c==1){   //左转
      if(MAG_Z < MAG_target)
      turn_left();
      else{
        Target_num = 1000;
        pos_flag = 1;
        c = 2;
        rota_filter();
        if(MAG_Z>0)
        {
          MAG_target = MAG_Z+70;
          if(MAG_target>250) MAG_target=MAG_target-500;
          MAG_target_2 = MAG_Z-70;  
        }
        else{
          MAG_target = MAG_Z+70;
          MAG_target_2 = MAG_Z-70;
          if(MAG_target_2 < -250)  MAG_target_2=-MAG_target_2+500;
        }
        MAG_target_3 = MAG_Z;
      }
    }else if(pos_flag != 1 && c==2){//右转后直行
      if(MAG_Z > MAG_target_2)
        turn_right();
      else {
        Target_num = 1000;
        pos_flag = 1;
        c = 3;
        rota_filter();
        if(MAG_Z>0)
        {
          MAG_target = MAG_Z+70;
          if(MAG_target>250) MAG_target=MAG_target-500;
          MAG_target_2 = MAG_Z-70;  
        }
        else{
          MAG_target = MAG_Z+70;
          MAG_target_2 = MAG_Z-70;
          if(MAG_target_2 < -250)  MAG_target_2=-MAG_target_2+500;
        }
        MAG_target_3 = MAG_Z;
      } 
    }else if(pos_flag != 1 && c==3){//右转后直行
      if(MAG_Z > MAG_target_2)
        turn_right();
      else {
        Target_num = 1000;
        pos_flag = 1;
        c = 0;}
    }else if(pos_flag != 1 && c== 0){
      a=0;
      //Target_speed = 0;
      break;
    }
  }
}
*/
    //if(transform == 1) decide = 1;//十字路口  1 1 1 1 1 1     直行
   // error = lamp_num[0][0]+lamp_num[0][1]+lamp_num[0][2]+lamp_num[0][3]+lamp_num[0][4];
 /* }else if ((lamp_Lsum < 2) && (lamp_Rsum > 2)) {
    if(transform == 1) decide = 2;//定位点
    error = 0;*/
 /* if(part_flag != 1 || part_flag != 2 || part_flag != 3 ){
    error= lamp_num[0][3]+ lamp_num[0][4];
  }*/
  /*  if(kong_flag == 0){
      if ((error > 0)&&(error < 30)) error = 35;//  0 0 0 0 0前面循迹迷失，以上次偏差情况进行反向补偿
      else if((error < 0)&&(error > -30)) error = -35; 
    }else error = 0;*/
 // if (lamp_Lsum+lamp_Rsum == 2) transform = 1;//这一次不在交叉线，在普通白线

/*
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
        if(decide == 1 || decide == 3){
          counter_X++;
          transform = 0;
          decide = 0;
          perform = 1;}
      }
  }
}
*/