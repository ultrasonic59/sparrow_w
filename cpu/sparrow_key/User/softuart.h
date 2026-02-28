#ifndef __SOFT_UART_H__
#define __SOFT_UART_H__

#include <stdint.h>
#include <ch32v00x.h>

///#include "../brd/enc6701_brd.h"
#include "ringbuffer.h"

#define SUART_TIM_PERIOD        159  ///for 19200      ////200
#define SUART_TIM_PRESC         2
#define NUM_SUART_STM8          0

///#define 	NUM_S_UARTS 	1 	// Max 8

#define 	SUART_TX_BUFF_SIZE	32
#define 	SoftUartRxBufferSize	32   ////64

#define 	SoftUart_DATA_LEN       8 	// Max 8 Bit

typedef enum {
	SoftUart_OK,
	SoftUart_Error
}SoftUartState_E;

typedef struct{
////	uint8_t			Tx[SoftUartTxBufferSize];
	uint8_t			Rx[SoftUartRxBufferSize];
}SoftUartBuffer_S;

typedef struct {
	uint8_t			tx_ena;
 	uint8_t			cur_tx_data;
       
	uint8_t 		TxNComplated;
	uint8_t			RxEnable;
	uint8_t 		TxBitShift,TxBitCounter;
	uint8_t 		RxBitShift,RxBitCounter;
////	uint8_t			TxIndex,TxSize;
	uint8_t			RxIndex;
        ringbuffer_t rx_buffer;
        ringbuffer_t tx_buffer;
        
	SoftUartBuffer_S	*Buffer;
	GPIO_TypeDef               *TxPort;
	GPIO_TypeDef               *RxPort;
	uint16_t 		TxPin;
////	GPIO_TypeDef  		*RxPort;
	uint16_t 		RxPin;
	uint8_t 		RxTimingFlag;
	uint8_t 		RxBitOffset;
} SoftUart_S;

//Call Every (0.2)*(1/9600) = 20.83 uS
void 		SoftUartHandler(void);
extern SoftUart_S          SUart;

extern void 		SoftUartWaitUntilTxComplate(void);
extern uint8_t 	SoftUartRxAlavailable(void);
extern SoftUartState_E SoftUartPuts(uint8_t *Str,uint8_t Len);
extern SoftUartState_E SoftUartEnableRx(void);
extern SoftUartState_E SoftUartDisableRx(void);

extern SoftUartState_E SoftUartInit(GPIO_TypeDef *TxPort,uint16_t TxPin,GPIO_TypeDef *RxPort,uint16_t RxPin);

SoftUartState_E SoftUartReadRxBuffer(uint8_t *Buffer,uint8_t Len);
extern SoftUartState_E SUartPutChar(uint8_t idat);
extern void send_char_suart(char ch);
extern void SoftUartTransmitBit(SoftUart_S *SU,uint8_t Bit0_1);

extern uint8_t get_char(void);
extern uint8_t check_rx_rdy(void);
#endif
