//
//  mt9v034.c
//  mt9v034_use
//
//  Created by 傅凯 on 2018/11.
//  Copyright © 2018年 赛博智能车实验室. All rights reserved.
//

#include "mt9v034.h"
#include "oled.h"

//此处定义了摄像头的一些储存图像的数组
//摄像头状态机
Mt9v034StatusNode Mt9v034Status = WAIT_INIT;
//摄像头原始图像数组
unsigned char ImageOriginalData[FRAME_HEIGHT][FRAME_WIDTH];
//摄像头压缩后的数组
unsigned char ImageCmprsData[IMG_ROWS][IMG_COLUMN];
//二值化图像数组
unsigned char ImageBinarizationData[IMG_ROWS][IMG_COLUMN];
//跳变点数组
int JumpRow[IMG_ROWS],JumpColumn[IMG_COLUMN];


/********SCCB通信部分********/

//摄像头SCCB引脚初始化函数
//注意：引脚需要初始化为推挽输出，且内部上拉
void Mt9v034SccbGpioInit(void)
{
    gpio_init(MT9V034_SCL, GPO, 1);
    gpio_init(MT9V034_SDA, GPO, 1);

    port_init_NoALT(MT9V034_SCL, ODO | PULLUP);
    port_init_NoALT(MT9V034_SDA, ODO | PULLUP);
}

//简单延时函数
void SccbDelay(volatile unsigned int time)
{
    while(time)
    {
        time--;
    }
}

//SCCB起始信号
//SCL高电平期间，SDA出现一个下降沿表示起始信号
static void MtSccbStart(void)
{
    MT9V034_SDA_DIR_OUT();  //保证主机数据线输出
    MT9V034_SDA_H();   //数据线先保持为高，起始信号要该口的下降沿
    MT9V034_SCL_H();   //时钟线保持为高
    SCCB_DELAY();
    MT9V034_SDA_L();   //数据线拉低出现下降沿
    SCCB_DELAY();  //小延时，保证可靠的下降沿
    MT9V034_SCL_L();   //拉低时钟线，保证接下来数据线允许改变
}

//SCCB停止信号
//SCL高电平期间，SDA出现一个上升沿表示停止信号
static void MtSccbStop(void)
{
    MT9V034_SDA_DIR_OUT();  //保证主机数据线输出
    MT9V034_SCL_H();   //时钟线保持为高
    MT9V034_SDA_L();   //保证数据线为低电平
    SCCB_DELAY();   //延时，保证一个可靠的电平信号
    MT9V034_SDA_H();   //数据出现上升沿
    SCCB_DELAY();   //延时保证可靠的上升沿
}

//产生ACK应答
//SDA低电平期间，SCL出现一个上升沿即为应答
static void MtSccbAck(void)
{
    MT9V034_SDA_DIR_OUT();  //保证主机数据线输出
    MT9V034_SCL_L();
    MT9V034_SDA_L();   //数据线保持低电平
    SCCB_DELAY();   //延时，保证一个可靠的电平信号
    MT9V034_SCL_H();   
    SCCB_DELAY();   //延时保证可靠的上升沿
    MT9V034_SCL_L();   //应答完成后拉低时钟线，保证接下来数据线允许改变
}
//不产生ACK应答
//SDA高电平电平期间，SCL出现一个上升沿即为非应答
static void MtSccbNoAck(void)
{
    MT9V034_SDA_DIR_OUT();  //保证主机数据线输出
    MT9V034_SCL_L();
    MT9V034_SDA_H();   //数据线保持高电平
    SCCB_DELAY();   //延时，保证一个可靠的电平信号
    MT9V034_SCL_H();   
    SCCB_DELAY();   //延时保证可靠的上升沿
    MT9V034_SCL_L();   //非应答完成后拉低时钟线，保证接下来数据线允许改变
}

//主机等待应答信号的到来
//返回值：0，有应答； 1，无应答
static unsigned char MtSccbWaitAck(void)
{
    unsigned char ack_time;
    
    MT9V034_SDA_DIR_IN();   //将SDA设置为输入
    MT9V034_SCL_H();    //时钟线拉高
    SCCB_DELAY();
    //等待接收器拉低数据线，如果接收器把SDA拉低，表示响应了；如果接收器释放了SDA总线，表示不响应
    while(MT9V034_SDA_READ())  
    {
        ack_time++;
        if(ack_time > 250)
        {
            //认为非应答，停止信号
            MtSccbStop();
            return 1;   //无应答
        }
    }
    MT9V034_SCL_L();    //拉低时钟线，保证接下来数据线允许改变
    return 0;   //有应答
}

