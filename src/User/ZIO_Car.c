#include "include.h"
#include "ZIO_Car.h"

float KP = 0; 
float KD = 0;

float P = 95; 
float I = 125;//110;//88.25;
float D = 80;

float VP = 90; 
float VI = 5;
float VD = 17;
/*
三角形=0；
正方形=1
六边形=2；
圆 = 3；
N字=4；
五角星=5
5 3 1
4 2 0
5 4 3 2 1 0*/
int const supplies_bi[3][4]= {
  {0, 5, 0, 3},             //第一个数：物料所在位置；第二个数：物料下料区位置，第三个数：物料形状，第四个数：物料盒位置
  {3, 3, 1, 1},
  {2, 4, 3 ,2},
};

uint64_t Lspeed_temp_1;
uint64_t Rspeed_temp_1;
uint64_t Lspeed_temp_2 = 0;
uint64_t Rspeed_temp_2 = 0;

int shangpo = 1;
int shang = 0;
int side=1;
int perform=1;
uint8 counter_temp=1;//十字交叉口计数

int lamp[3][12];//前后循迹情况
int lamp_num[3][12];//前后循迹偏差的对应赋值
/////////////////////////////////////////////////////////////////////////////////////////////
uint8 visual[] =                   { 0x55,0x55,0x54,0x50 };//拍照
/////////////////////////////////////////////////////////////////////////////////////////////
uint8_t key[] =                   { 0x55,0x55,0x05,0x06,0x00,0x01,0x00 };//开机动作
uint8_t openbox[] =               { 0x55,0x55,0x05,0x06,0x01,0x01,0x00 };//打开盒子
uint8_t smooth[] =                { 0x55,0x55,0x05,0x06,0x02,0x01,0x00 };//平稳运行时
/////////////////////////////////////////////////////////////////////////////////////////////
uint8_t prepare[] =               { 0x55,0x55,0x05,0x06,0x03,0x01,0x00 };//准备扫描——节省时间
uint8_t arm[] =                   { 0x55,0x55,0x05,0x06,0x14,0x01,0x00 };//测试
uint8_t scan_up[] =               { 0x55,0x55,0x05,0x06,0x05,0x01,0x00 };//扫描上面的
uint8_t scan_down[] =             { 0x55,0x55,0x05,0x06,0x06,0x01,0x00 };//扫描下面的
/////////////////////////////////////////////////////////////////////////////////////////////
uint8_t grab_up_to_front[] =      { 0x55,0x55,0x05,0x06,0x07,0x01,0x00 };//抓上面的到外面的盒子
uint8_t grab_up_to_rear[] =       { 0x55,0x55,0x05,0x06,0x08,0x01,0x00 };//抓上面的到里面的盒子
/////////////////////////////////////////////////////////////////////////////////////////////
uint8_t grab_down_to_front[] =    { 0x55,0x55,0x05,0x06,0x09,0x01,0x00 };//抓下面的到外面的盒子
uint8_t grab_down_to_rear[] =     { 0x55,0x55,0x05,0x06,0x32,0x01,0x00 };//抓下面的到里面的盒子
/////////////////////////////////////////////////////////////////////////////////////////////
//uint8 grab_triangle_to_rear[] =       { 0x55,0x55,0x05,0x06,0x13,0x01,0x00 };//抓近放，小三角形
//uint8 put_triangle_to_rear[] =       { 0x55,0x55,0x05,0x06,0x14,0x01,0x00 };//抓远放，大小三角形
/////////////////////////////////////////////////////////////////////////////////////////////
//uint8 grab_down_to_rear_small[] =     { 0x55,0x55,0x05,0x06,0x26,0x01,0x00 };//抓下面的到里面的盒子         其它小物料
//uint8 grab_down_to_rear_bigzheng[] =     { 0x55,0x55,0x05,0x06,0x31,0x01,0x00 };//抓下面的到里面的盒子      大正方形
/////////////////////////////////////////////////////////////////////////////////////////////
uint8_t scan_blanking[] =         { 0x55,0x55,0x05,0x06,0x18,0x01,0x00 };//扫描下料区
/////////////////////////////////////////////////////////////////////////////////////////////
uint8_t put_front[] =             { 0x55,0x55,0x05,0x06,0x04,0x01,0x00 };//放外面的物料
uint8_t put_rear[] =              { 0x55,0x55,0x05,0x06,0x03,0x01,0x00 };//放里面的物料
/////////////////////////////////////////////////////////////////////////////////////////////
//uint8 put_rear_bigzheng[] =              { 0x55,0x55,0x05,0x06,0x32,0x01,0x00 };//放里面的物料             大正方形
//uint8 put_rear_bigsan[] =              { 0x55,0x55,0x05,0x06,0x33,0x01,0x00 };//放里面的物料               大三角形  
//uint8 put_rear_bigwujiaoxin[] =              { 0x55,0x55,0x05,0x06,0x34,0x01,0x00 };//放里面的物料         五角星
//uint8 put_rear_smallNliu[] =              { 0x55,0x55,0x05,0x06,0x35,0x01,0x00 };//放里面的物料            小N小六边形
//uint8 put_rear_smallsan[] =              { 0x55,0x55,0x05,0x06,0x36,0x01,0x00 };//放里面的物料             小三角形
//uint8 put_rear_zhengyuanwu[] =              { 0x55,0x55,0x05,0x06,0x37,0x01,0x00 };//放里面的物料          小正方形/小圆形/小五角星
//uint8 put_front_bigsmallsan[] =             { 0x55,0x55,0x05,0x06,0x38,0x01,0x00 };//放外面的物料          大三角形/小三角形
/////////////////////////////////////////////////////////////////////////////////////////////
//uint8 put_front_bigN[] =             { 0x55,0x55,0x05,0x06,0x39,0x01,0x00 };//放外面的物料                 大N
//uint8 put_front_smallliu[] =             { 0x55,0x55,0x05,0x06,0x40,0x01,0x00 };//放外面的物料             小六边形
//uint8 put_front_bigwu[] =             { 0x55,0x55,0x05,0x06,0x41,0x01,0x00 };//放外面的物料                大五角星
//uint8 put_front_smallwu[] =             { 0x55,0x55,0x05,0x06,0x42,0x01,0x00 };//放外面的物料              小五角星
//uint8 put_front_smallyuanN[] =             { 0x55,0x55,0x05,0x06,0x43,0x01,0x00 };//放外面的物料           小圆小N
/////////////////////////////////////////////////////////////////////////////////////////////
uint8_t wd_wl[] = { 0x55,0x55,0x05,0x06,0x15,0x01,0x00 };//稳定物块
////////////////////
int decide=0; //元素判断（直线or转弯）
volatile uint8_t pos_flag = 0;//定位标志位
volatile uint8_t double_flag = 0;//单轮运转标志位（累计脉冲数）
int kong_flag = 1;//循迹出去时的盲走开启
int part_flag = 0;//功能标志位
int transform=0;
int transform_now=0;//达到先亮后灭的情况，表示经过一条黑线
float error = 0;//循迹偏差

int16 Right_front_speed;//四轮速度
int16 Left_front_speed;
int16 Right_rear_speed;
int16 Left_rear_speed;

int16 Left_front_goalspeed;//四轮目标速度
int16 Right_front_goalspeed;
int16 Left_rear_goalspeed;
int16 Right_rear_goalspeed;

static volatile int temp_num = 0;//脉冲数计数
uint64_t Target_left = 0;//目标脉冲数
uint64_t Target_right = 0;//目标脉冲数
static volatile uint64_t temp_right=0;
static volatile uint64_t temp_left=0;
int Target_num = 0;//目标脉冲数

volatile int Target_speed = 0;//目标速度

_pid_t pid_left_front;    //左轮pid结构体
_pid_t pid_right_front;		//右轮pid结构体
_pid_t pid_track;				  //循迹pid结构体
_pid_t pid_rotate;				  //循迹pid结构体

int8 dingwei_flag = 0;//定位标志位
int8 rear_flag = 1;//后轮标志位
int8 wan_flag = 0;//标志位
volatile int step_flag = 0;   //步进电机标志位
volatile uint8_t pit0_flag;
volatile uint8_t pit2_flag;

int positing_flag = 0;
int  wandao_flag = 1;
int once = 0;
int xialiao_flag = 0;
int heixian_flag = 0;
//extern _pid_param_t  track_bzy;20 0 8    12   0.3   6
_pid_param_t  track_bzy =      //循迹PID
{
     ._kp             =  20.0 ,         //P
     ._ki             =  0.0 ,         //I
     ._kd             =  8.0 ,         //D
     ._imax           =  3000 ,         //积分限幅
                                       
     ._pid_out_p      =   0,           //KP输出
     ._pid_out_i      =   0,           //KI输出
     ._pid_out_d      =   0,           //KD输出
     ._pid_out        =   0,           //pid输出
     
     ._integrator     =   0,           //积分值
     ._last_error     =   0,           //上一次误差
     ._last_derivative=   0,           //上次微分
     ._last_t         =   0,           //上次时间
     .scaler          =   10,         //缩放比
};
_pid_param_t  track_wan =      //转弯PID
{
     ._kp             =  9.0 ,         //P
     ._ki             =  0.42 ,         //I
     ._kd             =  8.0 ,         //D
     ._imax           =  1000 ,         //积分限幅
                                       
     ._pid_out_p      =   0,           //KP输出
     ._pid_out_i      =   0,           //KI输出
     ._pid_out_d      =   0,           //KD输出
     ._pid_out        =   0,           //pid输出
     
     ._integrator     =   0,           //积分值
     ._last_error     =   0,           //上一次误差
     ._last_derivative=   0,           //上次微分
     ._last_t         =   0,           //上次时间
     .scaler          =   10,         //缩放比
};
_pid_param_t  track_Obstacle =      //避障PID
{
     ._kp             =  9.0 ,         //P
     ._ki             =  0.0 ,         //I
     ._kd             =  0.0 ,         //D
     ._imax           =  3000 ,         //积分限幅
                                       
     ._pid_out_p      =   0,           //KP输出
     ._pid_out_i      =   0,           //KI输出
     ._pid_out_d      =   0,           //KD输出
     ._pid_out        =   0,           //pid输出
     
     ._integrator     =   0,           //积分值
     ._last_error     =   0,           //上一次误差
     ._last_derivative=   0,           //上次微分
     ._last_t         =   0,           //上次时间
     .scaler          =   10,         //缩放比
};
_pid_param_t  track_chasu =      //差速PID
{
     ._kp             =  4.0 ,         //P
     ._ki             =  0.02 ,         //I
     ._kd             =  0.4 ,         //D
     ._imax           =  1000 ,         //积分限幅
                                       
     ._pid_out_p      =   0,           //KP输出
     ._pid_out_i      =   0,           //KI输出
     ._pid_out_d      =   0,           //KD输出
     ._pid_out        =   0,           //pid输出
     
     ._integrator     =   0,           //积分值
     ._last_error     =   0,           //上一次误差
     ._last_derivative=   0,           //上次微分
     ._last_t         =   0,           //上次时间
     .scaler          =   1,         //缩放比
};
_pid_param_t  track_Rote =      //旋转PID
{
     ._kp             =  2.0 ,         //P
     ._ki             =  0.0 ,         //I
     ._kd             =  0.0 ,         //D
     ._imax           =  3000 ,         //积分限幅
                                       
     ._pid_out_p      =   0,           //KP输出
     ._pid_out_i      =   0,           //KI输出
     ._pid_out_d      =   0,           //KD输出
     ._pid_out        =   0,           //pid输出
     
     ._integrator     =   0,           //积分值
     ._last_error     =   0,           //上一次误差
     ._last_derivative=   0,           //上次微分
     ._last_t         =   0,           //上次时间
     .scaler          =   10,         //缩放比
};
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】主程序
-------------------------------------------------------------------------------------------------------*/
void GCXL(void)
{
  Sys_init_all();
  UART_PutBuff(UART3, key, 7);
  start();
  UART_PutStr (UART1, "#000P1510T0000!");//开机播报
  while(1)
  {
    PART_all();
    if(perform == 1) Part_Choose();
    track_PID();counter_sum();
    //Obstacle();
    //rotating();
  }
}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】初始化，在最后面
-------------------------------------------------------------------------------------------------------*/
void Sys_init_all(void)
{
  Motor_Init();   //电机初始化
  Infrared_Init();//循迹灯
  STEP_Init();//步进电机
  Uart_Init();//串口通讯
  LED_Init();
  KEY_Init();   
  FTM_ABInit(FTM1);
  FTM_ABInit(FTM2);
  Pid_Init();//速度环的四轮pid参数，实际只用了前轮，后轮依照其特性公式改变速度
  Timer_Init(); //定时刷新速度环，PIT0，5ms
}

