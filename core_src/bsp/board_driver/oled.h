//
//  oled.h
//  camera smartcar
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//

#ifndef oled_h
#define oled_h


#include "common.h"
#include "MK60_gpio.h"
#include "MK60_port.h"


//初始化引脚定义
//直接在这里更改IO口即可
#define OLED_SCL_PIN    PTA29
#define OLED_SDA_PIN    PTA27
#define OLED_RES_PIN    PTA25
#define OLED_DC_PIN     PTA24

#define OLED_SCL_OUT    PTA29_OUT
#define OLED_SDA_OUT    PTA27_OUT
#define OLED_RES_OUT    PTA25_OUT
#define OLED_DC_OUT     PTA24_OUT

#define OledDelayMs(ms)     OledDelayMs(ms)

//OLED端口定义
//OLED的时钟引脚(SCL)
#define OLED_SCL_L      (OLED_SCL_OUT = 0)
#define OLED_SCL_H      (OLED_SCL_OUT = 1)
//OLED的数据引脚(SDA)
#define OLED_SDA_L      (OLED_SDA_OUT = 0)
#define OLED_SDA_H      (OLED_SDA_OUT = 1)
//OLED的复位引脚(RES)
#define OLED_RST_L      (OLED_RES_OUT = 0)
#define OLED_RST_H      (OLED_RES_OUT = 1)
//OLED的数据/命令控制引脚(DC)
#define OLED_DC_L       (OLED_DC_OUT = 0)
#define OLED_DC_H       (OLED_DC_OUT = 1)

//模式
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//函数声明
void OledCls(void);//清屏函数
void OledInit(void);//OLED初始化
void OledShow6x8Char(unsigned char x, unsigned char y, char ch);//显示一个6x8标准ASCII字符
void OledShow6x8Str(unsigned char x, unsigned char y, char *ch);//写入一组6x8标准ASCII字符串
void OledPrintValueI(unsigned char x, unsigned char y, int data);//将一个整型数转换成5位数进行显示	
void OledPrintValueF(unsigned char x, unsigned char y, float data, unsigned char num);//将一个float型数转换成5位数带小数和符号的数据进行显示
void OledPrintImage(unsigned char *pucTable, unsigned int usRowNum, unsigned int usColumnNum);//显示图像
void OledTimeValueShow(unsigned char x, unsigned char y, int data);//用于时间显示
void OledYearValueShow(unsigned char x, unsigned char y, int data);//年份显示


#endif


