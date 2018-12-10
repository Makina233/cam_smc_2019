#ifndef pwm_h
#define pwm_h


//电机PWM结构体
typedef struct
{
    unsigned int duty_motor1;    //电机1PWM占空比
    unsigned int duty_motor2;
    unsigned int duty_motor3;
    unsigned int duty_motor4;
}MotorPwmNode;

//舵机PWM结构体
typedef struct
{
    unsigned int servo_duty_mid;
    unsigned int servo_duty_min;
    unsigned int servo_duty_max;
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
extern PwmInfoNode PwmInfo;

#endif


