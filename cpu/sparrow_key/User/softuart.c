
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
SoftUartState_E SoftUartInit(GPIO_TypeDef *TxPort,uint16_t TxPin,GPIO_TypeDef *RxPort,uint16_t RxPin)
{
SUart.TxNComplated=0;

SUart.RxBitCounter=0;
SUart.RxBitShift=0;
SUart.RxIndex=0;

SUart.tx_ena=0;
SUart.RxEnable=0;

SUart.TxBitCounter=0;
SUart.TxBitShift=0;
////SUart[SoftUartNumber].TxIndex=0;

/////SUart[SoftUartNumber].TxSize=0;

SUart.Buffer=&SUBuffer;

SUart.RxPort=RxPort;
SUart.RxPin=RxPin;

SUart.TxPort=TxPort;
SUart.TxPin=TxPin;

SUart.RxTimingFlag=0;
SUart.RxBitOffset=0;
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

// Enable Soft Uart Receiving
SoftUartState_E SoftUartEnableRx(void)
{
	SUart.RxEnable=1;
	return SoftUart_OK;
}

// Disable Soft Uart Receiving
SoftUartState_E SoftUartDisableRx(void)
{
SUart.RxEnable=0;
return SoftUart_OK;
}

// Read Size of Received Data in buffer
uint8_t SoftUartRxAlavailable(void)
{
	return SUart.RxIndex;
}

// Move Received Data to Another Buffer
SoftUartState_E SoftUartReadRxBuffer(uint8_t *Buffer,uint8_t Len)
{
	int i;
	for(i=0;i<Len;i++)
	{
		Buffer[i]=SUart.Buffer->Rx[i];
	}
	for(i=0;i<SUart.RxIndex;i++)
	{
		SUart.Buffer->Rx[i]=SUart.Buffer->Rx[i+Len];
	}
	SUart.RxIndex-=Len;
	return SoftUart_OK;
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
    SU->TxNComplated=1;
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
      //// Reset Bit Counter
      SU->TxBitCounter=0;
      //// Ready To Send Another Data
 ////     SU->TxIndex++;
      if(get_ringbuffer_cnt(&SU->tx_buffer))
        
			// Check Size of Data
///			if(SU->TxSize > SU->TxIndex)
			{
				// Continue Sending
				SU->TxNComplated=1;
				SU->tx_ena=1;
			}
			else
			{
				// Finish
				SU->TxNComplated=0;
				SU->tx_ena=0;
			}
		}
	}
}

// Soft Uart Receive Data Process
void SoftUartRxDataBitProcess(SoftUart_S *SU,uint8_t B0_1)
{
if(SU->RxEnable)
  {
// Start
  if(SU->RxBitCounter==0)
    {
    // Start Bit is 0
    if(B0_1)
       return;

    SU->RxBitShift=0;
    SU->RxBitCounter++;
    SU->Buffer->Rx[SU->RxIndex]=0;
    }
		// Data
		else if(SU->RxBitCounter<SoftUart_DATA_LEN_C1)
		{
			SU->Buffer->Rx[SU->RxIndex]|=((B0_1&0x01)<<SU->RxBitShift);
			SU->RxBitCounter++;
			SU->RxBitShift++;
		}
		// Stop and Complete
		else if(SU->RxBitCounter==SoftUart_DATA_LEN_C1)
		{
			SU->RxBitCounter=0;
			SU->RxTimingFlag=0;

			//Stop Bit must be 1
			if(B0_1)
			{
				// Received successfully
				// Change RX Buffer Index
				if((SU->RxIndex)<(SoftUartRxBufferSize-1))(SU->RxIndex)++;
			}
			// if not : ERROR -> Overwrite data
		}
	}
}

// Wait Until Transmit Completed
// You do not usually need to use this function!
void SoftUartWaitUntilTxComplate(void)
{
	while(SUart.TxNComplated);
}

// Copy Data to Transmit Buffer and Start Sending
SoftUartState_E SoftUartPuts(uint8_t *Str,uint8_t Len)
{
	int i;

	if(SUart.TxNComplated)
          return SoftUart_Error;

////	SUart[SoftUartNumber].TxIndex=0;
////	SUart[SoftUartNumber].TxSize=Len;
////==================================================        ();
 for(i=0;i<Len;i++)
	{
////	SUart[SoftUartNumber].Buffer->Tx[i]= Str[i];
        ringbuffer_putc(&SUart.tx_buffer,Str[i]);
	}
        
	SUart.TxNComplated=1;
	SUart.tx_ena=1;

	return SoftUart_OK;
}
SoftUartState_E SUartPutChar(uint8_t idat)
{
if(SUart.TxNComplated)
  return SoftUart_Error;
ringbuffer_putc(&SUart.tx_buffer,idat);
  
SUart.TxNComplated=1;
SUart.tx_ena=1;
return SoftUart_OK;
}
void send_char_suart(char ch)
{
 SUartPutChar((uint8_t)ch);
 SoftUartWaitUntilTxComplate();
}

// Capture RX and Get BitOffset
uint8_t SoftUartScanRxPorts(void)
{
	uint8_t Buffer=0x00,Bit;

		// Read RX GPIO Value
		Bit=SoftUartGpioReadPin(SUart.RxPort,SUart.RxPin);

		// Starting conditions
		if(!SUart.RxBitCounter && !SUart.RxTimingFlag && !Bit)
		{
			// Save RX Bit Offset
			// Calculate middle position of data puls
			SUart.RxBitOffset=((SU_Timer+2)%5);

			// Timing Offset is Set
			SUart.RxTimingFlag=1;
		}

		// Add all RX GPIO State to Buffer
		Buffer|=((Bit&0x01)<<0);

	return Buffer;
}

// SoftUartHandler must call in interrupt every 0.2*(1/BR)
// if BR=9600 then 0.2*(1/9600)=20.8333333 uS
void SoftUartHandler(void)
{
	uint8_t 	SU_DBuffer;

	// Capture RX and Get BitOffset
	SU_DBuffer = SoftUartScanRxPorts();

		// Receive Data if we in middle data pulse position
		if(SUart.RxBitOffset == SU_Timer)
		{
			SoftUartRxDataBitProcess(&SUart,((SU_DBuffer>>0)&0x01));
		}


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
uint8_t get_char(void)
{
    uint8_t ch;
    while(SoftUartRxAlavailable()==0);
    SoftUartReadRxBuffer(&ch,1);
    return ch;
}
uint8_t check_rx_rdy(void)
{
return SoftUartRxAlavailable();
}

////===========================================


#endif
