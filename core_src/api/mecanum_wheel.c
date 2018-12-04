//
//  mecanum_wheel.c
//  cam_smc_2019
//
//  Created by fukai on 2018/11.
//  Copyright © 2018年 Cyber01. All rights reserved.
//

#include "mecanum_wheel.h"


//正逆运动学原理 
//Forward and inverse kinematics
//小车俯视图：自转矢量w，逆时针为正，轮子编号左上角为1，逆时针依次递增
//                	^Y
//					|
//					|
//					|
//			2		|		1
//					|
//					|
//	  --------------|------------>X
//					|
//					|
//			3		|		4
//					|


//小车逆运动学分析
//输入：小车的整体期望速度
//输出：小车各个轮子的期望转速
//MecanumWheelParmNode:小车麦克纳姆轮参数结构体
void InverseKinematicsAnalysis(MecanumWheelParmNode *parm)
{
	float k = (parm->car_length + parm->car_width) / 2.0;
	
	parm->WheelEV.wheel1_ev = parm->CarEV.y_axis_ev - parm->CarEV.x_axis_ev + k * parm->CarEV.z_axis_ew;
	parm->WheelEV.wheel2_ev = parm->CarEV.y_axis_ev + parm->CarEV.x_axis_ev - k * parm->CarEV.z_axis_ew;
	parm->WheelEV.wheel3_ev = parm->CarEV.y_axis_ev - parm->CarEV.x_axis_ev - k * parm->CarEV.z_axis_ew;
	parm->WheelEV.wheel4_ev = parm->CarEV.y_axis_ev + parm->CarEV.x_axis_ev + k * parm->CarEV.z_axis_ew;
}

//小车正运动学分析
//输入：小车各个轮子的期望转速
//输出：小车的整体期望速度
//MecanumWheelParmNode:小车麦克纳姆轮参数结构体
void ForwardKinematicsAnalysis(MecanumWheelParmNode *parm)
{
	float k = (parm->car_length + parm->car_width) / 2.0;
	
	parm->CarEV.x_axis_ev = 0.25 * (- parm->WheelEV.wheel1_ev+ parm->WheelEV.wheel2_ev - parm->WheelEV.wheel3_ev + parm->WheelEV.wheel4_ev);
	parm->CarEV.y_axis_ev = 0.25 * ( parm->WheelEV.wheel1_ev + parm->WheelEV.wheel2_ev + parm->WheelEV.wheel3_ev + parm->WheelEV.wheel4_ev);
	parm->CarEV.z_axis_ew = 0.25 * ( parm->WheelEV.wheel1_ev - parm->WheelEV.wheel2_ev - parm->WheelEV.wheel3_ev + parm->WheelEV.wheel4_ev) / k;
}






