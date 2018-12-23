//
//  motion_planning.h
//  camera smartcar
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//

#ifndef motion_planning_h
#define motion_planning_h


//赛车赛道动作状态结构体
typedef enum
{
    STOP = 0,   //小车停止
    NORMAL_DRIVING,   //小车正常行驶
}SpeedwayMotionStatusNode;


//电机控制结构体
typedef struct
{
    int *wheel1_pwm;
    int *wheel2_pwm;
    int *wheel3_pwm;
    int *wheel4_pwm;
}MotorCtrlNode;

//舵机控制结构体
typedef struct
{
    int *servo_pwm_duty_mid;
    int *servo_pwm_delta_duty;
}ServoCtrlNode;

//小车赛道运动结构体
typedef struct
{
    SpeedwayMotionStatusNode SpeedwayMotionStatus;
    MotorCtrlNode MotorCtrl;
    ServoCtrlNode ServoCtrl;
}SpeedwayMotionNode;

void ServoCtrlClosedLoop(void);//舵机控制(闭环)
void MotorCtrlOpenLoop(void);//电机控制(开环)
void MotorCtrlClosedLoop(void);//电机控制(闭环)
extern SpeedwayMotionNode SpeedwayMotion;


#endif


