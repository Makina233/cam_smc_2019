//
//  main.c
//  camera smartcar
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//

#include "../bsp/board_driver/key.h"
#include "../bsp/board_driver/oled.h"
#include "../bsp/board_driver/lcd_sgp18t.h"
#include "../bsp/board_driver/mt9v034.h"
#include "../bsp/board_driver/pwm.h"
#include "path_planning.h"
#include "timer_interrupt.h"
#include "user_interface.h"


//定义屏幕使用OLED or LCD
#define OLED 
//#define LCD

//定义摄像头的使用
#define MT9V034         //神眼摄像头
//#define OV7725_EAGLE  //鹰眼摄像头

//定义使用模式
#define SPEEDWAY        //赛车跑道
//#define BEACON        //信标

//定义主从机(master or slave)
#define MASTER          //主机
//#define SLAVE         //从机


//定义主机主循环状态机
typedef enum
{
    MASTER_GET_INFO,        //摄像头获取信息
    MASTER_ANALYZE_INFO,    //处理采集到的信息
    MASTER_SHOW_INFO,       //显示信息
    MASTER_SEND_INFO,       //发送信息
}MasterMainStatusNode;
//初始化主机循环状态机
MasterMainStatusNode MasterMainStatus = MASTER_GET_INFO;

//硬件初始化
void BoardInit(void)
{
    KeyInit();
    SwitchKeyInit();
    
    #ifdef OLED
    OledInit();
    #elif LCD
    LCD_Init();
    LCD_display_full(BLACK1);
    #endif
    
    #ifdef MT9V034
    Mt9v034Init();
    #endif
    
    #ifdef SPEEDWAY
    DoubleMotorPwmInit();
    #elif BEACON
    //FourMotorPwmInit();
    #endif
    
    ServoPwmInit();
    pit_init_us(PIT0, 3000);    //初始化PIT0，定时时间为： 3ms
}

//系统初始化
void SystemInit(void)
{
    SetIrq();
}

int main()
{
    BoardInit();    //硬件初始化
    SystemInit();   //系统初始化

    #ifdef MASTER
    Mt9v034Status = START;  //首先让摄像头准备采集(等待场中断)
    while(1)
    {
        switch(MasterMainStatus)
        {
            case MASTER_GET_INFO:       //信息采集
                if(Mt9v034Status == FINISH) //此时摄像头一帧图像已采集完成，开始进行二值化处理
                {
                    //GetBinarizedImage();//图像二值化处理
                    AutoBinarizedImage();//图像自动阈值二值化
                    Mt9v034Status = START;  //处理完成后继续等待场中断触发
                    MasterMainStatus = MASTER_ANALYZE_INFO;
                }
                break;
                
            case MASTER_ANALYZE_INFO:   //信息分析
                SpeedwayPathPlanning();//赛道路径规划算法
                TrajectoryPlanning();//赛道轨迹规划算法
                MasterMainStatus = MASTER_SHOW_INFO;
                break;
                
            case MASTER_SHOW_INFO:      //信息显示
                UserInterfaceOled();
                //show_img(5,0,119+5,59,(uint8 *)MtImgCfg.image_cmprs);
                //show_2_img(5,80,119+5,139,(uint8 *)MtImgCfg.image_binarization);
                MasterMainStatus = MASTER_SEND_INFO;
                break;
                
            case MASTER_SEND_INFO:      //信息发送
              
                MasterMainStatus = MASTER_GET_INFO;
                break;
        }
    }
    #endif
}

