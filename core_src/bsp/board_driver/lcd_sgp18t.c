//
//  lcd_sgp18t.c
//  beacon_car
//
//  此驱动是在龙邱的例程里修改
//


#include "common.h"
#include "MK60_gpio.h"  
#include "lcd_sgp18t.h"


//IC寄存器寻址  8080 SERIES
void LCD_write_command(uint8 cmd)
{
    uint8 i;

    SGP18T_A0_L();;;;   //A0=0发送命令
    SGP18T_CS_L();;;;   
    for(i=0;i<8;i++)
    {
        SGP18T_SCK_L();;;;

        if(cmd&0x80)
        {
            SGP18T_SDI_H();;;;;
        }
        else
        {
            SGP18T_SDI_L();;;;;
        }

        SGP18T_SCK_H();;;;
        cmd=(cmd<<1);
    }
    SGP18T_CS_H();;;;
}

//IC寄存器写数据 8080 SERIES
void LCD_write_para8(uint8 dat)
{
    uint8 i;

    SGP18T_A0_H();;;;       //A0=1发送数据  
    SGP18T_CS_L();;;;
    for(i=0;i<8;i++)
    {
        SGP18T_SCK_L();;;;

        if(dat&0x80)
        {
            SGP18T_SDI_H();;;;;
        }
        else
        {
            SGP18T_SDI_L();;;;;
        }

        SGP18T_SCK_H();;;;
        dat=(dat<<1);
    }
    SGP18T_CS_H();;;;
}

// IC GDRAM 写数据 16BITS 8080 SERIES
void LCD_write_para16(uint16 dat)
{
    uint8 i,buf;

    SGP18T_A0_H();;;;       //A0=1发送数据  
    SGP18T_CS_L();;;;

    buf=(uint8)(0xFF&(dat>>8));
    for(i=0;i<8;i++)
    {
        SGP18T_SCK_L();;;;
        if(buf&0x80)
        {
            SGP18T_SDI_H();;;;;
        }
        else
        {
            SGP18T_SDI_L();;;;;
        }
        SGP18T_SCK_H();;;;
        buf=(buf<<1);
    }
    SGP18T_CS_H();;;;

    buf=(uint8)(0xFF&(dat));
    SGP18T_CS_L();;;;
    for(i=0;i<8;i++)
    {
        SGP18T_SCK_L();;;;
        if(buf&0x80)
        {
            SGP18T_SDI_H();;;;;
        }
        else
        {
            SGP18T_SDI_L();;;;;
        }
        SGP18T_SCK_H();;;;
        buf=(buf<<1);
    }
    SGP18T_CS_H();;;;
}

//lcd初始化
void LCD_Init(void)
{
    gpio_init(SGP18T_A0,GPO,1);
    gpio_init(SGP18T_CS,GPO,1);
    gpio_init(SGP18T_RST,GPO,1);
    gpio_init(SGP18T_SCK,GPO,1);
    gpio_init(SGP18T_SDI,GPO,1);

    SGP18T_RST_L();;;       
    DELAY_MS(5);
    SGP18T_RST_H();;;;       
    DELAY_MS(10);

    LCD_write_command(0x11);    //关闭睡眠，振荡器工作
    DELAY_MS(2);

    LCD_write_command(0x3a);    //每次传送16位数据，每个像素16位
    LCD_write_para8(0x55);

    LCD_write_command(0x26);    
    LCD_write_para8(0x04);

    LCD_write_command(0xf2);    
    LCD_write_para8(0x01);

    LCD_write_command(0xe0);    
    LCD_write_para8(0x3f);
    LCD_write_para8(0x25);
    LCD_write_para8(0x1c);
    LCD_write_para8(0x1e);
    LCD_write_para8(0x20);
    LCD_write_para8(0x12);
    LCD_write_para8(0x2a);
    LCD_write_para8(0x90);
    LCD_write_para8(0x24);
    LCD_write_para8(0x11);
    LCD_write_para8(0x00);
    LCD_write_para8(0x00);
    LCD_write_para8(0x00);
    LCD_write_para8(0x00);
    LCD_write_para8(0x00);

    LCD_write_command(0xe1);    
    LCD_write_para8(0x20);
    LCD_write_para8(0x20);
    LCD_write_para8(0x20);
    LCD_write_para8(0x20);
    LCD_write_para8(0x05);
    LCD_write_para8(0x00);
    LCD_write_para8(0x15);
    LCD_write_para8(0xa7);
    LCD_write_para8(0x3d);
    LCD_write_para8(0x18);
    LCD_write_para8(0x25);
    LCD_write_para8(0x2a);
    LCD_write_para8(0x2b);
    LCD_write_para8(0x2b);
    LCD_write_para8(0x3a);

    LCD_write_command(0xb1);        //设置屏幕刷新率
    LCD_write_para8(0x08);          //DIVA=8
    LCD_write_para8(0x08);          //VPA=8,约90Hz

    LCD_write_command(0xb4);    
    LCD_write_para8(0x07);      //NLA=1,NLB=1,NLC=1

    LCD_write_command(0xc0);    
    LCD_write_para8(0x0a);
    LCD_write_para8(0x02);

    LCD_write_command(0xc1);    
    LCD_write_para8(0x02);

    LCD_write_command(0xc5);    
    LCD_write_para8(0x4f);
    LCD_write_para8(0x5a);

    LCD_write_command(0xc7);    
    LCD_write_para8(0x40);

    LCD_write_command(0x2a);        //配置MCU可操作的LCD内部RAM横坐标的起始、结束参数  
    LCD_write_para8(0x00);          //横坐标的起始地址0x0000
    LCD_write_para8(0x00);
    LCD_write_para8(0x00);          //横坐标的结束地址0x007f(127)
    LCD_write_para8(0x7f);

    LCD_write_command(0x2b);        //配置MCU可操作的LCD内部RAM纵坐标的起始、结束参数  
    LCD_write_para8(0x00);          //纵坐标的起始地址0x0000
    LCD_write_para8(0x00);
    LCD_write_para8(0x00);          //纵坐标的结束地址0x009f(159)
    LCD_write_para8(0x9f);

    LCD_write_command(0x36);    //配置MPU和DDRAM的对应关系
    LCD_write_para8(0xc0);      //MX=1,MY=1

    LCD_write_command(0xb7);    
    LCD_write_para8(0x00);      //CRL=0

    LCD_write_command(0x29);    //开启屏幕显示  
    LCD_write_para8(0x2c);      //设置为LCD接受数据/命令模式

}