void start(void)
{ 
  GPIO_PinInit(PTD0, GPI_DOWN, 0);
  uint8 z=1;
  while (z)
  {  
    switch(KEY_Read(0))     
    {
      case 1:LED_Reverse(1);Target_speed =50;z = 0;break;  //开机键
      case 2:UART_PutBuff(UART2, visual, 4);break; //向树莓派发送消息
      case 3:UART_PutBuff(UART3, grab_down_to_rear, 7);break;//测试机械臂动作组
      default: break; 
    }delayms(100);
    if(GPIO_PinRead(PTD0)==1){//一键开关
      delayms(1000);
      if(GPIO_PinRead(PTD0)==0){
        LED_Reverse(2);Target_speed =50;z = 0;break;
      }
    }
  }
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
  lamp[0][8] = GPIO_PinRead(PTC13);
	lamp[0][9] = GPIO_PinRead(PTC14);
	lamp[0][10] = GPIO_PinRead(PTC15);
	lamp[0][11] = GPIO_PinRead(PTC16);

  lamp[1][0] = GPIO_PinRead(PTE7);
	lamp[1][1] = GPIO_PinRead(PTE8);
	lamp[1][2] = GPIO_PinRead(PTD14);
	lamp[1][3] = GPIO_PinRead(PTD15);

  float error_current = 0;   //当前偏差
  float error_back = 0;
  uint8 lamp_Lsum = 0;    //左半边灯循迹到黑线的数量
  uint8 lamp_Rsum = 0;

  for(int j = 0; j < 12; j ++){     //对不同的灯判断到黑线进行相应的赋值
    if(lamp[0][j] == 1){
     // lamp_num[0][j] = 2*j - 11;            // -11 -9 -7 -5 -3 -1  1  3  5  7  9  11
      if(xialiao_flag == 0){//在普通直线
        if(j == 0) lamp_num[0][j] = -11;
        else if(j == 1) lamp_num[0][j] = -9;  
        else if(j == 2) lamp_num[0][j] = -7;    
        else if(j == 3) lamp_num[0][j] = -5;           
        else if(j == 4) lamp_num[0][j] = -3;    
        else if(j == 5) lamp_num[0][j] = -1;    
        else if(j == 6) lamp_num[0][j] =  1;    
        else if(j == 7) lamp_num[0][j] =  3;    
        else if(j == 8) lamp_num[0][j] =  5;    
        else if(j == 9) lamp_num[0][j] =  7;    
        else if(j == 10) lamp_num[0][j] = 9;    
        else if(j == 11) lamp_num[0][j] = 11; 
      }else if(xialiao_flag == 2){//上坡时因为中间是空的
        if(j == 0) lamp_num[0][j] = -7;
        else if(j == 1) lamp_num[0][j] = -5;  
        else if(j == 2) lamp_num[0][j] = -3;    
        else if(j == 3) lamp_num[0][j] = -1;           
        else if(j == 4) lamp_num[0][j] =  0;    
        else if(j == 5) lamp_num[0][j] =  0;    
        else if(j == 6) lamp_num[0][j] =  0;    
        else if(j == 7) lamp_num[0][j] =  0;    
        else if(j == 8) lamp_num[0][j] =  1;    
        else if(j == 9) lamp_num[0][j] =  3;    
        else if(j == 10) lamp_num[0][j] = 5;    
        else if(j == 11) lamp_num[0][j] = 7;    
        //lamp_num[0][j] = -lamp_num[0][j];
      }else if(xialiao_flag == 3){
        if(j == 0) lamp_num[0][j] = -7;
        else if(j == 1) lamp_num[0][j] = -5;  
        else if(j == 2) lamp_num[0][j] = -3;    
        else if(j == 3) lamp_num[0][j] = -1;           
        else if(j == 4) lamp_num[0][j] =  0;    
        else if(j == 5) lamp_num[0][j] =  0;    
        else if(j == 6) lamp_num[0][j] =  0;    
        else if(j == 7) lamp_num[0][j] =  0;    
        else if(j == 8) lamp_num[0][j] =  1;    
        else if(j == 9) lamp_num[0][j] =  3;    
        else if(j == 10) lamp_num[0][j] = 5;    
        else if(j == 11) lamp_num[0][j] = 7;    
        lamp_num[0][j] = -lamp_num[0][j];
      }
      if(j < 6)lamp_Lsum ++;//左右亮的灯的数量
      else lamp_Rsum ++;
    }else lamp_num[0][j] = 0;
    error_current += lamp_num[0][j];//前循迹偏差值
  }
  if (positing_flag == 1)  {
    for(int i = 0; i < 4; i ++){     //对不同的灯判断到黑线进行相应的赋值
      if(lamp[1][i] == 1){
        if(i == 0) lamp_num[1][i] = -2;
        else if(i == 1) lamp_num[1][i] = -1;  
        else if(i == 2) lamp_num[1][i] =  1;    
        else if(i == 3) lamp_num[1][i] =  2;                 //   -3 -1  1  3    
      }else lamp_num[1][i] = 0;
      error_back += lamp_num[1][i];//前循迹偏差值
    }
    if(lamp_num[1][0] == 1&&lamp_num[1][1] == 1&&lamp_num[1][2] == 1) error_back == 0;
    error_current = error_current - error_back;
  }
  /*
  if(wan_flag == 1){
     if((lamp[0][0] == 1 || lamp[0][2] == 1) && lamp[0][1] == 1 || lamp[0][0] == 1){
       error = - 18;
     }else if((lamp[0][2] == 1 || lamp[0][4] == 1) && lamp[0][3] == 1 || lamp[0][2] == 1){
       error = - 14;
     }else if((lamp[0][11] == 1 || lamp[0][9] == 1) && lamp[0][10] == 1 || lamp[0][11] == 1){
       error =   18;
     }else if((lamp[0][7] == 1 || lamp[0][8] == 1) && lamp[0][9] == 1 || lamp[0][8] == 1){
       error =   14;
     }else if((lamp[0][3] == 1 || lamp[0][5] == 1) && lamp[0][4] == 1 ){
       error = - 6;
     }else if((lamp[0][8] == 1 || lamp[0][6] == 1) && lamp[0][7] == 1 ){
       error =   6;
     }
  }*/
  if(part_flag > 10 || part_flag==2 || part_flag==3 ){
    if (lamp_Rsum > 3){
      decide = 1;
      if (lamp_Rsum > 4 && lamp[0][5] == 1 && lamp[0][4] != 1) error = 0;   //0 0 0 0 0 1 1 1 1 1 1 1
      else if (lamp[0][5] != 1 && lamp[0][6] != 1 && lamp[0][7] == 1 && lamp[0][8] == 1) error = 3;
      else if (lamp[0][6] == 1 && lamp[0][5] != 1) error = 1;
      else if (lamp[0][5] == 1 && lamp[0][4] == 1 && lamp[0][3] != 1) error = -2;
      
      else error = error / 2;
    }else error = error_current;
  }else error = error_current;
/////////////////////////////////////////////////////////////////////////////////////////////
  /*if(part_flag==13 && (lamp_Lsum >3 || lamp_Rsum > 3)&&wan_flag == 1){
    counter_temp = 6;
    heixian_flag = 1;
    perform = 1;
  }*//*
  if(part_flag==3 ){
    if (lamp[0][0] == 1 || lamp[0][1] == 1 || lamp[0][2] == 1) error = 0;
  }*/
/////////////////////////////////////////////////////////////////////////////////////////////
  if(lamp_Lsum > 2 && lamp_Rsum > 2 && xialiao_flag != 2) error = 0;

/////////////////////////////////////////////////////////////////////////////////////////////
  if(xialiao_flag == 0){
    if ((lamp_Rsum == 3 || lamp_Lsum == 3) && lamp[0][5] != 1 && lamp[0][6] != 1)
    error = error*2 / 3 ;
  }
/////////////////////////////////////////////////////////////////////////////////////////////
  if(lamp_Lsum >2  && lamp_Rsum > 2)
      error = 0 ;
 /* if(dingwei_flag == 0){
    if(lamp[0][3] == 1 && lamp[0][4] == 1 || lamp[0][4] == 1 && lamp[0][5] == 1 || lamp[0][6] == 1 && lamp[0][7] == 1 || lamp[0][7] == 1 && lamp[0][8] == 1)  wan_flag = 1;
    else  wan_flag = 0;
   
  }*/
/////////////////////////////////////////////////////////////////////////////////////////////
 /* if(part_flag == 1 ){
    if(lamp_Lsum > 2 && lamp_Rsum > 2 && shang == 1){
      error = 0;
      perform = 1;
      counter_temp == 2;
    }
  }*/
  if(lamp_Rsum < 1 && lamp_Lsum < 1){
    if(part_flag == 26){
       //part_flag = 4;
       //perform = 1;
		Left_front_goalspeed = 60; Right_front_goalspeed = 60;
		Left_rear_goalspeed = 60; Right_rear_goalspeed = 60;
      PIT_Delayms(PIT3,700);//用来停车，简单粗暴
      Target_speed = 0;
      UART_PutStr(UART1, "#000P1509T0000!");
      PIT_Delayms(PIT3,50000);
			//UART_PutStr(UART1, "#000P1509T0000!");
    }
  }
  //if (lamp_Rsum < 2 || lamp_Lsum < 2) error = error;
}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】侧边循迹计数
          加中间量是为了判断灯一亮一灭的变换再计数，避免加多了，并且这里用了两盏灯同时变化，目的也是减少干扰
