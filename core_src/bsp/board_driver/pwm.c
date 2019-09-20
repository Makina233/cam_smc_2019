//
//  pwm.c
//  camera smartcar
//
//  Copyright © 2019年 赛博智能车实验室. All rights reserved.
//

#include "common.h"
#include "MK60_port.h"  //port复用配置和各种功能配置
#include "MK60_gpio.h"  
#include "MK60_ftm.h"
#include "pwm.h"


//结构体初始化
PwmInfoNode PwmInfo =
{
    .MotorPwm.duty_max = 1950,
    .MotorPwm.duty_min = 50,
    .MotorPwm.duty_motor1 = 1000,
    .MotorPwm.duty_motor2 = 1000,
    .MotorPwm.duty_motor3 = 1000,
    .MotorPwm.duty_motor4 = 1000,
    
    .ServoPwm.servo_delta_duty = 0,  //舵机
    .ServoPwm.servo_duty_mid = 3700,    //舵机中值
    .ServoPwm.servo_duty_min = 3700 - 5000,
    .ServoPwm.servo_duty_max = 3700 + 5000,
};  


//双电机PWM初始化
//FTM0
//PTC1,PTC2,PTC3,PTC4
//PWM频率11.25kHZ
void DoubleMotorPwmInit(void)
{
    port_init(PTC1, ALT4);//设置引脚C1引脚为FTM0_CH0功能
    port_init(PTC2, ALT4);//设置引脚C2引脚为FTM0_CH1功能
    port_init(PTC3, ALT4);//设置引脚C3引脚为FTM0_CH2功能
    port_init(PTC4, ALT4);//设置引脚C4引脚为FTM0_CH3功能

    SIM_SCGC6|=SIM_SCGC6_FTM0_MASK;//使能FTM0时钟
    FTM0_SC=FTM_SC_CLKS(1)|FTM_SC_PS(2);//中断禁止，时钟源来自总线时钟180MHz,8分频后为22.5M
    FTM0_MODE |= FTM_MODE_WPDIS_MASK;//写保护禁止
    FTM0_MODE |=FTM_MODE_FTMEN_MASK;//FTMEN=1
    FTM0_QDCTRL&=~FTM_QDCTRL_QUADEN_MASK;//禁止正交解码模式
    
    FTM0_COMBINE|=FTM_COMBINE_COMBINE0_MASK;//FTM0通道0,1联合使用
    FTM0_COMBINE|=FTM_COMBINE_COMP0_MASK;//FTM0通道0,1互补输出
    FTM0_COMBINE|=FTM_COMBINE_SYNCEN0_MASK;//使能FTM0通道0,1 PWM同步
    
    FTM0_COMBINE|=FTM_COMBINE_COMBINE1_MASK;//FTM0通道2,3联合使用
    FTM0_COMBINE|=FTM_COMBINE_COMP1_MASK;//FTM0通道2,3互补输出
    FTM0_COMBINE|=FTM_COMBINE_SYNCEN1_MASK;//使能FTM0通道2,3 PWM同步
  
    FTM0_SYNCONF|=FTM_SYNCONF_SWWRBUF_MASK;//使能MOD,CNTIN,CV寄存器的软件触发同步
    FTM0_SYNC=FTM_SYNC_CNTMIN_MASK|FTM_SYNC_CNTMAX_MASK;//设置load point
    
    FTM0_C0SC |= FTM_CnSC_ELSB_MASK;//ELSB=1，ELSA=0，左对齐，先高后低
    FTM0_C0SC &= ~FTM_CnSC_ELSA_MASK;
    FTM0_C1SC |= FTM_CnSC_ELSB_MASK;//ELSB=1，ELSA=0，左对齐，先高后低
    FTM0_C1SC &= ~FTM_CnSC_ELSA_MASK;
    
    FTM0_C2SC |= FTM_CnSC_ELSB_MASK;//ELSB=1，ELSA=0，左对齐，先高后低
    FTM0_C2SC &= ~FTM_CnSC_ELSA_MASK;
    FTM0_C3SC |= FTM_CnSC_ELSB_MASK;//ELSB=1，ELSA=0，左对齐，先高后低
    FTM0_C3SC &= ~FTM_CnSC_ELSA_MASK;
    

    FTM0_OUTMASK=0XF0;//通道0,1,2,3输出,屏蔽其它通道
    FTM0_CNTIN=0;//FTM0计数器初始值为0
    FTM0_MOD=2000;//结束值，周期为（MOD-CNTIN+1）*时钟周期;PWM频率为1/T=22.5M/(2000+1)=11.25KHZ
    
    FTM0_C0V=0;//高电平1.5ms
    FTM0_C1V=PwmInfo.MotorPwm.duty_motor1;
    
    FTM0_C2V=0;//高电平1.5ms
    FTM0_C3V=PwmInfo.MotorPwm.duty_motor2;

    FTM0_CNT=0;
    FTM0_SYNC|=FTM_SYNC_SWSYNC_MASK;//使能软件触发
}

