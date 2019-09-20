//
//  motion_planning.c
//  camera smartcar
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//

//动作规划算法文件
//接受上层的1.偏差值，2.最小速度距离，3.路径状态，4.期望速度
//从而改变动作状态和对电机以及舵机的pwm的控制
//文件传递顺序为路径规划->轨迹规划->动作规划

#include "../bsp/board_driver/pwm.h"
#include "path_planning.h"
#include "motion_planning.h"


//赛道运动结构体初始化
SpeedwayMotionNode SpeedwayMotion =
{
    .SpeedwayMotionStatus = STOP,   //初始化动作状态为停止运行
    //舵机控制结构体初始化
    .ServoCtrl.servo_pwm_duty_mid = &PwmInfo.ServoPwm.servo_duty_mid,
    .ServoCtrl.servo_pwm_delta_duty = &PwmInfo.ServoPwm.servo_delta_duty,
    //电机控制结构体初始化
    .MotorCtrl.wheel1_pwm = &PwmInfo.MotorPwm.duty_motor1,
    .MotorCtrl.wheel2_pwm = &PwmInfo.MotorPwm.duty_motor2,
    .MotorCtrl.wheel3_pwm = &PwmInfo.MotorPwm.duty_motor3,
    .MotorCtrl.wheel4_pwm = &PwmInfo.MotorPwm.duty_motor4,
};


//舵机控制(闭环)
void ServoCtrlClosedLoop(void)
{
    SetServoPwmDuty();
}

//电机控制(开环)
void MotorCtrlOpenLoop(void)
{
    //如果出界，则动作状态变为停止
    if(SpeedwayPath.SpeedwayStatus == OUT_OF_BOUND)
    {
        SpeedwayMotion.SpeedwayMotionStatus = STOP;
    }
    
    switch(SpeedwayMotion.SpeedwayMotionStatus)
    {
        case STOP: 
            *SpeedwayMotion.MotorCtrl.wheel1_pwm = 1000;
            *SpeedwayMotion.MotorCtrl.wheel2_pwm = 1000;
            break;

        case NORMAL_DRIVING: 
            *SpeedwayMotion.MotorCtrl.wheel1_pwm = 1300;
            *SpeedwayMotion.MotorCtrl.wheel2_pwm = 1300;
            break;
    }
    SetMotorPwmDuty();
}

//电机控制(闭环)
void MotorCtrlClosedLoop(void)
{
    
}








