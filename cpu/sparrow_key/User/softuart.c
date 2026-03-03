
#include "softuart.h"
void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast"))); /* Interrupt handler function */

#if 1
// Some internal define
#define SoftUart_DATA_LEN_C1 (SoftUart_DATA_LEN+1)
#define SoftUart_DATA_LEN_C2 (SoftUart_DATA_LEN+2)

// All Soft Uart Config and State
SoftUart_S       	SUart ;

// TX RX Data Buffer
SoftUartBuffer_S 	SUBuffer;

// For timing division
__IO  uint8_t 		SU_Timer=0;

// Read RX single Pin Value
uint8_t SoftUartGpioReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	return GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);
}

// Write TX single Pin Value
void SoftUartGpioWritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, BitAction PinState)
{
	GPIO_WriteBit(GPIOx,GPIO_Pin,PinState);
}

// Initial Soft Uart
SoftUartState_E SoftUartTxInit(GPIO_TypeDef *TxPort,uint16_t TxPin)
{
SUart.TxNotComplated=0;

SUart.tx_ena=0;

SUart.TxBitCounter=0;
SUart.TxBitShift=0;

///SUart.Buffer=&SUBuffer;

SUart.TxPort=TxPort;
SUart.TxPin=TxPin;

if(ringbuffer_init(&SUart.tx_buffer,SUART_TX_BUFF_SIZE)==0)
  {
////  printk("\r\n+++ringbuffer_init[rx_msp_buffer] error!!! +++\r\n");
  return SoftUart_Error;
  }
return SoftUart_OK;
}

void TIM1_UP_IRQHandler(void)
////void TIM7_IRQHandler(void)
{
 if (TIM_GetFlagStatus(TIM1, TIM_FLAG_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

///TIM7->SR &= ~TIM_SR_UIF; //
    SoftUartHandler();
    }
}



// Send one bit to TX pin
void SoftUartTransmitBit(SoftUart_S *SU,uint8_t Bit0_1)
{
    SoftUartGpioWritePin(SU->TxPort,SU->TxPin,(BitAction)Bit0_1);
}

////volatile uint8_t vtmp;
// Soft Uart Transmit Data Process
void SoftUartTxProcess(SoftUart_S *SU)
{
///vtmp=  SU->tx_ena;
uint8_t tbyte;
if(SU->tx_ena != 0)
  {
  /// Start
  if(SU->TxBitCounter==0)
    {
    SU->TxNotComplated=1;
////    SU->TxBitShift=0;
    SoftUartTransmitBit(SU,0);
    SU->TxBitCounter++;
    ringbuffer_getc(&SU->tx_buffer,&tbyte);  
    SU->cur_tx_data=tbyte;
    }
    //// Data
    else if(SU->TxBitCounter < SoftUart_DATA_LEN_C1)
      {
////			SoftUartTransmitBit(SU,((SU->Buffer->Tx[SU->TxIndex])>>(SU->TxBitShift))&0x01);
      SoftUartTransmitBit(SU,SU->cur_tx_data&0x01); 
      SU->cur_tx_data>>=1;
      SU->TxBitCounter++;
////			SU->TxBitShift++;
      }
//// Stop
    else if(SU->TxBitCounter==SoftUart_DATA_LEN_C1)
      {
      SoftUartTransmitBit(SU,1);
      SU->TxBitCounter++;
      }
    ////Complete
    else if(SU->TxBitCounter==SoftUart_DATA_LEN_C2)
      {
        uint16_t t_cnt=0;
      //// Reset Bit Counter
      SU->TxBitCounter=0;
      //// Ready To Send Another Data
 ////     SU->TxIndex++;
      t_cnt=get_ringbuffer_cnt(&SU->tx_buffer);
      if(t_cnt)

			// Check Size of Data
///			if(SU->TxSize > SU->TxIndex)
			{
				// Continue Sending
				SU->TxNotComplated=1;
				SU->tx_ena=1;
			}
			else
			{
				// Finish
				SU->TxNotComplated=0;
				SU->tx_ena=0;
			}
		}
	}
}

// Wait Until Transmit Completed
// You do not usually need to use this function!
void SoftUartWaitUntilTxComplate(void)
{
	while(SUart.TxNotComplated);
}

// Copy Data to Transmit Buffer and Start Sending
SoftUartState_E SoftUartPuts(uint8_t *Str,uint8_t Len)
{
	int i;

	if(SUart.TxNotComplated)
          return SoftUart_Error;

////	SUart[SoftUartNumber].TxIndex=0;
////	SUart[SoftUartNumber].TxSize=Len;
////==================================================        ();
 for(i=0;i<Len;i++)
	{
////	SUart[SoftUartNumber].Buffer->Tx[i]= Str[i];
        ringbuffer_putc(&SUart.tx_buffer,Str[i]);
	}
        
	SUart.TxNotComplated=1;
	SUart.tx_ena=1;

	return SoftUart_OK;
}
SoftUartState_E SUartPutChar(uint8_t idat)
{
if(SUart.TxNotComplated)
  return SoftUart_Error;
ringbuffer_putc(&SUart.tx_buffer,idat);
  
SUart.TxNotComplated=1;
SUart.tx_ena=1;
return SoftUart_OK;
}
void send_char_suart(char ch)
{
 SUartPutChar((uint8_t)ch);
 SoftUartWaitUntilTxComplate();
}


// SoftUartHandler must call in interrupt every 0.2*(1/BR)
// if BR=9600 then 0.2*(1/9600)=20.8333333 uS
void SoftUartHandler(void)
{
///	uint8_t 	SU_DBuffer;
#if 0
	// Capture RX and Get BitOffset
	SU_DBuffer = SoftUartScanRxPorts();

		// Receive Data if we in middle data pulse position
		if(SUart.RxBitOffset == SU_Timer)
		{
			SoftUartRxDataBitProcess(&SUart,((SU_DBuffer>>0)&0x01));
		}
#endif
	// Sending always happens in the first time slot
	if(SU_Timer==0)
	{
		// Transmit Data
			SoftUartTxProcess(&SUart);
	}

	// Timing process
	SU_Timer++;
	if(SU_Timer >= 5)
          SU_Timer=0;
}

////===========================================


#endif
