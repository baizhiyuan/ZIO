#ifndef __ZIO_CAR_H
#define __ZIO_CAR_H

typedef struct
{
	float err;
	float last_err;
	float last_last_err;
	float kp,ki,kd;
	float result;
}_pid_t;

/*
#define M_PI 3.1415926
#define _fCut   20                     //低通滤波  截止频率
#define RC 1/(2*M_PI*_fCut)
typedef struct
{
    float                _kp;         //P
    float                _ki;         //I
    float                _kd;         //D
    float                _imax;       //积分限幅
    
    float                _pid_out_p;  //KP输出
    float                _pid_out_i;  //KI输出
    float                _pid_out_d;  //KD输出
    float                _pid_out;    //pid输出
    
    float                _integrator; ///< integrator value
    float                _last_error; ///< last error for derivative
    float                _last_derivative;///< last derivative for low-pass filter
    uint32_t             _last_t;    ///< last time  
    float                scaler;      //比例
}_pid_param_t;

typedef struct
{
    float (* get_pid)(_pid_param_t * pid_param, float error);
    void (* reset)(_pid_param_t * pid_param);
    void (* reset_I)(_pid_param_t * pid_param);
}_pid_s;



_pid_param_t  Motor_left_pid =      //电机PID参数结构体
{
     ._kp             =  1.0 ,         //P
     ._ki             =  0.0 ,         //I
     ._kd             =  0.0 ,         //D
     ._imax           =  500 ,         //积分限幅
                                       
     ._pid_out_p      =   0,           //KP输出
     ._pid_out_i      =   0,           //KI输出
     ._pid_out_d      =   0,           //KD输出
     ._pid_out        =   0,           //pid输出
     
     ._integrator     =   0,           //积分值
     ._last_error     =   0,           //上一次误差
     ._last_derivative=   0,           //上次微分
     ._last_t         =   0,           //上次时间
     .scaler          =   100,         //缩放比

};
_pid_param_t  Motor_right_pid =      //电机PID参数结构体
{
     ._kp             =  1.0 ,         //P
     ._ki             =  0.0 ,         //I
     ._kd             =  0.0 ,         //D
     ._imax           =  500 ,         //积分限幅
                                       
     ._pid_out_p      =   0,           //KP输出
     ._pid_out_i      =   0,           //KI输出
     ._pid_out_d      =   0,           //KD输出
     ._pid_out        =   0,           //pid输出
     
     ._integrator     =   0,           //积分值
     ._last_error     =   0,           //上一次误差
     ._last_derivative=   0,           //上次微分
     ._last_t         =   0,           //上次时间
     .scaler          =   100,         //缩放比

};
_pid_param_t  Imu_pid =               //角度PID参数结构体  用于直立环控制
{
     ._kp             =  12.0 ,
     ._ki             =   0.5 ,
     ._kd             =  0.02 ,
     ._imax           =   30  , 
     
     ._pid_out_p      =   0,  
     ._pid_out_i      =   0,  
     ._pid_out_d      =   0,  
     ._pid_out        =   0,
     
     ._integrator     =   0, 
     ._last_error     =   0, 
     ._last_derivative=   0,
     ._last_t         =   0,  
     .scaler          =   1,  

};
void Control_Motor(int16_t velocity_left, int16_t velocity_right);*/


void Motor_Init(void);
void Infrared_Init(void);
void STEP_Init(void);
void Sys_init_all(void);

void Uart_Init(void);
void Timer_Init(void);
void Timer_IQR_handle();
void Show_IQR_handle();
void Part_Choose(void);
void PART(void);
void PART_all(void);
void start(void);
void GCXL(void);
void Obstacle(void);
void rotating(void);
void rota_filter(void);
void rotate_pid(_pid_t *pid, int16 actual, int16 set);
void run_straight(void);
void turn_left(void);
void turn_right(void);

void PID_Control(_pid_t *pid, int16 actual, int16 set);
void Pid_Init(void);
void Pwm_Init(void);

void Left_front_pwm_set(int16 Left_frontvalue);
void Right_front_pwm_set(int16 Right_frontvalue);
void Left_rear_pwm_set(int16 Left_rearvalue);
void Right_rear_pwm_set(int16 Right_rearvalue);
void All_pwm_set(int16 Left_frontvalue, int16 Right_frontvalue, int16 Left_rearvalue, int16 Right_rearvalue);

void ADC_turn(void);
void read_lamp_values(void);
void counter_sum(void);
void calc_pid(_pid_t *pid);

void track_PID (void);
void rotate_PID(int16_t MAG_Tar);

void diff_speed();

#define SAVE 			0x00
#define CALSW 		0x01
#define RSW 			0x02
#define RRATE			0x03
#define BAUD 			0x04
#define AXOFFSET	0x05
#define AYOFFSET	0x06
#define AZOFFSET	0x07
#define GXOFFSET	0x08
#define GYOFFSET	0x09
#define GZOFFSET	0x0a
#define HXOFFSET	0x0b
#define HYOFFSET	0x0c
#define HZOFFSET	0x0d
#define D0MODE		0x0e
#define D1MODE		0x0f
#define D2MODE		0x10
#define D3MODE		0x11
#define D0PWMH		0x12
#define D1PWMH		0x13
#define D2PWMH		0x14
#define D3PWMH		0x15
#define D0PWMT		0x16
#define D1PWMT		0x17
#define D2PWMT		0x18
#define D3PWMT		0x19
#define IICADDR		0x1a
#define LEDOFF 		0x1b
#define GPSBAUD		0x1c

#define YYMM				0x30
#define DDHH				0x31
#define MMSS				0x32
#define MS					0x33
#define AX					0x34
#define AY					0x35
#define AZ					0x36
#define GX					0x37
#define GY					0x38
#define GZ					0x39
#define HX					0x3a
#define HY					0x3b
#define HZ					0x3c			
#define Roll				0x3d
#define Pitch				0x3e
#define Yaw					0x3f
#define TEMP				0x40
#define D0Status		0x41
#define D1Status		0x42
#define D2Status		0x43
#define D3Status		0x44
      
#define DIO_MODE_AIN 0
#define DIO_MODE_DIN 1
#define DIO_MODE_DOH 2
#define DIO_MODE_DOL 3
#define DIO_MODE_DOPWM 4
#define DIO_MODE_GPS 5		


#endif