#include "common.h"
#include "../bsp/board_driver/key.h"
#include "../bsp/board_driver/mt9v034.h"
#include "timer_interrupt.h"



//系统中断设置
void SetIrq(void)
{
    NVIC_SetPriority(PORTD_IRQn,0);
    NVIC_SetPriority(PIT0_IRQn,1);
    NVIC_SetPriority(DMA0_IRQn,2); 
    //NVIC_SetPriority(UART3_RX_TX_IRQn,1);   //串口
    
    //NVIC_SetPriority(PIT3_IRQn,4);

    set_vector_handler(DMA0_VECTORn , Mt9v034DmaHandler);    //摄像头DMA传输中断
    set_vector_handler(PORTD_VECTORn , Mt9v034IrqHandler);   //摄像头行场中断
    //set_vector_handler(UART3_RX_TX_VECTORn,UART_IRQHandler);
    set_vector_handler(PIT0_VECTORn,PIT0_IRQHandler);   
    //set_vector_handler(PIT3_VECTORn,PIT3_IRQHandler);  

    
    NVIC_EnableIRQ(PORTD_IRQn);
    NVIC_EnableIRQ(DMA0_IRQn); 
    NVIC_EnableIRQ(PIT0_IRQn);
    //NVIC_EnableIRQ(PIT3_IRQn);
    
    //uart_rx_irq_en(UART3); 
    //uart_tx_irq_dis(UART3);
}

//PIT0中断服务函数  //3ms
void PIT0_IRQHandler(void)  //3ms
{
    KeyContinuousCheck();
  
    PIT_Flag_Clear(PIT0);       //清中断标志位
}





