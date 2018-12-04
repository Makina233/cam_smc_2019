//
//  lcd_sgp18t.h
//  beacon_car
//
//  此驱动是在龙邱的例程里修改
//

#ifndef lcd_sgp18t_h
#define lcd_sgp18t_h

#include "MK60_port.h"

#define     SGP18T_CS       PTA28
#define     SGP18T_SCK      PTA29
#define     SGP18T_SDI      PTA27
#define     SGP18T_A0       PTA25
#define     SGP18T_RST      PTA24

#define     SGP18T_SCK_H()  PTXn_T(SGP18T_SCK,OUT)=1       
#define     SGP18T_SCK_L()  PTXn_T(SGP18T_SCK,OUT)=0

#define     SGP18T_SDI_H()  PTXn_T(SGP18T_SDI,OUT)=1     
#define     SGP18T_SDI_L()  PTXn_T(SGP18T_SDI,OUT)=0

#define     SGP18T_CS_H()   PTXn_T(SGP18T_CS,OUT)=1    
#define     SGP18T_CS_L()   PTXn_T(SGP18T_CS,OUT)=0

#define     SGP18T_A0_H()   PTXn_T(SGP18T_A0,OUT)=1
#define     SGP18T_A0_L()   PTXn_T(SGP18T_A0,OUT)=0

#define     SGP18T_RST_H()  PTXn_T(SGP18T_RST,OUT)=1
#define     SGP18T_RST_L()  PTXn_T(SGP18T_RST,OUT)=0

#define		RED		0xf800
#define		GREEN		0x07e0
#define		BLUE		0x001f
#define		PURPLE		0xf81f
#define		YELLOW		0xffe0
#define		CYAN		0x07ff 		//蓝绿色
#define		ORANGE		0xfc08
#define		BLACK1		0x0000
#define		WHITE1		0xffff

#define R  0
#define G  1
#define B  2



void LCD_Init(void);
void LCD_display_full(uint16 color);  //全屏显示某种颜色
void LCD_display(uint8 ys ,uint8 xs ,uint8 ye ,uint8 xe ,uint16 color);
void LCD_draw_part(uint8 xs ,uint8 ys ,uint8 xe ,uint8 ye ,uint16 color_dat); //填充矩形区域
void LCD_draw_dot(uint8 x ,uint8 y ,uint16 color_dat); //画点
void LCD_draw_line(uint8 xs ,uint8 ys ,uint8 xe ,uint8 ye ,uint16 color_dat); //画直线
void LCD_draw_rectangle(uint8 xs ,uint8 ys ,uint8 xe ,uint8 ye ,uint16 color_dat); //画矩形边框
void LCD_draw_circle(uint8 x ,uint8 y ,uint8 r ,uint16 color_dat);//画圆形边框
void show_pic(uint8 xs,uint8 ys,uint8 xe,uint8 ye,uint8 *ppic);    //显示图片
void show_img(uint8 xs,uint8 ys,uint8 xe,uint8 ye,uint8 *ppic);
void show_2_img(uint8 xs,uint8 ys,uint8 xe,uint8 ye,uint8 *ppic);

void LCD_P6X8(uint8 x, uint8 y, uint8 c_dat,uint16 word_color,uint16 back_color);
void LCD_P8X8(uint8 x, uint8 y, uint8 c_dat,uint16 word_color,uint16 back_color);
void LCD_P8X16(uint8 x, uint8 y, uint8 c_dat,uint16 word_color,uint16 back_color);
void LCD_P6X8Str(uint8 x, uint8 y, uint8 *s_dat,uint16 word_color,uint16 back_color);
void LCD_P8X8Str(uint8 x, uint8 y, uint8 *s_dat,uint16 word_color,uint16 back_color);
void LCD_P8X16Str(uint8 x, uint8 y, uint8 *s_dat,uint16 word_color,uint16 back_color);

 void LCD18_PrintValueF(u8 x, u8 y, float data, u8 num);
 void LCD_PrintValueI(u8 x, u8 y,signed int data);
 void LCD_PrintValueC(u8 x, u8 y, signed char data);

 void LCD_P6X8NUM(uint8 x, uint8 y, uint16 num, uint8 num_bit,uint16 word_color,uint16 back_color);
 void LCD_P8X8NUM(uint8 x, uint8 y, uint16 num, uint8 num_bit,uint16 word_color,uint16 back_color);



#endif


