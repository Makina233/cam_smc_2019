# camera smartcar

### 项目介绍
摄像头智能车程序麦克纳姆轮（H车模）

### IO口配置说明


### 程序框架

**该程序框架为三部分**

1. app：用于存放用户函数，如 main.c，定时器中断服务函数，循迹算法等；
2. api：用于存放公用算法，如 pid，麦克纳姆轮算法；
3. bsp：用于存放驱动函数，如 芯片驱动，主板外设驱动（oled，摄像头）；

#### APP

##### 1.主函数状态机

``` c
typedef enum
{
    MASTER_GET_INFO,        //摄像头获取信息
    MASTER_ANALYZE_INFO,    //处理采集到的信息
    MASTER_SHOW_INFO,       //显示信息
    MASTER_SEND_INFO,       //发送信息
}MasterMainStatusNode;
```

#### BSP

##### 1.摄像头配置

1. 以下为摄像头对外结构体：

- 用户可通过更改light_threshold_max与light_threshold_min的值来控制软件自动阈值的范围，
- 如果为固定阈值，只需更改light_threshold

``` c
//mt9v034.c
MtImgCfgNode MtImgCfg =
{
    .light_threshold = 150,
    .light_threshold_max = 200, //高于此值一定为白色
    .light_threshold_min = 10,  //低于此值一定为黑色
    
    .image_cmprs = &ImageCmprsData[0][0],
    .image_binarization = &ImageBinarizationData[0][0],
};
```

2. 关于摄像头IO口与图像大小配置

- 用户在此处更改摄像头IO口与图像大小

``` c
//mt9v034.h

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

//定义摄像头原始图像大小
#define FRAME_WIDTH     170
#define FRAME_HEIGHT    115

//压缩后使用的图像大小
#define IMG_ROWS        60
#define IMG_COLUMN      120
```

3. 摄像头对外函数声明

``` c
void Mt9v034Init(void); //摄像头初始化函数
void Mt9v034IrqHandler(void);//MT9V034摄像头采集中断服务函数
void Mt9v034DmaHandler(void);//MT9V034摄像头DMA传输中断服务函数
void GetBinarizedImage(void);//图像二值化处理
void AutoBinarizedImage(void);//图像自动阈值二值化

extern Mt9v034StatusNode Mt9v034Status; //摄像头状态
extern MtImgCfgNode MtImgCfg;//摄像头配置结构体
```

4. 摄像头的使用

- 摄像头采集状态机

``` c
//摄像头采集状态机
typedef enum
{
    WAIT_INIT=0,    
    START,      //摄像头到这个状态需要在主函数中手动设置
    CAPTRUEING,
    FINISH,
}Mt9v034StatusNode;
```

- 使用时首先要初始化状态机为 START
- 等待场中断结束时再触发主循环状态机进入下一状态

``` c
int main()
{
    ...

    Mt9v034Status = START;  //首先让摄像头准备采集(等待场中断)
    while(1)
    {
        switch(MasterMainStatus)
        {
            case MASTER_GET_INFO:       //信息采集
                if(Mt9v034Status == FINISH) //此时摄像头一帧图像已采集完成，开始进行二值化处理
                {
                    //GetBinarizedImage();//图像二值化处理
                    AutoBinarizedImage();//图像自动阈值二值化
                    Mt9v034Status = START;  //处理完成后继续等待场中断触发
                    MasterMainStatus = MASTER_ANALYZE_INFO;
                }
                break;
                
            case MASTER_ANALYZE_INFO:   //信息分析
            ...
}            
```


