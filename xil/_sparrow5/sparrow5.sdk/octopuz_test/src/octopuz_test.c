/*
    FreeRTOS V8.2.1 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?".  Have you defined configASSERT()?  *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    ***************************************************************************
     *                                                                       *
     *   Investing in training allows your team to be as productive as       *
     *   possible as early as possible, lowering your overall development    *
     *   cost, and enabling you to bring a more robust product to market     *
     *   earlier than would otherwise be possible.  Richard Barry is both    *
     *   the architect and key author of FreeRTOS, and so also the world's   *
     *   leading authority on what is the world's most popular real time     *
     *   kernel for deeply embedded MCU designs.  Obtaining your training    *
     *   from Richard ensures your team will gain directly from his in-depth *
     *   product knowledge and years of usage experience.  Contact Real Time *
     *   Engineers Ltd to enquire about the FreeRTOS Masterclass, presented  *
     *   by Richard Barry:  http://www.FreeRTOS.org/contact
     *                                                                       *
    ***************************************************************************

    ***************************************************************************
     *                                                                       *
     *    You are receiving this top quality software for free.  Please play *
     *    fair and reciprocate by reporting any suspected issues and         *
     *    participating in the community forum:                              *
     *    http://www.FreeRTOS.org/support                                    *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
/* Xilinx includes. */
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpiops.h"
///#include "axi_udef.h"
#include "drv_utrk.h"
////#include "umka2_config.h"
#include "octopuz_types.h"
#include "ringbuffer.h"
#include "my_misc.h"
///==================================
#include "xscugic.h"
///#include "xgpiops.h"
///===================================
extern int init_uart0(void);

#define TIMER_ID	1
#define DELAY_10_SECONDS	10000UL
#define DELAY_1_SECOND		1000UL
#define TIMER_CHECK_THRESHOLD	9
/*-----------------------------------------------------------*/
extern  circ_buffer_t cb_rezult;

uint8_t g_camera_on=0;
uint8_t contr_connected=0;
#if 0
u8 *tmp_rez_buff1;
u8 *tmp_rez_buff2;
u8 *tmp_rez_buff3;
#endif

static void prvTxTask( void *pvParameters );
///=====================================================
#define TST_PIN 0
#define LED_PIN 53
///================================================
XGpioPs  BTNInst;
XScuGic INTCInst;

#define INTC_DEVICE_ID 		XPAR_PS7_SCUGIC_0_DEVICE_ID
#define BTNS_DEVICE_ID		XPAR_AXI_GPIO_0_DEVICE_ID
////#define INTC_GPIO_INTERRUPT_ID XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR
#define INTC_DEF_DEVICE_ID 		XPAR_FABRIC_IRQ_CPU_INTR


XGpioPs Gpio;
int init_mio_gpio(void)
{
int Status;
XGpioPs_Config *GPIOConfigPtr;
//GPIO Initilization
GPIOConfigPtr = XGpioPs_LookupConfig(XPAR_PS7_GPIO_0_DEVICE_ID);
Status = XGpioPs_CfgInitialize(&Gpio, GPIOConfigPtr, GPIOConfigPtr ->BaseAddr);
if (Status != XST_SUCCESS) {
return XST_FAILURE;
}
XGpioPs_SetDirectionPin(&Gpio, TST_PIN, 1);
XGpioPs_SetOutputEnablePin(&Gpio, TST_PIN, 1);
return 	XST_SUCCESS;
}
void put_tst_pin(int on_off)
{
if(on_off&0x1)
	XGpioPs_WritePin(&Gpio, TST_PIN, 0x1);
else
	XGpioPs_WritePin(&Gpio, TST_PIN, 0x0);

}

long vdelay(long idelay)
{
long rez=1;
long ii;
for(ii=1;ii<idelay;ii++)
	rez*=rez*ii;
return rez;
}
volatile uint16_t *udef_base_addr;

extern void init_hdlc_bt(void);
extern void defectoscope_thread_entry(void *p);

/* The queue used by the Tx and Rx tasks, as described at the top of this
file. */
static TaskHandle_t xTxTask;
///static TaskHandle_t xRxTask;
///static QueueHandle_t xQueue = NULL;
///static TimerHandle_t xTimer = NULL;
char HWstring[15] = "Hello World";
long RxtaskCntr = 0;
#if 0
udef_base_addr=(uint16_t*)XPAR_AXI_UDEF_0_S_AXI_BASEADDR;
////	uint32_t *udef_base_addr=(uint32_t*)XPAR_AXI_UDEF_0_S_AXI_BASEADDR;
	init_platform();
	init_mio_gpio() ;
