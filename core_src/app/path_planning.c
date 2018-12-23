//
//  path_planning.c
//  camera smartcar
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//

//路径规划算法文件
//向下层轨迹规划和动作规划传递：1.偏差值，2.最小速度距离，3.路径状态
//文件传递顺序为路径规划->轨迹规划->动作规划

#include "path_planning.h"

unsigned char ImgProc[IMG_ROW_PROC][IMG_ROLUMN_PROC];    //定义处理后的图像显示数组

/********赛道路径规划算法********/

SpeedwayPathNode SpeedwayPath =
{
    .error = 0,

    .Coordinate.left_order_num = 0,  //有效点数量初始化为0
    .Coordinate.right_order_num = 0,
    .Coordinate.center_order_num = 0,

    .SpeedwayStatus = NORMAL,   //初始化赛道类型为正常赛道
};

//初始化赛道路径数组
void SpeedwayPathInit(void)
{
    SpeedwayPath.Coordinate.left_order_num = 0; //有效点初始化
    SpeedwayPath.Coordinate.right_order_num = 0;
    SpeedwayPath.Coordinate.center_order_num = 0;
    memset(SpeedwayPath.Coordinate.LeftEdge, 0, sizeof(SpeedwayPath.Coordinate.LeftEdge));
    memset(SpeedwayPath.Coordinate.RightEdge, 0, sizeof(SpeedwayPath.Coordinate.RightEdge));
    memset(SpeedwayPath.Coordinate.CenterLine, 0, sizeof(SpeedwayPath.Coordinate.CenterLine));
    memset(ImgProc, 0, sizeof(ImgProc));
}

//赛道出界检查
//检查图像底部5行，如果超过3行是全黑行，则为出界
//待增加，电感值采集判断
//      ^ y  
//      |
//      |
//      |   赛道图像点集在第一象限
//      |
//      |
//  ---0|-------------> x
//      |
void SpeedwayPathOutCheck(void)
{
    int x,y;
    int valid_point_num = 0;//有效点数量
    int valid_row_num = 0;  //有效行数量

    for(y=y_AXIS_MIN; y<y_AXIS_MIN+5; ++y)
    {
        valid_point_num = 0;
        for(x=X_AXIS_MIN; x<X_AXIS_MAX; ++x)
        {
            if(IMG(x,y) == BLACK) //黑点
            {
                ++valid_point_num;
            }
        }
        if(valid_point_num > X_AXIS_MAX - 10)
        {
            ++valid_row_num;
        }
        if(valid_row_num > 3)
        {
            SpeedwayPath.SpeedwayStatus = OUT_OF_BOUND; //出界
        }
    }
}

