#include <string.h>
////#include <stdio.h>
#include "uart.h"
#include "../brd/sparrow_brd.h"

////===================================
void uart1_rx_handler(void)
{
uint8_t tdat;
tdat=(uint8_t)USART1->DATAR;
///hdlc_on_bytein(tdat);
}
////=================================================
uint8_t uart_get_byte(void) 
{
while((USART1->STATR & USART_FLAG_RXNE)!=0);
return ((uint8_t)USART1->DATAR);
}
uint8_t get_dat(void)
{
  return (uint8_t )USART1->DATAR;
}

uint8_t rx_rdy(void)
{
uint8_t tmp;
tmp=USART1->STATR;
if(tmp&USART_FLAG_RXNE)
  return 1;
else if(tmp&USART_FLAG_ORE)
  return 1;
else
  return 0;

}
///========================================================================
void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
/*********************************************************************
 * @fn      USART1_IRQHandler
 *
 * @brief   This function handles USART3 global interrupt request.
 *
 * @return  none
 */
void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uart1_rx_handler();
     }
}
/*********************************************************************
 * @fn      USART1_CFG
 *
 * @brief   Initializes the USART1 peripheral.
 *
 * @return  none
 */
void USART1_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

    /* USART1   RX-->D.6 */
    GPIO_InitStructure.GPIO_Pin = URX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(URX_GPIO, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = DEF_BR;///460800;///115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode =  USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

