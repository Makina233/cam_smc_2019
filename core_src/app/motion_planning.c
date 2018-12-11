//
//  motion_planning.c
//  camera smartcar
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//

#include "../bsp/board_driver/pwm.h"
#include "motion_planning.h"


//舵机控制结构体初始化
ServoCtrlNode ServoCtrl =
{
    .servo_pwm_duty_mid = &PwmInfo.ServoPwm.servo_duty_mid,
    .servo_pwm_delta_duty = &PwmInfo.ServoPwm.servo_delta_duty,
};

//电机控制结构体初始化
MotorCtrlNode MotorCtrl =
{
    .wheel1_pwm = &PwmInfo.MotorPwm.duty_motor1,
    .wheel2_pwm = &PwmInfo.MotorPwm.duty_motor2,
    .wheel3_pwm = &PwmInfo.MotorPwm.duty_motor3,
    .wheel4_pwm = &PwmInfo.MotorPwm.duty_motor4,
};


//舵机控制(闭环)
void ServoCtrlClosedLoop(void)
{
    SetServoPwmDuty();
}

//电机控制(开环)
void MotorCtrlOpenLoop(void)
{
    SetMotorPwmDuty();
}

//电机控制(闭环)
void MotorCtrlClosedLoop(void)
{
    
}








