/*
 * brook_kbd_brd.h
 *
 *  Created on: Oct 29, 2025
 *      Author: vagol
 */

#ifndef _BROOK_KBD_BRD_H_
#define _BROOK_KBD_BRD_H_

#include "ch32v00x_conf.h"

///========================================
#define UART_DEF_BR 115200
///==============================
#define VERS "1.0"

////=============LED0==================================
  #define LED0_PIN          GPIO_Pin_4
  #define LED0_GPIO         GPIOC
////=============LED1==================================
  #define LED1_PIN          GPIO_Pin_7
  #define LED1_GPIO         GPIOC
////=============LED2==================================
  #define LED2_PIN          GPIO_Pin_6
  #define LED2_GPIO         GPIOC
////=============LED3==================================
  #define LED3_PIN          GPIO_Pin_3
  #define LED3_GPIO         GPIOD
////=============LED4==================================
  #define LED4_PIN          GPIO_Pin_4
  #define LED4_GPIO         GPIOD
////============= B_A ==================================
  #define B_A_PIN          GPIO_Pin_5
  #define B_A_GPIO         GPIOC
////=============I2C_SCL==================================
  #define SCL_PIN          GPIO_Pin_2
  #define SCL_GPIO         GPIOC
////=============I2C_SDA==================================
  #define SDA_PIN          GPIO_Pin_1
  #define SDA_GPIO         GPIOC
////=============XRST==================================
  //#define XRST_PIN          GPIO_Pin_1
  //#define XRST_GPIO         GPIOA
////=============INT_POW==================================
  ///#define INT_POW_PIN          GPIO_Pin_1
  //#define INT_POW_GPIO         GPIOC
////=============KEY==================================
  ///#define KEY_PIN          GPIO_Pin_4
  ///#define KEY_GPIO         GPIOD
////=============VREF==================================
  #define VREF_PIN          GPIO_Pin_2
  #define VREF_GPIO         GPIOD
////====================================================
  extern uint8_t cur_led4;
  extern uint8_t state;

///========================================
extern void init_gpio(void);
extern void init_hw(void);
extern void set_led4(uint8_t idat);
extern void set_input_b_a(uint8_t idat);

#define onoff_gled set_led4
extern void set_scl(uint8_t idat);
extern void set_sda(uint8_t idat);

#endif /* _BROOK_KBD_BRD_H_ */
