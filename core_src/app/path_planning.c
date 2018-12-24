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
                    if(BASIC_RIGHT_EDGE(x-offset,y))
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
                    if(BASIC_RIGHT_EDGE(x+offset,y))
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

//赛道中心线基础计算
void CenterLineCalcBasic(void)
{
    int temp = 1, num = 1, center_num = 1;
    int change_num;
    int start_y;    //定义起始y坐标

    if(SpeedwayPath.Coordinate.left_order_num == 0)
    {
        SpeedwayPath.Coordinate.left_order_num = 1;
        SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x = X_AXIS_MIN;
        SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y = y_AXIS_MIN;
    }
    if(SpeedwayPath.Coordinate.right_order_num == 0)
    {
        SpeedwayPath.Coordinate.right_order_num = 1;
        SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x = X_AXIS_MAX;
        SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y = y_AXIS_MIN;
    }
    
    //计算中心线的起始有效点数，方便之后的起始区补线
    start_y = (SpeedwayPath.Coordinate.LeftEdge[1].y + SpeedwayPath.Coordinate.RightEdge[1].y) >> 1;

    //左边界有效点数量多
    if(SpeedwayPath.Coordinate.left_order_num >= SpeedwayPath.Coordinate.right_order_num)
    {
        temp = 1;
        center_num = start_y;
        change_num = SpeedwayPath.Coordinate.left_order_num / SpeedwayPath.Coordinate.right_order_num;
        for(num=1; num<=SpeedwayPath.Coordinate.left_order_num; ++num)
        {
            if((num % change_num == 0) && (temp < SpeedwayPath.Coordinate.right_order_num)) //防止超范围
            {
                ++temp;
            }
            SpeedwayPath.Coordinate.CenterLine[center_num].x = (SpeedwayPath.Coordinate.LeftEdge[num].x + SpeedwayPath.Coordinate.RightEdge[temp].x) >> 1;
            SpeedwayPath.Coordinate.CenterLine[center_num].y = (SpeedwayPath.Coordinate.LeftEdge[num].y + SpeedwayPath.Coordinate.RightEdge[temp].y) >> 1;
            ++center_num;
        }
        SpeedwayPath.Coordinate.center_order_num = center_num - 1;
    }
    //右边界有效点数量多
    else
    {
        temp = 1;
        center_num = start_y;
        change_num = SpeedwayPath.Coordinate.right_order_num / SpeedwayPath.Coordinate.left_order_num;
        for(num=1; num<=SpeedwayPath.Coordinate.right_order_num; ++num)
        {
            if((num % change_num == 0) && (temp < SpeedwayPath.Coordinate.left_order_num))
            {
                ++temp;
            }
            SpeedwayPath.Coordinate.CenterLine[center_num].x = (SpeedwayPath.Coordinate.LeftEdge[temp].x + SpeedwayPath.Coordinate.RightEdge[num].x) >> 1;
            SpeedwayPath.Coordinate.CenterLine[center_num].y = (SpeedwayPath.Coordinate.LeftEdge[temp].y + SpeedwayPath.Coordinate.RightEdge[num].y) >> 1;
            ++center_num;
        }
        SpeedwayPath.Coordinate.center_order_num = center_num - 1;
    }
    //起始区补线
    for(num=1; num<start_y; ++num)
    {
        SpeedwayPath.Coordinate.CenterLine[num].x = SpeedwayPath.Coordinate.CenterLine[start_y].x;
        SpeedwayPath.Coordinate.CenterLine[num].y = num;
    }
}




//显示赛道边界以及中线
void ShowSpeedwayImgProc(void)
{
    int order_num;
    int row, column;

    //显示中心基准线
    for(row = IMG_ROW_PROC-1; row > IMG_ROW_PROC - 10; --row)
    {
        if(row % 3 == 0)
        {
            ImgProc[row][X_AXIS_CENTER] = 0;
        }
        else
        {
            ImgProc[row][X_AXIS_CENTER] = 1;
        }
    }

    //显示左边界
    for(order_num=1; order_num<=SpeedwayPath.Coordinate.left_order_num; ++order_num)
    {
        //坐标还原
        row = IMG_ROW_PROC - 1 - SpeedwayPath.Coordinate.LeftEdge[order_num].y;
        column = SpeedwayPath.Coordinate.LeftEdge[order_num].x;
        ImgProc[row][column] = 1;
    }
    //显示右边界
    for(order_num=1; order_num<=SpeedwayPath.Coordinate.right_order_num; ++order_num)
    {
        //坐标还原
        row = IMG_ROW_PROC - 1 - SpeedwayPath.Coordinate.RightEdge[order_num].y;
        column = SpeedwayPath.Coordinate.RightEdge[order_num].x;
        ImgProc[row][column] = 1;
    }
    //显示中心线
    for(order_num=1; order_num<=SpeedwayPath.Coordinate.center_order_num; ++order_num)
    {
        //坐标还原
        row = IMG_ROW_PROC - 1 - SpeedwayPath.Coordinate.CenterLine[order_num].y;
        column = SpeedwayPath.Coordinate.CenterLine[order_num].x;
        ImgProc[row][column] = 1;
    }
}

//赛道路径规划算法
void SpeedwayPathPlanning(void)
{
    SpeedwayPathInit();//初始化赛道路径数组
    //SpeedwayPathOutCheck();//出界检查
    switch(SpeedwayPath.SpeedwayStatus)
    {
        case OUT_OF_BOUND: 
            SpeedwayPath.error = 0;
            break;

        case NORMAL: 
            LeftEdgeScanBasic();    //左右边界基础寻线
            RightEdgeScanBasic();
            CenterLineCalcBasic();
            break;

        case CROSS: 
            break;

        case CUT_OFF: 
            break;
    }
    ShowSpeedwayImgProc();//显示赛道边界以及中线
}





/********信标路径规划算法********/




//信标路径规划算法
void BeaconPathPlanning(void)
{

}




