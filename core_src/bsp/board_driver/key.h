//
//  key.h
//  camera smartcar
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//

#ifndef key_h
#define key_h

 
/********按键部分********/

//此处定义按键IO口
#define KEY1        PTA11
#define KEY2        PTA10
#define KEY3        PTA9
#define KEY4        PTA8
#define KEY5        PTA7
#define KEY6        PTA6

#define KEY1_READ   PTA11_IN
#define KEY2_READ   PTA10_IN
#define KEY3_READ   PTA9_IN
#define KEY4_READ   PTA8_IN
#define KEY5_READ   PTA7_IN
#define KEY6_READ   PTA6_IN    

//定义按键状态
#define KEY_ON		0	//按键按下
#define KEY_OFF		1	//按键松开

//按键功能
typedef enum
{
	KEY_NULL = 0,	//无按键按下
	KEY_START,	//菜单键
	KEY_MENU,	//返回键
	KEY_DIRECTION_UP,//方向上键
	KEY_DIRECTION_DOWN,//方向下键
	KEY_ADD,//加
	KEY_DECREASE,//减
}KeyModeNode;

//按键结构体
typedef struct
{
	unsigned char long_press_mode;	//按键长按模式启动,1为长按启动，0为长按关闭
	unsigned char long_press_check;	//按键长按检查
	unsigned char long_press_time;	//按键长按检测时间
	
	KeyModeNode KeyValue;
}KeyMessageNode;


extern KeyMessageNode KeyMessage;

void KeyInit(void);	//按键初始化函数
void KeyContinuousCheck(void);	//按键长按检测
KeyModeNode KeyScan(void);	//按键扫描
void DebugC(char *Regulated_quantity,char number);
void DebugI(int *Regulated_quantity,int number);
void DebugD(double *Regulated_quantity,double number);
void DebugF(float *Regulated_quantity,float number);


/********拨码开关部分********/

//拨码开关IO口定于
#define SWITCH1             PTA19
#define SWITCH2             PTA18
#define SWITCH3             PTA17
#define SWITCH4             PTA16
#define SWITCH5             PTA15
#define SWITCH6             PTA14
#define SWITCH7             PTA13
#define SWITCH8             PTA12
#define SWITCH1_READ        PTA19_IN
#define SWITCH2_READ        PTA18_IN
#define SWITCH3_READ        PTA17_IN
#define SWITCH4_READ        PTA16_IN
#define SWITCH5_READ        PTA15_IN
#define SWITCH6_READ        PTA14_IN
#define SWITCH7_READ        PTA13_IN
#define SWITCH8_READ        PTA12_IN




void SwitchKeyInit(void);




#endif


