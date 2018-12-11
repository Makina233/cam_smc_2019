#ifndef pwm_h
#define pwm_h


//电机PWM结构体
typedef struct
{
    int duty_max;
    int duty_min;
    int duty_motor1;    //电机1PWM占空比
    int duty_motor2;
    int duty_motor3;
    int duty_motor4;
}MotorPwmNode;

//舵机PWM结构体
typedef struct
{
    int servo_delta_duty;
    int servo_duty_mid;
    int servo_duty_min;
    int servo_duty_max;
}ServoPwmNode;

//PWM结构体
typedef struct
{
    MotorPwmNode MotorPwm;
    ServoPwmNode ServoPwm;
}PwmInfoNode;

void DoubleMotorPwmInit(void);//双电机PWM初始化
void FourMotorPwmInit(void);//四电机PWM初始化
void ServoPwmInit(void);//舵机PWM初始化
void SetServoPwmDuty(void);//设置舵机PWM占空比
void SetMotorPwmDuty(void);//设置电机PWM占空比
extern PwmInfoNode PwmInfo;

#endif


