/*
 * enc6701_brd.h
 *
 *  Created on: Feb 28, 2026
 *      Author: vagol
 */

#ifndef _SPARROW_BRD_H_
#define _SPARROW_BRD_H_

#include "ch32v00x_conf.h"
///========================================
////=============dbg_rx==================================
 /// #define SOFT_RX_PIN          GPIO_Pin_1
///  #define SOFT_RX_GPIO         GPIOA
/// #define SOFT_RX_TIM           TIM1
////============= dbg_tx ==================================
  #define SOFT_TX_PIN          GPIO_Pin_2
  #define SOFT_TX_GPIO         GPIOA
  #define SOFT_TX_TIM           TIM2
////=============led==================================
  #define LED_PIN          GPIO_Pin_1
  #define LED_GPIO         GPIOC
////=============ON_POW==================================
  #define ON_POW_PIN          GPIO_Pin_2
  #define ON_POW_GPIO         GPIOC
////=============KEY==================================
  #define KEY_PIN          GPIO_Pin_4
  #define KEY_GPIO         GPIOC
////=============uart_rx==================================
  #define URX_PIN          GPIO_Pin_6
  #define URX_GPIO         GPIOD
///========================================
#define TIMER_PER 1000
#define TIMER_PRE 100

///#define RXAdderss   0x02

extern void init_gpio(void);
extern void set_led(uint8_t on_off);
extern void set_tx(uint8_t on_off);
extern void set_on_pow(uint8_t on_off);
extern uint8_t get_key();
extern void init_hw(void);

#endif /* _SPARROW_BRD_H__ */