//设置终止坐标  重新定位输入信息位置
void LCD_SetPos(uint8 xs,uint8 ys,uint8 xe,uint8 ye)
{
    LCD_write_command(0x2A);    //Column addRSTs set  
    LCD_write_para8(0x00);      //行起始坐标高8位，始终为零
    LCD_write_para8(xs);        //行起始坐标低8位   
    LCD_write_para8(0x00);      //行终止坐标高8位，始终为零
    LCD_write_para8(xe);        //行终止坐标低8位

    LCD_write_command(0x2B);    //Column addRSTs set  
    LCD_write_para8(0x00);      //列起始坐标高8位，始终为零
    LCD_write_para8(ys);        //列起始坐标低8位   
    LCD_write_para8(0x00);      //列终止坐标高8位，始终为零
    LCD_write_para8(ye);        //列终止坐标低8位

    LCD_write_command(0x2C);    //GRAM接受MCU数据或命令
}

//重制地址
void LCD_address_rst(void)
{
    LCD_write_command(0x2a);    //配置MCU可操作的LCD内部RAM横坐标起始、结束参数  
    LCD_write_para8(0x00);      //横坐标的起始地址0x0000
    LCD_write_para8(0x00);
    LCD_write_para8(0x00);      //横坐标结束地址0x007f(127)
    LCD_write_para8(0x7f);

    LCD_write_command(0x2b);    //配置MCU可操作的LCD内部RAM纵坐标的起始、结束参数  
    LCD_write_para8(0x00);      //纵坐标的起始地址0x0000
    LCD_write_para8(0x00);
    LCD_write_para8(0x00);      //纵坐标的结束地址0x009f(159)
    LCD_write_para8(0x9f);

    LCD_write_command(0x2C);    //GRAM接受MCU数据或命令 
}


//全屏显示单色画面
//color为常用的需要显示的颜色（已经提前定义）
void LCD_display_full(uint16 color)
{
    unsigned int i,j;

    LCD_address_rst();

    for(i=0;i<160;i++)
    {
        for(j=0;j<128;j++)
        {
            LCD_write_para16(color);
        }
    }
}

void LCD_display(uint8 ys ,uint8 xs ,uint8 ye ,uint8 xe ,uint16 color)
{
    unsigned int i,j;

    LCD_SetPos(xs,ys,xe,ye);

    for(i=0;i<ye-ys;i++)
    {
        for(j=0;j<xe-xs;j++)
        {
            LCD_write_para16(color);
        }
    }
}

//填充矩形区域
//起始、终止横坐标（0-127），纵坐标（0-159），显示颜色uint16
//0<=xs<xe<=127    0<=ys<ye<=159
void LCD_draw_part(uint8 xs ,uint8 ys ,uint8 xe ,uint8 ye ,uint16 color_dat)
{
    uint8 i,j;

    LCD_SetPos(xs,ys,xe,ye);

    for(j=0;j<(ye-ys+1);j++)
    {
        for(i=0;i<(xe-xs+1);i++)
        {
            LCD_write_para16(color_dat);
        }
    }
}

//画点
//起始、终止横坐标（0-127），纵坐标（0-159），显示颜色uint16
//0<=xs<xe<=127    0<=ys<ye<=159
void LCD_draw_dot(uint8 x ,uint8 y ,uint16 color_dat)
{
    LCD_SetPos(x,y,x,y);
    LCD_write_para16(color_dat);
}


//画直线
//起始、终止横坐标（0-127），纵坐标（0-159），显示颜色uint16
//0<=xs<xe<=127    0<=ys<ye<=159
void LCD_draw_line(uint8 xs ,uint8 ys ,uint8 xe ,uint8 ye ,uint16 color_dat)
{
    uint8 i,ds;
    int dx,dy,inc_x,inc_y;
    int xerr=0,yerr=0;      //初始化变量

    if(xs==xe)      //如果是画垂直线则只需对竖直坐标计数
    {
        LCD_SetPos(xs,ys,xe,ye);
        for(i=0;i<(ye-ys+1);i++)
        {
            LCD_write_para16(color_dat);
        }
    }
    else if(ys==ye)      //如果是画水平线则只需对水平坐标计数
    {
        LCD_SetPos(xs,ys,xe,ye);
        for(i=0;i<(xe-xs+1);i++)
        {
            LCD_write_para16(color_dat);
        }
    }
    else        //如果是斜线，则重新计算，使用画点函数画出直线
    {
        dx=xe-xs;       //计算坐标增量  
        dy=ye-ys;

        if(dx>0)    inc_x=1;    //设置单步方向
        else
        {
            inc_x=-1;
            dx=-dx;
        }
        if(dy>0)    inc_y=1;
        else
        {
            inc_y=-1;
            dy=-dy;
        }

        if(dx>dy)   ds=dx;  //选取基本增量坐标轴
        else        ds=dy;

        for(i=0;i<=ds+1;i++)    //画线输出  
        {
            LCD_draw_dot(xs,ys,color_dat);  //画点  
            xerr += dx;
            yerr += dy;
            if(xerr > ds)
            {
                xerr-=ds;
                xs+=inc_x;
            }
            if(yerr>ds)
            {
                yerr-=ds;
                ys+=inc_y;
            }
        }
    }
}

