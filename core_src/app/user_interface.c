//
//  user_interface.c
//  camera smartcar
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//

#include "../bsp/board_driver/key.h"
#include "../bsp/board_driver/oled.h"
#include "../bsp/board_driver/mt9v034.h"
#include "pid.h"
#include "path_planning.h"
#include "motion_planning.h" 
#include "user_interface.h"
extern PosiPidNode CountServoPwm_Pid;

//结构体初始化
InterfaceCfgNode InterfaceCfg =
{
    .MenuStatus = INFO_START,
    .LineStatus = LINE1,
};

//界面操作逻辑部分
//更改所在行，所在页
void FunctionSelection()
{
    KeyMessage.KeyValue = KeyScan();
    
    switch(KeyMessage.KeyValue)
    {
        case KEY_NULL:
            break;
        
        case KEY_START:
            break;
        
        case KEY_MENU:
            OledCls();
            InterfaceCfg.MenuStatus++;
            if(InterfaceCfg.MenuStatus > INFO_PID)
            {
                InterfaceCfg.MenuStatus = INFO_START;
            }
            break;
        
        case KEY_DIRECTION_UP:
            OledCls();
            InterfaceCfg.LineStatus--;
            if(InterfaceCfg.LineStatus < LINE1)
            {
                InterfaceCfg.LineStatus = LINE8;
            }
            break;
        
        case KEY_DIRECTION_DOWN:
            OledCls();
            InterfaceCfg.LineStatus++;
            if(InterfaceCfg.LineStatus > LINE8)
            {
                InterfaceCfg.LineStatus = LINE1;
            }
            break;
        
        case KEY_ADD:
            break;
        
        case KEY_DECREASE:
            break;
    }
}

//参数调节
 void DebugParameter(void)
 {
    switch(InterfaceCfg.MenuStatus)
    {
        case INFO_START:
            switch(InterfaceCfg.LineStatus)
            {
                case LINE1:
                    DebugI(&MtImgCfg.light_threshold,1);
                    break;
                case LINE2:
                    DebugI(&MtImgCfg.light_threshold_max,1);
                    break;
                case LINE3:
                    DebugI(&MtImgCfg.light_threshold_min,1);
                    break;
                case LINE4:
                    DebugI(&*SpeedwayMotion.MotorCtrl.wheel1_pwm,10);
                    break;
                case LINE5:
                    DebugI(&*SpeedwayMotion.MotorCtrl.wheel2_pwm,10);
                    break;
                case LINE6:
                    DebugI(&*SpeedwayMotion.ServoCtrl.servo_pwm_duty_mid,10);
                    break;
                case LINE7:
                  
                    break;
                case LINE8:
                  
                    break;
            }
            break;
            
        case INFO_IMAGE:
            break;

        case INFO_EDGE:
            break;

        case INFO_PID:
            switch (InterfaceCfg.LineStatus)
            {
            case LINE1:
                DebugF(&CountServoPwm_Pid.kp,1.0f);
                break;
            case LINE2:
                DebugF(&CountServoPwm_Pid.ki,0.01f);
                break;
            case LINE3:
                DebugF(&CountServoPwm_Pid.kd,1.0);
            }
            break;
    }
 }

//OLED显示信息
void OledDisplay(void)
{
    switch(InterfaceCfg.MenuStatus)
    {
        case INFO_START:
            OledShow6x8Str(6*0,LINE1,"threshold");  OledPrintValueI(6*12,LINE1,MtImgCfg.light_threshold);
            OledShow6x8Str(6*0,LINE2,"thre_max");   OledPrintValueI(6*12,LINE2,MtImgCfg.light_threshold_max);
            OledShow6x8Str(6*0,LINE3,"thre_mix");   OledPrintValueI(6*12,LINE3,MtImgCfg.light_threshold_min);
            OledShow6x8Str(6*0,LINE4,"left_pwm");   OledPrintValueI(6*12,LINE4,*SpeedwayMotion.MotorCtrl.wheel1_pwm);
            OledShow6x8Str(6*0,LINE5,"right_pwm");  OledPrintValueI(6*12,LINE5,*SpeedwayMotion.MotorCtrl.wheel2_pwm);
            OledShow6x8Str(6*0,LINE6,"servo_pwm");  OledPrintValueI(6*12,LINE6,*SpeedwayMotion.ServoCtrl.servo_pwm_duty_mid);
            OledShow6x8Str(110,InterfaceCfg.LineStatus,"<-");
            break;
        case INFO_IMAGE:
            OledPrintImage(&ImageBinarizationData[0][0],60,120);
            break;
        case INFO_EDGE:
            OledPrintImage(&ImgProc[0][0],60,120);
            OledShow6x8Str(110,InterfaceCfg.LineStatus,"<-");
            break;
            
        case INFO_PID:
            OledShow6x8Str(6*0,LINE1,"servo_kp");   OledPrintValueF(6*12,LINE1,CountServoPwm_Pid.kp,2);
            OledShow6x8Str(6*0,LINE2,"servo_ki");   OledPrintValueF(6*12,LINE2,CountServoPwm_Pid.ki,2);
            OledShow6x8Str(6*0,LINE3,"servo_kd");   OledPrintValueF(6*12,LINE3,CountServoPwm_Pid.kd,2);
            OledShow6x8Str(110,InterfaceCfg.LineStatus,"<-");
            break;
    }
}


//发车函数
//用于中断中
//set_times:发车时间
void CarStartCheck(int set_times)
{
    static int times = 0;
    static int car_start_flag = 0;
    if(KeyMessage.KeyValue == KEY_START)
    {
        car_start_flag = 1;
    }
    if(car_start_flag == 1)
    {
        times++;
    }
    if(times == set_times)
    {
        times = 0;
        car_start_flag = 0;

        SpeedwayPath.SpeedwayStatus = NORMAL;   //赛道类型正常
        SpeedwayMotion.SpeedwayMotionStatus = NORMAL_DRIVING; //小车正常行驶状态
    }
}


//用户界面(OLED)
void UserInterfaceOled(void)
{
    FunctionSelection();//功能选择
	DebugParameter();//参数调节
	OledDisplay();//OLED屏幕显示
}


