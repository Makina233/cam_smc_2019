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
#define y_AXIS_MIN          (0)         //y轴范围：0～59           
#define y_AXIS_MAX          (IMG_ROWS-1)    
#define X_AXIS_MIN          (1)         //x轴范围：1～119，其中x=0无效    
#define X_AXIS_MAX          (IMG_COLUMN-1)  
#define X_AXIS_CENTER       (IMG_COLUMN/2)//图像中心：x=60 
//定义处理图像点(x,y为坐标系点)
#define BLACK                       (0)
#define WHITE                       (1)
#define IMG(x,y)                    (ImageBinarizationData[59-y][x])
#define BASIC_LEFT_EDGE(x,y)        (IMG(x,y)==WHITE && IMG(x-1,y)==BLACK)  //记录的是白点；左边界，左黑右白
#define BASIC_RIGHT_EDGE(x,y)       (IMG(x,y)==WHITE && IMG(x+1,y)==BLACK)  //记录的是白点；左边界，左白右黑




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
    PointNode Point[200];   //假定一条线最多为200个点
    unsigned int order_num;  //定义次序编号
}CoordinateNode;

//赛道路径状态
//特殊元素
typedef enum
{
    OUT_OF_BOUND = 0,   //出界
    CROSS,  //十字
}SpeedwayStatusNode;

//赛道路径信息结构体
typedef struct
{
    CoordinateNode Coordinate;

    SpeedwayStatusNode SpeedwayStatus;
}SpeedwayPathNode;






#endif