//画矩形边框
//起始、终止横坐标（0-127），纵坐标（0-159），显示颜色uint16
//0<=xs<xe<=127    0<=ys<ye<=159
void LCD_draw_rectangle(uint8 xs ,uint8 ys ,uint8 xe ,uint8 ye ,uint16 color_dat)
{
    LCD_draw_line(xs,ys,xs,ye,color_dat);       //画矩形左边
    LCD_draw_line(xe,ys,xe,ye,color_dat);       //画矩形右边
    LCD_draw_line(xs,ys,xe,ys,color_dat);       //画矩形上边
    LCD_draw_line(xs,ye,xe,ye,color_dat);       //画矩形下边
}

//画圆形边框（仅支持屏幕内画圆）
//起始、终止横坐标（0-127），纵坐标（0-159），显示颜色uint16
//0<=xs<xe<=127    0<=ys<ye<=159
void LCD_draw_circle(uint8 x ,uint8 y ,uint8 r ,uint16 color_dat)
{
    unsigned char dx,dy=r;

    if((x>=r)&&((128-x)>=r)&&(y>=r)&&((160-y)>=r))  //确定所画圆在屏幕内
    {
        for(dx=0;dx<=r;dx++)
        {
            while((r*r+1 - dx*dx) < (dy*dy))    dy--;

            LCD_draw_dot(x+dx ,y-dy ,color_dat);
            LCD_draw_dot(x-dx ,y-dy ,color_dat);
            LCD_draw_dot(x-dx ,y+dy ,color_dat);
            LCD_draw_dot(x+dx ,y+dy ,color_dat);

            LCD_draw_dot(x+dy ,y-dx ,color_dat);
            LCD_draw_dot(x-dy ,y-dx ,color_dat);
            LCD_draw_dot(x-dy ,y+dx ,color_dat);
            LCD_draw_dot(x+dy ,y+dx ,color_dat);
        }
    }
}

//显示图片
void show_pic(uint8 xs,uint8 ys,uint8 xe,uint8 ye,uint8 *ppic)
{
	unsigned int i,j,k=0;

	LCD_SetPos(xs,ys,xe,ye);
	for(i=0;i<=ye-ys;i++)
	{ 
	    for(j=0;j<=xe-xs;j++)
		{			
		    LCD_write_para16(((uint16)ppic[k])<<8|ppic[k+1]);			
		    k+=2;
		}
	}    
 }

void show_RGB(uint8 xs,uint8 ys,uint8 xe,uint8 ye,uint8 *ppic,int num)
{
	unsigned int i,j,k=0;

	LCD_SetPos(xs,ys,xe,ye);
	for(i=0;i<=ye-ys;i++)
	{ 
	    for(j=0;j<=xe-xs;j++)
		{
            if(num==R)
		        LCD_write_para16(((uint16)ppic[k])<<8);
            else if(num==G)
                LCD_write_para16(((uint16)ppic[k])<<3); 
            else if(num==B)
                LCD_write_para16(((uint16)ppic[k])>>3);
		    k++;
		}
	}    
 }

void show_img(uint8 xs,uint8 ys,uint8 xe,uint8 ye,uint8 *ppic)
{
	unsigned int i,j,k=0;
    uint16 pix1;

	LCD_SetPos(xs,ys,xe,ye);
	for(i=0;i<=ye-ys;i++)
	{ 
	   	for(j=0;j<=xe-xs;j++)
		{	
            pix1=ppic[k] & 0xf8;
		    LCD_write_para16( ( (uint16) (ppic[k] & 0xf8) )<<8 | pix1>>3| pix1<<3);			
		    k++; 
		}
	}    
 }

void show_2_img(uint8 xs,uint8 ys,uint8 xe,uint8 ye,uint8 *ppic)
{
	unsigned int i,j,k=0;
	LCD_SetPos(xs,ys,xe,ye);
	for(i=0;i<=ye-ys;i++)
	{ 
	   	for(j=0;j<=xe-xs;j++)
		{	
            if(ppic[k]==1)
		        LCD_write_para16(0xffff);
            else
                LCD_write_para16(0);
		    k++; 
		}
	}    
 }


