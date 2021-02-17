///#include ""
///#include "xparameters.h"
#include "xplatform_info.h"
#include "xuartps.h"
#include "xil_exception.h"
#include "xil_printf.h"
///#include "xintc.h"
#include "xscugic.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "_hdlc.h"
#include "ringbuffer.h"

extern hdlc_stat_t g_hdlc_bt;

#define UART_INT_IRQ_ID		XPAR_XUARTPS_0_INTR
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID

#define TEST_BUFFER_SIZE	100

XUartPs Uart0Ps;
#define INTC		XScuGic


//// INTC InterruptController;	/* Instance of the Interrupt Controller */
static int SetupInterruptSystem(INTC *IntcInstancePtr,
				XUartPs *UartInstancePtr,
				u16 UartIntrId);

void Handler(void *CallBackRef, u32 Event, unsigned int EventData);


static u8 SendBuffer[TEST_BUFFER_SIZE];	/* Buffer for Transmitting Data */
static u8 RecvBuffer[TEST_BUFFER_SIZE];	/* Buffer for Receiving Data */

volatile int TotalReceivedCount;
volatile int TotalSentCount;
int TotalErrorCount;
extern XScuGic xInterruptController;
#define InterruptController xInterruptController
///==================================================
#if 0
static XUartPs xUARTInstance;
extern XScuGic xInterruptController;
void prvUART_Handler( void *pvNotUsed );

xComPortHandle xSerialPortInitMinimal( uint32_t ulWantedBaud, UBaseType_t uxQueueLength )
{
BaseType_t xStatus;
XUartPs_Config *pxConfig;

	/* Create the queue used to hold received characters.  NOTE THE COMMENTS AT
	THE TOP OF THIS FILE REGARDING THE USE OF QUEUES FOR THIS PURPSOE. */
	xRxQueue = xQueueCreate( uxQueueLength, sizeof( char ) );
	configASSERT( xRxQueue );

	/* Create the semaphore used to signal the end of a transmission, then take
	the semaphore so it is in the correct state the first time
	xSerialSendString() is called.  A block time of zero is used when taking
	the semaphore as it is guaranteed to be available (it was just created). */
	xTxCompleteSemaphore = xSemaphoreCreateBinary();
	configASSERT( xTxCompleteSemaphore );
	xSemaphoreTake( xTxCompleteSemaphore, 0 );

	/* Look up the UART configuration then initialise the dirver. */
	pxConfig = XUartPs_LookupConfig( XPAR_XUARTPS_0_DEVICE_ID );

	/* Initialise the driver. */
	xStatus = XUartPs_CfgInitialize( &xUARTInstance, pxConfig, XPAR_PS7_UART_1_BASEADDR );
	configASSERT( xStatus == XST_SUCCESS );
	( void ) xStatus; /* Remove compiler warning if configASSERT() is not defined. */

	/* Misc. parameter configuration. */
	XUartPs_SetBaudRate( &xUARTInstance, ulWantedBaud );
	XUartPs_SetOperMode( &xUARTInstance, XUARTPS_OPER_MODE_NORMAL );

	/* Install the interrupt service routine that is defined within this
	file. */
	xStatus = XScuGic_Connect( &xInterruptController, XPAR_XUARTPS_1_INTR,  (Xil_ExceptionHandler) prvUART_Handler, (void *) &xUARTInstance );
	configASSERT( xStatus == XST_SUCCESS );
	( void ) xStatus; /* Remove compiler warning if configASSERT() is not defined. */

	/* Ensure interrupts start clear. */
	XUartPs_WriteReg( XPAR_PS7_UART_1_BASEADDR, XUARTPS_ISR_OFFSET, XUARTPS_IXR_MASK );

	/* Enable the UART interrupt within the GIC. */
	XScuGic_Enable( &xInterruptController, XPAR_XUARTPS_1_INTR );

	/* Enable the interrupts of interest in the UART. */
	XUartPs_SetInterruptMask( &xUARTInstance, XUARTPS_IXR_RXFULL | XUARTPS_IXR_RXOVR | XUARTPS_IXR_TOUT | XUARTPS_IXR_TXEMPTY );

	/* Set the receive timeout. */
	XUartPs_SetRecvTimeout( &xUARTInstance, 8 );

	return ( xComPortHandle ) 0;
}
#endif

///==================================================

