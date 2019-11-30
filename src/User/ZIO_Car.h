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

void fenduan(void);                                                                  //后轮速度分段函数
void target_slow(int tar,int time);                                                //匀加减速

void Motor_Init(void);                                                                //电机初始化
void Infrared_Init(void);                                                             //循迹初始化
void STEP_Init(void);                                                                //步进电机初始化
void Uart_Init(void);
void Timer_Init(void);
void Timer_IQR_handle();
void Show_IQR_handle();
void Sys_init_all(void);                                                             //所有的初始化
void Posit_slow(int tarnum,int tarspeed);

void zhuanwan(void);

void GCXL(void);
void Part_Choose(void);
void PART(void);
void PART_all(void);
void start(void);
void rotate_bzy(void);
void Obstacle(void);
void rotating(void);
void rota_filter(void);
void rotate_pid(_pid_t *pid, int16 actual, int16 set);
void run_straight(void);
void turn_left(void);
void turn_right(void);
void read_lamp_back(void);
void Posit_up(int tarnum,int tarspeed);
void Pos_flags(int tarnum, int tarspeed);
void Posit(int tarnum, int tarspeed);
void PID_Control(_pid_t *pid, int16 actual, int16 set);
void Pid_Init(void);
void Pwm_Init(void);
//void Show_PI(void);
void material_bi(void);
void Pos_back(int tarnum);
void Pos_back_2(int tarnum);
void Left_front_pwm_set(int16 Left_frontvalue);
void Right_front_pwm_set(int16 Right_frontvalue);
void Left_rear_pwm_set(int16 Left_rearvalue);
void Right_rear_pwm_set(int16 Right_rearvalue);
void All_pwm_set(int16 Left_frontvalue, int16 Right_frontvalue, int16 Left_rearvalue, int16 Right_rearvalue);
void shangshang(void);
void ADC_turn(void);
void read_lamp_values(void);
void counter_sum(void);
void counter_sum_1(void);
void calc_pid(_pid_t *pid);
void shangroate(int tarnum,int tarspeed);
void track_PID (void);
void rotate_PID(int16_t MAG_Tar);
void Posit_p(int tarnum,int tarspeed);
void Posit_k(int tarnum,int tarspeed);
void diff_speed();
void target_slow_t(int tar,int time);
void track_houtui(void);
void weizhi_pid(void);
void complete(void);
void Posit_stop(int tarnum, int tarspeed);
void speed_control(void);
void zhuanwan(void);
void chasu(float Turn_direction);
void chasu_speed(void);
void material_bi_blanking(void);
void material_bi_feeding(void);


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



float get_pid(_pid_param_t * pid_param, float error);
void reset(_pid_param_t * pid_param) ;
void reset_I(_pid_param_t * pid_param);
#endif