/*--------------------------------------------------------------
//字码库的引索
  (0)  !(1)  "(2)  #(3)  $(4)  %(5)  &(6)  '(7)
 ((8)  )(9)  *(10) +(11) ,(12) -(13) .(14) /(15)
 0(16) 1(17) 2(18) 3(19) 4(20) 5(21) 6(22) 7(23)
 8(24) 9(25) :(26) ;(27) <(28) =(29) >(30) ?(31)
 @(32) A(33) B(34) C(35) D(36) E(37) F(38) G(39)
 H(40) I(41) J(42) K(43) L(44) M(45) N(46) O(47)
 P(48) Q(49) R(50) S(51) T(52) U(53) V(54) W(55)
 X(56) Y(57) Z(58) [(59) \(60) ](61) ^(62) _(63)
 `(64) a(65) b(66) c(67) d(68) e(69) f(70) g(71)
 h(72) i(73) j(74) k(75) l(76) m(77) n(78) o(79)
 p(80) q(81) r(82) s(83) t(84) u(85) v(86) w(87)
 x(88) y(89) z(90) {(91) |(92) }(93) ~(94)
--------------------------------------------------------------*/
//字体：8*6字体
//取码规则：低位在前，列行扫描，阴码（1-亮 0-灭）
//查表方法：要显示的ASCII码-32就可以得到字库码的指针
unsigned char Font_code8[][6] = {
{0x00,0x00,0x00,0x00,0x00,0x00},// (0)
{0x00,0x00,0x00,0x4F,0x00,0x00},//!(1)
{0x00,0x00,0x07,0x00,0x07,0x00},//"(2)
{0x00,0x14,0x7F,0x14,0x7F,0x14},//#(3)
{0x00,0x24,0x2A,0x7F,0x2A,0x12},//$(4)
{0x00,0x23,0x13,0x08,0x64,0x62},//%(5)
{0x00,0x36,0x49,0x55,0x22,0x50},//&(6)
{0x00,0x00,0x05,0x03,0x00,0x00},//'(7)
{0x00,0x00,0x1C,0x22,0x41,0x00},//((8)
{0x00,0x00,0x41,0x22,0x1C,0x00},//)(9)
{0x00,0x14,0x08,0x3E,0x08,0x14},//*(10)
{0x00,0x08,0x08,0x3E,0x08,0x08},//+(11)
{0x00,0x00,0x50,0x30,0x00,0x00},//,(12)
{0x00,0x08,0x08,0x08,0x08,0x08},//-(13)
{0x00,0x00,0x60,0x60,0x00,0x00},//.(14)
{0x00,0x20,0x10,0x08,0x04,0x02},///(15)
{0x00,0x3E,0x51,0x49,0x45,0x3E},//0(16)
{0x00,0x00,0x42,0x7F,0x40,0x00},//1(17)
{0x00,0x42,0x61,0x51,0x49,0x46},//2(18)
{0x00,0x21,0x41,0x45,0x4B,0x31},//3(19)
{0x00,0x18,0x14,0x12,0x7F,0x10},//4(20)
{0x00,0x27,0x45,0x45,0x45,0x39},//5(21)
{0x00,0x3C,0x4A,0x49,0x49,0x30},//6(22)
{0x00,0x01,0x71,0x09,0x05,0x03},//7(23)
{0x00,0x36,0x49,0x49,0x49,0x36},//8(24)
{0x00,0x06,0x49,0x49,0x29,0x1E},//9(25)
{0x00,0x00,0x36,0x36,0x00,0x00},//:(26)
{0x00,0x00,0x56,0x36,0x00,0x00},//;(27)
{0x00,0x08,0x14,0x22,0x41,0x00},//<(28)
{0x00,0x14,0x14,0x14,0x14,0x14},//=(29)
{0x00,0x00,0x41,0x22,0x14,0x08},//>(30)
{0x00,0x02,0x01,0x51,0x09,0x06},//?(31)
{0x00,0x32,0x49,0x79,0x41,0x3E},//@(32)
{0x00,0x7E,0x11,0x11,0x11,0x7E},//A(33)
{0x00,0x7F,0x49,0x49,0x49,0x3E},//B(34)
{0x00,0x3E,0x41,0x41,0x41,0x22},//C(35)
{0x00,0x7F,0x41,0x41,0x22,0x1C},//D(36)
{0x00,0x7F,0x49,0x49,0x49,0x41},//E(37)
{0x00,0x7F,0x09,0x09,0x09,0x01},//F(38)
{0x00,0x3E,0x41,0x49,0x49,0x7A},//G(39)
{0x00,0x7F,0x08,0x08,0x08,0x7F},//H(40)
{0x00,0x00,0x41,0x7F,0x41,0x00},//I(41)
{0x00,0x20,0x40,0x41,0x3F,0x01},//J(42)
{0x00,0x7F,0x08,0x14,0x22,0x41},//K(43)
{0x00,0x7F,0x40,0x40,0x40,0x40},//L(44)
{0x00,0x7F,0x02,0x04,0x02,0x7F},//M(45)
{0x00,0x7F,0x04,0x08,0x10,0x7F},//N(46)
{0x00,0x3E,0x41,0x41,0x41,0x3E},//O(47)
{0x00,0x7F,0x09,0x09,0x09,0x06},//P(48)
{0x00,0x3E,0x41,0x51,0x21,0x5E},//Q(49)
{0x00,0x7F,0x09,0x19,0x29,0x46},//R(50)
{0x00,0x46,0x49,0x49,0x49,0x31},//S(51)
{0x00,0x01,0x01,0x7F,0x01,0x01},//T(52)
{0x00,0x3F,0x40,0x40,0x40,0x3F},//U(53)
{0x00,0x1F,0x20,0x40,0x20,0x1F},//V(54)
{0x00,0x3F,0x40,0x38,0x40,0x3F},//W(55)
{0x00,0x63,0x14,0x08,0x14,0x63},//X(56)
{0x00,0x03,0x04,0x78,0x04,0x03},//Y(57)
{0x00,0x61,0x51,0x49,0x45,0x43},//Z(58)
{0x00,0x00,0x7F,0x41,0x41,0x00},//[(59)
{0x00,0x15,0x16,0x7C,0x16,0x15},//\(60)
{0x00,0x00,0x41,0x41,0x7F,0x00},//](61)
{0x00,0x04,0x02,0x01,0x02,0x04},//^(62)
{0x00,0x40,0x40,0x40,0x40,0x40},//_(63)
{0x00,0x00,0x01,0x02,0x04,0x00},//`(64)
{0x00,0x20,0x54,0x54,0x54,0x78},//a(65)
{0x00,0x7F,0x48,0x44,0x44,0x38},//b(66)
{0x00,0x38,0x44,0x44,0x44,0x20},//c(67)
{0x00,0x38,0x44,0x44,0x48,0x7F},//d(68)
{0x00,0x38,0x54,0x54,0x54,0x18},//e(69)
{0x00,0x08,0x7E,0x09,0x01,0x02},//f(70)
{0x00,0x0C,0x52,0x52,0x52,0x3E},//g(71)
{0x00,0x7F,0x08,0x04,0x04,0x78},//h(72)
{0x00,0x00,0x44,0x7D,0x40,0x00},//i(73)
{0x00,0x20,0x40,0x44,0x3D,0x00},//j(74)
{0x00,0x7F,0x10,0x28,0x44,0x00},//k(75)
{0x00,0x00,0x41,0x7F,0x40,0x00},//l(76)
{0x00,0x7E,0x02,0x0C,0x02,0x7C},//m(77)
{0x00,0x7E,0x04,0x02,0x02,0x7C},//n(78)
{0x00,0x38,0x44,0x44,0x44,0x38},//o(79)
{0x00,0x7C,0x14,0x14,0x14,0x08},//p(80)
{0x00,0x08,0x14,0x14,0x18,0x7C},//q(81)
{0x00,0x7C,0x08,0x04,0x04,0x08},//r(82)
{0x00,0x48,0x54,0x54,0x54,0x20},//s(83)
{0x00,0x04,0x3F,0x44,0x40,0x20},//t(84)
{0x00,0x3C,0x40,0x40,0x20,0x7C},//u(85)
{0x00,0x1C,0x20,0x40,0x20,0x1C},//v(86)
{0x00,0x3C,0x40,0x30,0x40,0x3C},//w(87)
{0x00,0x44,0x28,0x10,0x28,0x44},//x(88)
{0x00,0x0C,0x50,0x50,0x50,0x3C},//y(89)
{0x00,0x44,0x64,0x54,0x4C,0x44},//z(90)
{0x00,0x00,0x08,0x36,0x41,0x00},//{(91)
{0x00,0x00,0x00,0x7F,0x00,0x00},//|(92)
{0x00,0x00,0x41,0x36,0x08,0x00},//}(93)
{0x00,0x08,0x04,0x08,0x10,0x08},//~(94)
{0x00,0x08,0x08,0x2A,0x1C,0x08},//->箭头右(127)
{0x00,0x08,0x1C,0x2A,0x08,0x08},//<-箭头左(128)
{0x00,0x04,0x02,0x7F,0x02,0x04},//⬆箭头上(129)
{0x00,0x10,0x20,0x7F,0x20,0x10},//箭头下(130)
{0x00,0x1C,0x2A,0x32,0x2A,0x1C},//笑面(131)
{0x00,0x1C,0x22,0x44,0x22,0x1C}};//爱心(132)

