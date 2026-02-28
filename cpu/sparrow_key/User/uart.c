#include <string.h>
////#include <stdio.h>
#include "uart.h"
#include "../brd/enc6701_brd.h"
#include "hdlc.h"

////===================================
void uart1_rx_handler(void)
{
uint8_t tdat;  
tdat=(uint8_t)USART1->DATAR;
hdlc_on_bytein(tdat);
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
uint8_t uart_send_buff(uint8_t *buff, uint16_t len)
{
 uint16_t ii;
 ///printf( "send :");

 for(ii=0;ii<len;ii++){
     uart_put_byte(buff[ii]);
 ///    printf( "[%02x]",buff[ii]);
}
 ////printf( "\r\n");

 return len;
}
////=================================================
#if 0
volatile uint8_t v_tmp=0;
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
  case PUT_VIBRO:
////    sscanf((const char*)(cmd->buff+1),"%d",&htmp);
    htmp=str_to_half((char*)(cmd->buff+1));
    set_vibro(htmp);
///    rez=0;
    break;
  case PUT_LEDS:
////    sscanf((const char*)(cmd->buff+1),"%x",&htmp);
 ////   htmp=str_to_byte((char*)(cmd->buff+1));
    htmp=char_to_hex(cmd->buff[1]);
    put_leds(htmp);
///    rez=0;
    break;
  case PUT_POW_GEN:
 ////   htmp=str_to_byte((char*)(cmd->buff+1));
    htmp=char_to_hex(cmd->buff[1]);
    onoff_pow_gen(htmp);
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
///========================================================================
#if 1
uint8_t _uart_send_buff(uint8_t *buff, uint16_t len)
{
uint8_t rez=0;
DMA_Cmd(DMA1_Channel4, DISABLE);
DMA1_Channel4->MADDR=(uint32_t)buff;   ///set addr mem
///DMA_SetCurrDataCounter(DMA2_Channel5, len);
DMA1_Channel4->CNTR = len;            ///set len dat
DMA_Cmd(DMA1_Channel4, ENABLE);
DMA_ClearFlag(DMA1_FLAG_TC4);
///DMA2->INTFCR=0x50000;

///USART_DMACmd(DEF_UART, USART_DMAReq_Tx, ENABLE);

while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET) /* Wait until USART2 RX DMA1 Transfer Complete */
{
#if 0
    tmpreg = DMA2->INTFR;
    if(prev_tmpreg!= tmpreg)
    {
        prev_tmpreg= tmpreg ;
    fprintf(stderr,"\n\rINTFR[%x]" ,tmpreg);
    }
 ///   DMA2->INTFCR=tmpreg;
  ///  tmpreg = DMA2->INTFR;
  ///  fprintf(stderr,"\n\rINTFR[%x]" ,tmpreg);

  ///  tmpreg = DEF_UART->CTLR3;
 ///   fprintf(stderr,"\n\rCTLR3[%x]" ,tmpreg);
///    tmpreg = DEF_UART->STATR;
 ////   fprintf(stderr,"\n\rSTATR[%x]" ,tmpreg);

    Delay_Ms(2);
#endif
;////vTaskDelay(1);
}
while ((USART1->STATR&USART_FLAG_TC)==0)
{
;////vTaskDelay(1);
}

////taskEXIT_CRITICAL();
/*
    while ((rs485_ports[num_port].uart_port->STATR&USART_FLAG_TXE)==0)
    {
    vTaskDelay(1);
    }
    */
////printf("\n\rena_tx485(num_port,0)");
////set_tst1(0);

///ena_tx485(num_port,0);
////set_tst1(0);
return rez;
}
#endif

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

    /* USART1 TX-->D.5   RX-->D.6 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = BR_ENC;///460800;///115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    DMA_Cmd(DMA1_Channel4, ENABLE); /* USART1 Tx */

    USART_Cmd(USART1, ENABLE);
}
void DMA_INIT(void)
{
    DMA_InitTypeDef DMA_InitStructure = {0};
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA1_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DATAR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_hdlc.snd_buff;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = g_hdlc.snd_buff_len;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);

}