int init_uart0(void)
{
int Status;
XUartPs_Config *ua0cfg;
int Index;
u32 IntrMask;
int BadByteCount = 0;
///return XST_FAILURE;

ua0cfg = XUartPs_LookupConfig(0);
if (NULL == ua0cfg)
	{
	return XST_FAILURE;
	}
Status = XUartPs_CfgInitialize(&Uart0Ps, ua0cfg, ua0cfg->BaseAddress);
if (Status != XST_SUCCESS) {
	return XST_FAILURE;
}

/* Check hardware build */
Status = XUartPs_SelfTest(&Uart0Ps);
if (Status != XST_SUCCESS) {
	return XST_FAILURE;
}
#if 1
Status = SetupInterruptSystem(&InterruptController, &Uart0Ps, UART_INT_IRQ_ID );
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif
XUartPs_SetHandler(&Uart0Ps, (XUartPs_Handler)Handler, &Uart0Ps);
#if 0
IntrMask =
		XUARTPS_IXR_TOUT | XUARTPS_IXR_PARITY | XUARTPS_IXR_FRAMING |
		XUARTPS_IXR_OVER | XUARTPS_IXR_TXEMPTY | XUARTPS_IXR_RXFULL |
		XUARTPS_IXR_RXOVR;
#endif

#if 1
IntrMask =	XUARTPS_IXR_TXEMPTY|XUARTPS_IXR_TTRIG
		| XUARTPS_IXR_RXFULL|XUARTPS_IXR_RXOVR;
#endif

XUartPs_SetInterruptMask(&Uart0Ps, IntrMask);
////XUartPs_SetRecvTimeout(&Uart0Ps, 8);

#if 0
XUartPs_SetOperMode(&Uart0Ps, XUARTPS_OPER_MODE_LOCAL_LOOP);


XUartPs_SetRecvTimeout(&Uart0Ps, 8);

for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {

		SendBuffer[Index] = (Index % 26) + 'A';

		RecvBuffer[Index] = 0;
	}

XUartPs_Recv(&Uart0Ps, RecvBuffer, TEST_BUFFER_SIZE);

XUartPs_Send(&Uart0Ps, SendBuffer, TEST_BUFFER_SIZE);
while (1) {
	if ((TotalSentCount == TEST_BUFFER_SIZE) &&
	    (TotalReceivedCount == TEST_BUFFER_SIZE)) {
		break;
	}
}

/* Verify the entire receive buffer was successfully received */
for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
	if (RecvBuffer[Index] != SendBuffer[Index]) {
		BadByteCount++;
	}
}
#endif
/* Set the UART in Normal Mode */
XUartPs_SetOperMode(&Uart0Ps, XUARTPS_OPER_MODE_NORMAL);

////u8 XUartPs_GetFifoThreshold(XUartPs *InstancePtr)
Status=XUartPs_GetFifoThreshold(&Uart0Ps);

 XUartPs_SetFifoThreshold(&Uart0Ps, 1);

Status=XUartPs_GetFifoThreshold(&Uart0Ps);

/* If any bytes were not correct, return an error */
///if (BadByteCount != 0) {
///	return XST_FAILURE;
///}

return XST_SUCCESS;
}
void Uart0_InterruptHandler(void);

///================================================================
static int SetupInterruptSystem(INTC *IntcInstancePtr,
				XUartPs *UartInstancePtr,
				u16 UartIntrId)
{
int Status;

	XScuGic_Config *IntcConfig; /* Config for interrupt controller */

	/* Initialize the interrupt controller driver */
IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
if (NULL == IntcConfig)
	{
	return XST_FAILURE;
	}
///====================================
///	return XST_SUCCESS;
///====================================

	Status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect the interrupt controller interrupt handler to the
	 * hardware interrupt handling logic in the processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler) XScuGic_InterruptHandler,
				IntcInstancePtr);