//字体：GulimChel12
//取码规则：低位在前，列行扫描，阴码（1-亮 0-灭）
//查表方法：要显示的ASCII码-32就可以得到字库码的指针
unsigned char Font_code16[][16] = {
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},// (0)
{0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x10,0x10,0x00,0x00},//!(1)
{0x00,0x00,0x24,0x24,0x24,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//"(3)
{0x00,0x00,0x48,0x48,0x48,0xFE,0x24,0x24,0x24,0x24,0x24,0x7F,0x12,0x12,0x12,0x00},//#(4)
{0x00,0x10,0x7C,0x92,0x92,0x12,0x14,0x18,0x30,0x50,0x90,0x92,0x92,0x7C,0x10,0x00},//$(5)
{0x00,0x00,0x8C,0x92,0x52,0x32,0x2C,0x10,0x08,0x68,0x94,0x92,0x92,0x60,0x00,0x00},//%(6)
{0x00,0x00,0x18,0x24,0x24,0x24,0x18,0x98,0xA4,0xA2,0x42,0x42,0xA2,0x9C,0x00,0x00},//&(7)
{0x00,0x00,0x10,0x10,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//'(8)
{0x00,0x20,0x10,0x08,0x08,0x04,0x04,0x04,0x04,0x04,0x04,0x08,0x08,0x10,0x20,0x00},//((9)
{0x00,0x04,0x08,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x10,0x08,0x04,0x00},//)(10)
{0x00,0x00,0x00,0x00,0x00,0x08,0x2A,0x1C,0x14,0x22,0x00,0x00,0x00,0x00,0x00,0x00},//*(11)
{0x00,0x00,0x00,0x00,0x10,0x10,0x10,0xFE,0x10,0x10,0x10,0x00,0x00,0x00,0x00,0x00},//+(12)
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x08,0x00},//,(13)
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//-(14)
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x00,0x00},//.(15)
{0x00,0x00,0x40,0x40,0x20,0x20,0x10,0x10,0x08,0x08,0x04,0x04,0x02,0x02,0x00,0x00},///(16)
{0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00},//0(17)
{0x00,0x00,0x10,0x1C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},
{0x00,0x00,0x3C,0x42,0x42,0x40,0x40,0x20,0x10,0x08,0x04,0x02,0x02,0x7E,0x00,0x00},
{0x00,0x00,0x3C,0x42,0x42,0x40,0x40,0x3C,0x40,0x40,0x40,0x42,0x42,0x3C,0x00,0x00},
{0x00,0x00,0x60,0x60,0x50,0x50,0x48,0x48,0x44,0x44,0xFE,0x40,0x40,0x40,0x00,0x00},
{0x00,0x00,0x7E,0x02,0x02,0x02,0x3E,0x42,0x40,0x40,0x40,0x42,0x42,0x3C,0x00,0x00},
{0x00,0x00,0x3C,0x42,0x42,0x02,0x02,0x3E,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},
{0x00,0x00,0x7E,0x40,0x20,0x20,0x10,0x10,0x10,0x08,0x08,0x08,0x08,0x08,0x00,0x00},
{0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x3C,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},
{0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x7C,0x40,0x40,0x42,0x42,0x3C,0x00,0x00},//9(25)
{0x00,0x00,0x00,0x00,0x08,0x08,0x00,0x00,0x00,0x00,0x08,0x08,0x00,0x00,0x00,0x00},//:(26)
{0x00,0x00,0x00,0x00,0x10,0x10,0x00,0x00,0x00,0x00,0x10,0x10,0x10,0x08,0x00,0x00},//;(27)
{0x00,0x00,0x00,0x40,0x30,0x08,0x06,0x01,0x06,0x08,0x30,0x40,0x00,0x00,0x00,0x00},//<(28)
{0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00},//=(29)
{0x00,0x00,0x00,0x02,0x0C,0x10,0x60,0x80,0x60,0x10,0x0C,0x02,0x00,0x00,0x00,0x00},//>(30)
{0x00,0x00,0x38,0x44,0x82,0x82,0x40,0x20,0x10,0x10,0x10,0x00,0x10,0x10,0x00,0x00},//?(31)
{0x00,0x00,0x38,0x44,0x82,0xB2,0xAA,0xAA,0xAA,0xAA,0xD2,0x02,0x84,0x78,0x00,0x00},//@(32)
{0x00,0x00,0x10,0x10,0x10,0x28,0x28,0x28,0x44,0x44,0x7C,0x82,0x82,0x82,0x00,0x00},//A(33)
{0x00,0x00,0x3E,0x42,0x82,0x82,0x42,0x3E,0x42,0x82,0x82,0x82,0x42,0x3E,0x00,0x00},
{0x00,0x00,0x38,0x44,0x82,0x82,0x02,0x02,0x02,0x02,0x82,0x82,0x44,0x38,0x00,0x00},
{0x00,0x00,0x3E,0x42,0x42,0x82,0x82,0x82,0x82,0x82,0x82,0x42,0x42,0x3E,0x00,0x00},
{0x00,0x00,0xFE,0x02,0x02,0x02,0x02,0x7E,0x02,0x02,0x02,0x02,0x02,0xFE,0x00,0x00},
{0x00,0x00,0xFE,0x02,0x02,0x02,0x02,0x7E,0x02,0x02,0x02,0x02,0x02,0x02,0x00,0x00},
{0x00,0x00,0x38,0x44,0x82,0x02,0x02,0x02,0xE2,0x82,0x82,0x82,0xC4,0xB8,0x00,0x00},
{0x00,0x00,0x82,0x82,0x82,0x82,0x82,0xFE,0x82,0x82,0x82,0x82,0x82,0x82,0x00,0x00},
{0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},
{0x00,0x00,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x22,0x22,0x1C,0x00,0x00},
{0x00,0x00,0x82,0x42,0x22,0x12,0x0A,0x06,0x06,0x0A,0x12,0x22,0x42,0x82,0x00,0x00},
{0x00,0x00,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x7E,0x00,0x00},
{0x00,0x00,0x82,0x82,0xC6,0xC6,0xAA,0xAA,0x92,0x92,0x92,0x82,0x82,0x82,0x00,0x00},
{0x00,0x00,0x82,0x86,0x86,0x8A,0x8A,0x92,0x92,0xA2,0xA2,0xC2,0xC2,0x82,0x00,0x00},
{0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00},
{0x00,0x00,0x3E,0x42,0x82,0x82,0x82,0x42,0x3E,0x02,0x02,0x02,0x02,0x02,0x00,0x00},
{0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0xA2,0x44,0xB8,0x00,0x00},
{0x00,0x00,0x3E,0x42,0x82,0x82,0x82,0x42,0x3E,0x42,0x82,0x82,0x82,0x82,0x00,0x00},
{0x00,0x00,0x7C,0x82,0x82,0x02,0x04,0x18,0x20,0x40,0x80,0x82,0x82,0x7C,0x00,0x00},
{0x00,0x00,0xFE,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},
{0x00,0x00,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x7C,0x00,0x00},
{0x00,0x00,0x82,0x82,0x82,0x44,0x44,0x44,0x28,0x28,0x28,0x10,0x10,0x10,0x00,0x00},
{0x00,0x00,0x92,0x92,0x92,0x92,0x92,0xAA,0xAA,0xAA,0x44,0x44,0x44,0x44,0x00,0x00},
{0x00,0x00,0x82,0x82,0x44,0x44,0x28,0x10,0x28,0x28,0x44,0x44,0x82,0x82,0x00,0x00},
{0x00,0x00,0x82,0x82,0x44,0x44,0x28,0x28,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},
{0x00,0x00,0x7E,0x40,0x20,0x20,0x10,0x10,0x08,0x08,0x04,0x04,0x02,0x7E,0x00,0x00},
{0x00,0x78,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x78,0x00},
{0x00,0x00,0x92,0x92,0x92,0xFF,0x92,0xAA,0xAA,0xAA,0x44,0x44,0x44,0x44,0x00,0x00},
{0x00,0x1E,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1E,0x00},
{0x00,0x10,0x28,0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00},
{0x00,0x04,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x40,0x7C,0x42,0x42,0x42,0xBC,0x00,0x00},
{0x00,0x00,0x02,0x02,0x02,0x02,0x3A,0x46,0x82,0x82,0x82,0x82,0x46,0x3A,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x84,0x02,0x02,0x02,0x02,0x84,0x78,0x00,0x00},
{0x00,0x00,0x80,0x80,0x80,0x80,0xB8,0xC4,0x82,0x82,0x82,0x82,0xC4,0xB8,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x44,0x82,0xFE,0x02,0x02,0x84,0x78,0x00,0x00},
{0x00,0x00,0x70,0x08,0x08,0x08,0x7E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x58,0x64,0x42,0x42,0x42,0x64,0x58,0x42,0x3C,0x00},
{0x00,0x00,0x02,0x02,0x02,0x02,0x3A,0x46,0x42,0x42,0x42,0x42,0x42,0x42,0x00,0x00},
{0x00,0x00,0x10,0x10,0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},
{0x00,0x00,0x10,0x10,0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x0C,0x00},
{0x00,0x00,0x02,0x02,0x02,0x42,0x22,0x12,0x0A,0x06,0x0A,0x12,0x22,0x42,0x00,0x00},
{0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x6E,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x3A,0x46,0x42,0x42,0x42,0x42,0x42,0x42,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x3A,0x46,0x82,0x82,0x82,0x46,0x3A,0x02,0x02,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xB8,0xC4,0x82,0x82,0x82,0xC4,0xB8,0x80,0x80,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x74,0x0C,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x02,0x0C,0x30,0x40,0x42,0x3C,0x00,0x00},
{0x00,0x00,0x08,0x08,0x08,0x08,0x7E,0x08,0x08,0x08,0x08,0x08,0x08,0x70,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x7C,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x82,0x82,0x44,0x44,0x28,0x28,0x10,0x10,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x82,0x92,0x92,0x92,0xAA,0x6C,0x44,0x44,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x82,0x44,0x28,0x10,0x10,0x28,0x44,0x82,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x82,0x82,0x44,0x44,0x28,0x28,0x10,0x10,0x0E,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x40,0x20,0x10,0x08,0x04,0x02,0x7E,0x00,0x00},
{0x00,0x60,0x10,0x10,0x10,0x10,0x10,0x0C,0x10,0x10,0x10,0x10,0x10,0x10,0x60,0x00},
{0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00},
{0x00,0x06,0x08,0x08,0x08,0x08,0x08,0x30,0x08,0x08,0x08,0x08,0x08,0x08,0x06,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x8C,0x72,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};


