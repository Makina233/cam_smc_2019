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
    .Coordinate.CenterFarPoint.x = 0,
    .Coordinate.CenterFarPoint.y = 0,
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
    memset(SpeedwayPath.Coordinate.FiveDirectPoint, 0, sizeof(SpeedwayPath.Coordinate.FiveDirectPoint));
    memset(SpeedwayPath.Coordinate.EigthOutPoint, 0, sizeof(SpeedwayPath.Coordinate.EigthOutPoint));
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
//滚动寻线
//      ^ y  
//      |
//      |
//      |   赛道图像点集在第一象限
//      |
//      |
//  ---0|-------------> x
//      |
//滚动寻找左边界
void LeftEdgeScanRoll(void)
{
    int x,y;
    int i;
    PointNode LastOrderPoint = 
    {
        .x=0,
        .y=0,
    };
    //在底部20行内找到起点
    for(y=y_AXIS_MIN; y<20; ++y)
    {
        for(x=X_AXIS_CENTER+10; x>X_AXIS_MIN; --x)
        {
            if(BASIC_LEFT_EDGE(x,y))  //左边界
            {
                LastOrderPoint.x=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x;
                LastOrderPoint.y=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y;
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
    if(SpeedwayPath.Coordinate.left_order_num > 3)  //存在起始点，且起始点有效，开始搜线
    {
        //滚动寻线
        do
        {
            //初始化九宫格外八点(左上角为一号点，顺时针方向)
            SpeedwayPath.Coordinate.EigthOutPoint[1].x=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x-1;
            SpeedwayPath.Coordinate.EigthOutPoint[1].y=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y+1;

            SpeedwayPath.Coordinate.EigthOutPoint[2].x=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x;
            SpeedwayPath.Coordinate.EigthOutPoint[2].y=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y+1;
            
            SpeedwayPath.Coordinate.EigthOutPoint[3].x=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x+1;
            SpeedwayPath.Coordinate.EigthOutPoint[3].y=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y+1;

            SpeedwayPath.Coordinate.EigthOutPoint[4].x=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x+1;
            SpeedwayPath.Coordinate.EigthOutPoint[4].y=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y;

            SpeedwayPath.Coordinate.EigthOutPoint[5].x=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x+1;
            SpeedwayPath.Coordinate.EigthOutPoint[5].y=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y-1;

            SpeedwayPath.Coordinate.EigthOutPoint[6].x=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x;
            SpeedwayPath.Coordinate.EigthOutPoint[6].y=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y-1;

            SpeedwayPath.Coordinate.EigthOutPoint[7].x=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x-1;
            SpeedwayPath.Coordinate.EigthOutPoint[7].y=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y-1;

            SpeedwayPath.Coordinate.EigthOutPoint[8].x=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x-1;
            SpeedwayPath.Coordinate.EigthOutPoint[8].y=SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y;
            //确定上一边界点在九宫格外八点位置
            for ( i = 1; i < 8; i++)
            {
                if (LastOrderPoint.x==SpeedwayPath.Coordinate.EigthOutPoint[i].x && LastOrderPoint.y==SpeedwayPath.Coordinate.EigthOutPoint[i].y)
                {
                    break; 
                }       
            }
            //寻找下个边界点
            for (      ; i <= 8; i++)
            {
                if (IMG(SpeedwayPath.Coordinate.EigthOutPoint[i].x, SpeedwayPath.Coordinate.EigthOutPoint[i].y) == BLACK)
                {
                    if (i>=8)    i=0;
                    if (IMG(SpeedwayPath.Coordinate.EigthOutPoint[i+1].x, SpeedwayPath.Coordinate.EigthOutPoint[i+1].y) == WHITE)
                    {
                        LastOrderPoint.x = SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x;//更新坐标
                        LastOrderPoint.y = SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y;
                        ++SpeedwayPath.Coordinate.left_order_num;
                        SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x = SpeedwayPath.Coordinate.EigthOutPoint[i+1].x;
                        SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y = SpeedwayPath.Coordinate.EigthOutPoint[i+1].y;
                        break;
                    }
                }
                if (i>=8)    i=0;
            }
        } while (SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x<119 \
                && SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].x>1 \
                && SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y<59 \
                && SpeedwayPath.Coordinate.LeftEdge[SpeedwayPath.Coordinate.left_order_num].y>0);   
    }

}

//滚动寻找右边界
void RightEdgeScanRoll(void)
{
    int x,y;
    int i;
    PointNode LastOrderPoint = 
    {
        .x=0,
        .y=0,
    };
    //在底部20行内找到起点
    for(y=y_AXIS_MIN; y<20; ++y)
    {
        for(x=X_AXIS_CENTER-10; x<X_AXIS_MAX; ++x)
        {
            if(BASIC_RIGHT_EDGE(x,y))  //右边界
            {
                LastOrderPoint.x=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x;
                LastOrderPoint.y=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y;
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
    if(SpeedwayPath.Coordinate.right_order_num > 3)  //存在起始点，且起始点有效，开始搜线
    {
        //滚动寻线
        do
        {
            //初始化九宫格外八点(右上角为一号点，逆时针方向)
            SpeedwayPath.Coordinate.EigthOutPoint[1].x=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x+1;
            SpeedwayPath.Coordinate.EigthOutPoint[1].y=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y+1;

            SpeedwayPath.Coordinate.EigthOutPoint[2].x=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x;
            SpeedwayPath.Coordinate.EigthOutPoint[2].y=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y+1;
            
            SpeedwayPath.Coordinate.EigthOutPoint[3].x=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x-1;
            SpeedwayPath.Coordinate.EigthOutPoint[3].y=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y+1;

            SpeedwayPath.Coordinate.EigthOutPoint[4].x=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x-1;
            SpeedwayPath.Coordinate.EigthOutPoint[4].y=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y;

            SpeedwayPath.Coordinate.EigthOutPoint[5].x=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x-1;
            SpeedwayPath.Coordinate.EigthOutPoint[5].y=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y-1;

            SpeedwayPath.Coordinate.EigthOutPoint[6].x=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x;
            SpeedwayPath.Coordinate.EigthOutPoint[6].y=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y-1;

            SpeedwayPath.Coordinate.EigthOutPoint[7].x=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x+1;
            SpeedwayPath.Coordinate.EigthOutPoint[7].y=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y-1;

            SpeedwayPath.Coordinate.EigthOutPoint[8].x=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x+1;
            SpeedwayPath.Coordinate.EigthOutPoint[8].y=SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y;
            //确定上一边界点在九宫格外八点位置
            for ( i = 1; i < 8; i++)
            {
                if (LastOrderPoint.x==SpeedwayPath.Coordinate.EigthOutPoint[i].x && LastOrderPoint.y==SpeedwayPath.Coordinate.EigthOutPoint[i].y)
                {
                    break;
                }        
            }
            //寻找下个边界点
            for (      ; i <= 8; i++)
            {
                if (IMG(SpeedwayPath.Coordinate.EigthOutPoint[i].x, SpeedwayPath.Coordinate.EigthOutPoint[i].y) == BLACK)
                {
                    if (i>=8)   i=0;
                    if (IMG(SpeedwayPath.Coordinate.EigthOutPoint[i+1].x, SpeedwayPath.Coordinate.EigthOutPoint[i+1].y) == WHITE)
                    {
                        LastOrderPoint.x = SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x;//更新坐标
                        LastOrderPoint.y = SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y;
                        ++SpeedwayPath.Coordinate.right_order_num;
                        SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x = SpeedwayPath.Coordinate.EigthOutPoint[i+1].x;
                        SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y = SpeedwayPath.Coordinate.EigthOutPoint[i+1].y;
                        break;
                    }
                }
                if (i>=8)    i=0;
            }
            
        } while (SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x<119 \
                && SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].x>1 \
                && SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y<59\
                && SpeedwayPath.Coordinate.RightEdge[SpeedwayPath.Coordinate.right_order_num].y>0);  
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

//寻找主道最远点
void FindFarthestPoint(void)
{
    int Num=0;
    int x,y;
    int x_start=0;
    int x_end=0;
    int x_flag=0;
    int y_flag=0;
    int width=0;//主道最远点所处横线的宽度

    //寻找横向最多白点段
    for (x = 1; x <= IMG_COLUMN; x++)
    {
        if (IMG(x,0)==WHITE && IMG(x+1,0)==WHITE)//是连续白点
        {
            Num++;
        }
        if (IMG(x,0)==WHITE && IMG(x+1,0)==BLACK)//连续白点结束
        {
            if (Num > x_end-x_start)
            {
                x_end = x;
                x_start  = x-Num+1;
            }
            Num =0;//清零计数
        }
    }

    //寻找横向最多白点段
    Num = 0;//清零计数
    for(x=x_start; x<=x_end; x++)
    { 
        for(y=0; y<60; y++)
        {
            if(IMG(x,y)==BLACK) break;
        }
         if(y>Num) //高度大于后一个
        {
            Num=y;//更新Num值为纵坐标
            x_flag=x;//记录横坐标
            y_flag=y;
            width=0;//清零i值
        }
        if(y==Num) ++width;
    }
    SpeedwayPath.Coordinate.CenterFarPoint.x = x_flag-width/2;
    SpeedwayPath.Coordinate.CenterFarPoint.y = y_flag;

    // //显示主道最远点
    // int row = 59 - SpeedwayPath.Coordinate.CenterFarPoint.y;
    // int column = SpeedwayPath.Coordinate.CenterFarPoint.x;
    // for (int i=-2; i<=2; i++)
    // {
    //     ImgProc[row][column+i] = 1;
    // }
    // for (int j=-2; j<=2; j++)
    // {
    //     ImgProc[row+j][column] = 1;
    // }
}

//五方投射
//      ^ y  
//      |   \    |    /
//      |    \   |   /
//      |     \  |  /
//      |      \ | /
//      |       \|/
//  ---0|--------|-----> x
//      |
void FiveDirections(void)
{
    int x = IMG_COLUMN/2;
    int y = 0;
    int Num =0;
    //一号点
    for ( x = IMG_COLUMN/2; x >= 0; x--)
    {
        if(IMG(x,0) == BLACK)
        {
            Num++;
            SpeedwayPath.Coordinate.FiveDirectPoint[Num].x = x;
            SpeedwayPath.Coordinate.FiveDirectPoint[Num].y = y;
            break;
        }
    }
    //二号点
    for ( x = IMG_COLUMN/2; x >= 0; x--)
    {
        for ( y = 0; y <= IMG_ROWS; y++)
        {
            if(IMG(x,y) == BLACK)
            {
                Num++;
                SpeedwayPath.Coordinate.FiveDirectPoint[Num].x = x;
                SpeedwayPath.Coordinate.FiveDirectPoint[Num].y = y;
                break;
            }            
        }   
    }
    //三号点
    for ( y = 0; y <= IMG_ROWS; y++)
    {
        if(IMG(IMG_COLUMN/2,y) == BLACK)
        {
            Num++;
            SpeedwayPath.Coordinate.FiveDirectPoint[Num].x = x;
            SpeedwayPath.Coordinate.FiveDirectPoint[Num].y = y;
            break;
        }  
    }
    //四号点
    for ( x = IMG_COLUMN/2; x <= IMG_COLUMN; x++)
    {
        for ( y = 0; y <= IMG_ROWS; y++)
        {
            if(IMG(x,y) == BLACK)
            {
                Num++;
                SpeedwayPath.Coordinate.FiveDirectPoint[Num].x = x;
                SpeedwayPath.Coordinate.FiveDirectPoint[Num].y = y;
                break;
            }
        } 
    }
    //五号点
    for ( x = IMG_COLUMN/2; x <= IMG_COLUMN; x++)
    {
        if(IMG(x,0) == BLACK)
        {
            Num++;
            SpeedwayPath.Coordinate.FiveDirectPoint[Num].x = x;
            SpeedwayPath.Coordinate.FiveDirectPoint[Num].y = y;
            break;
        } 
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
    // //显示中心线
    // for(order_num=1; order_num<=SpeedwayPath.Coordinate.center_order_num; ++order_num)
    // {
    //     //坐标还原
    //     row = IMG_ROW_PROC - 1 - SpeedwayPath.Coordinate.CenterLine[order_num].y;
    //     column = SpeedwayPath.Coordinate.CenterLine[order_num].x;
    //     ImgProc[row][column] = 1;
    // }
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
            LeftEdgeScanRoll(); //滚动寻边界
            RightEdgeScanRoll();
            //FindFarthestPoint();
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