//四电机PWM初始化，适用于麦克纳姆轮
//FTM0
//PTC1,PTC2,PTC3,PTC4,PTD4,PTD5,PTD6,PTD7
//PWM频率11.25kHZ
void FourMotorPwmInit(void)
{
    port_init(PTC1, ALT4);//设置引脚C1引脚为FTM0_CH0功能
    port_init(PTC2, ALT4);//设置引脚C2引脚为FTM0_CH1功能
    port_init(PTC3, ALT4);//设置引脚C3引脚为FTM0_CH2功能
    port_init(PTC4, ALT4);//设置引脚C4引脚为FTM0_CH3功能
  
    port_init(PTD4, ALT4);//设置引脚D4引脚为FTM0_CH4功能
    port_init(PTD5, ALT4);//设置引脚D5引脚为FTM0_CH5功能
    port_init(PTD6, ALT4);//设置引脚D6引脚为FTM0_CH6功能
    port_init(PTD7, ALT4);//设置引脚D7引脚为FTM0_CH7功能

    SIM_SCGC6|=SIM_SCGC6_FTM0_MASK;//使能FTM0时钟
    FTM0_SC=FTM_SC_CLKS(1)|FTM_SC_PS(2);//中断禁止，时钟源来自总线时钟180MHz,8分频后为22.5M
    FTM0_MODE |= FTM_MODE_WPDIS_MASK;//写保护禁止
    FTM0_MODE |=FTM_MODE_FTMEN_MASK;//FTMEN=1
    FTM0_QDCTRL&=~FTM_QDCTRL_QUADEN_MASK;//禁止正交解码模式
   
    FTM0_COMBINE|=FTM_COMBINE_COMBINE0_MASK;//FTM0通道0,1联合使用
    FTM0_COMBINE|=FTM_COMBINE_COMP0_MASK;//FTM0通道0,1互补输出
    FTM0_COMBINE|=FTM_COMBINE_SYNCEN0_MASK;//使能FTM0通道0,1 PWM同步
    
    FTM0_COMBINE|=FTM_COMBINE_COMBINE1_MASK;//FTM0通道2,3联合使用
    FTM0_COMBINE|=FTM_COMBINE_COMP1_MASK;//FTM0通道2,3互补输出
    FTM0_COMBINE|=FTM_COMBINE_SYNCEN1_MASK;//使能FTM0通道2,3 PWM同步
    
    FTM0_COMBINE|=FTM_COMBINE_COMBINE2_MASK;//FTM0通道4,5联合使用
    FTM0_COMBINE|=FTM_COMBINE_COMP2_MASK;//FTM0通道4,5互补输出
    FTM0_COMBINE|=FTM_COMBINE_SYNCEN2_MASK;//使能FTM0通道4,5 PWM同步
    
    FTM0_COMBINE|=FTM_COMBINE_COMBINE3_MASK;//FTM0通道6,7联合使用
    FTM0_COMBINE|=FTM_COMBINE_COMP3_MASK;//FTM0通道6,7互补输出
    FTM0_COMBINE|=FTM_COMBINE_SYNCEN3_MASK;//使能FTM0通道6,7 PWM同步
  
    FTM0_SYNCONF|=FTM_SYNCONF_SWWRBUF_MASK;//使能MOD,CNTIN,CV寄存器的软件触发同步
    FTM0_SYNC=FTM_SYNC_CNTMIN_MASK|FTM_SYNC_CNTMAX_MASK;//设置load point
    
    FTM0_C0SC |= FTM_CnSC_ELSB_MASK;//ELSB=1，ELSA=0，左对齐，先高后低
    FTM0_C0SC &= ~FTM_CnSC_ELSA_MASK;
    FTM0_C1SC |= FTM_CnSC_ELSB_MASK;//ELSB=1，ELSA=0，左对齐，先高后低
    FTM0_C1SC &= ~FTM_CnSC_ELSA_MASK;
    
    FTM0_C2SC |= FTM_CnSC_ELSB_MASK;//ELSB=1，ELSA=0，左对齐，先高后低
    FTM0_C2SC &= ~FTM_CnSC_ELSA_MASK;
    FTM0_C3SC |= FTM_CnSC_ELSB_MASK;//ELSB=1，ELSA=0，左对齐，先高后低
    FTM0_C3SC &= ~FTM_CnSC_ELSA_MASK;
    
    FTM0_C4SC |= FTM_CnSC_ELSB_MASK;//ELSB=1，ELSA=0，左对齐，先高后低
    FTM0_C4SC &= ~FTM_CnSC_ELSA_MASK;
    FTM0_C5SC |= FTM_CnSC_ELSB_MASK;//ELSB=1，ELSA=0，左对齐，先高后低
    FTM0_C5SC &= ~FTM_CnSC_ELSA_MASK;
    
    FTM0_C6SC |= FTM_CnSC_ELSB_MASK;//ELSB=1，ELSA=0，左对齐，先高后低
    FTM0_C6SC &= ~FTM_CnSC_ELSA_MASK;
    FTM0_C7SC |= FTM_CnSC_ELSB_MASK;//ELSB=1，ELSA=0，左对齐，先高后低
    FTM0_C7SC &= ~FTM_CnSC_ELSA_MASK;
    

    //FTM0_OUTMASK=0X0F;//此处不需要屏蔽其它通道，因此注释掉
    FTM0_CNTIN=0;//FTM0计数器初始值为0
    FTM0_MOD=2000;//结束值，周期为（MOD-CNTIN+1）*时钟周期=10ms;PWM频率为1/T=22.5M/(2000+1)=11.25KHZ
    
    FTM0_C0V=0;//高电平1.5ms
    FTM0_C1V=PwmInfo.MotorPwm.duty_motor1;
    
    FTM0_C2V=0;//高电平1.5ms
    FTM0_C3V=PwmInfo.MotorPwm.duty_motor2;
    
    FTM0_C4V=0;//高电平1.5ms
    FTM0_C5V=PwmInfo.MotorPwm.duty_motor3;
    
    FTM0_C6V=0;//高电平1.5ms
    FTM0_C7V=PwmInfo.MotorPwm.duty_motor4;

    FTM0_CNT=0;
    FTM0_SYNC|=FTM_SYNC_SWSYNC_MASK;//使能软件触发
}

