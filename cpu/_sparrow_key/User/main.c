/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *Multiprocessor communication mode routine:
 *Master:USART1_Tx(PD5)\USART1_Rx(PD6).
 *This routine demonstrates that USART1 receives the data sent by CH341 and inverts
 *it and sends it (baud rate 115200).
 *
 *Hardware connection:PD5 -- Rx
 *                     PD6 -- Tx
 *
 */

#include <string.h>
#include <stdlib.h>
#include "debug.h"

#include "../brd/brook_kbd_brd.h"
#include "my_misc.h"
#include "uart.h"


volatile uint16_t cur_adc_dat=0;
volatile uint8_t cur_cmd=0;
uint8_t asd_bat=0; ///0-bat,1-asd

/* Global Variable */
vu8 val;

///===================================================

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
#define LEN_BUF 32
int main(void)
{
uint8_t for_send=0;
///uint8_t tst=0;

asd_bat=0;     ///bat

NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
SystemCoreClockUpdate();
Delay_Init();
#if 0
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(UART_DEF_BR);
#endif
////    printf("SystemClk:%d\r\n",SystemCoreClock);
////    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

///    USARTx_CFG();
#endif
    init_hw();
#if 0
for(;;)
  {
 ////   send_dat(0x42);
  tst++;
  set_nled(tst);
 /// onoff_gled(tst);
//  set_input_b_a(tst);
////    uart_put_byte(0x33) ;
  delay_ms(500);
  }
#endif

 ///   onoff_gled(0);

 ////   init_gpio();
#if 0
    for(;;)
    {
        set_reles(tst);
 ////    tst++;
     tst^=0xff;
     Delay_Ms(500);
    }
#endif
 ////   enableInterrupts();
    __enable_irq();
    ////uart_init();
    ////CLK_CKDIVR=0;
    ///tstl=CLK_ICKR;
    cur_adc_dat=0xff;
    #if 1
    for(;;)
      {
        if(rx_rdy())
        {
        cur_cmd = get_dat();
         if(cur_cmd){
          for_send=obr_cmd(cur_cmd);
          cur_cmd=0;
          tx_send(for_send);
          }
         /*
         if(tx_cmd.len)
          {
          tx_send(&tx_cmd );
         }
         */
      }
      }
    #endif

#if 0
    while(1)
    {

        while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        {
            /* waiting for receiving finish */
        }
        val = (USART_ReceiveData(USART1));
        in_buf[cur_cnt]=val;
        if(cur_cnt<LEN_BUF)
            cur_cnt++;
        if(val=='\r')
            {
            in_buf[cur_cnt-1]='\0';
            t_db=atoi(in_buf);
            set_rele_db(t_db);
            itoa(t_db+1,out_buf,16);
 ////           sprintf(out_buf,"%x\r\n",t_db);
            for(ii=0;ii< strlen(out_buf) ;ii++)
                {
                USART_SendData(USART1, out_buf[ii]);
                while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
                    {
                 /* waiting for sending finish */
                    }
                }
            cur_cnt=0;
            }

    }
#endif
}