//液晶字符输出（6*8字体）
//x：0-20（行）
//y：0-19（列）
void LCD_P6X8(uint8 x, uint8 y, uint8 c_dat,uint16 word_color,uint16 back_color)
{
	uint8 i,j;

	LCD_SetPos(x*6,y*8,(x+1)*6-1,(y+1)*8-1);			//定位字符显示区域

	for(j=0; j<8; j++)
	{
		for(i=0; i<6; i++)
		{
			if((Font_code8[c_dat-32][i]) & (0x01<<j))
				LCD_write_para16(word_color);
			else
				LCD_write_para16(back_color);
		}
	}
}

//液晶字符输出（8*8字体）
//x：0-15（行）
//y：0-19（列）
void LCD_P8X8(uint8 x, uint8 y, uint8 c_dat,uint16 word_color,uint16 back_color)
{
	uint8 i,j;

	LCD_SetPos(x*8,y*8,(x+1)*8-1,(y+1)*8-1);			//定位字符显示区域

	for(j=0; j<8; j++)
	{
		LCD_write_para16(back_color);
		for(i=0; i<6; i++)
		{
			if((Font_code8[c_dat-32][i]) & (0x01<<j))
				LCD_write_para16(word_color);
			else
				LCD_write_para16(back_color);
		}
		LCD_write_para16(back_color);
	}
}

