//
//  key.c
//  camera smartcar
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//
#include "common.h"
#include "MK60_port.h"  //port复用配置和各种功能配置
#include "MK60_gpio.h"
#include "key.h"


/********按键部分********/

KeyMessageNode KeyMessage=
{
	.long_press_mode = 0,	//关闭按键长按模式
	.long_press_check = 0,	//关闭按键长按检测
	.long_press_time = 0,	//初始化按键长按检测时间
};

//简单延时
static void KeyDelay(unsigned int time)
{
	while(--time);
}

//按键初始化函数
void KeyInit(void)
{
	gpio_init(KEY1,GPI,HIGH);
    gpio_init(KEY2,GPI,HIGH);
    gpio_init(KEY3,GPI,HIGH);
    gpio_init(KEY4,GPI,HIGH);
    gpio_init(KEY5,GPI,HIGH);
    gpio_init(KEY6,GPI,HIGH);
    port_init_NoALT(KEY1,PULLUP);
    port_init_NoALT(KEY2,PULLUP);
    port_init_NoALT(KEY3,PULLUP);
    port_init_NoALT(KEY4,PULLUP);
    port_init_NoALT(KEY5,PULLUP);
    port_init_NoALT(KEY6,PULLUP);
}

//按键长按检测
//需要用于定时器中断当中
void KeyContinuousCheck(void)
{
	if(KeyMessage.long_press_check == 1)	//开始按键长按检测
	{
		++KeyMessage.long_press_time;
	}
	else
	{
		KeyMessage.long_press_time = 0;
	}
	
	if(KeyMessage.long_press_time == 200)	//已经按下了x的时间
	{
		KeyMessage.long_press_time = 0;
		KeyMessage.long_press_mode = 1;
	}
}

//按键扫描
KeyModeNode KeyScan(void)
{
	static unsigned char key_scan_flag = 1;	//按键扫描标志
	
	if(KeyMessage.long_press_mode == 1)
	{
		key_scan_flag = 1;
	}
	
	if(key_scan_flag &&(KEY1_READ==0 || KEY2_READ==0 || KEY3_READ==0 || KEY4_READ==0 || KEY5_READ==0 || KEY6_READ==0))
	{
		KeyDelay(50);	//消除抖动
		key_scan_flag = 0;	//按键扫描关闭
		//限制特定的按键才能启动长按功能
		//if(KEY2_READ==0 || KEY3==0 || KEY4==0 || KEY5==0)
		{
			KeyMessage.long_press_check = 1;	//启动按键长按检测
		}
		if(KeyMessage.long_press_mode == 1)
		{
			KeyDelay(600000);	//防止长按时数据变化太快
			//KeyDelay(600000);	//防止长按时数据变化太快
		}
		
		//按键返回键值		
		if(KEY2_READ==0)			return KEY_START;
		else if(KEY1_READ==0)		return KEY_MENU;
		else if(KEY4_READ==0)		return KEY_DIRECTION_UP;
		else if(KEY3_READ==0)		return KEY_ADD;
		else if(KEY6_READ==0)		return KEY_DIRECTION_DOWN;
		else if(KEY5_READ==0)		return KEY_DECREASE;
	}
	else if(KEY1_READ==1&&KEY2_READ==1&&KEY3_READ==1&&KEY4_READ==1&&KEY5_READ==1&&KEY6_READ==1)
	{
		key_scan_flag = 1;
		KeyMessage.long_press_check = 0;	//关闭按键长按检测
		KeyMessage.long_press_mode = 0;	//关闭按键长按模式
	}	
	return KEY_NULL;
}

//调试函数      
//Regulated_quantity:被调量
//number：调节量
void DebugC(char *Regulated_quantity,char number) 
{
    if(KeyMessage.KeyValue==KEY_ADD)
      *Regulated_quantity+=number;
    if(KeyMessage.KeyValue==KEY_DECREASE)
      *Regulated_quantity-=number;
}

void DebugI(int *Regulated_quantity,int number) 
{
    if(KeyMessage.KeyValue==KEY_ADD)
      *Regulated_quantity+=number;
    if(KeyMessage.KeyValue==KEY_DECREASE)
      *Regulated_quantity-=number;
}

void DebugD(double *Regulated_quantity,double number) 
{
    if(KeyMessage.KeyValue==KEY_ADD)
      *Regulated_quantity+=number;
    if(KeyMessage.KeyValue==KEY_DECREASE)
      *Regulated_quantity-=number;
}

void DebugF(float *Regulated_quantity,float number) 
{
    if(KeyMessage.KeyValue==KEY_ADD)
      *Regulated_quantity+=number;
    if(KeyMessage.KeyValue==KEY_DECREASE)
      *Regulated_quantity-=number;
}

/********拨码开关部分********/

//拨码开关初始化
void SwitchKeyInit(void)
{
    //gpio_init(SWITCH1,GPI,HIGH);
    //gpio_init(SWITCH2,GPI,HIGH);
    //gpio_init(SWITCH3,GPI,HIGH);
    //gpio_init(SWITCH4,GPI,HIGH);
    gpio_init(SWITCH5,GPI,HIGH);
    gpio_init(SWITCH6,GPI,HIGH);
    gpio_init(SWITCH7,GPI,HIGH);
    gpio_init(SWITCH8,GPI,HIGH);
    //port_init_NoALT(SWITCH1,PULLUP);
    //port_init_NoALT(SWITCH2,PULLUP);
    //port_init_NoALT(SWITCH3,PULLUP);
    //port_init_NoALT(SWITCH4,PULLUP);
    port_init_NoALT(SWITCH5,PULLUP);
    port_init_NoALT(SWITCH6,PULLUP);
    port_init_NoALT(SWITCH7,PULLUP);
    port_init_NoALT(SWITCH8,PULLUP);
}








