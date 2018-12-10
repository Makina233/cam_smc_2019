//
//  oled.c
//  camera smartcar
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//

#include "oled.h"

const unsigned char Oled6x8[][6] =
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
	{ 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
	{ 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
	{ 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
	{ 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
	{ 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
	{ 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
	{ 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
	{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
	{ 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
	{ 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
	{ 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
	{ 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
	{ 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
	{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
	{ 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
	{ 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
	{ 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
	{ 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
	{ 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
	{ 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
	{ 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
	{ 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
	{ 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
	{ 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
	{ 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
	{ 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
	{ 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
	{ 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
	{ 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
	{ 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
	{ 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
	{ 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
	{ 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
	{ 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
	{ 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
	{ 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
	{ 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
	{ 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
	{ 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
	{ 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
	{ 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
	{ 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
	{ 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
	{ 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
	{ 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
	{ 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
	{ 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
	{ 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
	{ 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
	{ 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
	{ 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
	{ 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
	{ 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [ 91
	{ 0x00, 0x02, 0x04 ,0x08, 0x10, 0x20 },    // \92
	{ 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
	{ 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
	{ 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
	{ 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
	{ 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
	{ 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
	{ 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
	{ 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
	{ 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
	{ 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
	{ 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
	{ 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
	{ 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
	{ 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
	{ 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
	{ 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
	{ 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
	{ 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
	{ 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
	{ 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
	{ 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
	{ 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
	{ 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
	{ 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
	{ 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
	{ 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
	{ 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};

//延时函数
void OledDelayMs(unsigned int ms)
{
    unsigned int a;
    while(ms)
    {
        a=6675;
        while(a--);
        ms--;
    }
    return;
}

//OLEDIO口初始化函数
void OledPinInit(void)
{
    gpio_init(OLED_SCL_PIN,GPO,1); // 时钟初始化定义，初始化为高电平
    gpio_init(OLED_SDA_PIN,GPO,1); 
    gpio_init(OLED_RES_PIN,GPO,1);
    gpio_init(OLED_DC_PIN,GPO,1);
}

//向SSD1306写入一个字节
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OledWrByte(unsigned char dat, unsigned char cmd)
{
	char i;
	if(cmd)		//数据模式
	{
		OLED_DC_H;		//设置指定数据端口位
	}
	else
	{
		OLED_DC_L;		//清除指定的数据端口位
	}
	for(i=0;i<8;i++)
	{
		OLED_SCL_L;
		if(dat&0x80)
		   OLED_SDA_H;
		else 
		   OLED_SDA_L;
		OLED_SCL_H;
		dat<<=1;   
	}
	OLED_DC_H; 
}

//设置坐标函数
void OledSetPos(unsigned char x, unsigned char y)
{
	OledWrByte((0xb0+y),OLED_CMD);
	OledWrByte((((x&0xf0)>>4)|0x10),OLED_CMD);
	OledWrByte(((x&0x0f)|0x00),OLED_CMD);
}

//------------------------------------------------------
//      对全屏写入同一个字符函数
//      如 LED_Fill(0x01);  对于某一位0为不亮 1为亮
//-------------------------------------------------------
//                 	          共128列
//
//
//		1			1		         1
//	共     	0      			0		         0
//	8     	0      		    0		         0
//	组     	0	   ......   0     ......        0
//	字     	0      			0		         0
//	符     	0      			0		         0
//	即     	0      			0		         0
//	8    	0     			0		         0
//	页	1			1
//	64	0			0
//	行	.			.
//		.			.
//		.			.
//		.			.
//		0			0
//------------------------------------------------------
void OledFill(unsigned char bmp_data)
{
	unsigned char x,y;
	for(y=0;y<8;y++)	//oled屏幕上最多能显示8页（即能显示8行字符串）
	{
		OledWrByte((0xb0+y),OLED_CMD);	//0xb0+0~7 表示0～7页
		OledWrByte((0x00),OLED_CMD);	//0x00+0~16表示将128列分为16组其他地址在某组中的第几列
		OledWrByte((0x10),OLED_CMD);	//0x10+0~16表示将128列分为16组其地址所在第几组
		for(x=0;x<128;x++)
		{
			OledWrByte(bmp_data,OLED_DATA);
		}
	}
}

//清屏函数
void OledCls(void)
{
	unsigned char x,y;
	for(y=0;y<8;y++)	//oled屏幕上最多能显示8页（即能显示8行字符串）
	{
		OledWrByte((0xb0+y),OLED_CMD);	//0xb0+0~7 表示0～7页
		OledWrByte((0x00),OLED_CMD);	//0x00+0~16表示将128列分为16组其他地址在某组中的第几列
		OledWrByte((0x10),OLED_CMD);	//0x10+0~16表示将128列分为16组其地址所在第几组
		for(x=0;x<128;x++)
		{
			OledWrByte(0,OLED_DATA);
		}
	}
}

void SetStartColumn(unsigned char d)
{
	OledWrByte((0x00+d%16),OLED_CMD);// Set Lower Column Start Address for Page Addressing Mode
	OledWrByte((0x10+d/16),OLED_CMD);// Set Higher Column Start Address for Page Addressing Mode
}

void SetAddressingMode(unsigned char d)
{
	OledWrByte((0x20),OLED_CMD);	// Set Memory Addressing Mode
	OledWrByte((d),OLED_CMD);
}

void SetColumnAddress(unsigned char a, unsigned char b)
{
	OledWrByte((0x21),OLED_CMD);// Set Column Address
	OledWrByte((a),OLED_CMD);
	OledWrByte((b),OLED_CMD);
}

void SetPageAddress(unsigned char a, unsigned char b)
{
	OledWrByte((0x22),OLED_CMD);// Set Page Address
	OledWrByte((a),OLED_CMD);
	OledWrByte((b),OLED_CMD);
}

void SetStartLine(unsigned char d)
{
	OledWrByte((0x40|d),OLED_CMD);
}

void SetContrastControl(unsigned char d)
{
	OledWrByte((0x81),OLED_CMD);
	OledWrByte((d),OLED_CMD);
}

void SetChargePump(unsigned char d)
{
	OledWrByte((0x8D),OLED_CMD);
	OledWrByte((0x10|d),OLED_CMD);
}

void SetSegmentRemap(unsigned char d)
{
	OledWrByte((0xA0|d),OLED_CMD);
}

void SetEntireDisplay(unsigned char d)
{
	OledWrByte((0xA4|d),OLED_CMD);
}

void SetInverseDisplay(unsigned char d)
{
	OledWrByte((0xA6|d),OLED_CMD);
}

void SetMultiplexRatio(unsigned char d)
{
	OledWrByte((0xA8),OLED_CMD);
	OledWrByte((d),OLED_CMD);
}

void SetDisplayOnOff(unsigned char d)
{
	OledWrByte((0xAE|d),OLED_CMD);
}

void SetStartPage(unsigned char d)
{
	OledWrByte((0xB0|d),OLED_CMD);
}

void SetCommonRemap(unsigned char d)
{
	OledWrByte((0xC0|d),OLED_CMD);
}

void SetDisplayOffset(unsigned char d)
{
	OledWrByte((0xD3),OLED_CMD);
	OledWrByte((d),OLED_CMD);
}

void SetDisplayClock(unsigned char d)
{
	OledWrByte((0xD5),OLED_CMD);
	OledWrByte((d),OLED_CMD);
}

void SetPrechargePeriod(unsigned char d)
{
	OledWrByte((0xD9),OLED_CMD);
	OledWrByte((d),OLED_CMD);
}

void SetCommonConfig(unsigned char d)
{
	OledWrByte((0xDA),OLED_CMD);
	OledWrByte((0x02|d),OLED_CMD);
}

void SetVcomh(unsigned char d)
{
	OledWrByte((0xDB),OLED_CMD);
	OledWrByte((d),OLED_CMD);
}

void SetNop(void)
{
	OledWrByte((0xE3),OLED_CMD);
}

//OLED初始化
void OledInit(void)
{
	OledPinInit();
	
	OLED_SCL_H;;;
	OLED_RST_L;;;
	OledDelayMs(50);
	OLED_RST_H;;;
	
	SetDisplayOnOff(0x00);		  // Display Off (0x00/0x01)
	SetDisplayClock(0x80);		  // Set Clock as 100 Frames/Sec
	SetMultiplexRatio(0x3F);	  // 1/64 Duty (0x0F~0x3F)
	SetDisplayOffset(0x00);		  // Shift Mapping RAM Counter (0x00~0x3F)
	SetStartLine(0x00);			  // Set Mapping RAM Display Start Line (0x00~0x3F)
	SetChargePump(0x04);		  // Enable Embedded DC/DC Converter (0x00/0x04)
	SetAddressingMode(0x02);	  // Set Page Addressing Mode (0x00/0x01/0x02)
	SetSegmentRemap(0x01);	  // Set SEG/Column Mapping     0x00◊Û”“∑¥÷√ 0x01’˝≥£
	SetCommonRemap(0x08);	  // Set COM/Row Scan Direction 0x00…œœ¬∑¥÷√ 0x08’˝≥£
	SetCommonConfig(0x10);	  // Set Sequential Configuration (0x00/0x10)
	SetContrastControl(0xCF); // Set SEG Output Current
	SetPrechargePeriod(0xF1);	  // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	SetVcomh(0x40);			  // Set VCOM Deselect Level
	SetEntireDisplay(0x00);		  // Disable Entire Display On (0x00/0x01)
	SetInverseDisplay(0x00);	  // Disable Inverse Display On (0x00/0x01)
	SetDisplayOnOff(0x01);		  // Display On (0x00/0x01)
	OledFill(0x00);                       
	OledSetPos(0,0);
}

//显示一个6x8标准ASCII字符
//x为显示的横坐标0～122，y为页范围0～7，ch要显示的字符
void OledShow6x8Char(unsigned char x, unsigned char y, char ch)
{
	unsigned char c=0, i=0;
	c =ch-32;
	if(x>122)
	{
		x=0;
		y++;
	}
	OledSetPos(x,y);
	for(i=0;i<6;i++)
	{
		OledWrByte((Oled6x8[c][i]),OLED_DATA);
	}
}

//写入一组6x8标准ASCII字符串
//x为显示的横坐标0～122，y为页范围0～7，ch[]要显示的字符串
void OledShow6x8Str(unsigned char x, unsigned char y, char *ch)
{
	unsigned char c=0, i=0, j=0;
	while(ch[j] != '\0')
	{
		c =ch[j]-32;
		if(x>126)
		{
			x=0;
			y++;
		}
		OledSetPos(x,y);
		for(i=0;i<6;i++)
		{
			OledWrByte((Oled6x8[c][i]),OLED_DATA);
		}
		x+=6;
		j++;
	}
}

//将一个两位整数进行显示
//用于时间显示
void OledTimeValueShow(unsigned char x, unsigned char y, int data)
{
	unsigned char j,k;

	j = data/10;
	k = data%10;
	
	OledShow6x8Char(x,y,j+48);
	OledShow6x8Char(x+6,y,k+48);
}

//年份显示
//四位数字
void OledYearValueShow(unsigned char x, unsigned char y, int data)
{
	unsigned char i,j,k,m;

	m= (data%10000)/1000;
	i = (data%1000)/100;
	j = (data%100)/10;
	k = data%10;

	OledShow6x8Char(x+0,y,m+48);
	OledShow6x8Char(x+6,y,i+48);
	OledShow6x8Char(x+12,y,j+48);
	OledShow6x8Char(x+18,y,k+48);
}


//将一个整型数转换成5位数进行显示
// x:0~120 y为页的范围0～7 data为需要转化显示的数值
void OledPrintValueI(unsigned char x, unsigned char y, int data)
{
	unsigned char i,j,k,l,m;
	if(data < 0)
	{
		OledShow6x8Char(x,y,'-');
		data = - data;
	}
	else
	{
		OledShow6x8Char(x,y,'+');
	}
	
	l  = data/10000;
	m= (data%10000)/1000;
	i = (data%1000)/100;
	j = (data%100)/10;
	k = data%10;
	
	OledShow6x8Char(x+6,y,l+48);
	OledShow6x8Char(x+12,y,m+48);
	OledShow6x8Char(x+18,y,i+48);
	OledShow6x8Char(x+24,y,j+48);
	OledShow6x8Char(x+30,y,k+48);
}

void OledPrintValueFP(unsigned char x, unsigned char y, unsigned int data, unsigned char num)
{
	unsigned char m,i,j,k;
	OledShow6x8Char(x,y,'.');
	m= data/1000;
	i = (data%1000)/100;
	j = (data%100)/10;
	k = data%10;
	switch(num)
	{
		case 1:
			OledShow6x8Char(x+6,y,k+48);
			break;
		case 2:
			OledShow6x8Char(x+6,y,j+48);
			OledShow6x8Char(x+12,y,k+48);
			break;
		case 3:
			OledShow6x8Char(x+6,y,i+48);
			OledShow6x8Char(x+12,y,j+48);
			OledShow6x8Char(x+18,y,k+48);
			break;
		case 4:
			OledShow6x8Char(x+6,y,m+48);
			OledShow6x8Char(x+12,y,i+48);
			OledShow6x8Char(x+18,y,j+48);
			OledShow6x8Char(x+24,y,k+48);
			break;
	}
}

//将一个float型数转换成5位数带小数和符号的数据进行显示
// x:0~62 y为页的范围0～7 data为需要转化显示的数值，显示的整数部分最多5位 num表示保留小数位0～4
void OledPrintValueF(unsigned char x, unsigned char y, float data, unsigned char num)
{
	unsigned char l,m,i,j,k;  //万千百十个
 	unsigned char databiti = 6; //整数位数
 	unsigned int tempdataui = 0;
  	int tempdataii = (int)data; //整数部分
 	long int tempdatalp = (long int)((data - (int)data)*10000); //取小数后4位
	
	//整数部分显示
	if(data < 0.0000001)
	{
		OledShow6x8Char(x,y,'-');
	}
	else
	{
		OledShow6x8Char(x,y,'+');
	}
	if(tempdataii < 0)
	{	
		tempdataii = - tempdataii;  //去掉整数部分负号
	}
	tempdataui = tempdataii;
	l  = tempdataui/10000;
	m= (tempdataui%10000)/1000;
	i = (tempdataui%1000)/100;
	j = (tempdataui%100)/10;
	k = tempdataui%10;
	if (l != 0)  //五位
 	{
 		OledShow6x8Char(x+6,y,l+48);
 		OledShow6x8Char(x+12,y,m+48);
		OledShow6x8Char(x+18,y,i+48);
		OledShow6x8Char(x+24,y,j+48);
		OledShow6x8Char(x+30,y,k+48);
 	}
 	else if(m != 0) //四位
 	{
 		databiti = 5;
 		OledShow6x8Char(x+6,y,m+48);
 		OledShow6x8Char(x+12,y,i+48);
		OledShow6x8Char(x+18,y,j+48);
		OledShow6x8Char(x+24,y,k+48);
 	}
  	else if(i != 0) //3位
  	{
  		databiti = 4;
  	 	OledShow6x8Char(x+6,y,i+48);
 		OledShow6x8Char(x+12,y,j+48);
		OledShow6x8Char(x+18,y,k+48);
  	}
  	else if(j != 0) //两位
  	{
    	databiti = 3;
  		OledShow6x8Char(x+6,y,j+48);
 		OledShow6x8Char(x+12,y,k+48);
  	}
	else
	{
		databiti = 2;
		OledShow6x8Char(x+6,y,k+48);
	}
	if(tempdatalp < 0)
	{	
		tempdatalp = - tempdatalp;	//去掉小数部分负号
	}
	switch(num)
	{
		case 0: break;
		case 1:  OledPrintValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp / 1000),num);break;
		case 2:  OledPrintValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp / 100),num);break;
		case 3:  OledPrintValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp / 10),num);break;
		case 4:  OledPrintValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp),num);break;
	}
}

//显示图像
// pucTable -- 图像地址
// usRowNum -- 行数
// usColumnNum  -- 列数
void OledPrintImage(unsigned char *pucTable, unsigned int usRowNum, unsigned int usColumnNum)
{
	unsigned char ucData;
    unsigned int i,j,k,m,n;
    unsigned int usRowTmp;
	
	m = usRowNum >> 3;   //计算图片行数以8位为一组完整的组数
    n = usRowNum % 8;    //计算分完组后剩下的行数
	
	for(i = 0; i < m; i++) //完整组行扫描
	{
		OledSetPos(0,(unsigned char)i);
		usRowTmp = i << 3;    //计算当前所在行的下标
        for(j = 0; j < usColumnNum; j++) //列扫描
		{
			ucData = 0;
            for(k = 0; k < 8; k++) //在i组中对这8行扫描
			{
				ucData = ucData >> 1;
				if((pucTable + (usRowTmp + k) * usColumnNum)[j] == 1)
                {
                    ucData = ucData | 0x80;
                }
			}
			OledWrByte((ucData),OLED_DATA);
		}
	}
	OledSetPos(0,(unsigned char)i);
	usRowTmp = i << 3;       //计算当前所在行的下标
    for(j = 0; j < usColumnNum; j++) //列扫描
	{
		ucData = 0;
        for(k = 0; k < n; k++) //对剩下的行扫描
        {
            ucData = ucData >> 1;
            if((pucTable + (usRowTmp + k) * usColumnNum)[j] == 1)
            {
                ucData = ucData | 0x80;
            }

        }
		ucData = ucData >> (8 - n);
		OledWrByte((ucData),OLED_DATA);
	}
}