//液晶字符输出（8*16字体）
//x：0-15（行）
//y：0-9（列）
void LCD_P8X16(uint8 x, uint8 y, uint8 c_dat,uint16 word_color,uint16 back_color)
{
	uint8 i,j;

	LCD_SetPos(x*8,y*16,(x+1)*8-1,(y+1)*16-1);			//定位字符显示区域

	for(j=0; j <16; j++)
	{
		for(i=0; i<8; i++)
		{
			if((Font_code16[c_dat-32][j]) & (0x01<<i))
				LCD_write_para16(word_color);
			else
				LCD_write_para16(back_color);
		}
	}
}

//液晶字符串输出（6*8字体）
//x：0-20（行）
//y：0-19（列）
void LCD_P6X8Str(uint8 x, uint8 y, uint8 *s_dat,uint16 word_color,uint16 back_color)
{
	while(*s_dat) LCD_P6X8(x++, y, *s_dat++,word_color,back_color);
}

//液晶字符串输出（8*8字体）
//x：0-15（行）
//y：0-19（列）
void LCD_P8X8Str(uint8 x, uint8 y, uint8 *s_dat,uint16 word_color,uint16 back_color)
{
	while(*s_dat) LCD_P8X8(x++, y, *s_dat++,word_color,back_color);
}

//液晶字符串输出（8*16字体）
//x：0-15（行）
//y：0-7（列）
void LCD_P8X16Str(uint8 x, uint8 y, uint8 *s_dat,uint16 word_color,uint16 back_color)
{
	while(*s_dat) LCD_P8X16(x++, y, *s_dat++,word_color,back_color);
}


void LCD_P6X8Str_2value(uint8 x, uint8 y,uint8 c_dat)
{
	uint8 i,j;

	LCD_SetPos(x*6,y*8,(x+1)*6-1,(y+1)*8-1);			//∂®Œª◊÷∑˚œ‘ æ«¯”Ú

	for(j=0; j<8; j++)
	{
		for(i=0; i<6; i++)
		{
			if((Font_code8[c_dat-32][i]) & (0x01<<j))
				LCD_write_para16(WHITE1);
			else
				LCD_write_para16(BLACK1);
		}
	}
}

