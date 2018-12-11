//
//  mecanum_wheel.h
//  cam_smc_2019
//
//  Created by fukai on 2018/11.
//  Copyright © 2018年 Cyber01. All rights reserved.
//

#ifndef mecanum_wheel_h
#define mecanum_wheel_h

//
//正逆运动学原理 Forward and inverse kinematics
//小车俯视图：自转矢量w，逆时针为正，轮子编号右上角为1，逆时针依次递增
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



//小车各个轮子期望转速矢量
typedef struct
{
    float wheel1_ev;	//一号轮期望速度
	float wheel2_ev;	//二号轮期望速度
	float wheel3_ev;	//三号轮期望速度
	float wheel4_ev;	//四号轮期望速度
}WheelEVNode;

//小车整体期望速度矢量
typedef struct
{
	float x_axis_ev;	//x轴期望速度
	float y_axis_ev;	//y轴期望速度
	float z_axis_ew;	//z轴期望角速度，逆时针为正
}CarEVNode;

//小车麦克纳姆轮参数结构体
typedef struct
{
	float car_length;
	float car_width;
	
	WheelEVNode WheelEV;	//小车各个轮子期望转速矢量
	CarEVNode CarEV;		//小车整体期望速度矢量
}MecanumWheelParmNode;




void InverseKinematicsAnalysis(MecanumWheelParmNode *parm);//小车逆运动学分析
void ForwardKinematicsAnalysis(MecanumWheelParmNode *parm);//小车正运动学分析




#endif


