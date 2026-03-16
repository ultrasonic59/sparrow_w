/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *USART Print debugging routine:
 *USART1_Tx(PD6).
 *This example demonstrates the use of USART1(PD6) as a print debug port output.
 *
 */

#include "../brd/sparrow_brd.h"
#include "debug.h"
#include "softuart.h"
#define NUM_CNT_PUSH 8
static uint8_t prev_pow_on=0;
static uint8_t pow_on=0;

static uint8_t cnt_push=0;
static uint8_t cur_key=0;
static uint8_t prev_key=0xff;

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
   uint8_t ena_change=0;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    init_hw();
///    printf("SystemClk:%d\r\n", SystemCoreClock);
///    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
///    printf("This is printf example\r\n");
    prev_pow_on=0xfe;
   pow_on=0;

   cnt_push=0;
   cur_key=0xfe;
   prev_key=0xff;
    set_led(pow_on);
    set_on_pow(pow_on);

    while(1)
    {
        cur_key=  get_key();
        if(prev_key!=cur_key){
            prev_key=cur_key;
            cnt_push=0;
        }
        else if(!cur_key) {
             if(cnt_push<NUM_CNT_PUSH){
                cnt_push++;
                set_led(cnt_push);
                ena_change=1;
             }
             else if(ena_change){
                 pow_on=~pow_on;
                 if(prev_pow_on!=pow_on){
                     prev_pow_on=pow_on;
                     set_led(pow_on);
                     set_on_pow(pow_on);
                     ena_change=0;
                 }
             }
 ///       }
        Delay_Ms(200);
       }
///      send_char_suart('5');

    }
}