//SCCB发送一个字节
static void MtSccbSendByte(unsigned char send_byte)
{
    unsigned char count=0;  //定义一个计数变量
    
    MT9V034_SDA_DIR_OUT();  //主机数据线输出
    MT9V034_SCL_L();    //拉低时钟线，保证接下来数据线允许改变
    for(count=0; count<8; ++count)
    {
        if(send_byte & 0x80)    //按位与
        {
            MT9V034_SDA_H();
        }
        else
        {
            MT9V034_SDA_L();
        }
        send_byte <<= 1;    //保证将要发送的位一直是最高位
        SCCB_DELAY();   //延时，保证一个可靠的电平
        MT9V034_SCL_H();    //时钟线拉高，此时数据线不得改变，用于对方读取数据
        SCCB_DELAY();   //延时，保证一个可靠的电平
        MT9V034_SCL_L();
        SCCB_DELAY();   //延时，保证一个可靠的电平
    }
}

//SCCB读取一个字节
//读1个字节，ack=1时，发送ACK， ack=0时，发送nack
static unsigned char MtSccbReadByte(unsigned char ack)
{
    unsigned char count=0;  //定义计数变量
    unsigned char receive=0;    //定义接受变量
    
    MT9V034_SDA_DIR_IN();   //将SDA设置为输入
    for(count=0; count<8; ++count)
    {
        MT9V034_SCL_L();    //拉低时钟线，保证接下来数据线允许改变
        SCCB_DELAY();   //延时，保证一个可靠的电平
        MT9V034_SCL_H();
        receive <<=1;
        if(MT9V034_SDA_READ())
        {
            receive |= 0x01;
        }
        SCCB_DELAY();   
    }
    if(!ack)
    {
        MtSccbNoAck();    //不需要应答，则给出非应答信号，不在继续
    }
    else
    {
        MtSccbAck();      //需要应答，则给应答
    }
    return receive;
}


//写数据
//device_addr:器件地址
//data_addr: 数据地址
//data: 数据
void MtSccbWriteData(unsigned char device_addr, unsigned char data_addr, unsigned int data)
{
    int i=2;
    while(i--)
    {
        MtSccbStart();    //起始信号
        MtSccbSendByte(device_addr);  //发送器件地址
        MtSccbWaitAck();  //等待应答
        MtSccbSendByte(data_addr);    //发送数据地址
        MtSccbWaitAck();  //等待应答
        MtSccbSendByte(data>>8); //发送数据
        MtSccbWaitAck();  //等待应答
        MtSccbSendByte(data); //发送数据
        MtSccbWaitAck();  //等待应答
        MtSccbStop(); //结束信号
    }
}

//读数据
//device_addr:器件地址;摄像头ID
//data_addr: 数据地址
unsigned int MtSccbReadData(unsigned char device_addr, unsigned char data_addr)
{
    unsigned int data=0;  //定义计数变量
    MtSccbStart();    //起始信号
    MtSccbSendByte(device_addr);  //发送器件地址
    MtSccbWaitAck();  //等待应答
    MtSccbSendByte(data_addr);    //发送数据地址
    MtSccbWaitAck();  //等待应答

    MtSccbStart();    //起始信号
    MtSccbSendByte(device_addr + 1);  //发送器件地址读模式
    MtSccbWaitAck();  //等待应答
    //读数据
    data=MtSccbReadByte(1);
    data <<= 8;
    data |= MtSccbReadByte(0);//最后一位数据不需要应答
    MtSccbStop(); //停止信号
    return data;
}


/********MT9V034摄像头配置部分********/

//摄像头引脚初始化
void Mt9v034GpioInit(void)
{
    port_init(MT9V034_FRM_IO, ALT1 | IRQ_RISING | PULLDOWN | PF);   //上升沿触发，内部下拉，带滤波
    port_init(MT9V034_LINE_IO, ALT1 | IRQ_RISING | PULLDOWN | PF);   //上升沿触发，内部下拉，带滤波
}

//摄像头DMA传输初始化
//DADDR:DMA传输目的地址，如摄像头原始图像数组的地址
//count:DMA采集次数
void Mt9v034DmaInit(void *DADDR, int count)
{
    dma_portx2buff_init(MT9V034_DMA_CH, (void *)&DMA_DATA_IO, DADDR, DMA_PIX, DMA_BYTE1, count, DADDR_RECOVER);
}

//摄像头DMA传输地址设置
//注意此函数必须在清DMA中断标志位之后使用
void Mt9v034DmaDaddrSet(void *DADDR)
{
    DMA_DADDR(MT9V034_DMA_CH) = (unsigned int)DADDR;
}

//采样点初始化
void JumpInit(void)
{
    float jump_row=0, jump_column=0;
    char count=0;

    jump_row = (float)FRAME_HEIGHT / (float)IMG_ROWS;
    jump_column = (float)FRAME_WIDTH / (float)IMG_COLUMN;

    for(count = 0; count < IMG_ROWS; ++count)
    {
        JumpRow[count] = (int)(jump_row * count);
    }
    for(count = 0; count < IMG_COLUMN; ++count)
    {
        JumpColumn[count] = (int)(jump_column * count);
    }
}

//图像压缩
static void ImageCompression(void)
{
    int row=0, column=0;
    for(row = 0; row < IMG_ROWS; ++row)
    {
        for(column = 0; column < IMG_COLUMN; ++column)
        {
            ImageCmprsData[row][column] = ImageOriginalData[JumpRow[row]][JumpColumn[column]];
        }
    }
}