#endif
///=============================================
static void def_intr_handler(void *baseaddr_p);

int InterruptSystemSetup(XScuGic *XScuGicInstancePtr)
{
	// Enable interrupt
///	XGpio_InterruptEnable(&BTNInst, BTN_INT);
////	XGpio_InterruptGlobalEnable(&BTNInst);

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler) XScuGic_InterruptHandler,
			XScuGicInstancePtr);
	Xil_ExceptionEnable();

	return XST_SUCCESS;

}
extern XScuGic xInterruptController;
#define InterruptController xInterruptController


////extern	int enaInterruptUart0(void);
///int IntcInitFunction(u16 DeviceId, XGpioPs *GpioInstancePtr)
int IntcInitFunction(u16 DeviceId, void *GpioInstancePtr)
{
XScuGic_Config *IntcConfig;
int status;

/// Interrupt controller initialisation
IntcConfig = XScuGic_LookupConfig(DeviceId);
////status = XScuGic_CfgInitialize(&INTCInst, IntcConfig,
		status = XScuGic_CfgInitialize(&InterruptController, IntcConfig,
		IntcConfig->CpuBaseAddress);
if (status != XST_SUCCESS)
	return XST_FAILURE;

/// Call to interrupt setup
///status = InterruptSystemSetup(&INTCInst);
status = InterruptSystemSetup(&InterruptController);
	if (status != XST_SUCCESS)
		return XST_FAILURE;

		// Connect GPIO interrupt to handler
////	status = XScuGic_Connect(&INTCInst, INTC_DEF_DEVICE_ID,
status = XScuGic_Connect(&InterruptController, INTC_DEF_DEVICE_ID,
////				(Xil_ExceptionHandler) def_intr_handler, (void *) GpioInstancePtr);
			(Xil_ExceptionHandler) def_intr_handler, 0);
		if (status != XST_SUCCESS)
			return XST_FAILURE;

		// Enable GPIO interrupts interrupt
///		XGpio_InterruptEnable(GpioInstancePtr, 1);
////		XGpio_InterruptGlobalEnable(GpioInstancePtr);
		// Enable GPIO and timer interrupts in the controller
///		XScuGic_Enable(&INTCInst, INTC_DEF_DEVICE_ID);
		XScuGic_Enable(&InterruptController, INTC_DEF_DEVICE_ID);

		return XST_SUCCESS;
	}
///==============================================
extern void clr_ug_irq(void);

static void def_intr_handler(void *InstancePtr)
{
static uint8_t t_tst=0;
	// Disable GPIO interrupts
////	XGpio_InterruptDisable(&BTNInst, BTN_INT);
	// Ignore additional button presses
////	if ((XGpio_InterruptGetStatus(&BTNInst) & BTN_INT) != BTN_INT) {
////		return;
////	}

////	btn_value = XGpio_DiscreteRead(&BTNInst, 1);
	// Increment counter based on button value
	// Reset if centre button pressed
///===========================
	put_tst_pin(t_tst)	;
	t_tst++;
	clr_ug_irq();
	trk_irq();
	///===========================
////	(void) XGpio_InterruptClear(&BTNInst, BTN_INT);
	// Enable GPIO interrupts
////	XGpio_InterruptEnable(&BTNInst, BTN_INT);
}

int ps7_init1()
{
  // Get the PS_VERSION on run time
///  unsigned long si_ver = ps7GetSiliconVersion ();
  int ret;
  //int pcw_ver = 0;
}

