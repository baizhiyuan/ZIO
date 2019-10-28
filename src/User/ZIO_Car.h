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
void start(void);
void GCXL(void);

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

void diff_speed();



#endif