void LCD18_PrintValueFP(u8 x, u8 y, unsigned int data, u8 num)
 {
 	u8 m,i,j,k;
 	LCD_P6X8Str_2value(x, y, '.');
	m= data/1000;
	i = (data%1000)/100;
	j = (data%100)/10;
	k = data%10;
	switch(num)
	{
		case 1:  	LCD_P6X8Str_2value(x+1,y,k+48);
				break;
		case 2:  	LCD_P6X8Str_2value(x+1,y,j+48);
				LCD_P6X8Str_2value(x+2,y,k+48);
				break;
		case 3:	LCD_P6X8Str_2value(x+1,y,i+48);
				LCD_P6X8Str_2value(x+2,y,j+48);
				LCD_P6X8Str_2value(x+3,y,k+48);
				break;
		case 4: 	LCD_P6X8Str_2value(x+1,y,m+48);
				LCD_P6X8Str_2value(x+2,y,i+48);
				LCD_P6X8Str_2value(x+3,y,j+48);
				LCD_P6X8Str_2value(x+4,y,k+48);
				break;
	}
 }

 //将一个float型数转换成整数部分5位带小数和符号的数据并显示
 //x的范围位0-62，y为页的范围0-7，data为需要转化显示的数值整数部分最多5位 num表示保留的小数位0-4
 void LCD18_PrintValueF(u8 x, u8 y, float data, u8 num)
 {
 	u8 l,m,i,j,k;  //万千百十个
 	u8 databiti = 6; //整数位数
 	unsigned int tempdataui = 0;
  	int tempdataii = (int)data; //整数部分
 	long int tempdatalp = (long int)((data - (int)data)*10000); //取小数位后4位

 	//整数部分显示
 	if(data < 0.0000001)  LCD_P6X8Str_2value(x, y,'-');
 	else LCD_P6X8Str_2value(x, y,'+');
	if(tempdataii < 0)tempdataii = - tempdataii;  //去掉整数部分负号
	tempdataui = tempdataii;
 	l  = tempdataui/10000;
	m= (tempdataui%10000)/1000;
	i = (tempdataui%1000)/100;
	j = (tempdataui%100)/10;
	k = tempdataui%10;
 	if (l != 0)  //五位
 	{
 		LCD_P6X8Str_2value(x+1,y,l+48);
 		LCD_P6X8Str_2value(x+2,y,m+48);
		LCD_P6X8Str_2value(x+3,y,i+48);
		LCD_P6X8Str_2value(x+4,y,j+48);
		LCD_P6X8Str_2value(x+5,y,k+48);
 	}
 	else if(m != 0) //四位
 	{
 		databiti = 5;
 		LCD_P6X8Str_2value(x+1,y,m+48);
 		LCD_P6X8Str_2value(x+2,y,i+48);
		LCD_P6X8Str_2value(x+3,y,j+48);
		LCD_P6X8Str_2value(x+4,y,k+48);
 	}
  	else if(i != 0) //三位
  	{
  		databiti = 4;
  	 	LCD_P6X8Str_2value(x+1,y,i+48);
 		LCD_P6X8Str_2value(x+2,y,j+48);
		LCD_P6X8Str_2value(x+3,y,k+48);
  	}
  	else if(j != 0) //两位
  	{
    		databiti = 3;
  		LCD_P6X8Str_2value(x+1,y,j+48);
 		LCD_P6X8Str_2value(x+2,y,k+48);
  	}
	else
	{
		databiti = 2;
		LCD_P6X8Str_2value(x+1,y,k+48);
	}
 	if(tempdatalp < 0)tempdatalp = - tempdatalp;	//去掉小数部分负号
	switch(num)
	{
		case 0: break;
		case 1:  LCD18_PrintValueFP(x + databiti , y, (unsigned int)(tempdatalp / 1000),num);break;
		case 2:  LCD18_PrintValueFP(x + databiti , y, (unsigned int)(tempdatalp / 100),num);break;
		case 3:  LCD18_PrintValueFP(x + databiti , y, (unsigned int)(tempdatalp / 10),num);break;
		case 4:  LCD18_PrintValueFP(x + databiti , y, (unsigned int)(tempdatalp),num);break;
	}
 }

void LCD_PrintValueI(u8 x, u8 y,signed int data)
{
	u8 i,j,k,l,m;
	if(data < 0)
	{
		LCD_P6X8Str_2value(x,y,'-');
		data = - data;
	}
	else
	{
		LCD_P6X8Str_2value(x,y,'+');
	}

	l  = data/10000;
	m= (data%10000)/1000;
	i = (data%1000)/100;
	j = (data%100)/10;
	k = data%10;
    LCD_P6X8Str_2value(x+1,y,l+48);
	LCD_P6X8Str_2value(x+2,y,m+48);
	LCD_P6X8Str_2value(x+3,y,i+48);
	LCD_P6X8Str_2value(x+4,y,j+48);
	LCD_P6X8Str_2value(x+5,y,k+48);
}

void LCD_PrintValueC(u8 x, u8 y, signed char data)
{
	u8 i,j,k;
	if(data < 0)
	{
		LCD_P6X8Str_2value(x,y,'-');
		data = - data;
	}
	else
	{
		LCD_P6X8Str_2value(x,y,'+');
	}
	i = data/100;
	j = (data%100)/10;
	k = data%10;
	LCD_P6X8Str_2value(x+1,y,i+48);
	LCD_P6X8Str_2value(x+2,y,j+48);
	LCD_P6X8Str_2value(x+3,y,k+48);
}



//定位输出数字
//x：0-20（行）
//y：0-19（列）
//num：0-65535 要显示的数字
//num_bit：0-5 数字的位数
void LCD_P6X8NUM(uint8 x, uint8 y, uint16 num, uint8 num_bit,uint16 word_color,uint16 back_color)
{
  
    LCD_display(y*8,x*6,(y+1)*8-1,(x+num_bit)*6-1,back_color);
	char i;
	uint8 ii;
	uint8 dat[6];
	for(i = 0; i < 6; i++) dat[i] = 0; i = 0;	//初始化数据
	while(num / 10)								//拆位
	{
		dat[i] = num % 10;						//最低位
		num /= 10; i++;		
	}
	dat[i] = num;								//最高位
	ii = i;										//保存dat的位数
	for(; i >= 0; i--)	dat[i] += 48;			//转化成ASCII
	for(i = 0; i < num_bit; i++)
	LCD_P6X8(x, y + i, ' ',word_color,back_color);		//清显示区域
	for(i = ii; i >= 0; i--)
	LCD_P6X8(x++, y, dat[i],word_color,back_color);		// 输出数值
}

//定位输出数字
//x：0-15（行）
//y：0-19（列）
//num：0-65535 要显示的数字
//num_bit：0-5 数字的位数
void LCD_P8X8NUM(uint8 x, uint8 y, uint16 num, uint8 num_bit,uint16 word_color,uint16 back_color)
{
	char i;
	uint8 ii;
	uint8 dat[6];
	for(i = 0; i < 6; i++) dat[i] = 0; i = 0;	//初始化数据
	while(num / 10)								//拆位
	{
		dat[i] = num % 10;						//最低位
		num /= 10; i++;		
	}
	dat[i] = num;								//最高位
	ii = i;										//保存dat的位数
	for(; i >= 0; i--)	dat[i] += 48;			//转化成ASCII
	for(i = 0; i < num_bit; i++)
	LCD_P8X8(x, y + i, ' ',word_color,back_color);					//清显示区域
	for(i = ii; i >= 0; i--)
	LCD_P8X8(x, y++, dat[i],word_color,back_color);					// 输出数值
}

