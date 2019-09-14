//
//  mt9v034.h
//  mt9v034_use
//
//  Created by 傅凯 on 2018/11.
//  Copyright © 2018年 赛博智能车实验室. All rights reserved.
//

#ifndef mt9v034_h
#define mt9v034_h

//芯片驱动包
#include "common.h"
#include "MK60_port.h"  //port复用配置和各种功能配置
#include "MK60_gpio.h"
#include "MK60_dma.h"

/********摄像头IO口配置（K60）********/
//摄像头IIC引脚
#define MT9V034_SCL_CFG         PTD11   
#define MT9V034_SDA_CFG         PTD10 
//摄像头行场中断部分
#define MT9V034_FRM_IO_CFG      PTD15   //场中断
#define MT9V034_FRM_IO_NUM      15
#define MT9V034_LINE_IO_CFG     PTD13   //行中断
#define MT9V034_LINE_IO_NUM     13
//摄像头DMA传输部分
#define MT9V034_DMA_CH_CFG      DMA_CH0     //定义摄像头DMA传输通道
#define DMA_DATA_IO_CFG         PTD_B0_IN   //定义8位数据输入口
#define DMA_PIX_CFG             PTD14       //定义DMA时钟信号，此处为摄像头的PIX引脚

/********摄像头图像配置********/
//定义摄像头原始图像大小
#define FRAME_WIDTH_CFG     180
#define FRAME_HEIGHT_CFG    120
//压缩后使用的图像大小
#define IMG_ROWS_CFG        60
#define IMG_COLUMN_CFG      120


/********SCCB通信部分********/

//定义摄像头IIC引脚
#define MT9V034_SCL     MT9V034_SCL_CFG
#define MT9V034_SDA     MT9V034_SDA_CFG

//设置数据线主机输入输出方向（主机发送数据or主机接收数据）
#define MT9V034_SDA_DIR_OUT()   (PTXn_T(MT9V034_SDA,DDR) = 1)   //数据线方向为输出
#define MT9V034_SDA_DIR_IN()    (PTXn_T(MT9V034_SDA,DDR) = 0)   //数据线方向为输入

//设置主机发送时钟线和数据线的电平
#define MT9V034_SCL_H()     (PTXn_T(MT9V034_SCL,OUT) = 1)
#define MT9V034_SCL_L()     (PTXn_T(MT9V034_SCL,OUT) = 0)
#define MT9V034_SDA_H()     (PTXn_T(MT9V034_SDA,OUT) = 1)
#define MT9V034_SDA_L()     (PTXn_T(MT9V034_SDA,OUT) = 0)

//设置主机读取数据线电平
#define MT9V034_SDA_READ()  (PTXn_T(MT9V034_SDA,IN))    //主机读取SDA线的电平

//SCCB延时时间
#define SCCB_DELAY()    SccbDelay(500);


/********MT9V034摄像头配置部分********/

//摄像头采集状态机
typedef enum
{
    WAIT_INIT=0,    
    START,      //摄像头到这个状态需要在主函数中手动设置
    CAPTRUEING,
    FINISH,
}Mt9v034StatusNode;

//定义摄像头原始图像大小
#define FRAME_WIDTH     FRAME_WIDTH_CFG
#define FRAME_HEIGHT    FRAME_HEIGHT_CFG

//压缩后使用的图像大小
#define IMG_ROWS        IMG_ROWS_CFG
#define IMG_COLUMN      IMG_COLUMN_CFG


//摄像头行场中断部分
#define MT9V034_FRM_IO      MT9V034_FRM_IO_CFG   //场中断
#define MT9V034_LINE_IO     MT9V034_LINE_IO_CFG  //行中断

#define MT9V034_FRM_START       (PORTD_ISFR & (1<<MT9V034_FRM_IO_NUM))  //场中断触发
#define MT9V034_FRM_CLEAN       (PORTD_ISFR = (1<<MT9V034_FRM_IO_NUM))  //清场中断标志位    
#define MT9V034_LINE_START      (PORTD_ISFR & (1<<MT9V034_LINE_IO_NUM)) //场中断触发
#define MT9V034_LINE_CLEAN      (PORTD_ISFR = (1<<MT9V034_LINE_IO_NUM)) //清场中断标志位  

//摄像头DMA传输部分
#define MT9V034_DMA_CH      MT9V034_DMA_CH_CFG  //定义摄像头DMA传输通道
#define DMA_DATA_IO         DMA_DATA_IO_CFG     //定义8位数据输入口
#define DMA_PIX             DMA_PIX_CFG         //定义DMA时钟信号，此处为摄像头的PIX引脚

