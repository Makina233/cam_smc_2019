//
//  path_planning.c
//  camera smartcar
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//

//路径规划算法文件
//传出偏差值和最小速度距离值
#include "path_planning.h"

char ImgProc[IMG_ROW_PROC][IMG_ROLUMN_PROC];    //定义处理后的图像显示数组

SpeedwayPathNode SpeedwayPath =
{
    .Coordinate.order_num = 0,  //有效点数量初始化为0
    .SpeedwayStatus = NORMAL,   //初始化赛道类型为正常赛道
};

//初始化赛道路径数组
void SpeedwayPathInit(void)
{
    memset(SpeedwayPath.Coordinate.LeftEdge, 0, sizeof(SpeedwayPath.Coordinate.LeftEdge));
    
}