-------------------------------------------------------------------------------------------------------*/
void counter_sum(void){

//if(part_flag!=13 && heixian_flag == 0){

  lamp[2][0] = GPIO_PinRead(PTD13);
  lamp[2][1] = GPIO_PinRead(PTE9);
  if(lamp[2][0] == 1 && lamp[2][1] == 1){
    if(side==0){
      counter_temp++;
      perform = 1;
    }side =1;
  }else if(lamp[2][0] == 0 && lamp[2][1] == 0) side =0;

//}
  
  /*if(counter_temp == 1 && lamp[0][4] == 1 && lamp[0][5] == 1 && lamp[0][6] == 1 && lamp[0][7] == 1&& lamp[0][4] == 1 && lamp[0][5] == 1){
    counter_temp=2;
  }*/
}

/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】上料区处理
三角形=0；
正方形=1
六边形=2；
圆 = 3；
N字=4；
六边形=5
5 3 1
4 2 0
5 4 3 2 1 0
int supplies_bi[3][4]= {
  {0, 1, 2, 2},             //第一个数：物料所在位置；第二个数：物料下料区位置，第三个数：物料形状，第四个数：物料盒位置
  {3, 3, 5, 3},
  {5, 5, 4, 1},
};
-------------------------------------------------------------------------------------------------------*/

void material_bi_feeding(void){                        //上料区扫描识别抓取
  int down,up;
  int j = 7;
  int k = 7;
  int supplies_temp = 7;
  switch (part_flag)
  {
  case 11:down = 0;up = 1;break;
  case 12:down = 2;up = 3;break;
  case 13:down = 4;up = 5;break;
  default:down = 7;up = 7;break;
  }
  //UART_PutStr(UART1, "#000P1503T0000!");          //语音播报
  if (step_flag == 2) {
	  step_flag = 3;
  }
  UART_PutBuff(UART3, scan_down, 7);        //识别下面
  PIT_Delayms(PIT3, 3000);                               //等待树莓派处理
  for(int i = 0;i<3;i++){                      //计算盒子位置
    if(down == supplies_bi[i][0]){
      step_flag = supplies_bi[i][3];
      supplies_temp = down;
	  j = i;
    }
  }
  if(supplies_temp != 7)   {//这次有收获
    UART_PutStr(UART1, "#000P1511T0000!");          //语音播报成功
    if(supplies_bi[j][3] == 2)   
    {
     UART_PutBuff(UART3, grab_down_to_rear, 7);    //如果是中间盒子需要放里面,抓下面的到里面的盒子
    }
    else {
     UART_PutBuff(UART3, grab_down_to_front, 7);                //抓下面的盒子到外面
    }
    PIT_Delayms(PIT3,4000);
  } else UART_PutStr(UART1, "#000P1512T0000!");          //语音播报失败
/////////////////////////////////////////////////////////////////////////////////////////////
  if (step_flag == 2) {
	  step_flag = 3;
  }
  UART_PutBuff(UART3, scan_up, 7);                 //识别上面
  PIT_Delayms(PIT3, 3000);                                  //等待它动作稳定后拍照
  for(int i = 0;i<3;i++){                      //计算盒子位置
    if(up == supplies_bi[i][0]){
      step_flag = supplies_bi[i][3];
      supplies_temp = up;
	  k = i;
    }
  }
  if(supplies_temp != 7 && supplies_temp != down)   {//这次有收获
    UART_PutStr(UART1, "#000P1504T0000!");          //语音播报成功
	if (supplies_bi[k][3] == 2) {
		UART_PutBuff(UART3, grab_up_to_rear, 7);    //如果是中间盒子需要放里面,抓上面的到里面的盒子
	}
    else UART_PutBuff(UART3, grab_up_to_front, 7);                //抓上面的盒子到外面
    PIT_Delayms(PIT3,4000);
  }else UART_PutStr(UART1, "#000P1505T0000!");          //语音播报失败
}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】下料区处理
int supplies_bi[3][4]= {
  {0, 1, 5, 2},             //第一个数：物料所在位置；第二个数：物料下料区位置，第三个数：物料形状，第四个数：物料盒位置
  {2, 3, 1, 3},
  {5, 5, 3, 1},
};
-------------------------------------------------------------------------------------------------------*/

void material_bi_blanking(void){                        //下料区扫描识别放置
  int blanking;
  int t = 7;
  int supplies_temp = 7;
  switch (part_flag)
  {
  case 21:blanking = 0;break;
  case 22:blanking = 1;break;
  case 23:blanking = 2;break;
  case 24:blanking = 3;break;
  case 25:blanking = 4;break;
  case 26:blanking = 5;break;
  default:blanking = 7;break;
  }
  for(int i = 0;i<3;i++){                      //计算盒子位置
    if(blanking == supplies_bi[i][1]){
      step_flag = supplies_bi[i][3];
      supplies_temp = blanking;
	  t = i;
    }
  }
  //UART_PutStr(UART1, "#000P1511T0000!");          //语音播报识别
  UART_PutBuff(UART3, arm, 7);        //识别
  PIT_Delayms(PIT3, 4000);                         //等待它动作稳定后拍照

  if(supplies_temp != 7)   {//这次有收获
    UART_PutStr(UART1, "#000P1511T0000!");          //语音播报成功
    PIT_Delayms(PIT3, 500);                               //等待树莓派处理
    UART_PutStr(UART1, "#000P1515T0000!");          //语音播报成功
    if(supplies_bi[t][3] == 2)	UART_PutBuff(UART3, put_rear, 7);
		//UART_PutBuff(UART3, put_rear, 7);
      /*if(supplies_bi[t][2] == 0)   UART_PutBuff(UART3,  grab_triangle_to_rear, 7);
      else if(supplies_bi[t][2] == 1)   UART_PutBuff(UART3, put_rear_bigzheng, 7);
      else if(supplies_bi[t][2] == 2)   UART_PutBuff(UART3, put_rear_smallNliu, 7);
      else if(supplies_bi[t][2] == 3)   UART_PutBuff(UART3, put_rear, 7);
      else if(supplies_bi[t][2] == 4 || supplies_bi[t][2] == 5)   UART_PutBuff(UART3,  put_rear_smallNliu, 7);*/

    //放里面的物料//UART_PutBuff(UART3, put_front, 7);                //放外面的物料
    else 
    {
		UART_PutBuff(UART3, put_front, 7);
	  /*if(supplies_bi[t][2] == 0)   UART_PutBuff(UART3,  put_triangle_to_rear, 7);
      else if(supplies_bi[t][2] == 1)   UART_PutBuff(UART3, put_front, 7);
      else if(supplies_bi[t][2] == 2)   UART_PutBuff(UART3, put_front_smallliu, 7);
      else if(supplies_bi[t][2] == 3 || supplies_bi[t][2] == 4)   UART_PutBuff(UART3, put_front_smallyuanN, 7);
      else if(supplies_bi[t][2] == 5)   UART_PutBuff(UART3,  put_front_smallliu, 7);*/
    }
    PIT_Delayms(PIT3,4000);
  } else UART_PutStr(UART1, "#000P1505T0000!");          //语音播报失败

}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】倒车循迹程序
-------------------------------------------------------------------------------------------------------*/
void read_lamp_back(void)  //根据传感器计算出偏移量error
{
  lamp[1][0] = GPIO_PinRead(PTE7);
	lamp[1][1] = GPIO_PinRead(PTE8);
	lamp[1][2] = GPIO_PinRead(PTD14);
	lamp[1][3] = GPIO_PinRead(PTD15);

  int error_current = 0;   //当前偏差
  uint8 lamp_Lsum = 0;    //左半边灯循迹到黑线的数量
  uint8 lamp_Rsum = 0;
  
  for(int j = 0; j < 4; j ++){     //对不同的灯判断到黑线进行相应的赋值
    if(lamp[1][j] == 1){
      lamp_num[1][j] = 2*j - 3;            //   -3 -1  1  3    
      if(j < 4)lamp_Lsum ++;
      else lamp_Rsum ++;
    }else lamp_num[1][j] = 0;
    error_current += lamp_num[1][j];//前循迹偏差值
  }
  if (lamp_Lsum > 1){
    decide = 1;
    if (lamp[1][0]==1 && lamp[1][1]==1 && lamp[1][2]==1 && lamp[1][3]==1) error = 0;
    else error = error_current;
  }
  else if (lamp[1][0]==1 && lamp[1][1]==1 && lamp[1][2]==1 && lamp[1][3]==1) error = 0;
  else if (lamp[1][0]==0 && lamp[1][1]==0 && lamp[1][2]==0 && lamp[1][3]==0) error = 0;
  else error = error_current;

}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】根据侧面循迹累计的数值来判断到什么地方了
-------------------------------------------------------------------------------------------------------*/