//MT9V034摄像头模式初始化
unsigned char Mt9v034ModeInit(void)
{
    unsigned int chip_name = 0; //芯片ID
    
    Mt9v034SccbGpioInit();
    
    chip_name = MtSccbReadData(MT9V034_ADDR,MTREG_ChipVersion);
    
    if(chip_name == 4900)
    {
        //LED_PrintValueI(10,3,chip_name);
      
        MtSccbWriteData(MT9V034_ADDR, MTREG_ChipControl, 0x0188);//0x07 芯片控制; 逐行扫描,自动曝光,禁用立体视觉,启用并行输出,像素读取和曝光同时进行
        MtSccbWriteData(MT9V034_ADDR,MTREG_ColumnStartContextA,30);//0x01 起始列（1~752）
        MtSccbWriteData(MT9V034_ADDR,MTREG_RowStartContextA,10);//0x02 起始行（4~482）
        MtSccbWriteData(MT9V034_ADDR,MTREG_WindowWidthContextA,752);//0x04 设置窗口宽度（1~752）
        MtSccbWriteData(MT9V034_ADDR,MTREG_WindowHeightContextA,480);//0x03 设置窗口高度（1~480）  
        MtSccbWriteData(MT9V034_ADDR, MTREG_ReadModeContextA,0x33a);//0x0d 读取模式; row 4bin, column 4bin, 行颠倒,列颠倒
        //MtSccbWriteData(MT9V034_ADDR, MTREG_HorizontalBlankingContextA,91);//0x05 水平消隐（61~1023）
        //MtSccbWriteData(MT9V034_ADDR, MTREG_VerticalBlankingContextA,45);//0x06 垂直消隐（2~32288）
        MtSccbWriteData(MT9V034_ADDR, MTREG_AEC_AGC_DesiredBin,50);//0xa5 图像亮度（1~64）
        MtSccbWriteData(MT9V034_ADDR, MTREG_AEC_MInimumExposure, 1);//0xac 最小粗条快门宽度 (1～32765)
        MtSccbWriteData(MT9V034_ADDR, MTREG_AEC_MaximumExposure, 200);//0xad 最大粗条快门宽度 (1～32765) 
        MtSccbWriteData(MT9V034_ADDR, MTREG_AEC_AGC_EnableA_B, 0);//0xaf 开启自动曝光，开启自动增益,若要关闭自动曝光与自动增益，将“3”改为“0”
        
        return 0;
    }
    
    return 1 ;
}

//MT9V034摄像头初始化函数
void Mt9v034Init(void)
{
    while(Mt9v034ModeInit());
    
    Mt9v034GpioInit();  //摄像头中断IO口初始化
    Mt9v034DmaInit((void *)ImageOriginalData, FRAME_WIDTH);
    
    memset(ImageOriginalData,0,sizeof(ImageOriginalData));
    memset(ImageCmprsData,0,sizeof(ImageCmprsData));
    memset(ImageBinarizationData,0,sizeof(ImageBinarizationData));
    memset(JumpRow,0,sizeof(JumpRow));
    memset(JumpColumn,0,sizeof(JumpColumn));
    
    JumpInit();
}

//MT9V034摄像头采集中断服务函数
void Mt9v034IrqHandler(void)
{
    if(MT9V034_FRM_START)     //场中断来了
    {
        MT9V034_FRM_CLEAN;    //清场中断标志位
        if(Mt9v034Status == START)
        {
            Mt9v034Status = CAPTRUEING; //场中断来了，如果摄像头状态机处于START状态，则开始行中断
            return;
        }
    }
    if(MT9V034_LINE_START)    //行中断来了
    {
        MT9V034_LINE_CLEAN;   //清行中断标志位
        if(Mt9v034Status == CAPTRUEING)
        {
            //开启DMA采集中断
            MT9V034_DMA_EN();
            MT9V034_DMA_IRQ_EN();
        }
    }
}

//MT9V034摄像头DMA传输中断服务函数
void Mt9v034DmaHandler(void)
{
    static int row_use=0;   //采集的行数
    MT9V034_DMA_DIS();      //禁止通道硬件DMA请求
    MT9V034_DMA_IRQ_CLEAN();    //清中断标志位，重新使能硬件请求

    Mt9v034DmaDaddrSet((void *)&ImageOriginalData[row_use][0]);
    ++row_use;
    if(row_use == FRAME_HEIGHT)
    {
        row_use = 0;
        ImageCompression(); //图像采集完成，进行图像压缩
        Mt9v034Status = FINISH;
    }    
}

//图像二值化处理
int LightThreshold = 50;
void GetBinarizedImage(void)
{
    int row=0, column=0;
    for(row=0; row<IMG_ROWS; ++row)
    {
        for(column=0; column<IMG_COLUMN; ++column)
        {
            if(ImageCmprsData[row][column] >= LightThreshold)
            {
                ImageBinarizationData[row][column] = 1;
            }
            else
            {
                ImageBinarizationData[row][column] = 0;
            }
        }
    }
}




