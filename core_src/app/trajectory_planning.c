//
//  trajectory_planning.c
//  camera smartcar
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//

#include "trajectory_planning.h"
#include "path_planning.h"
#include "pwm.h"
#include "pid.h"

//舵机打角计算PID结构体初始化
PosiPidNode CountServoPwm_Pid=
{
    .kp = 0.0f,
    .ki = 0.0f,
    .kd = 0.0f,

    .error_curr = 0.0f,
    .error_last = 0.0f,
    .sum_error = 0.0f,

    .limit_out_abs = 1950
};

//计算舵机PWM
void CountServoPwm()
{
    float expc = 0;
    float DeltaX = 0;
    DeltaX = (float)SpeedwayPath.Coordinate.CenterLine[IMG_ROWS/2].x - IMG_COLUMN/2; //得到偏差
    PwmInfo.ServoPwm.servo_delta_duty = (int)CalcPosiPidOut(&CountServoPwm_Pid, expc, DeltaX);   //计算位置式PID
}

//赛道轨迹规划算法 
void TrajectoryPlanning()
{
    CountServoPwm();
}