#define MT9V034_DMA_EN()        DMA_EN(MT9V034_DMA_CH) //使能通道硬件DMA请求
#define MT9V034_DMA_DIS()       DMA_DIS(MT9V034_DMA_CH) //禁止通道硬件DMA请求
#define MT9V034_DMA_IRQ_EN()    DMA_IRQ_EN(MT9V034_DMA_CH)  //允许DMA通道传输
#define MT9V034_DMA_IRQ_DIS()   DMA_IRQ_DIS(MT9V034_DMA_CH) //禁止DMA通道传输
#define MT9V034_DMA_IRQ_CLEAN() DMA_IRQ_CLEAN(MT9V034_DMA_CH)   //清中断标志位，重新使能硬件请求


//MT9V034摄像头器件地址
#define MT9V034_ADDR            (0xB8)
//MT9V034摄像头寄存器地址
#define MTREG_ChipVersion 										0x00
#define MTREG_ColumnStartContextA 							    0x01
#define MTREG_RowStartContextA 								    0x02
#define MTREG_WindowHeightContextA 						        0x03
#define MTREG_WindowWidthContextA 						        0x04
#define MTREG_HorizontalBlankingContextA 			            0x05
#define MTREG_VerticalBlankingContextA 				            0x06
#define MTREG_ChipControl 										0x07
#define MTREG_CoarseShutterWidth1ContextA 		                0x08
#define MTREG_CoarseShutterWidth2ContextA 		                0x09
#define MTREG_ShutterWidthCtrlContextA 				            0x0a
#define MTREG_CoarseTotalShutterWidthContextA                   0x0b
#define MTREG_Reset 										    0x0c
#define MTREG_ReadModeContextA 								    0x0d
#define MTREG_ReadModeContextB 								    0x0e
#define MTREG_SensorTypeHDREnable 						        0x0f
#define MTREG_LED_OUTCtrl 										0x1b
#define MTREG_Companding 										0x1c
#define MTREG_VREF_ADCControl 								    0x2c
#define MTREG_V1ContextA 										0x31
#define MTREG_V2ContextA 										0x32
#define MTREG_V3ContextA 								        0x33
#define MTREG_V4ContextA 										0x34
#define MTREG_AnalogGainContextA 							    0x35
#define MTREG_AnalogGainContextB 							    0x36
#define MTREG_V1ControlContextB 							    0x39
#define MTREG_V2ControlContextB 							    0x3a
#define MTREG_V3ControlContextB 							    0x3b
#define MTREG_V4ControlContextB 							    0x3c
#define MTREG_FrameDarkAverage 								    0x42
#define MTREG_DarkAvgThresholds 							    0x46
#define MTREG_BLCalibControl 									0x47
#define MTREG_BlackLevelCalibrationValue 			            0x48
#define MTREG_BLCalibStepSize 								    0x4c
#define MTREG_RowNoiseCorrControl 						        0x70
#define MTREG_RowNoiseConstant 								    0x71
#define MTREG_PixclkFVLVCtrl 									0x72
#define MTREG_DigitalTestPattern 							    0x7f
#define MTREG_TileWeight_GainX0_Y0 						        0x80
#define MTREG_TileWeight_GainX1_Y0 						        0x81
#define MTREG_TileWeight_GainX2_Y0 						        0x82
#define MTREG_TileWeight_GainX3_Y0 						        0x83
#define MTREG_TileWeight_GainX4_Y0 						        0x84
#define MTREG_TileWeight_GainX0_Y1 						        0x85
#define MTREG_TileWeight_GainX1_Y1 						        0x86
#define MTREG_TileWeight_GainX2_Y1 						        0x87
#define MTREG_TileWeight_GainX3_Y1 						        0x88
#define MTREG_TileWeight_GainX4_Y1 						        0x89
#define MTREG_TileWeight_GainX0_Y2 						        0x8a
#define MTREG_TileWeight_GainX1_Y2 						        0x8b
#define MTREG_TileWeight_GainX2_Y2 						        0x8c
#define MTREG_TileWeight_GainX3_Y2 						        0x8d
#define MTREG_TileWeight_GainX4_Y2 						        0x8e
#define MTREG_TileWeight_GainX0_Y3 						        0x8f
#define MTREG_TileWeight_GainX1_Y3 						        0x90
#define MTREG_TileWeight_GainX2_Y3 						        0x91
#define MTREG_TileWeight_GainX3_Y3 						        0x92
#define MTREG_TileWeight_GainX4_Y3 						        0x93
#define MTREG_TileWeight_GainX0_Y4 						        0x94
#define MTREG_TileWeight_GainX1_Y4 						        0x95
#define MTREG_TileWeight_GainX2_Y4 						        0x96
#define MTREG_TileWeight_GainX3_Y4 						        0x97
#define MTREG_TileWeight_GainX4_Y4 						        0x98
#define MTREG_TileCoordX0_5 									0x99
#define MTREG_TileCoordX1_5 									0x9a
#define MTREG_TileCoordX2_5 									0x9b
#define MTREG_TileCoordX3_5 									0x9c
#define MTREG_TileCoordX4_5 									0x9d
#define MTREG_TileCoordX5_5 									0x9e
#define MTREG_TileCoordY0_5 									0x9f
#define MTREG_TileCoordY1_5 									0xa0
#define MTREG_TileCoordY2_5 									0xa1
#define MTREG_TileCoordY3_5 									0xa2
#define MTREG_TileCoordY4_5 									0xa3
#define MTREG_TileCoordY5_5 									0xa4
#define MTREG_AEC_AGC_DesiredBin 							    0xa5
#define MTREG_AEC_UpdateFrequency 						        0xa6
#define MTREG_AEC_LPF 											0xa8
#define MTREG_AGC_UpdateFrequency 						        0xa9
#define MTREG_AGC_LPF 											0xaa
#define MTREG_MaxAnalogGain 									0xab
#define MTREG_AEC_MInimumExposure 						        0xac
#define MTREG_AEC_MaximumExposure 						        0xad
#define MTREG_BinDifferenceThreshold 					        0xae
#define MTREG_AEC_AGC_EnableA_B 							    0xaf
#define MTREG_AEC_AGC_PixCount 								    0xb0
#define MTREG_LVDSMasterCtrl 									0xb1
#define MTREG_DataStreamLatency 							    0xb4
#define MTREG_StereoscopErrorCtrl 						        0xb7
#define MTREG_StereoscopErrorFlag 						        0xb8
#define MTREG_AGC_GainOutput 									0xba
#define MTREG_AEC_GainOutput 									0xbb
#define MTREG_AGC_AEC_CurrentBin	 						    0xbc
#define MTREG_InterlaceFieldBlank 						        0xbf
#define MTREG_MonModeCaptureCtrl 							    0xc0
#define MTREG_AntiEclipseControls 						        0xc2
#define MTREG_NTSV_FV_LV_Control 							    0xc6
#define MTREG_NTSC_HorizBlankCtrl 						        0xc7
#define MTREG_NTSC_VertBlankCtrl 							    0xc8
#define MTREG_ColumnStartContextB 						        0xc9
#define MTREG_RowStartContextB 								    0xca
#define MTREG_WindowHeightContextB 						        0xcb
#define MTREG_WindowWidthContextB 						        0xcc
#define MTREG_HorizontalBlankingContextB 			            0xcd
#define MTREG_VerticalBlankingContextB 				            0xce
#define MTREG_CoarseSW1ContextB 							    0xcf
#define MTREG_CoarseSW2ContextB 							    0xd0
#define MTREG_ShutterWidthCtrlContextB 				            0xd1
#define MTREG_CoarseShutterWidthTotalContextB                   0xd2
#define MTREG_FineSW1ContextA 								    0xd3
#define MTREG_FineSW2ContextA 								    0xd4
#define MTREG_FineShutterWidthTotalContextA 	                0xd5
#define MTREG_FineSW1ContextB 								    0xd6
#define MTREG_FineSW2ContextB 								    0xd7
#define MTREG_FineShutterWidthTotalContextB 	                0xd8
#define MTREG_MonitorMode 										0xd9
#define MTREG_BytewiseAddr 										0xf0
#define MTREG_RegisterLock 	                                    0xfe

