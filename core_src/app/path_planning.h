//
//  path_planning.h
//  Motion Planning
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//

#ifndef path_planning_h
#define path_planning_h


//建立图像坐标点
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
typedef enum
{
    OUT_OF_BOUND = 0,
}SpeedwayStatusNode;


//赛道路径信息结构体
typedef struct
{
    CoordinateNode Coordinate;

    SpeedwayStatusNode SpeedwayStatus;
}SpeedwayPathNode;



#endif


