#include <string.h>
////#include <stdio.h>
#include "uart.h"
#include "cmd.h"
#include "my_misc.h"

///volatile uint8_t rx_buff_ready=0;
////static uint8_t rx_buff[LEN_RX_BUFF];
///static uint8_t len_rx_dat=0;
uart_cmd_t rx_cmd={0};
///uart_cmd_t tx_cmd;
////===================================
void uart1_rx_handler(void)
{
uint8_t tdat;  
if(USART1->STATR&USART_FLAG_RXNE)
{
 tdat=(uint8_t)USART1->DATAR;
     if(rx_cmd.len<LEN_RX_BUFF){
         rx_cmd.buff[ rx_cmd.len]= tdat;
         rx_cmd.len++;
      }
  }
}
///==========================================
int uart_put_byte(uint8_t idat) 
{
while((USART1->STATR &USART_FLAG_TXE)==0);
USART1->DATAR = idat;
return 0;
}
int send_dat(uint8_t i_dat)
{
USART1->DATAR = i_dat;
return 0;
}
////=================================================
void tx_send(uint8_t odat)
{
uart_put_byte(odat) ;
}
/*
void tx_send(uart_cmd_t *cmd)
{
uint8_t ii=0;
while(cmd->len)
  {
  uart_put_byte(cmd->buff[ii]) ;
  cmd->len--;
  ii++;
  };
}
*/
volatile uint8_t v_tmp=0;
uint8_t cur_led4=0;
//===========================================
uint8_t  obr_cmd(uint8_t cmd)
{
uint8_t rez=0;
if(cmd&BIT_CMD_SET_ALL)
  {
  set_input_b_a(cmd>>5);
  if(cmd&BIT_CMD_SET_ASD)
    asd_bat=0x1;            ///asd
  else
    asd_bat=0x0;            ///bat
  curr_asd = cmd&0xf;
  if(cmd&LED4_BIT)
    cur_led4=1;
  else
    cur_led4=0;
  rez=cur_adc_dat>>5;
  if(rez<2)
    rez=2;
  }
else if(cmd&BIT_CMD_SET_INP)
  {
  set_input_b_a(cmd&MASK_INP);
  rez=0x1;
  }
return rez;
}
///==========================================

#if 0
void obr_uart_cmd(uart_cmd_t *cmd)
{
 //// int tmp;
////uint16_t rez=0;
uint16_t htmp;
uint8_t btmp;
///char tstr[32];
if(cmd->len==0)
  return ;
btmp= *cmd->buff;
v_tmp=btmp;
switch(btmp)
  {
  case PUT_LEDS:
////    sscanf((const char*)(cmd->buff+1),"%x",&htmp);
 ////   htmp=str_to_byte((char*)(cmd->buff+1));
    htmp=char_to_hex(cmd->buff[1]);
    put_leds(htmp);
///    rez=0;
    break;
   case GET_VERS:
     tx_cmd.buff[0]=GET_VERS;
     
     strcpy((char*)&tx_cmd.buff[1],VERS);
  
     strcpy((char*)&tx_cmd.buff[1+strlen(VERS)],__DATE__);
     tx_cmd.len=strlen((char*)tx_cmd.buff)+1;
      break;
    case GET_BAT:
 ////   char ctmp[5];
 ///   sprintf((char*)buf,"%05d",v_bat);
    htmp=cur_adc_dat;
    tx_cmd.buff[0]=GET_BAT;
 ////   hex_half_to_str(htmp, (char*)&tx_cmd.buff[1]);
 ////   htmp=0x1234;
    hex_half_to_str(htmp, (char*)&tx_cmd.buff[1]);
    tx_cmd.buff[5]='\0';
    tx_cmd.len=6;
#if 0
    rez=adc_results[0];
    rez+=adc_results[0]>>1;
     sprintf((char*)buf,"%c%05d",GET_BAT,rez);
   rez=strlen((char*)buf)+1;
    }
#endif
    break;
  case GET_TEMP:
#if 0    
    tmp=conv_adc_temper(adc_results[1]);
      sprintf((char*)buf,"%c%d",GET_TEMP,tmp);
    rez=strlen((char*)buf)+1;
#endif
      break;
  default:
 ///   rez=0;
    break;
}
////return rez;
}
#endif
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

#if 0
void uart_init(void)
{
UART1_DeInit() ;
UART1_Init(UART_DEF_BR, UART1_WORDLENGTH_8D, 
           UART1_STOPBITS_1, UART1_PARITY_NO, 
           UART1_SYNCMODE_CLOCK_DISABLE ,
           UART1_MODE_TXRX_ENABLE);
UART1_Cmd( ENABLE);
UART1_ITConfig( UART1_IT_RXNE, ENABLE);

}
#endif