#if 0
	/*
	 * Connect a device driver handler that will be called when an
	 * interrupt for the device occurs, the device driver handler
	 * performs the specific interrupt processing for the device
	 */
	Status = XScuGic_Connect(IntcInstancePtr, UartIntrId,
				  (Xil_ExceptionHandler) XUartPs_InterruptHandler,
				  (void *) UartInstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

#endif
#if 1
	/*
	 * Connect a device driver handler that will be called when an
	 * interrupt for the device occurs, the device driver handler
	 * performs the specific interrupt processing for the device
	 */
	Status = XScuGic_Connect(IntcInstancePtr, UartIntrId,
				  (Xil_ExceptionHandler)Uart0_InterruptHandler,
				  (void *) UartInstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

#endif

/* Enable the interrupt for the device */
XScuGic_Enable(IntcInstancePtr, UartIntrId);

	/* Enable interrupts */
Xil_ExceptionEnable();


return XST_SUCCESS;
}
#if 0
int enaInterruptUart0(void)
{
int Status;
INTC IntcInstance;
INTC *IntcInstancePtr=(INTC *)&IntcInstance;
XScuGic_Config *IntcConfig; /* Config for interrupt controller */

/* Initialize the interrupt controller driver */
IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
if (NULL == IntcConfig)
	{
	return XST_FAILURE;
	}
///====================================
///	return XST_SUCCESS;
///====================================
Status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	XScuGic_Enable(IntcInstancePtr, UART_INT_IRQ_ID);
	return XST_SUCCESS;
}
#endif
///=========================================================
#if 1
void on_int_tx_uart_bt(void)
{
uint32_t t_mask;
t_mask=XUartPs_GetInterruptMask(&Uart0Ps);
t_mask |= XUARTPS_IXR_TXEMPTY;
XUartPs_SetInterruptMask(&Uart0Ps, t_mask);

}
#endif
#if 1
void off_int_tx_uart_bt(void)
{
uint32_t t_mask;
t_mask=XUartPs_GetInterruptMask(&Uart0Ps);
t_mask &= ~XUARTPS_IXR_TXEMPTY;
XUartPs_SetInterruptMask(&Uart0Ps, t_mask);
}
#endif
extern ringbuffer_t rx_msp_buffer;

///================================================================
void Handler(void *CallBackRef, u32 Event, unsigned int EventData)
{
	int ii;
uint8_t rsv_dat;
uint8_t ch;
signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
signed portBASE_TYPE xTaskWoken= pdFALSE;

	/* All of the data has been sent */
if (Event == XUARTPS_EVENT_SENT_DATA)
	{
		TotalSentCount = EventData;
#if 0
	    if (xQueueReceiveFromISR(g_hdlc_bt.ua_snd_dat, &ch, &xTaskWoken ) == pdPASS)
///		if (xQueueReceive(g_hdlc_bt.ua_snd_dat, &ch, 0 ) == pdPASS)
	      {
			XUartPs_WriteReg(Uart0Ps.Config.BaseAddress,
					   XUARTPS_FIFO_OFFSET,ch);

////	      UART_BT->DR = ch & 0x1FF;
	      }
	    if (xQueueIsQueueEmptyFromISR(g_hdlc_bt.ua_snd_dat))
	      {
	    	off_int_tx_uart_bt();
///	      UART_BT->CR1 &= ~USART_CR1_TXEIE;		      // disable TX interrupt if nothing to send
	      }
#endif
///	      UART_BT->SR &= ~USART_IT_TXE;              ///  USART_ClearITPendingBit(UART_BT, USART_IT_TXE);
	}

	/* All of the data has been received */
	if (Event == XUARTPS_EVENT_RECV_DATA)
	{
		TotalReceivedCount = EventData;
		if(EventData>0)
		{
		for(ii=0;ii<EventData;ii++)
		{
#if 1

		rsv_dat=XUartPs_ReadReg(Uart0Ps.Config.BaseAddress,XUARTPS_FIFO_OFFSET)	;
		ringbuffer_putc(&rx_msp_buffer,rsv_dat);
///		XUartPs_ReadReg(InstancePtr->Config.
///			  BaseAddress,
///			  XUARTPS_FIFO_OFFSET);
#if 1
		 if (hdlc1_on_bytein(&g_hdlc_bt, rsv_dat) > 0)
		    {
		    xQueueSendFromISR(g_hdlc_bt.ev_rsv_frame, &rsv_dat, &xHigherPriorityTaskWoken);
///		    xQueueSend(g_hdlc_bt.ev_rsv_frame, &rsv_dat,0);
		    }
#endif
		}
		}
#endif
	}
#if 1
	/*
	 * Data was received, but not the expected number of bytes, a
	 * timeout just indicates the data stopped for 8 character times
	 */
	if (Event == XUARTPS_EVENT_RECV_TOUT) {
		TotalReceivedCount = EventData;
	}

	/*
	 * Data was received with an error, keep the data but determine
	 * what kind of errors occurred
	 */
	if (Event == XUARTPS_EVENT_RECV_ERROR) {
		TotalReceivedCount = EventData;
		TotalErrorCount++;
	}

	/*
	 * Data was received with an parity or frame or break error, keep the data
	 * but determine what kind of errors occurred. Specific to Zynq Ultrascale+
	 * MP.
	 */
	if (Event == XUARTPS_EVENT_PARE_FRAME_BRKE) {
		TotalReceivedCount = EventData;
		TotalErrorCount++;
	}

	/*
	 * Data was received with an overrun error, keep the data but determine
	 * what kind of errors occurred. Specific to Zynq Ultrascale+ MP.
	 */
	if (Event == XUARTPS_EVENT_RECV_ORERR) {
		TotalReceivedCount = EventData;
		TotalErrorCount++;
	}
#endif
}
///==============================================
void send_byte_uart0_(uint8_t ch)
{
while ((XUartPs_IsTransmitFull(Uart0Ps.Config.BaseAddress)));

XUartPs_WriteReg(Uart0Ps.Config.BaseAddress,
		   XUARTPS_FIFO_OFFSET,ch);
}

void send_byte_uart0(uint8_t ch)
{
	while(XUartPs_Send(&Uart0Ps, &ch, 1)!=1);
}
///=========================================================
int chk_key(void)
{
return XUartPs_IsReceiveData(STDOUT_BASEADDRESS);
}
uint8_t get_key(void)
{
return XUartPs_RecvByte(STDOUT_BASEADDRESS)	;
}
///===========================================
#if 0
static void ReceiveDataHandler(XUartPs *InstancePtr)
{
	/*
	 * If there are bytes still to be received in the specified buffer
	 * go ahead and receive them. Removing bytes from the RX FIFO will
	 * clear the interrupt.
	 */
	 if (InstancePtr->ReceiveBuffer.RemainingBytes != (u32)0) {
		(void)XUartPs_ReceiveBuffer(InstancePtr);
	}

	 /* If the last byte of a message was received then call the application
	 * handler, this code should not use an else from the previous check of
	 * the number of bytes to receive because the call to receive the buffer
	 * updates the bytes ramained
	 */
	if (InstancePtr->ReceiveBuffer.RemainingBytes == (u32)0) {
		InstancePtr->Handler(InstancePtr->CallBackRef,
				XUARTPS_EVENT_RECV_DATA,
				(InstancePtr->ReceiveBuffer.RequestedBytes -
				InstancePtr->ReceiveBuffer.RemainingBytes));
	}

}
#endif
static void SendDataHandler(XUartPs *InstancePtr, u32 IsrStatus)
{

	/*
	 * If there are not bytes to be sent from the specified buffer then disable
	 * the transmit interrupt so it will stop interrupting as it interrupts
	 * any time the FIFO is empty
	 */
	if (InstancePtr->SendBuffer.RemainingBytes == (u32)0) {
		XUartPs_WriteReg(InstancePtr->Config.BaseAddress,
				XUARTPS_IDR_OFFSET,
				((u32)XUARTPS_IXR_TXEMPTY | (u32)XUARTPS_IXR_TXFULL));

		/* Call the application handler to indicate the sending is done */
		InstancePtr->Handler(InstancePtr->CallBackRef,
					XUARTPS_EVENT_SENT_DATA,
					InstancePtr->SendBuffer.RequestedBytes -
					InstancePtr->SendBuffer.RemainingBytes);
	}

	/* If TX FIFO is empty, send more. */
	else if((IsrStatus & ((u32)XUARTPS_IXR_TXEMPTY)) != (u32)0) {
		(void)XUartPs_SendBuffer(InstancePtr);
	}
	else {
		/* Else with dummy entry for MISRA-C Compliance.*/
		;
	}
}
static void ReceiveErrorHandler(XUartPs *InstancePtr, u32 IsrStatus)
{
	u32 EventData;
	u32 Event;

	InstancePtr->is_rxbs_error = 0;

	if ((InstancePtr->Platform == XPLAT_ZYNQ_ULTRA_MP) &&
		(IsrStatus & ((u32)XUARTPS_IXR_PARITY | (u32)XUARTPS_IXR_RBRK
					| (u32)XUARTPS_IXR_FRAMING))) {
		InstancePtr->is_rxbs_error = 1;
	}
	/*
	 * If there are bytes still to be received in the specified buffer
	 * go ahead and receive them. Removing bytes from the RX FIFO will
	 * clear the interrupt.
	 */

	(void)XUartPs_ReceiveBuffer(InstancePtr);

	if (!(InstancePtr->is_rxbs_error)) {
		Event = XUARTPS_EVENT_RECV_ERROR;
		EventData = InstancePtr->ReceiveBuffer.RequestedBytes -
			InstancePtr->ReceiveBuffer.RemainingBytes;

		/*
		 * Call the application handler to indicate that there is a receive
		 * error or a break interrupt, if the application cares about the
		 * error it call a function to get the last errors.
		 */
		InstancePtr->Handler(InstancePtr->CallBackRef,
					Event,
					EventData);
	}
}

static void ReceiveTimeoutHandler(XUartPs *InstancePtr)
{
	u32 Event;

	/*
	 * If there are bytes still to be received in the specified buffer
	 * go ahead and receive them. Removing bytes from the RX FIFO will
	 * clear the interrupt.
	 */
	if (InstancePtr->ReceiveBuffer.RemainingBytes != (u32)0) {
		(void)XUartPs_ReceiveBuffer(InstancePtr);
	}

	/*
	 * If there are no more bytes to receive then indicate that this is
	 * not a receive timeout but the end of the buffer reached, a timeout
	 * normally occurs if # of bytes is not divisible by FIFO threshold,
	 * don't rely on previous test of remaining bytes since receive
	 * function updates it
	 */
	if (InstancePtr->ReceiveBuffer.RemainingBytes != (u32)0) {
		Event = XUARTPS_EVENT_RECV_TOUT;
	} else {
		Event = XUARTPS_EVENT_RECV_DATA;
	}

	/*
	 * Call the application handler to indicate that there is a receive
	 * timeout or data event
	 */
	InstancePtr->Handler(InstancePtr->CallBackRef, Event,
				 InstancePtr->ReceiveBuffer.RequestedBytes -
				 InstancePtr->ReceiveBuffer.RemainingBytes);

}


void Uart0_InterruptHandler(void)
{
u32 IsrStatus;
u32 CsrRegister;
XUartPs *InstancePtr=(XUartPs *)&Uart0Ps;
uint8_t rsv_dat;
uint8_t ch;
signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
signed portBASE_TYPE xTaskWoken= pdFALSE;

///	Xil_AssertVoid(InstancePtr != NULL);
///	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	/*
	 * Read the interrupt ID register to determine which
	 * interrupt is active
	 */
	IsrStatus = XUartPs_ReadReg(InstancePtr->Config.BaseAddress,
				   XUARTPS_IMR_OFFSET);

	IsrStatus &= XUartPs_ReadReg(InstancePtr->Config.BaseAddress,
				   XUARTPS_ISR_OFFSET);

	/* Dispatch an appropriate handler. */
	if((IsrStatus & ((u32)XUARTPS_IXR_RXOVR |
			(u32)XUARTPS_IXR_RXFULL)) != (u32)0)
		{
///====================================================
		for(;;)
		{
		CsrRegister = XUartPs_ReadReg(InstancePtr->Config.BaseAddress,
					XUARTPS_SR_OFFSET);

		/*
		 * Loop until there is no more data in RX FIFO or the specified
		 * number of bytes has been received
		 */
////		while((ReceivedCount <= InstancePtr->ReceiveBuffer.RemainingBytes)&&
////			(((CsrRegister & XUARTPS_SR_RXEMPTY) == (u32)0))){
		if((CsrRegister & XUARTPS_SR_RXEMPTY) != (u32)0)
			break;
		rsv_dat=XUartPs_ReadReg(Uart0Ps.Config.BaseAddress,XUARTPS_FIFO_OFFSET)	;
		ringbuffer_putc(&rx_msp_buffer,rsv_dat);
		if (hdlc1_on_bytein(&g_hdlc_bt, rsv_dat) > 0)
		    {
		    xQueueSendFromISR(g_hdlc_bt.ev_rsv_frame, &rsv_dat, &xHigherPriorityTaskWoken);
		    }
		}
///====================================================
		/* Received data interrupt */
////		ReceiveDataHandler(InstancePtr);
		}

	if((IsrStatus & ((u32)XUARTPS_IXR_TXEMPTY | (u32)XUARTPS_IXR_TXFULL))
									 != (u32)0) {
		/* Transmit data interrupt */
		SendDataHandler(InstancePtr, IsrStatus);
	}

	/* XUARTPS_IXR_RBRK is applicable only for Zynq Ultrascale+ MP */
	if ((IsrStatus & ((u32)XUARTPS_IXR_OVER | (u32)XUARTPS_IXR_FRAMING |
			(u32)XUARTPS_IXR_PARITY | (u32)XUARTPS_IXR_RBRK)) != (u32)0) {
		/* Received Error Status interrupt */
		ReceiveErrorHandler(InstancePtr, IsrStatus);
	}

	if((IsrStatus & ((u32)XUARTPS_IXR_TOUT)) != (u32)0) {
		/* Received Timeout interrupt */
		ReceiveTimeoutHandler(InstancePtr);
	}

	if((IsrStatus & ((u32)XUARTPS_IXR_DMS)) != (u32)0) {
		/* Modem status interrupt */
////		ModemHandler(InstancePtr);
	}

	/* Clear the interrupt status. */
	XUartPs_WriteReg(InstancePtr->Config.BaseAddress, XUARTPS_ISR_OFFSET,
		IsrStatus);

}


///===========================================
