//
//  path_planning.h
//  camera smartcar
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//

#ifndef path_planning_h
#define path_planning_h

#include "../bsp/board_driver/mt9v034.h"
//图像坐标点转换
//图像点集在第一象限
//      ^ y
//      |
//      |
//      |
//      |
//      |
//  ---0|-------------> x
//      |
//定义处理图像的大小
#define IMG_ROW_PROC        (IMG_ROWS)  //共60行
#define IMG_ROLUMN_PROC     (IMG_COLUMN)//共120列
#define y_AXIS_MIN          (0)         //y轴范围：0～59           
#define y_AXIS_MAX          (IMG_ROW_PROC-1)    
#define X_AXIS_MIN          (1)         //x轴范围：1～119，其中x=0无效    
#define X_AXIS_MAX          (IMG_ROLUMN_PROC-1)  
#define X_AXIS_CENTER       (IMG_ROLUMN_PROC/2)//图像中心：x=60 
//定义处理图像点(x,y为坐标系点)
#define BLACK                       (0)
#define WHITE                       (1)
#define IMG(x,y)                    (ImageBinarizationData[59-y][x])
#define BASIC_LEFT_EDGE(x,y)        (IMG(x,y)==WHITE && IMG(x-1,y)==BLACK)  //记录的是白点；左边界，左黑右白
#define BASIC_RIGHT_EDGE(x,y)       (IMG(x,y)==WHITE && IMG(x+1,y)==BLACK)  //记录的是白点；左边界，左白右黑

//定义左右搜线最大偏移量
#define OFFSET_MAX      10
//定义左右搜线偏移方向
typedef enum
{
    OFFSET_LEFT = 0,
    OFFSET_RIGHT,
}OffsetDirNode;


//建立图像坐标点结构体
//图像点集在第一象限
//      ^ y
//      |
//      |
//      |
//      |
//      |
//  ---0|-------------> x
//      |
typedef struct
{
    unsigned int x;
    unsigned int y;    
}PointNode;

//建立坐标系
typedef struct
{
    PointNode LeftEdge[200]; //左边界,第1个点为起始点,第0点无效
    PointNode RightEdge[200];//右边界
    PointNode CenterLine[200];//中线
    unsigned int left_order_num;  //定义次序编号，编号1为起始点位置
    unsigned int right_order_num;
    unsigned int center_order_num;
}CoordinateNode;

//赛道路径状态
//特殊元素
typedef enum
{
    OUT_OF_BOUND = 0,   //出界
    NORMAL, //正常赛道类型
    CROSS,  //十字
    CUT_OFF,//断路区
}SpeedwayStatusNode;

//赛道路径信息结构体
typedef struct
{
    float error;    //赛道偏差

    CoordinateNode Coordinate;  //坐标系
    SpeedwayStatusNode SpeedwayStatus; //赛道路径状态
}SpeedwayPathNode;





void SpeedwayPathPlanning(void);//赛道路径规划算法
void BeaconPathPlanning(void);//信标路径规划算法
extern SpeedwayPathNode SpeedwayPath;
extern unsigned char ImgProc[IMG_ROW_PROC][IMG_ROLUMN_PROC];

#endif