///==============================================
int main( void )
{
int status;
///========================
	udef_base_addr=(uint16_t*)ADDR_UG_PAR;
///========================
	xil_printf( "\r\nOctopuz test\r\n" );
	init_mio_gpio() ;
///	status = IntcInitFunction(INTC_DEVICE_ID, &BTNInst);
	status = IntcInitFunction(INTC_DEVICE_ID, 0);
	_printk("\r\nIntcInitFunction[%x]",status);
#if 0
///=======================================
_printk("\r\n==par_contr_t[%x] par_trk_t[%x] par_izm_t[%x] par_pow_t[%x] \n\r par_takt_t[%x] " \
		"par_gen_t[%x] par_us_t[%x] \n\rpar_strb_t[%x] par_vrch_t[%x]"
		,sizeof(par_contr_t),sizeof(par_trk_t),sizeof(par_izm_t)
		,sizeof(par_pow_t),sizeof(par_takt_t)
		,sizeof(par_gen_t),sizeof(par_us_t)
		,sizeof(par_strb_t),sizeof(par_vrch_t)
		);
///========================================================
#endif
///========================================

///=================================
	init_hdlc_bt();
	init_uart0();
#if 0
	if(cb_init(&cb_rezult, DEF_CB_REZ_LEN,MAX_LEN_REZ)==NULL)
	 	 {
		_printk("\r\nError cb_init [cb_rez]");
	 	 }
	else
	 	 {
		_printk("\r\ncb_init [cb_rez] ok");
	 	 }
	tmp_rez_buff1=(uint8_t *)pvPortMalloc(MAX_LEN_REZ*2);
	if(tmp_rez_buff1==NULL)
	 	 {
		_printk("\r\n+++Error init tmp_rez_buff1+++");
	 	 }
	else
	 	 {
		_printk("\r\ninit tmp_rez_buff1 ok");
	 	 }
	tmp_rez_buff2=(uint8_t *)pvPortMalloc(MAX_LEN_REZ*2);
	if(tmp_rez_buff2==NULL)
	 	 {
		_printk("\r\n+++Error init tmp_rez_buff2+++");
	 	 }
	else
	 	 {
		_printk("\r\ninit tmp_rez_buff2 ok");
	 	 }
	tmp_rez_buff3=(uint8_t *)pvPortMalloc(MAX_LEN_REZ*8);	///for summ dat from mod
	if(tmp_rez_buff3==NULL)
	 	 {
		_printk("\r\n+++Error init tmp_rez_buff3+++");
	 	 }
	else
	 	 {
		_printk("\r\ninit tmp_rez_buff3 ok");
	 	 }
#endif
////		init_uart_contr();
///		hdlc_contr_init();


///=================================
#if 1
	xTaskCreate( 	defectoscope_thread_entry, 					/* The function that implements the task. */
					( const char * ) "defectoscope", 		/* Text name for the task, provided to assist debugging only. */
					configMINIMAL_STACK_SIZE*8, 	/* The stack allocated to the task. */
					NULL, 						/* The task parameter is not used, so set to NULL. */
					tskIDLE_PRIORITY,			/* The task runs at the idle priority. */
					NULL );
#endif

	xTaskCreate( 	prvTxTask, 					/* The function that implements the task. */
					( const char * ) "Tx", 		/* Text name for the task, provided to assist debugging only. */
					configMINIMAL_STACK_SIZE, 	/* The stack allocated to the task. */
					NULL, 						/* The task parameter is not used, so set to NULL. */
					tskIDLE_PRIORITY,			/* The task runs at the idle priority. */
					&xTxTask );
#if 0
	xTaskCreate( prvRxTask,
				 ( const char * ) "GB",
				 configMINIMAL_STACK_SIZE,
				 NULL,
				 tskIDLE_PRIORITY + 1,
				 &xRxTask );

	/* Create the queue used by the tasks.  The Rx task has a higher priority
	than the Tx task, so will preempt the Tx task and remove values from the
	queue as soon as the Tx task writes to the queue - therefore the queue can
	never have more than one item in it. */
	xQueue = xQueueCreate( 	1,						/* There is only one space in the queue. */
							sizeof( HWstring ) );	/* Each space in the queue is large enough to hold a uint32_t. */

	/* Check the queue was created. */
	configASSERT( xQueue );

	/* Create a timer with a timer expiry of 10 seconds. The timer would expire
	 after 10 seconds and the timer call back would get called. In the timer call back
	 checks are done to ensure that the tasks have been running properly till then.
	 The tasks are deleted in the timer call back and a message is printed to convey that
	 the example has run successfully.
	 The timer expiry is set to 10 seconds and the timer set to not auto reload. */
	xTimer = xTimerCreate( (const char *) "Timer",
							x10seconds,
							pdFALSE,
							(void *) TIMER_ID,
							vTimerCallback);

	/* Check the timer was created. */
	configASSERT( xTimer );

	/* start the timer with a block time of 0 ticks. This means as soon
	   as the schedule starts the timer will start running and will expire after
	   10 seconds */
	xTimerStart( xTimer, 0 );
#endif
	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for( ;; );
}

extern int chk_key(void);
extern uint8_t get_key(void);