void PART_all(void)
{  
    if(counter_temp == 0) part_flag=0;
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
   // else if(counter_temp == 13) {part_flag = 4;Target_speed = 0;}
    //else part_flag=0;
}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】功能执行，这里用perform变量来保证这些只执行一次
-------------------------------------------------------------------------------------------------------*/
void Part_Choose(void)
{
  switch(part_flag){
		case  0: 
      break;
////////////////////////////////////开机部分/////////////////////////////////////////////////////////
    case  1:
        target_slow(50,10);
        Posit(700, 60);//出发走出一段距离
        UART_PutBuff(UART3, openbox, 7);
        xialiao_flag = 0;
       // Posit(1400, 40);//出发走出一段距离
        //xialiao_flag = 2;
       // Posit(800, 50);
        //target_slow(80, 1000);
       // Target_speed = 0;//停车打开盒子
        //PIT_Delayms(PIT3, 2000);
        //target_slow(50, 100);
        Target_speed = 40;
        //shang = 1;
        wan_flag = 0;
      //  shangshang();

			break; 
////////////////////////////////////////上坡/////////////////////////////////////////////////////
    case  2:
			//Target_speed = 40;
      wan_flag = 0;
      xialiao_flag = 2;
			UART_PutStr(UART1, "#000P1501T0000!");
      //shangroate(800,50);
      //dingwei_flag = 1;
		//	Posit(20, 30);//（脉冲数，之后的速度）线上坡一段距离在加速方便上破
     // dingwei_flag = 0;
      //target_slow(110,3);
			Posit_up(680, 100);//上坡距离
      Posit_p(200, 40);
      //Posit(300, 30);
      xialiao_flag = 0;
      wan_flag = 0;
      //xialiao_flag = 2;
      Target_speed = 30;
      //dingwei_flag = 1;
      //target_slow(30,200);
      dingwei_flag = 1;
	    
			break;
//////////////////////////////////////////上料区定位//////////////////////////////////////////////////
    case 11:
      rear_flag = 0;
      wan_flag = 0;
      Posit_slow(8, 20);Target_speed = 0;UART_PutStr(UART1, "#000P1503T0000!");
			material_bi_feeding();
      //positing_flag = 1;
			Target_speed = 20;
      wan_flag = 0;
      //positing_flag = 1;
			break;
/////////////////////////////////////////////////////////////////////////////////////////////
    case 12:
			Posit_slow(8, 20);Target_speed = 0;UART_PutStr(UART1, "#000P1503T0000!");
			material_bi_feeding();
			Target_speed = 20;
			break;
/////////////////////////////////////////////////////////////////////////////////////////////
    case 13:
			rear_flag = 1;Posit_slow(8, 20);Target_speed = 0;UART_PutStr(UART1, "#000P1503T0000!");
			material_bi_feeding();
      UART_PutBuff(UART3, wd_wl, 7);   //匀速缓慢下坡 
      xialiao_flag = 2;
      step_flag = 2;//UART_PutBuff(UART3, smooth, 7);           //摆放机械臂调整物料槽，平稳运行
			positing_flag = 0;
      Posit_k(700, 30);  
      wan_flag = 1;
      xialiao_flag = 0;
      Target_speed = 35;
      wan_flag = 1;dingwei_flag = 0;        //标志位改变
      Posit_slow(1000, 35);
      wan_flag = 0;
			break;
///////////////////////////////////////////避障//////////////////////////////////////////////////
		case  3:
			UART_PutStr(UART1, "#000P1502T0000!"); 
      wan_flag = 0;
		 // Obstacle();       //循迹测距     
      //rotating();       //避障
	    rotate_bzy();
      wan_flag = 1;
	    Posit(1000, 40);        //一段距离的速度          //改用定位的PID
      heixian_flag=0; 
      dingwei_flag = 0;//rear_flag = 0;
      Target_speed = 30;
      wan_flag = 0;
      //target_slow(30,200);
      xialiao_flag = 0;
      dingwei_flag = 1;
			break;
      //Target_speed = 60;Obstacle();rotating();Target_speed = 50;break;
      //Obstacle();break;//避障区域
      // Target_speed = 100;track_PID ();counter_sum();break; 
///////////////////////////////////////////下料//////////////////////////////////////////////////
    case 21:
			//positing_flag = 1;
      wan_flag = 0;rear_flag = 0;Target_speed = 20; xialiao_flag = 0;//速度和标志位改变
      Posit_slow(5, 20);Target_speed = 0;UART_PutStr(UART1, "#000P1507T0000!");
      material_bi_blanking();
			Target_speed = 20;
			break;
/////////////////////////////////////////////////////////////////////////////////////////////
    case 22:
      Posit_slow(5, 20);Target_speed = 0;UART_PutStr(UART1, "#000P1507T0000!");
      material_bi_blanking();
			Target_speed = 20;
			break;
/////////////////////////////////////////////////////////////////////////////////////////////
    case 23:
      Posit_slow(5, 20);Target_speed = 0;UART_PutStr(UART1, "#000P1507T0000!");
      material_bi_blanking();
			Target_speed = 20;
			break;
/////////////////////////////////////////////////////////////////////////////////////////////
    case 24:
      Posit_slow(5, 20);Target_speed = 0;UART_PutStr(UART1, "#000P1507T0000!");
      material_bi_blanking();
			Target_speed = 20;
			break;
/////////////////////////////////////////////////////////////////////////////////////////////
    case 25:
      Posit_slow(5, 20);Target_speed = 0;UART_PutStr(UART1, "#000P1507T0000!");
      material_bi_blanking();
			Target_speed = 20;
      break;
/////////////////////////////////////////////////////////////////////////////////////////////
    case 26:
      Posit_slow(5, 20);Target_speed = 0;UART_PutStr(UART1, "#000P1507T0000!");
      material_bi_blanking();
			positing_flag = 0;      //关闭后循迹
			Target_speed = 60;UART_PutBuff(UART3, smooth, 7);positing_flag = 0;
      xialiao_flag = 0;dingwei_flag = 0;
      wan_flag = 1;rear_flag = 1;
			break;
/////////////////////////////////////////结束////////////////////////////////////////////////////
    case  4:
			//Target_speed = 60; 
			//complete();
			//Posit(500, 75);
     // wan_flag = 0;
	//		Posit_stop(200, 70);
	//		UART_PutStr(UART1, "#000P1509T0000!");
    //  part_flag ++;
			break;
    default: Target_speed = 0;break;
    }
/////////////////////////////////////////perform置零////////////////////////////////////////////////////
  perform = 0;      //仅执行一次
}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】临时写的上坡用前面的循迹来判断，并开启陀螺仪来保证上坡时Z轴的直线度
-------------------------------------------------------------------------------------------------------*/
void shangshang(void){
  int a = 1;
  while(a){
    track_PID();
    lamp[0][4] = GPIO_PinRead(PTB4);
	  lamp[0][5] = GPIO_PinRead(PTB5);
  	lamp[0][6] = GPIO_PinRead(PTB6);
  	lamp[0][7] = GPIO_PinRead(PTB7);
    if(lamp[0][4]==1 && lamp[0][5]==1 && lamp[0][6]==1 && lamp[0][7]==1){
      a = 0;
      Init_9AX();
      counter_temp++;
      PART_all();
      Part_Choose();
      break;
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////
static float Angle_h = 0;
static float Angle_error = 0;
static float Angle_i = 0;
void shangroate(int tarnum,int tarspeed){
    int c = 1;
	  Target_num = tarnum;
    Target_speed = tarspeed;
	  pos_flag = 1;
    int p = 180;
    PIT_Close(PIT1);
    Init_9AX();
    int16_t ACC_X, ACC_Y, ACC_Z, GYRO_X, GYRO_Y, GYRO_Z, MAG_X, MAG_Y, MAG_Z;
		while (c)
		{
			Update9AX(&ACC_X, &ACC_Y, &ACC_Z, &GYRO_X, &GYRO_Y, &GYRO_Z, &MAG_X, &MAG_Y, &MAG_Z);
			Angle_i = -GYRO_Z / 131;
			Angle_h += Angle_i / 100;
			PIT_Delayms(PIT3, 10);
      Angle_error = get_pid(&track_Rote,Angle_h);
      Left_front_goalspeed = Target_speed * (0.001 *  Angle_error); Right_front_goalspeed = Target_speed * (- 0.001 *  Angle_error);
      Left_rear_goalspeed = Target_speed * (0.001 *  Angle_error); Right_rear_goalspeed = Target_speed * (- 0.001 *  Angle_error);
       if (pos_flag != 1) {
			  c = 0;
        PIT_Step(PIT1, 300);
		  	break;
		  }

		}
}	

/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】根据目标速度和减速时间实现匀减速
-------------------------------------------------------------------------------------------------------*/
void target_slow(int tar,int time){
  int a = 0;
  while(a){

    if(Target_speed > tar){
      Target_speed--;
    }else if(Target_speed < tar){
      Target_speed++;
    }else {
      a=0;
      break;
    }
    track_PID();
    PIT_Delayms(PIT3,time);
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////
void target_slow_t(int tar,int time){
  int a = 0;
  while(a){

    if(Target_speed > tar){
      Target_speed--;
    }else if(Target_speed < tar){
      Target_speed++;
    }else {
      a=0;
      break;
    }
    //track_PID();
    PIT_Delayms(PIT3,time);
  }
}

/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】走一定脉冲数
-------------------------------------------------------------------------------------------------------*/
void Posit_slow(int tarnum,int tarspeed)
{
	uint8 a = 1;
  //int b = 0;
	Target_speed = tarspeed;
	Target_num = tarnum;
	pos_flag = 1;
	while (a)
	{
   // b = tarnum - temp_num;
		track_PID();
   // if( (b%3) == 0)  Target_speed --;
    //Target_speed = (float)((tarnum - temp_num)/tarnum)*tarspeed;
		if (pos_flag != 1) {
			a = 0;
			break;
		}
	}

}
void Posit(int tarnum,int tarspeed)
{
	uint8 a = 1;
	Target_speed = tarspeed;
	Target_num = tarnum;
	pos_flag = 1;
	while (a)
	{
		track_PID();
		if (pos_flag != 1) {
			a = 0;
			break;
		}
	}
}
///////////////////////////////////////////上坡用，因为侧滑所以左右速度直接给定了//////////////////////////////////////////////////
void Posit_up(int tarnum,int tarspeed)
{
	uint8 a = 1;
	Target_speed = tarspeed;
	Target_num = tarnum;
	pos_flag = 1;
	while (a)
	{
		error = 0;
		//diff_speed();
    Left_front_goalspeed = tarspeed; Right_front_goalspeed = tarspeed *7.5/ 10;
	  Left_rear_goalspeed = tarspeed; Right_rear_goalspeed = tarspeed *7.5/ 10;
    //track_PID();
		if (pos_flag != 1) {
			a = 0;
			break;
		}
	}
}
void Posit_p(int tarnum,int tarspeed)
{
	uint8 a = 1;
	Target_speed = tarspeed;
	Target_num = tarnum;
	pos_flag = 1;
	while (a)
	{
		error = 0;
		//diff_speed();
    Left_front_goalspeed = tarspeed; Right_front_goalspeed = tarspeed;
	  Left_rear_goalspeed = tarspeed; Right_rear_goalspeed = tarspeed;
    //track_PID();
		if (pos_flag != 1) {
			a = 0;
			break;
		}
	}
}
void Posit_k(int tarnum,int tarspeed)
{
	uint8 a = 1;
	Target_speed = tarspeed;
	Target_num = tarnum;
	pos_flag = 1;
	while (a)
	{
		error = 0;
		//diff_speed();
    Left_front_goalspeed = tarspeed; Right_front_goalspeed = tarspeed*9.1/ 10;
	  Left_rear_goalspeed = tarspeed; Right_rear_goalspeed = tarspeed*9.1/ 10;
    //track_PID();
		if (pos_flag != 1) {
			a = 0;
			break;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////
void Posit_stop(int tarnum, int tarspeed)
{
	uint8 a = 1;
	Target_speed = tarspeed;
	Target_num = tarnum;
	pos_flag = 1;
	while (a)
	{
		error = 0;
		diff_speed();
		if (pos_flag != 1) {
			a = 0;
      Target_speed = 0;
			break;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////
void Pos_back(int tarnum)
{
	uint8 a = 1;
	Target_num = tarnum;
	pos_flag = 2;
  Target_speed = -30;
	while (a)
	{

   // error = 0;
    read_lamp_back();
    //error = -error;
    diff_speed();
		if (pos_flag != 2) {
			a = 0;
      Target_speed = 25;
			break;
		}
	}
}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】检测障碍物，用了小小的滤波
-------------------------------------------------------------------------------------------------------*/
void Obstacle(void){

  uint8 a=1;
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
    }
    dis = 0;
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
    if(MinDistance < 3200){
     a=0;
     UART_PutStr(UART1, "#000P1513T0000!"); 
    break;
    }else run_straight();//track_PID();
  }
}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】左右转动，契合车子的速度达到稳定的转弯
-------------------------------------------------------------------------------------------------------*/
int16 sp = 50;
void turn_left(void) {
	//error = -10;
	//calc_pid(&pid_track);
	//wan_flag = 2;
	//diff_speed();
	Left_front_goalspeed = 10; Right_front_goalspeed = sp;
	Left_rear_goalspeed = 10; Right_rear_goalspeed = 100;
	if (sp < 80) {
		sp++; PIT_Delayms(PIT3, 10);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////
void turn_right(void) {
	//error = 10;
	//calc_pid(&pid_track);
	//wan_flag = 2;
	//diff_speed();
	Left_front_goalspeed = sp; Right_front_goalspeed = 10;
	Left_rear_goalspeed = 100; Right_rear_goalspeed = 10;
	if (sp < 80) {
		sp ++; PIT_Delayms(PIT3, 10);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////
void run_straight(void) {
	//error = 0;
	//calc_pid(&pid_track);
	//wan_flag = 2;
	//diff_speed();
	Left_front_goalspeed = 30; Right_front_goalspeed = 30;
	Left_rear_goalspeed = 30; Right_rear_goalspeed = 30;
}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】用脉冲数走的避障\打滑严重不建议使用
-------------------------------------------------------------------------------------------------------*/
void rotating(void) {

	uint8 a = 1;
	uint8 c = 1;
	Target_speed = 50;
	Target_left = 880;
	Target_right = 880;
	double_flag = 1;
	while (a) {
		if (c == 1) {   //左转
			if (double_flag == 0)
			{
				Target_left = 250;
				Target_right = 250;
				double_flag = 1;
				c = 6;
				sp = 50;
			}
			else turn_left();
		}
    /////////////////////////////////////////////////////////////////////////////////////////////
		else if (c == 6) {//直行
			if (double_flag == 0)
			{
				Target_left = 1580;
				Target_right = 1580;
				double_flag = 2;
				c = 2;
				sp = 50;
			}
			else run_straight();
		}
    /////////////////////////////////////////////////////////////////////////////////////////////
		else if (c == 2) {//右转
			if (double_flag == 0)
			{
				Target_left = 0;
				Target_right = 0;
				double_flag = 1;
				c = 3;
				sp = 50;
			}
			else turn_right();
		}
    /////////////////////////////////////////////////////////////////////////////////////////////
		else if (c == 3) {//zhixing
			if (double_flag == 0)
			{
				Target_left = 0;
				Target_right = 0;
				double_flag = 2;
				c = 4;
				sp = 50;
			}
			else run_straight();
		}
    /////////////////////////////////////////////////////////////////////////////////////////////
		else if (c == 4) {
			if (double_flag == 0)
			{
				//Target_left=300;
				//Target_right=300;
				//double_flag = 1;
				c = 5;
				sp = 50;
			}
			else turn_right();
		}
    /////////////////////////////////////////////////////////////////////////////////////////////
		else if (c == 5) {
			lamp[0][9] = GPIO_PinRead(PTC14);
			lamp[0][8] = GPIO_PinRead(PTC13);
			if (lamp[0][8] == 1 && lamp[0][9] == 1)
			{
				//if(double_flag != 1)
			   // {
				Target_left = 950;
				Target_right = 950;
				double_flag = 1;
				c = 7;
				sp = 50;
			}
			else run_straight();
		}
    /////////////////////////////////////////////////////////////////////////////////////////////
		else if (c == 7) {//右转后直行
			if (double_flag == 0)
			{
				a = 0;
				//wan_flag = 0;
				break;
			}
			else turn_left();
		}

	}
}


/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】PID参数定义
-------------------------------------------------------------------------------------------------------*/
void Pid_Init(void)
{
	pid_left_front.kp=VP;
	pid_left_front.ki=4;
	pid_left_front.kd=VD;
	
	pid_right_front.kp=VP;
	pid_right_front.ki=VI;
	pid_right_front.kd=VD;
	
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

	pid_rotate.kp=P;
	pid_rotate.ki=I;
	pid_rotate.kd=D;
	
	pid_rotate.err=0;
	pid_rotate.last_err=0;
	pid_rotate.last_last_err=0;
	pid_rotate.result=0;
}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】增量式PID
-------------------------------------------------------------------------------------------------------*/
void PID_Control(_pid_t *pid, int16 actual, int16 set)//速度控制
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
/////////////////////////////////////////////////////////////////////////////////////////////
void calc_pid(_pid_t *pid)//循迹PID（后面没用）
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
/////////////////////////////////////////////////////////////////////////////////////////////
/*
float track_p = 0, track_i = 0, track_d = 0;
void calc_pid(_pid_t *pid)//得到速差
{
	pid->err = error;
	track_p = pid->err;
	track_i += pid->err;
	track_d = pid->err - pid->last_err ;
	pid->result = pid->kp * track_p +pid->ki * track_i + pid->kd * track_d;
	if(pid->result>2000)
		pid->result = 2000;
	else if(pid->result<-2000)
		pid->result = -2000;
	pid->last_err = pid->err;

}*/
/////////////////////////////////////////////////////////////////////////////////////////////
void rotate_pid(_pid_t *pid, int16 actual, int16 set)//旋转的角度PID
{
  int16 error=(actual - set);
	float p = 0, i = 0, d = 0;
	pid->err = error;
	p = pid->err - pid->last_err;
	i = pid->err;
	d = pid->err - 2 * pid->last_err + pid->last_last_err;
	pid->result = pid->kp * p +pid->ki * i + pid->kd * d;
	if(pid->result>1000)
		pid->result = 1000;
	else if(pid->result<-1000)
		pid->result = -1000;
	pid->last_last_err = pid->last_err;
	pid->last_err = pid->err;
}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】避障防侧滑的角度环
-------------------------------------------------------------------------------------------------------*/
static float Angle_1 = 0;
static float Angle = 0;

void rotate_bzy(void) {
	//int b = 1;
	//九轴数据刷新
	Target_speed = 40;
	uint8 a = 1;
	uint16_t dis = 0;
	uint16_t Distance[10] = { 0 };
	uint16_t Distance1 = 0;
	u8 i = 0;
	u8 j = 0;
	u8  measure_num = 8; //需要测量的次数
	uint16_t MinDistance = 3000;
	int16_t ACC_X, ACC_Y, ACC_Z, GYRO_X, GYRO_Y, GYRO_Z, MAG_X, MAG_Y, MAG_Z;
	//===注意，浮点计算稍微费时间，因此处理都已化为整型数处理（单位MM）===//
	//采集8个点
	while (a)
	{
		ADC_Init(ADC1);
		for (i = 0; i < measure_num; i++)
		{
			dis = (uint16_t)(ADC_Get(10)*0.806);          //计算距离dis
			Distance[i] = dis;
		}
		dis = 0;
		for (i = 0; i < measure_num - 1; i++)
		{
			for (j = 0; j < measure_num - 1 - i; j++)
			{
				if (Distance[j] > Distance[j + 1])
				{
					Distance1 = Distance[j];
					Distance[j] = Distance[j + 1];
					Distance[j + 1] = Distance1;
				}
			}
		}
		for (i = 1; i < measure_num; i++)
		{
			dis += Distance[i];
		}
		dis /= (measure_num - 2);//取平均
		MinDistance = dis;//最终值
		if (MinDistance < 3500) {
			a = 0;
			UART_PutStr(UART1, "#000P1513T0000!");
			break;
		}
		else track_PID();
	}
	 //PIT_Close(PIT0);
	PIT_Close(PIT1);//避免频繁的步进电机中断对姿态传感器的影响（对IIC通讯的影响）
		Init_9AX();//姿态传感器通讯初始化
		Angle = 0;
		Angle_1 = 0;
		int c = 1;
		sp = 50;//因为直接旋转给定速度，匀加速初始值，50
		while (c)
		{
			turn_left();
			Update9AX(&ACC_X, &ACC_Y, &ACC_Z, &GYRO_X, &GYRO_Y, &GYRO_Z, &MAG_X, &MAG_Y, &MAG_Z);//获取传感器数据，可以去库函数看
			Angle_1 = -GYRO_Z / 131;
			Angle += Angle_1 / 100;
			PIT_Delayms(PIT3, 10);//延时目的是保证IIC的通讯
			if (Angle < -4) {
				c = 0;
				break;
			}
		}
		int h = 1;
		Target_left = 250;
		Target_right = 250;
		double_flag = 1;
		sp = 50;
		while (h) {//直行
			run_straight();
			Update9AX(&ACC_X, &ACC_Y, &ACC_Z, &GYRO_X, &GYRO_Y, &GYRO_Z, &MAG_X, &MAG_Y, &MAG_Z);
			Angle_1 = -GYRO_Z / 131;
			Angle += Angle_1 / 100;
			PIT_Delayms(PIT3, 10);
			if (double_flag == 0)
			{
				h = 0;
				break;
			}
		}
    /////////////////////////////////////////////////////////////////////////////////////////////
		int d = 1;
		while (d)
		{
			turn_right();
			Update9AX(&ACC_X, &ACC_Y, &ACC_Z, &GYRO_X, &GYRO_Y, &GYRO_Z, &MAG_X, &MAG_Y, &MAG_Z);
			Angle_1 = -GYRO_Z / 131;
			Angle += Angle_1 / 100;
			PIT_Delayms(PIT3, 10);
			if (Angle > 0) {
				d = 0;
				break;
			}
		}
    /////////////////////////////////////////////////////////////////////////////////////////////
		int g = 1;
		Target_left = 200;
		Target_right = 200;
		double_flag = 1;
		sp = 50;
		while (g) {//直行
			Update9AX(&ACC_X, &ACC_Y, &ACC_Z, &GYRO_X, &GYRO_Y, &GYRO_Z, &MAG_X, &MAG_Y, &MAG_Z);
			Angle_1 = -GYRO_Z / 131;
			Angle += Angle_1 / 100;
			PIT_Delayms(PIT3, 10);
			if (double_flag == 0)
			{
				g = 0;
				break;
			}
			else run_straight();
		}
    /////////////////////////////////////////////////////////////////////////////////////////////
		int k = 1;
		while (k)
		{
			turn_right();
			Update9AX(&ACC_X, &ACC_Y, &ACC_Z, &GYRO_X, &GYRO_Y, &GYRO_Z, &MAG_X, &MAG_Y, &MAG_Z);
			Angle_1 = -GYRO_Z / 131;
			Angle += Angle_1 / 100;
			PIT_Delayms(PIT3, 10);
			if (Angle > 4) {
				k = 0;
				break;
			}
		}
    /////////////////////////////////////////////////////////////////////////////////////////////
		int e = 1;
		sp = 50;
		while (e) {
			run_straight();
			Update9AX(&ACC_X, &ACC_Y, &ACC_Z, &GYRO_X, &GYRO_Y, &GYRO_Z, &MAG_X, &MAG_Y, &MAG_Z);
			Angle_1 = -GYRO_Z / 131;
			Angle += Angle_1 / 100;
			PIT_Delayms(PIT3, 10);
			lamp[0][9] = GPIO_PinRead(PTC14);
			lamp[0][10] = GPIO_PinRead(PTC15);
			if (lamp[0][9] == 1 && lamp[0][10] == 1)
			{
				e = 0;
				break;
			}
		}
    /////////////////////////////////////////////////////////////////////////////////////////////
		int f = 1;
		sp = 50;
		while (f) {
			turn_left();
			Update9AX(&ACC_X, &ACC_Y, &ACC_Z, &GYRO_X, &GYRO_Y, &GYRO_Z, &MAG_X, &MAG_Y, &MAG_Z);
			Angle_1 = -GYRO_Z / 131;
			Angle += Angle_1 / 100;
			PIT_Delayms(PIT3, 10);
			if (Angle < 0)
			{
				f = 0;
				break;
			}
		}
		//PIT_Init(PIT0, 5);		//速度环更新
		PIT_Step(PIT1, 300);
}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】速度PID控制，这边挺绕的，知道啥用就行，就根据循迹的PID给误差再给速度赋值
-------------------------------------------------------------------------------------------------------*/
float pid_error_last;
void diff_speed(void){
  pit2_flag=1;
  float pid_error;
  if(pit2_flag==1){
    if(wan_flag == 0) pid_error = get_pid(&track_bzy,error); //直道PID
    else if(wan_flag == 1) pid_error = get_pid(&track_wan,error);//弯道PID
    else if(wan_flag == 2) pid_error = get_pid(&track_Obstacle,error);//避障PID
    //else if(wandao_flag == 1) pid_error = get_pid(&track_wan,error); 
    if(pid_error > 3000)
      pid_error = 3000;
    else if(pid_error < -3000)
      pid_error = -3000;
    if(((pid_error_last-pid_error) > 800 || (pid_error_last-pid_error) < -800) && wan_flag == 1) 	// 波动过大保持上次状态
      //pid_error = pid_error / 2;
      {
        //pid_error = pid_error_last;
      }else if(((pid_error_last-pid_error) < 800 && (pid_error_last-pid_error) > -800) && wan_flag == 1){
        if (Target_speed > 0){
          if(pid_error < 1000 && pid_error>-1000){
            Left_front_goalspeed = Target_speed * (1 +0.001 *  pid_error); Right_front_goalspeed = Target_speed * (1 - 0.001 *  pid_error);
            Left_rear_goalspeed = Target_speed * (1 + 0.001 *  pid_error); Right_rear_goalspeed = Target_speed * (1 - 0.001 *  pid_error);
          }else if(pid_error >= 1000 || pid_error <= -1000){
            Left_front_goalspeed = Target_speed * (0.001 *  pid_error); Right_front_goalspeed = Target_speed * (- 0.001 *  pid_error);
            Left_rear_goalspeed = Target_speed * (0.001 *  pid_error); Right_rear_goalspeed = Target_speed * (- 0.001 *  pid_error);
          }
        }else {
          Left_front_goalspeed = Target_speed * (1 -0.001 *  pid_error); Right_front_goalspeed = Target_speed * (1 + 0.001 *  pid_error);
          Left_rear_goalspeed = Target_speed * (1 - 0.001 *  pid_error); Right_rear_goalspeed = Target_speed * (1 + 0.001 *  pid_error);
        }
        pid_error_last = pid_error;
    }
  if(wan_flag != 1){
      if((pid_error_last-pid_error) > 1000 || (pid_error_last-pid_error) < -1000){
        pid_error = pid_error / 2;
        if (Target_speed > 0){
            Left_front_goalspeed = Target_speed * (1 +0.001 *  pid_error); Right_front_goalspeed = Target_speed * (1 - 0.001 *  pid_error);
            Left_rear_goalspeed = Target_speed * (1 + 0.001 *  pid_error); Right_rear_goalspeed = Target_speed * (1 - 0.001 *  pid_error);
        }else {
          Left_front_goalspeed = Target_speed * (1 -0.001 *  pid_error); Right_front_goalspeed = Target_speed * (1 + 0.001 *  pid_error);
          Left_rear_goalspeed = Target_speed * (1 - 0.001 *  pid_error); Right_rear_goalspeed = Target_speed * (1 + 0.001 *  pid_error);
        }
      }else  {
        if (Target_speed > 0){
            Left_front_goalspeed = Target_speed * (1 +0.001 *  pid_error); Right_front_goalspeed = Target_speed * (1 - 0.001 *  pid_error);
            Left_rear_goalspeed = Target_speed * (1 + 0.001 *  pid_error); Right_rear_goalspeed = Target_speed * (1 - 0.001 *  pid_error);
        }else {
          Left_front_goalspeed = Target_speed * (1 -0.001 *  pid_error); Right_front_goalspeed = Target_speed * (1 + 0.001 *  pid_error);
          Left_rear_goalspeed = Target_speed * (1 - 0.001 *  pid_error); Right_rear_goalspeed = Target_speed * (1 + 0.001 *  pid_error);
        }
      }
    pid_error_last = pid_error;
  }

  //if (error == 0) reset_I(&track_bzy);
    if(Left_front_goalspeed == Right_front_goalspeed) {reset_I(&track_bzy);reset_I(&track_wan);}
    pit2_flag=0;
  }
}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】PID循迹程序
-------------------------------------------------------------------------------------------------------*/
void chasu_speed(void){
  if(pit2_flag==1){
    float pid_error;
    pid_error = get_pid(&track_chasu,error); 
    if(pid_error > 20)
      pid_error = 20;
    else if(pid_error < -20)
      pid_error = -20;
    if((pid_error_last-pid_error) > 5 || (pid_error_last-pid_error)< -5) 	// 波动过大保持上次状态
      pid_error = pid_error /2;
    chasu(pid_error);
    pid_error_last = pid_error;
    pit2_flag=0;
  }
}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】PID循迹程序
-------------------------------------------------------------------------------------------------------*/
void track_PID (void)
{
  read_lamp_values();
  //calc_pid(&pid_track);
  diff_speed();
}
/////////////////////////////////////////////////////////////////////////////////////////////
void rotate_PID(int16_t MAG_Tar)
{
  //rotate_pid(&pid_rotate, MAG_Z, MAG_Tar);
  Left_front_goalspeed = Target_speed * (1 +0.001 * pid_rotate.result); Right_front_goalspeed = Target_speed * (1 - 0.001 * pid_rotate.result);
  Left_rear_goalspeed = Target_speed * (1 + 0.001 * pid_rotate.result); Right_rear_goalspeed = Target_speed * (1 - 0.001 * pid_rotate.result);
}

/****************************float****************************************************函数名：void  chasu(void)* **返回：无**函数功能描述：调整双电机差速******************************************************************************/
void  chasu(float Turn_direction)
{	
  float k_far = 1.2;
  int test_speed;       //差速比较用
 // v1=v0*(1-w/(k_far*l/er+w));
 // v0=v1*(1+(w*er)/(k_far*l));
  //式中，v1基准轮速度，v0为转弯轮速度
  //w为车宽度, l为车长度, er为转向偏差, k_far为正比于前瞻的可调值
  //*****************************************	
  if(Turn_direction>2)            //左转	//左边目标值缩小   
  Left_front_goalspeed=Left_front_goalspeed*(30625/((int)175+142*(int)Turn_direction/k_far))/175;  
  else if(Turn_direction<-2)           //右转 //右边目标值缩小
  Right_front_goalspeed=Right_front_goalspeed*(30625/((int)175+142*(int)(-Turn_direction)/k_far))/175;
  /******************************************************************************		差速控制器、用来控制实际的差速******************************************************************************/	
  //同时要加速，且与目标相差较大
  if((Right_front_goalspeed -Right_front_speed>10) && (Left_front_goalspeed -Left_front_speed>10)) 
  {	
    if (Turn_direction>3)          //左弯	
    {			//以左边为基准，放大作为右边的参考速度
      test_speed=Left_front_speed*(175+((int)142*Turn_direction)/k_far)/175;     	
      if((test_speed-Right_front_speed>10) || (Right_front_speed-test_speed>10))  //差速相差较远	
      {	
        if(Left_front_speed>120)    //左边速度要大于某一数值才限制		//以右边为基准，缩小作为左边的速度
        Left_front_goalspeed=Right_front_speed*(30625/(175+(int)142*Turn_direction/k_far))/175;   	
      }	
    }else if (Turn_direction<-3)         //右弯        
    {     //以右边为基准，放大作为左边的参考速度	
        test_speed=Right_front_speed*(175+((int)142*(-Turn_direction))/k_far)/175;   	
        if((test_speed-Left_front_speed>10) || (Left_front_speed-test_speed>10))    //差速相差较远	
      {	
        if(Right_front_speed>120)    //右边速度要大于某一数值才限制	//以左边为基准，缩小作为右边的速度
        Right_front_goalspeed=Left_front_speed*(30625/(175+(int)142*(-Turn_direction)/k_far))/175; 	
      }	
    }
  }else if((Right_front_speed -Right_front_goalspeed>10) && (Left_front_speed -Left_front_goalspeed>10) )	//同时要减速，且与目标相差较大
  {	
    if (Turn_direction>=0)          //左弯	
    {			//以左边为基准，放大作为右边的参考速度
      test_speed=Left_front_speed*(175+((int)142*Turn_direction)/k_far)/175;     			
      if((test_speed-Right_front_speed>10) || (Right_front_speed-test_speed>10))  //差速相差较远	    
      {	    //以左边为基准，放大作为右边的速度
        Right_front_goalspeed=Left_front_speed*(175+((int)142*Turn_direction)/k_far)/175;     	
      }	
    }else       //右弯	
    {    //以右边为基准，放大作为左边的参考速度
      test_speed=Right_front_speed*(175+((int)142*(-Turn_direction))/k_far)/175;     
      if((test_speed-Left_front_speed>10) || (Left_front_speed-test_speed>10))    //差速相差较远	
      {                                            //以右边为基准，放大作为左边的速度
      Left_front_goalspeed=Right_front_speed*(175+((int)142*(-Turn_direction))/k_far)/175;   	
      }		
    }
  }
  Left_rear_goalspeed = Left_front_goalspeed;Right_rear_goalspeed = Right_front_goalspeed;
}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】与树莓派的通信判断
-------------------------------------------------------------------------------------------------------*/
/*
三角形=1；
正方形=2
六边形=3；
圆 = 4；
N字=5；
六边形=6

6 4 2
5 3 1

6 5 4 3 2 1
*/
//unsigned char feeding[2][3];
//unsigned char blanking[6][1];

int supplies[2][6]= {
  {1,2,     4,1,      5,5},
  {6,1,     1,2,      3,5},
};

int supplies_pi[3]= {1    ,3    ,4};
int supplies_now[2][6]= {
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
};

int supplies_size[3][2]= {
  {0, 0},
  {0, 0},
  {0, 0},
};

int supplies_all[6][3]= {
  {0, 0},//顺序，种类，大小
  {0, 0},
  {0, 0},
  {0, 0},
  {0, 0},
  {0, 0},
};

int photo = 0;      //有效拍照的次数，只会有六次
int feeding_flag = 0;    //识别的位置
int blanking = 0;   //第几次下料识别
int grab_flag[3] = {0,0,0};    //识别的位置


void material_front_identify(void){                        //上料区扫描识别，仅仅是识别用

  UART_PutStr(UART1, "#000P1503T0000!");          //语音播报
  UART_PutBuff(UART3, grab_up_to_rear, 7);        //识别下面
  PIT_Delayms(PIT3, 500);                         //等待它动作稳定后拍照
  UART_PutBuff(UART2, visual, 4);                 //让树莓派拍照
  int a = 1;
  while(a){                                               //等待树莓派发送数据
    PIT_Delayms(PIT3, 500);                               //等待树莓派处理
    if(pi_getbuf[0] == 0x55){                             //有拍到东西

      if(pi_getbuf[1] == supplies_pi[0] || pi_getbuf[1] == supplies_pi[1] || pi_getbuf[1] == supplies_pi[2]){       //拍到了想要的

        supplies_all[photo][0] = pi_getbuf[0];               //二维数组赋值，第一个是照片顺序，第二个是种类，第三个是大小
        supplies_all[photo][1] = pi_getbuf[1];

      }else{                //拍到了不想要的

      }
      a = 0;
      photo++;
      break;
    }else {                                               //没拍到东西
      UART_PutBuff(UART2, visual, 4);                     //让树莓派拍照
    }

  }

  UART_PutBuff(UART3, grab_up_to_rear, 7);                 //识别上面
  PIT_Delayms(PIT3, 500);                                  //等待它动作稳定后拍照
  UART_PutBuff(UART2, visual, 4);                          //让树莓派拍照
  int b = 1;
  while(b){                                               //等待树莓派发送数据
    PIT_Delayms(PIT3, 500);                               //等待树莓派处理
    if(pi_getbuf[0] == 0x55){                             //有拍到东西

      if(pi_getbuf[1] == supplies_pi[0] || pi_getbuf[1] == supplies_pi[1] || pi_getbuf[1] == supplies_pi[2]){       //拍到了想要的

        supplies_all[photo][0] = pi_getbuf[0];               //二维数组赋值，根据照片顺序，supplies_all[photo][0]，是种类
        supplies_all[photo][1] = pi_getbuf[1];               //二维数组赋值，根据照片顺序，supplies_all[photo][1]，是大小

      }else{                //拍到了不想要的

      }
      b = 0;
      photo++;
      break;
    }else {                                               //没拍到东西
      UART_PutBuff(UART2, visual, 4);                     //让树莓派拍照
    }

  }
  feeding_flag ++;                                         //识别的位置，1/2/3根线
}



void material_front_grab(void){                         //计算需抓取物料的1/2/3线

  int k = 0;
  for(int t = 0; t < 2 ;t++){                           //三种物块各寻找一次
    for(int i = 0; i < 5 ;i++)                          
      if(supplies_all[i][0] == supplies_pi[t]){         //找到本次循环寻找的物料
        for(int j = i; j < 5 ;j++){                     
          if(supplies_all[j][0] == supplies_pi[0]){     //再次寻找，判断哪个大
            if(supplies_all[i][1] > supplies_all[j][1]){//根据大小确定抓哪个物料台
              grab_flag[k] = i;
              k++;
            }else{
              grab_flag[k] = j;
              k++;
            }
          }
        }
        if(grab_flag[k] == 0){                           //防止有一次错误识别，识别到一个的情况，记下来
          grab_flag[k] = i;
          k++;
        }
      }
    }
  }
/*
  for(int p = 0; p < 2 ;p++)
    if (grab_flag[p] != 0){
      
  }


}

*/

void material_choose(void){
  

}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】位置PID，还挺关键的，但会有累计误差，增量式PID可以避免，这个比较适合定位，但我也用在了循迹上
-------------------------------------------------------------------------------------------------------*/
//函数功能： PID 积分置零
//函数参数： PID参数结构体 
//函数返回值： 无


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
    
    uint32_t tnow = systime.get_time_ms();//获取时间
    //uint32_t tnow = PIT_TimeGet(PIT2)/1000;
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
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】后面两个电机的分段函数，是测出来的，如果没有霍尔传感器或者编码器接口不够才建议使用
-------------------------------------------------------------------------------------------------------*/
void fenduan(void){
  if(Target_speed>0 && Target_speed<=30){
    Left_rear_speed = 80 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed = 80 * Right_rear_goalspeed ;
  }else if(Target_speed>30 && Target_speed<=40){
    Left_rear_speed = 72 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed = 72 * Right_rear_goalspeed ;
  }else if(Target_speed>40 && Target_speed<=50){
    Left_rear_speed = 66 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed = 66 * Right_rear_goalspeed ;
  }else if(Target_speed>50 && Target_speed<=60){
    Left_rear_speed = 63 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed = 63 * Right_rear_goalspeed ;
  }else if(Target_speed>60 && Target_speed<=70){
    Left_rear_speed = 60 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed = 60 * Right_rear_goalspeed ;
  }else if(Target_speed>70 && Target_speed<=80){
    Left_rear_speed = 57 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed = 57 * Right_rear_goalspeed ;
  }else if(Target_speed>80 && Target_speed<=90){
    Left_rear_speed = 55 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed = 55 * Right_rear_goalspeed ;
  }else if(Target_speed>90 && Target_speed<=120){
    Left_rear_speed = 51 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed = 51 * Right_rear_goalspeed ;
  }else if(Target_speed>120 && Target_speed<=150){
    Left_rear_speed = 50 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed = 50 * Right_rear_goalspeed ;
  }else if(Target_speed<0 && Target_speed>=-30){
    Left_rear_speed =-80 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed =-80 * Right_rear_goalspeed ;
  }else if(Target_speed<-30 && Target_speed>=-40){
    Left_rear_speed =-72 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed =-72 * Right_rear_goalspeed ;
  }else if(Target_speed<-40 && Target_speed>=-50){
    Left_rear_speed =-66 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed =-66 * Right_rear_goalspeed ;
  }else if(Target_speed<-50 && Target_speed>=-60){
    Left_rear_speed =-63 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed =-63 * Right_rear_goalspeed ;
  }else if(Target_speed<-60 && Target_speed>=-70){
    Left_rear_speed =-60 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed =-60 * Right_rear_goalspeed ;
  }else if(Target_speed<-70 && Target_speed>=-80){
    Left_rear_speed =-57 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed =-57 * Right_rear_goalspeed ;
  }else if(Target_speed<-80 && Target_speed>=-90){
    Left_rear_speed =-55 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed =-55 * Right_rear_goalspeed ;
  }else if(Target_speed<-90 && Target_speed>=-120){
    Left_rear_speed =-51 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed =-51 * Right_rear_goalspeed ;
  }else if(Target_speed<-120 && Target_speed>=-150){
    Left_rear_speed =-50 * Left_rear_goalspeed ;//根据实验测试电机特性曲线算得
    Right_rear_speed =-50 * Right_rear_goalspeed ;
  }
  /*if(rear_flag == 0){
    Left_rear_speed = 50;//根据实验测试电机特性曲线算得
    Right_rear_speed = 50;
  }
*/
}
/*------------------------------------------------------------------------------------------------------
【函    数】Test_Motor
【功    能】FTM0输出PWM信号控制电机例子
【参    数】num  :  电机标号  用PTC1 和 PTC2控制电机1  用PTC3 和 PTC4控制电机2
【返 回 值】duty ： 占空比    范围 -FTM_PRECISON  到 +FTM_PRECISON
【实    例】Test_Motor(); //测试电机
【注意事项】引脚初始化
--------------------------------------------------------------------------------------------------------*/
void Left_front_pwm_set(int16 Left_frontvalue)//左前轮速度
{
	if(Left_frontvalue > 0){
    GPIO_PinWrite(PTC10, 1);
		GPIO_PinWrite(PTC11, 0);		
    FTM_PwmDuty(FTM0, FTM_CH0, Left_frontvalue);
	}else if(Left_frontvalue < 0){
		GPIO_PinWrite(PTC10, 0);
		GPIO_PinWrite(PTC11, 1);
    FTM_PwmDuty(FTM0, FTM_CH0, -Left_frontvalue);
	}else{
    GPIO_PinWrite(PTC10, 0);
		GPIO_PinWrite(PTC11, 0);
    FTM_PwmDuty(FTM0, FTM_CH0, 0);
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////////
void Left_rear_pwm_set(int16 Left_rearvalue)
{
	if(Left_rearvalue > 0){
		GPIO_PinWrite(PTE11, 0);
		GPIO_PinWrite(PTE12, 1);
    FTM_PwmDuty(FTM0, FTM_CH1, Left_rearvalue);
	}else if(Left_rearvalue < 0){
		GPIO_PinWrite(PTE11, 1);
		GPIO_PinWrite(PTE12, 0);
    FTM_PwmDuty(FTM0, FTM_CH1, -Left_rearvalue);
	}else{
    GPIO_PinWrite(PTE11, 0);
		GPIO_PinWrite(PTE12, 0);
    FTM_PwmDuty(FTM0, FTM_CH1, 0);
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////////
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

  GPIO_PinInit(PTC13,GPI,0);
  GPIO_PinInit(PTC14,GPI,0);
  GPIO_PinInit(PTC15,GPI,0);
  GPIO_PinInit(PTC16,GPI,0); 

  GPIO_PinInit(PTE7, GPI, 0);
  GPIO_PinInit(PTE8, GPI, 0);
  GPIO_PinInit(PTD14, GPI, 0);
  GPIO_PinInit(PTD15, GPI, 0);
    
  GPIO_PinInit(PTD13, GPI, 0);
  GPIO_PinInit(PTE9, GPI, 0);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void Motor_Init(void)
{
	GPIO_PinInit(PTC10, GPO, 0);
	GPIO_PinInit(PTC11, GPO, 0);
	GPIO_PinInit(PTE11, GPO, 0);//PTD2
	GPIO_PinInit(PTE12, GPO, 0);//PTD3
	GPIO_PinInit(PTD4, GPO, 0);
	GPIO_PinInit(PTD5, GPO, 0);
	GPIO_PinInit(PTD6, GPO, 0);
	GPIO_PinInit(PTD7, GPO, 0);
	FTM_PwmInit(FTM0, FTM_CH0, 10000, 0);
	FTM_PwmInit(FTM0, FTM_CH1, 10000, 0);
	FTM_PwmInit(FTM0, FTM_CH2, 10000, 0);
	FTM_PwmInit(FTM0, FTM_CH3, 10000, 0);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void STEP_Init(void)
{
	GPIO_PinInit(PTE27, GPO, 0);          
	GPIO_PinInit(PTE28, GPO, 0);
}
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】各种初始化
-------------------------------------------------------------------------------------------------------*/
extern uint8_t pi_getbuf[2];

void Uart_Init(void)
{
    UART_Init(UART2, 115200); 
    UART_Init(UART3, 9600);
    UART_Init(UART1, 115200); 
    NVIC_SetPriority(UART2_RX_TX_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3,4));     
    NVIC_SetPriority(UART3_RX_TX_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    NVIC_SetPriority(UART1_RX_TX_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3,4));
    NVIC_EnableIRQ(UART2_RX_TX_IRQn);			          //使能UART4_RX_TX_IRQn的中断
    //UART_PutBuff(UART3, arm, 7);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void Timer_Init(void)//中断初始化
{
    PIT_Init(PIT0, 5);		//速度环更新
    PIT_Step(PIT1, 300);  //300us，2200*0.3=660ms
    //PIT_TimeStart(PIT2);
    systime.init();                      //开启systick定时器
    //PIT_Init(PIT2, 5);	//循迹环刷新	
    NVIC_SetPriority(PIT0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    NVIC_SetPriority(PIT1_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3,4));					
   // NVIC_SetPriority(PIT2_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3,4));
    NVIC_EnableIRQ(PIT0_IRQn);			          //使能PIT0_IRQn的中断	
    NVIC_EnableIRQ(PIT1_IRQn);	                  //使能中断函数在IRQ_Handle内
   // NVIC_EnableIRQ(PIT2_IRQn);		 
    EnableInterrupts;         //开总中断
}
/////////////////////////////////////////////////////////////////////////////////////////////
void Timer_IQR_handle()//PIT0的中断程序
{
  uint64_t ABGet_left, ABGet_right;   //脉冲数记录
  ABGet_left = FTM_ABGet(FTM1);
  ABGet_right = FTM_ABGet(FTM2);
	Lspeed_temp_1 = ABGet_left * 12.6 ;//12.6根据减速比和脉冲数计算所得
	Rspeed_temp_1 = ABGet_right * 12.6 ;
	Left_front_speed = Lspeed_temp_1 + Lspeed_temp_2;//因为定时周期为5ms，增加了上次的脉冲数以得到10ms内的脉冲数
	Right_front_speed = Rspeed_temp_1 + Rspeed_temp_2;//获得的速度单位为  mm/100ms
	Lspeed_temp_2 = Lspeed_temp_1; Rspeed_temp_2 = Rspeed_temp_1;
/////////////////////////////////////////////////////////////////////////////////////////////
  if(pos_flag == 1){//根据需求记录左右轮脉冲数，到达目标脉冲数后置零，但不建议用中断传递参数
    temp_num += ABGet_right + ABGet_left;
    //temp_num = temp_num + ABGet_right;
    if(Target_num < temp_num){
      temp_num = 0;pos_flag = 0;}
  }else if (pos_flag == 2) {
	  temp_num += ABGet_right + ABGet_left;
	  //temp_num = temp_num + ABGet_right;
	  if (Target_num > temp_num) {
		  temp_num = 0; pos_flag = 0;
	  }
  }
/////////////////////////////////////////////////////////////////////////////////////////////
  if(double_flag == 1){
    //temp_left = temp_left + ABGet_left;
    temp_right = temp_right + ABGet_right;
    if(temp_right > Target_right){
      temp_left=0;temp_right=0;double_flag=0;
    }
  }else if (double_flag == 2) {
    temp_left = temp_left + ABGet_left;
    //temp_right = temp_right + ABGet_right;
    if (temp_left > Target_left) {
      temp_left = 0; temp_right = 0; double_flag = 0;
    }
  }
/////////////////////////////////////////////////////////////////////////////////////////////  
  PID_Control(&pid_left_front, Left_front_speed, Left_front_goalspeed);//增量式PID速度环计算
  PID_Control(&pid_right_front, Right_front_speed, Right_front_goalspeed);
  if(Left_front_goalspeed >= 0){
    if(pid_left_front.result <= 0)
      pid_left_front.result = 100;
  }else if(Left_front_goalspeed < 0){
    if(pid_left_front.result >= 0)
      pid_left_front.result = -100;
  }
  if(Right_front_goalspeed >= 0){
    if(pid_right_front.result <= 0)
      pid_right_front.result = 100;
  }else if(Right_front_goalspeed < 0){
    if(pid_right_front.result >= 0)
      pid_right_front.result = -100;
  }
/////////////////////////////////////////////////////////////////////////////////////////////
  fenduan();//后轮电机的分段函数
  if(Target_speed == 0){
    Left_rear_speed = 0;Right_rear_speed = 0;pid_left_front.result=0;pid_right_front.result=0;
  }//使其为0时能保证停止
 /////////////////////////////////////////////////////////////////////////////////////////////
	Left_front_pwm_set((int16)pid_left_front.result);     Right_front_pwm_set((int16)pid_right_front.result);//电机占空比赋值
  Left_rear_pwm_set(Left_rear_speed);                   Right_rear_pwm_set(Right_rear_speed); 
}

/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】显示屏，这边是用中断来刷新显示屏，但不建议用中断传递参数，容易死机
-------------------------------------------------------------------------------------------------------*/
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

  /*sprintf(txt,"Error:%1d ",error);
    OLED_P6x8Str(5,4,(uint8_t*)txt);
  sprintf(txt,"STEP:%1d ",step);
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
/*
void Show_PI(void)
{
  char txt[16]; 
  sprintf(txt,"BZY_Flag:%3d ",part_flag);
    OLED_P8x16Str(5,0,(uint8_t*)txt);  
  sprintf(txt,"PI:%c ",pi_getbuf[0]);
    OLED_P8x16Str(5,2,(uint8_t*)txt);  
  sprintf(txt,"kind:%c ",pi_getbuf[1]);
    OLED_P8x16Str(5,4,(uint8_t*)txt);   
  sprintf(txt,"size:%c ",pi_getbuf[2]);
    OLED_P8x16Str(5,6,(uint8_t*)txt);  
}
*/
/*-----------------------------------------------------------------------------------------------------
【编    写】BZY
【功    能】PID循迹程序，没用到
-------------------------------------------------------------------------------------------------------*/
void counter_sum_1(void) {
	
	lamp[0][3] = GPIO_PinRead(PTB3);
	lamp[0][4] = GPIO_PinRead(PTB4);
	lamp[0][5] = GPIO_PinRead(PTB5);
	lamp[0][6] = GPIO_PinRead(PTB6);
	if (lamp[0][3] == 1 && lamp[0][4] == 1 && lamp[0][5] == 1 && lamp[0][6] == 1) {
		if (side == 0) {
			counter_temp++;
			perform = 1;
		}side = 1;
	}
	else if (lamp[0][3] == 0 && lamp[0][4] == 0 && lamp[0][5] == 0 && lamp[0][6] == 0) side = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void zhuanwan(void){
  if (wandao_flag == 1){
    Target_left=600;
    Target_right=600;
    double_flag = 1;
    int a = 1;
    while (a){
        if(double_flag == 0)
        {
          wandao_flag = 0;
          a = 0;
          break;
        }else {
            turn_left();
        }
    }
  }
}