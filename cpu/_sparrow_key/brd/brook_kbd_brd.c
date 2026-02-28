/*
 * aatt_brd.c
 *
 *  Created on: Dec 21, 2023
 *      Author: vagol
 */
#include "brook_kbd_brd.h"
#include "adc.h"
#include "timers.h"

void init_gpio(void)
{
GPIO_InitTypeDef  GPIO_InitStructure = {0};
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
///=============LED0==================================
GPIO_InitStructure.GPIO_Pin = LED0_PIN;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;///GPIO_Speed_50MHz;
GPIO_Init(LED0_GPIO, &GPIO_InitStructure);
///=============LED1==================================
GPIO_InitStructure.GPIO_Pin = LED1_PIN;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;///GPIO_Speed_50MHz;
GPIO_Init(LED1_GPIO, &GPIO_InitStructure);
///=============LED2==================================
GPIO_InitStructure.GPIO_Pin = LED2_PIN;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;///GPIO_Speed_50MHz;
GPIO_Init(LED2_GPIO, &GPIO_InitStructure);
///=============LED3==================================
GPIO_InitStructure.GPIO_Pin = LED3_PIN;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;///GPIO_Speed_50MHz;
GPIO_Init(LED3_GPIO, &GPIO_InitStructure);
///=============LED4==================================
GPIO_InitStructure.GPIO_Pin = LED4_PIN;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;///GPIO_Speed_50MHz;
GPIO_Init(LED4_GPIO, &GPIO_InitStructure);
///=============B_A==================================
GPIO_InitStructure.GPIO_Pin = B_A_PIN;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;///GPIO_Speed_50MHz;
GPIO_Init(B_A_GPIO, &GPIO_InitStructure);
///=============SCL for debug ==================================
GPIO_InitStructure.GPIO_Pin = SCL_PIN;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;///GPIO_Speed_50MHz;
GPIO_Init(SCL_GPIO, &GPIO_InitStructure);
///=============SDA for debug ==================================
GPIO_InitStructure.GPIO_Pin = SDA_PIN;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;///GPIO_Speed_50MHz;
GPIO_Init(SDA_GPIO, &GPIO_InitStructure);

///=============VREF==================================
GPIO_InitStructure.GPIO_Pin = VREF_PIN;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
////GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;///GPIO_Speed_50MHz;
GPIO_Init(VREF_GPIO, &GPIO_InitStructure);
}
void set_scl(uint8_t idat)
{
 if(idat&0x1)
     GPIO_WriteBit(SCL_GPIO,SCL_PIN,Bit_SET);
 else
     GPIO_WriteBit(SCL_GPIO,SCL_PIN, Bit_RESET);
}
void set_sda(uint8_t idat)
{
 if(idat&0x1)
     GPIO_WriteBit(SDA_GPIO,SDA_PIN,Bit_SET);
 else
     GPIO_WriteBit(SDA_GPIO,SDA_PIN, Bit_RESET);
}

void set_led0(uint8_t idat)
{
 if(idat&0x1)
     GPIO_WriteBit(LED0_GPIO,LED0_PIN,Bit_SET);
 else
     GPIO_WriteBit(LED0_GPIO,LED0_PIN, Bit_RESET);
}
void set_led1(uint8_t idat)
{
 if(idat&0x1)
     GPIO_WriteBit(LED1_GPIO,LED1_PIN,Bit_SET);
 else
     GPIO_WriteBit(LED1_GPIO,LED1_PIN, Bit_RESET);
}
void set_led2(uint8_t idat)
{
 if(idat&0x1)
     GPIO_WriteBit(LED2_GPIO,LED2_PIN,Bit_SET);
 else
     GPIO_WriteBit(LED2_GPIO,LED2_PIN, Bit_RESET);
}
void set_led3(uint8_t idat)
{
 if(idat&0x1)
     GPIO_WriteBit(LED3_GPIO,LED3_PIN,Bit_SET);
 else
     GPIO_WriteBit(LED3_GPIO,LED3_PIN, Bit_RESET);
}
void set_led4(uint8_t idat)
{
 if(idat&0x1)
     GPIO_WriteBit(LED4_GPIO,LED4_PIN,Bit_SET);
 else
     GPIO_WriteBit(LED4_GPIO,LED4_PIN, Bit_RESET);
}

void set_input_b_a(uint8_t idat)
{
if(idat&0x1)
    GPIO_WriteBit(B_A_GPIO,B_A_PIN,Bit_SET);
 else
     GPIO_WriteBit(B_A_GPIO,B_A_PIN, Bit_RESET);
}
/*********************************************************************
 * @fn      USARTx_CFG
 *
 * @brief   Initializes the USART2 & USART3 peripheral.
 *
 * @return  none
 */
void USARTx_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
  ///  NVIC_InitTypeDef  NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

    /* USART1 TX-->D.5   RX-->D.6 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = UART_DEF_BR;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
/*
     NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);;
*/
    USART_Cmd(USART1, ENABLE);
}
//USART_HalfDuplexCmd(USART_TypeDef *USARTx, FunctionalState NewState)
///===================================================

void init_hw(void)
{
//===========================
///CLK_Config();
init_gpio();
USARTx_CFG();
USART_HalfDuplexCmd(USART1, ENABLE);
TIM1_Config();
TIM2_Config();
////    init_gpio();
////uart_init();
 ///   USARTx_CFG();

adc_init();
}