//舵机PWM初始化
//FTM1
//PTB0
//PWM频率80HZ,精度35000
void ServoPwmInit(void)
{
    FTM_PWM_init(FTM1,FTM_CH0,80,PwmInfo.ServoPwm.servo_duty_mid);
}

//设置舵机PWM占空比
void SetServoPwmDuty(void)
{
    int duty = 0;
    duty = PwmInfo.ServoPwm.servo_delta_duty + PwmInfo.ServoPwm.servo_duty_mid;
    //占空比范围限制
    if(duty > PwmInfo.ServoPwm.servo_duty_max)
    {
        duty = PwmInfo.ServoPwm.servo_duty_max;
    }
    else if(duty < PwmInfo.ServoPwm.servo_duty_min)
    {
        duty = PwmInfo.ServoPwm.servo_duty_min;
    }
    FTM1_C0V=duty;
    FTM1_SYNC |=FTM_SYNC_SWSYNC_MASK;   //使能软件触发
}

//设置电机PWM占空比
void SetMotorPwmDuty(void)
{
    //限制范围
    if(PwmInfo.MotorPwm.duty_motor1 > PwmInfo.MotorPwm.duty_max)
    {
        PwmInfo.MotorPwm.duty_motor1 = PwmInfo.MotorPwm.duty_max;
    }
    else if(PwmInfo.MotorPwm.duty_motor1 < PwmInfo.MotorPwm.duty_min)
    {
        PwmInfo.MotorPwm.duty_motor1 = PwmInfo.MotorPwm.duty_min;
    }
    
    if(PwmInfo.MotorPwm.duty_motor2 > PwmInfo.MotorPwm.duty_max)
    {
        PwmInfo.MotorPwm.duty_motor2 = PwmInfo.MotorPwm.duty_max;
    }
    else if(PwmInfo.MotorPwm.duty_motor2 < PwmInfo.MotorPwm.duty_min)
    {
        PwmInfo.MotorPwm.duty_motor2 = PwmInfo.MotorPwm.duty_min;
    }
    
    if(PwmInfo.MotorPwm.duty_motor3 > PwmInfo.MotorPwm.duty_max)
    {
        PwmInfo.MotorPwm.duty_motor3 = PwmInfo.MotorPwm.duty_max;
    }
    else if(PwmInfo.MotorPwm.duty_motor3 < PwmInfo.MotorPwm.duty_min)
    {
        PwmInfo.MotorPwm.duty_motor3 = PwmInfo.MotorPwm.duty_min;
    }
    
    if(PwmInfo.MotorPwm.duty_motor4 > PwmInfo.MotorPwm.duty_max)
    {
        PwmInfo.MotorPwm.duty_motor4 = PwmInfo.MotorPwm.duty_max;
    }
    else if(PwmInfo.MotorPwm.duty_motor4 < PwmInfo.MotorPwm.duty_min)
    {
        PwmInfo.MotorPwm.duty_motor4 = PwmInfo.MotorPwm.duty_min;
    }
  
    FTM0_C1V = PwmInfo.MotorPwm.duty_motor1;
    FTM0_C3V = PwmInfo.MotorPwm.duty_motor2;
    FTM0_C5V = PwmInfo.MotorPwm.duty_motor3;
    FTM0_C7V = PwmInfo.MotorPwm.duty_motor4;
    
    FTM0_SYNC |=FTM_SYNC_SWSYNC_MASK;   //使能软件触发
}




