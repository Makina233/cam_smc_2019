#include "common.h"
#include "MK60_port.h"  //port复用配置和各种功能配置
#include "MK60_gpio.h"
#include "key.h"




//拨码开关初始化
void SwitchKeyInit(void)
{
    //gpio_init(SWITCH1,GPI,HIGH);
    //gpio_init(SWITCH2,GPI,HIGH);
    //gpio_init(SWITCH3,GPI,HIGH);
    //gpio_init(SWITCH4,GPI,HIGH);
    gpio_init(SWITCH5,GPI,HIGH);
    gpio_init(SWITCH6,GPI,HIGH);
    gpio_init(SWITCH7,GPI,HIGH);
    gpio_init(SWITCH8,GPI,HIGH);
    //port_init_NoALT(SWITCH1,PULLUP);
    //port_init_NoALT(SWITCH2,PULLUP);
    //port_init_NoALT(SWITCH3,PULLUP);
    //port_init_NoALT(SWITCH4,PULLUP);
    port_init_NoALT(SWITCH5,PULLUP);
    port_init_NoALT(SWITCH6,PULLUP);
    port_init_NoALT(SWITCH7,PULLUP);
    port_init_NoALT(SWITCH8,PULLUP);
}