//基础寻线
//      ^ y  
//      |
//      |
//      |   赛道图像点集在第一象限
//      |
//      |
//  ---0|-------------> x
//      |
//寻找左边界
void LeftEdgeScanBasic(void)
{
    int x,y;
    int offset = 0; //偏移量
    OffsetDirNode OffsetDir = OFFSET_LEFT; //偏移方向

    //在底部20行内找到起点
    for(y=y_AXIS_MIN; y<20; ++y)
    {
        for(x=X_AXIS_CENTER+10; x>X_AXIS_MIN; --x)
        {
            if(BASIC_LEFT_EDGE(x,y))  //左边界
            {
                ++SpeedwayPath.Coordinate.left_order_num;   
                SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x = x;
                SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y = y;
            }
            if(SpeedwayPath.Coordinate.left_order_num > 3)  //确保起始点有效
            {
                break;
            }
        }
    }
    if(SpeedwayPath.Coordinate.left_order_num > 3)  //存在起始点，且起始点有效，开始左右边缘搜线
    {
        for(y=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y+1; y<y_AXIS_MAX; ++y)
        {
            x = SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x; //更新x轴坐标
            for(offset=0; offset<=OFFSET_MAX; )
            {
                //判断x偏移量是否超范围
                if(x+offset>X_AXIS_MAX || x-offset<X_AXIS_MIN)
                {
                    return;
                }

                if(OffsetDir == OFFSET_LEFT)
                {
                    if(BASIC_LEFT_EDGE(x-offset,y))
                    {
                        ++SpeedwayPath.Coordinate.left_order_num;   
                        SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x = x-offset;
                        SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y = y;
                        break;
                    }
                    ++offset;
                    OffsetDir = OFFSET_RIGHT;
                }
                else if(OffsetDir == OFFSET_RIGHT)
                {
                    if(BASIC_LEFT_EDGE(x+offset,y))
                    {
                        ++SpeedwayPath.Coordinate.left_order_num;   
                        SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x = x+offset;
                        SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y = y;
                        break;
                    }
                    OffsetDir = OFFSET_LEFT;
                }
            }
            if(offset > OFFSET_MAX)    //如果偏移量已达到最大值，则终止基础寻线
            {
                break;
            }
        }
    }
}
//寻找右边界
void RightEdgeScanBasic(void)
{
    int x,y;
    int offset = 0; //偏移量
    OffsetDirNode OffsetDir = OFFSET_RIGHT; //偏移方向

    //在底部20行内找到起点
    for(y=y_AXIS_MIN; y<20; ++y)
    {
        for(x=X_AXIS_CENTER-10; x<X_AXIS_MAX; ++x)
        {
            if(BASIC_RIGHT_EDGE(x,y))  //右边界
            {
                ++SpeedwayPath.Coordinate.right_order_num;   
                SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x = x;
                SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y = y;
            }
            if(SpeedwayPath.Coordinate.right_order_num > 3)  //确保起始点有效
            {
                break;
            }
        }
    }
    if(SpeedwayPath.Coordinate.right_order_num > 3)  //存在起始点，且起始点有效，开始左右边缘搜线
    {
        for(y=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y+1; y<y_AXIS_MAX; ++y)
        {
            x = SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x; //更新x轴坐标
            for(offset=0; offset<=OFFSET_MAX; )
            {
                //判断x偏移量是否超范围
                if(x+offset>X_AXIS_MAX || x-offset<X_AXIS_MIN)
                {
                    return;
                }

                if(OffsetDir == OFFSET_LEFT)
                {
                    if(BASIC_LEFT_EDGE(x-offset,y))
                    {
                        ++SpeedwayPath.Coordinate.right_order_num;   
                        SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x = x-offset;
                        SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y = y;
                        break;
                    }
                    OffsetDir = OFFSET_RIGHT;
                }
                else if(OffsetDir == OFFSET_RIGHT)
                {
                    if(BASIC_LEFT_EDGE(x+offset,y))
                    {
                        ++SpeedwayPath.Coordinate.right_order_num;   
                        SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x = x+offset;
                        SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y = y;
                        break;
                    }
                    ++offset;
                    OffsetDir = OFFSET_LEFT;
                }
            }
            if(offset > OFFSET_MAX)    //如果偏移量已达到最大值，则终止基础寻线
            {
                break;
            }
        }
    }
}



//显示赛道边界以及中线
void ShowSpeedwayImgProc(void)
{
    int order_num;
    //显示左边界
    for(order_num=1; order_num<=SpeedwayPath.Coordinate.left_order_num; ++order_num)
    {
        ImgProc[SpeedwayPath.Coordinate.LeftEdge[order_num].y][SpeedwayPath.Coordinate.LeftEdge[order_num].x] == 1;
    }
    //显示右边界
    for(order_num=1; order_num<=SpeedwayPath.Coordinate.right_order_num; ++order_num)
    {
        ImgProc[SpeedwayPath.Coordinate.RightEdge[order_num].y][SpeedwayPath.Coordinate.RightEdge[order_num].x] == 1;
    }
}

//赛道路径规划算法
void SpeedwayPathPlanning(void)
{
    SpeedwayPathInit();//初始化赛道路径数组
    SpeedwayPathOutCheck();//出界检查
    switch(SpeedwayPath.SpeedwayStatus)
    {
        case OUT_OF_BOUND: 
            SpeedwayPath.error = 0;
            break;

        case NORMAL: 
            LeftEdgeScanBasic();
            RightEdgeScanBasic();
            break;

        case CROSS: 
            break;

        case CUT_OFF: 
            break;
    }
    ShowSpeedwayImgProc();
}





/********信标路径规划算法********/




//信标路径规划算法
void BeaconPathPlanning(void)
{

}




