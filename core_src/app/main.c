#include "common.h"
#include "key.h"
#include "oled.h"
#include "lcd_sgp18t.h"
#include "../bsp/board_driver/mt9v034.h"
#include "timer_interrupt.h"


//主机主循环状态机
typedef enum
{
    MASTER_GET_INFO,        //摄像头获取信息
    MASTER_ANALYZE_INFO,    //处理采集到的信息
    MASTER_SHOW_INFO,       //显示信息
    MASTER_SEND_INFO,       //发送信息
}MasterMainStatusNode;

MasterMainStatusNode MasterMainStatus = MASTER_GET_INFO;

//硬件初始化
void BoardInit(void)
{
    //OLED_Init();
    SwitchKeyInit();
    Mt9v034Init();
    LCD_Init();
    LCD_display_full(BLACK1);
}

//系统初始化
void SystemInit(void)
{
    Set_IRQ();
}

int main()
{
    BoardInit();    //硬件初始化
    SystemInit();   //系统初始化
    
    //Mt9v034Status = START;  //首先让摄像头准备采集(等待场中断)
    while(1)
    {
        /*
        switch(MasterMainStatus)
        {
            case MASTER_GET_INFO:       //信息采集
                if(Mt9v034Status == FINISH) //此时摄像头一帧图像已采集完成，开始进行二值化处理
                {
                    GetBinarizedImage();//图像二值化处理
                    Mt9v034Status = START;  //处理完成后继续等待场中断触发
                    MasterMainStatus = MASTER_ANALYZE_INFO;
                }
                break;
                
            case MASTER_ANALYZE_INFO:   //信息分析
              
                MasterMainStatus = MASTER_SHOW_INFO;
                break;
                
            case MASTER_SHOW_INFO:      //信息显示
                //LED_PrintImage((uint8 *)ImageBinarizationData,60,120);
                show_img(0,0,119,59,(uint8 *)ImageCmprsData);
                MasterMainStatus = MASTER_SEND_INFO;
                break;
                
            case MASTER_SEND_INFO:      //信息发送
              
                MasterMainStatus = MASTER_GET_INFO;
                break;
        }
        */
    }
}