//摄像头配置结构体
typedef struct
{
    int light_threshold;   //图像阈值
    int delThreshold;       //阈值变化量
    int light_threshold_max;   //图像最大阈值
    int light_threshold_min;   //图像最小阈值
    
    unsigned char *image_cmprs; //图像压缩后的数组
    unsigned char *image_binarization;  //图像二值化数组
}MtImgCfgNode;


void Mt9v034SccbGpioInit(void);//摄像头SCCB引脚初始化函数
void Mt9v034Init(void);//摄像头初始化函数
void Mt9v034IrqHandler(void);//MT9V034摄像头采集中断服务函数
void Mt9v034DmaHandler(void);//MT9V034摄像头DMA传输中断服务函数
void GetBinarizedImage(void);//图像二值化处理
void AutoBinarizedImage(void);//图像自动阈值二值化
int  ThresItera(unsigned char (*ImageCmprsData)[IMG_COLUMN], int *threshold, int delThreshold);//阈值迭代法

extern Mt9v034StatusNode Mt9v034Status; //摄像头状态机
extern unsigned char ImageCmprsData[IMG_ROWS][IMG_COLUMN];  //摄像头压缩后的图像（灰度图）
extern unsigned char ImageBinarizationData[IMG_ROWS][IMG_COLUMN];   //摄像头二值化图像
extern MtImgCfgNode MtImgCfg;//摄像头配置结构体

#endif