/*-----------------------------------------------------------*/
static void prvTxTask( void *pvParameters )
{
///const TickType_t x1second = pdMS_TO_TICKS( DELAY_1_SECOND );
	int Status;
uint8_t t_char=0x3;

uint16_t xx=0x3;
uint32_t yy=0;
xil_printf( "\r\nprvTxTask\r\n" );
///=================================
#if 0
Status=	init_uart0();
xil_printf( "\r\ninit_uart0  " );
if (Status != XST_SUCCESS)
	{
	xil_printf( " BAD\r\n" );;
	}
else
	{
	xil_printf( " OK\r\n" );;
	}
#endif
///=================================================
////enaInterruptUart0();
///===================================================
	for( ;; )
	{

////		put_tst_pin(xx)	;
///		xx=*udef_base_addr;
#if 0

		*(udef_base_addr+0x0)=xx;
		xx++;
///===============================
		yy=*(udef_base_addr);
		xil_printf("\n\r 0[%x]",yy);
///==================================
////		*((uint32_t*)((uint32_t)udef_base_addr+0x1))=xx;
		*(udef_base_addr+0x1)=xx;
///===============================
		yy=*(udef_base_addr+1);
		xil_printf("\n\r 1[%x]",yy);
///===============================

#endif
		xx++;
#if 0
		*(udef_base_addr+0x2)=xx;
		xx++;
		///===============================
		yy=*(udef_base_addr+2);
		xil_printf("\n\r 2[%x]",yy);
		///===============================
		*(udef_base_addr+0x3)=xx;
		xx++;
		///===============================
		yy=*(udef_base_addr+3);
		xil_printf("\n\r 3[%x]",yy);
		///===============================
////		*(udef_base_addr+0x14)=xx;
///		xx++;
#endif
#if 0
		yy=*(udef_base_addr);
		xil_printf("\n\r 0[%x]",yy);
///		print((const char8 *)t_str);
		yy=*(udef_base_addr+1);
		xil_printf("\n\r 1[%x]",yy);
////		print((const char8 *)t_str);
		yy=*(udef_base_addr+2);
		xil_printf("\n\r 2[%x]",yy);
////		print((const char8 *)t_str);
		yy=*(udef_base_addr+3);
		xil_printf("\n\r 3[%x]",yy);
////		print((const char8 *)t_str);
#endif
		if(chk_key())
			{
			t_char=get_key();
			xil_printf("[%c]",t_char);

			}
		else
			{
			vTaskDelay( 20 );
			}
////        psk_trk();

////		vTaskDelay( 20 );
////		vdelay(1000);
#if 0
		/* Delay for 1 second. */
		vTaskDelay( x1second );

		/* Send the next value on the queue.  The queue should always be
		empty at this point so a block time of 0 is used. */
		xQueueSend( xQueue,			/* The queue being written to. */
					HWstring, /* The address of the data being sent. */
					0UL );			/* The block time. */
#endif
	}
}
#if 0
/*-----------------------------------------------------------*/
static void prvRxTask( void *pvParameters )
{
char Recdstring[15] = "";

	for( ;; )
	{
		/* Block to wait for data arriving on the queue. */
		xQueueReceive( 	xQueue,				/* The queue being read. */
						Recdstring,	/* Data is read into this address. */
						portMAX_DELAY );	/* Wait without a timeout for data. */

		/* Print the received data. */
		xil_printf( "Rx task received string from Tx task: %s\r\n", Recdstring );
		RxtaskCntr++;
	}
}

/*-----------------------------------------------------------*/
static void vTimerCallback( TimerHandle_t pxTimer )
{
	long lTimerId;
	configASSERT( pxTimer );

	lTimerId = ( long ) pvTimerGetTimerID( pxTimer );

	if (lTimerId != TIMER_ID) {
		xil_printf("FreeRTOS Hello World Example FAILED");
	}

	/* If the RxtaskCntr is updated every time the Rx task is called. The
	 Rx task is called every time the Tx task sends a message. The Tx task
	 sends a message every 1 second.
	 The timer expires after 10 seconds. We expect the RxtaskCntr to at least
	 have a value of 9 (TIMER_CHECK_THRESHOLD) when the timer expires. */
	if (RxtaskCntr >= TIMER_CHECK_THRESHOLD) {
		xil_printf("FreeRTOS Hello World Example PASSED");
	} else {
		xil_printf("FreeRTOS Hello World Example FAILED");
	}

	vTaskDelete( xRxTask );
	vTaskDelete( xTxTask );
}
#endif
