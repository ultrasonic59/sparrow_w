#ifndef __UART_H__
#define __UART_H_
#include "stdint.h"
////===================================
/*
///.6 =1 set led  .4-.0 leds
///.6=0, .5=1 set input
///.6=0,.5=0,4.=1 read adc
#define BIT_CMD_SET_ALL (0x1<<7) /// .5 inp,.4-.0 led
#define BIT_CMD_SET_ASD (0x1<<6)
#define BIT_CMD_SET_INP (0x1<<5)
////#define BIT_CMD_RD_ADC  (0x1<<4)
#define LED4_BIT  (0x1<<4)

#define MASK_ASD        0x0f
#define MASK_INP        0x1
*/
///==============================

#define LEN_RX_BUFF 4

typedef struct rx_cmd_s
{
uint8_t len;
uint8_t buff[LEN_RX_BUFF];
}uart_cmd_t;

extern uart_cmd_t rx_cmd;
///extern uart_cmd_t tx_cmd;

///extern uint8_t  obr_cmd(uart_cmd_t *cmd);
extern uint8_t  obr_cmd(uint8_t cmd);

///extern uint8_t rx_buff[LEN_RX_BUFF];
////extern void uart1_rx_handler(void);
extern volatile uint8_t rx_buff_ready;
///extern void uart_init(void);
extern int rx_receive(void* dat,uint16_t len_rx);
///extern void obr_uart_cmd(uart_cmd_t *cmd);
///extern void obr_uart_cmd(uart_cmd_t *cmd);

///extern void tx_send(uart_cmd_t *cmd);
extern void tx_send(uint8_t odat);


#endif
