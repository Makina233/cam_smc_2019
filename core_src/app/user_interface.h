//
//  user_interface.h
//  camera smartcar
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//

#ifndef user_interface_h
#define user_interface_h


//菜单显示状态
typedef enum
{
    INFO_START = 0,
    INFO_IMAGE,
    INFO_PID,
}MenuStatusNode;

//行显示状态
typedef enum
{
    LINE1 = 0,
    LINE2,
    LINE3,
    LINE4,
    LINE5,
    LINE6,
    LINE7,
    LINE8,
}LineStatusNode;

//显示界面配置结构体
//定义显示的菜单，行数
typedef struct
{
    MenuStatusNode MenuStatus;
    LineStatusNode LineStatus;
}InterfaceCfgNode;


void UserInterfaceOled(void);//用户界面(OLED)


#endif


