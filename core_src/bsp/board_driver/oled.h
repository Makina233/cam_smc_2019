#ifndef oled_h
#define oled_h



#include "MK60_gpio.h"
#include "MK60_port.h"


//初始化引脚定义
//直接在这里更改IO口即可
#define SCL_PIN PTA29
#define SDA_PIN PTA27
#define RST_PIN PTA25
#define DC_PIN  PTA24

#define SCL_OUT PTA29_OUT
#define SDA_OUT PTA27_OUT
#define RST_OUT PTA25_OUT
#define DC_OUT  PTA24_OUT


#define INIT 0

#define LED_SCL_Init  gpio_init(SCL_PIN,GPO,INIT) // 时钟初始化定义
#define LED_SDA_Init  gpio_init(SDA_PIN,GPO,INIT) // 数据口D0
#define LED_RST_Init  gpio_init(RST_PIN,GPO,INIT) //复位低能电平
#define LED_DC_Init   gpio_init(DC_PIN,GPO,INIT)  //偏置常低


//IO口定义OUTPUT
//直接在这里改IO就可以了
#define LED_SCLH  (SCL_OUT=1)// 时钟定义
#define LED_SCLL  (SCL_OUT=0)

#define LED_SDAH  (SDA_OUT=1)// 数据口D0
#define LED_SDAL  (SDA_OUT=0)

#define LED_RSTH  (RST_OUT=1)// 复位低能电平
#define LED_RSTL  (RST_OUT=0)

#define LED_DCH   (DC_OUT=1)
#define LED_DCL   (DC_OUT=0) // 偏置常低


void OLED_Init(void);
void LED_Fill(uint8 bmp_data);
void LED_CLS(void);//清屏函数
void LED_P6x8Char(uint8 x,uint8 y,uint8 ch);
void LED_P6x8Str(uint8 x,uint8 y,uint8 ch[]);
void LED_P8x16Str(uint8 x,uint8 y,uint8 ch[]);
void LED_P14x16Str(uint8 x,uint8 y,uint8 ch[]);
void LED_PXx16MixStr(uint8 x, uint8 y, uint8 ch[]);
void LED_PrintValueC(u8 x, u8 y, signed char data);
void LED_PrintValueI(u8 x, u8 y,signed int data);
void LED_PrintValueF(u8 x, u8 y, float data, u8 num);
void LED_PrintImage(uint8 *pucTable, uint16 usRowNum, uint16 usColumnNum);
void LED_Print_I(u8 x,u8 y,int data);
void showI(int x,int y,unsigned char s[],int c);
void showF(int x,int y,unsigned char s[],float data,u8 a);
void LED_virtual_oscilloscope(int data,int scope_top,int scope_buttom ,int scope_center,char *str);







#endif


