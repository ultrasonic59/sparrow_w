/*
 * sparrow_brd.c
 *
 *  Created on: Feb 28, 2026
 *      Author: vagol
 */
#include "sparrow_brd.h"
#include "uart.h"
#include "timers.h"
#include "softuart.h"


void init_gpio(void)
{
GPIO_InitTypeDef  GPIO_InitStructure = {0};
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
///=============dbg_rx==================================
///GPIO_InitStructure.GPIO_Pin = SOFT_RX_PIN;
///GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
///GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;///GPIO_Speed_50MHz;
///GPIO_Init(SOFT_RX_GPIO, &GPIO_InitStructure);
///============= dbg_tx ==================================
GPIO_PinRemapConfig(GPIO_Remap_PA1_2, DISABLE);  //

GPIO_InitStructure.GPIO_Pin = SOFT_TX_PIN;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;///GPIO_Speed_10MHz;///GPIO_Speed_50MHz;
GPIO_Init(SOFT_TX_GPIO, &GPIO_InitStructure);
///GPIO_WriteBit(SOFT_TX_GPIO,SOFT_TX_PIN,Bit_SET);
///=============led==================================
GPIO_InitStructure.GPIO_Pin = LED_PIN;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;///GPIO_Speed_50MHz;
GPIO_Init(LED_GPIO, &GPIO_InitStructure);
///=============POW==================================
GPIO_InitStructure.GPIO_Pin = ON_POW_PIN;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;///GPIO_Speed_50MHz;
GPIO_Init(ON_POW_GPIO, &GPIO_InitStructure);
///=============key==================================
GPIO_InitStructure.GPIO_Pin = KEY_PIN;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;///GPIO_Speed_50MHz;
GPIO_Init(KEY_GPIO, &GPIO_InitStructure);
}
///=============== led =======================================
void set_led(uint8_t on_off)
{
 if((on_off&0x1)!=0)
     GPIO_WriteBit(LED_GPIO,LED_PIN,Bit_SET);
 else
     GPIO_WriteBit(LED_GPIO,LED_PIN, Bit_RESET);
}
void set_tx(uint8_t on_off)
{
 if((on_off&0x1)!=0)
     GPIO_WriteBit(SOFT_TX_GPIO,SOFT_TX_PIN,Bit_SET);
 else
     GPIO_WriteBit(SOFT_TX_GPIO,SOFT_TX_PIN, Bit_RESET);
}
void set_on_pow(uint8_t on_off)
{
 if((on_off&0x1)!=0)
     GPIO_WriteBit(ON_POW_GPIO,ON_POW_PIN,Bit_SET);
 else
     GPIO_WriteBit(ON_POW_GPIO,ON_POW_PIN, Bit_RESET);
}

////extern void IIC_Init(u32 bound, u16 address);
uint8_t get_key()
{
    return GPIO_ReadInputDataBit(KEY_GPIO,KEY_PIN);
}

void init_hw(void)
{
init_gpio();
///USART1_CFG();

///TIM1_Config(); ///for softuart
///SoftUartTxInit(SOFT_TX_GPIO,SOFT_TX_PIN);
///SoftUartEnableRx();
///============================================

}


