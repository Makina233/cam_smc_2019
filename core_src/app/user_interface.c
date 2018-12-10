//
//  user_interface.c
//  camera smartcar
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//

#include "../bsp/board_driver/key.h"
#include "../bsp/board_driver/oled.h"
#include "../bsp/board_driver/mt9v034.h"
#include "user_interface.h"

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
                  
                    break;
                case LINE5:
                  
                    break;
                case LINE6:
                  
                    break;
                case LINE7:
                  
                    break;
                case LINE8:
                  
                    break;
            }
            break;
            
        case INFO_IMAGE:
            break;
            
        case INFO_PID:
            
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
            OledShow6x8Str(110,InterfaceCfg.LineStatus,"<-");
            break;
        case INFO_IMAGE:
            OledPrintImage(&ImageBinarizationData[0][0],60,120);
            break;
        case INFO_PID:
            OledShow6x8Str(110,InterfaceCfg.LineStatus,"<-");
            break;
    }
}

//用户界面(OLED)
void UserInterfaceOled(void)
{
    FunctionSelection();//功能选择
	DebugParameter();//参数调节
	OledDisplay();//OLED屏幕显示
}


