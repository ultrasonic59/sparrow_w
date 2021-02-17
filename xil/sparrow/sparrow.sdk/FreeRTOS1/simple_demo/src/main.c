/* Standard includes. */
#include <stdio.h>
#include <limits.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "led.h"

/* Xilinx includes. */
#include "platform.h"
#include "xparameters.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "xil_exception.h"

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

extern void main_application( void );

/*
 * The Xilinx projects use a BSP that do not allow the start up code to be
 * altered easily.  Therefore the vector table used by FreeRTOS is defined in
 * FreeRTOS_asm_vectors.S, which is part of this project.  Switch to use the
 * FreeRTOS vector table.
 */
extern void vPortInstallFreeRTOSVectorTable( void );

/* Prototypes for the standard FreeRTOS callback/hook functions implemented
within this file. */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );

/* The private watchdog is used as the timer that generates run time
stats.  This frequency means it will overflow quite quickly. */
XScuWdt xWatchDogInstance;

/*-----------------------------------------------------------*/

/* The interrupt controller is initialised in this file, and made available to
other modules. */
XScuGic xInterruptController;

/*-----------------------------------------------------------*/

int main( void )
{
	/* Configure the MicroZedBoard hardware*/
	prvSetupHardware();

	printf("Hardware configured successfully\r\n");

	main_application();

	/* Don't expect to reach here. */
	return 0;
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	BaseType_t xStatus;
	XScuGic_Config *pxGICConfig;

	/* Ensure no interrupts execute while the scheduler is in an inconsistent
	state.  Interrupts are automatically enabled when the scheduler is
	started. */
	portDISABLE_INTERRUPTS();

	/* Obtain the configuration of the GIC. */
	pxGICConfig = XScuGic_LookupConfig( XPAR_SCUGIC_SINGLE_DEVICE_ID );

	/* Sanity check the FreeRTOSConfig.h settings are correct for the
	hardware. */
	configASSERT( pxGICConfig );
	configASSERT( pxGICConfig->CpuBaseAddress == ( configINTERRUPT_CONTROLLER_BASE_ADDRESS + configINTERRUPT_CONTROLLER_CPU_INTERFACE_OFFSET ) );
	configASSERT( pxGICConfig->DistBaseAddress == configINTERRUPT_CONTROLLER_BASE_ADDRESS );

	/* Install a default handler for each GIC interrupt. */
	xStatus = XScuGic_CfgInitialize( &xInterruptController, pxGICConfig, pxGICConfig->CpuBaseAddress );
	configASSERT( xStatus == XST_SUCCESS );
	( void ) xStatus; /* Remove compiler warning if configASSERT() is not defined. */

	/* Initialise the LED port. */
	led_init();

	/* The Xilinx projects use a BSP that do not allow the start up code to be
	altered easily.  Therefore the vector table used by FreeRTOS is defined in
	FreeRTOS_asm_vectors.S, which is part of this project.  Switch to use the
	FreeRTOS vector table. */
	vPortInstallFreeRTOSVectorTable();
}

void vAssertCalled( const char * pcFile, unsigned long ulLine )
{
	volatile unsigned long ul = 0;

	( void ) pcFile;
	( void ) ulLine;

	taskENTER_CRITICAL();
	{
		/* Set ul to a non-zero value using the debugger to step out of this
		function. */
		while( ul == 0 )
		{
			portNOP();
		}
	}
	taskEXIT_CRITICAL();
}

void vApplicationTickHook( void )
{

}

void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected.
	pxCurrentTCB can be inspected in the debugger if the task
	name passed into this function is corrupt
     */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void vApplicationIdleHook( void )
{
	volatile size_t xFreeStackSpace;

	/* This is just a trivial example of an idle hook.  It is called on each
	cycle of the idle task.  It must *NOT* attempt to block.  In this case the
	idle task just queries the amount of FreeRTOS heap that remains.  See the
	memory management section on the http://www.FreeRTOS.org web site for memory
	management options.  If there is a lot of heap memory free then the
	configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
	RAM. */
	xFreeStackSpace  = xPortGetFreeHeapSize();

    if( xFreeStackSpace > 100 )
    {
        /* By now, the kernel has allocated everything it is going to, so
        if there is a lot of heap remaining unallocated then
        the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
        reduced accordingly. */
    }
}


void vInitialiseTimerForRunTimeStats( void )
{
	XScuWdt_Config *pxWatchDogInstance;
	uint32_t ulValue;
	const uint32_t ulMaxDivisor = 0xff, ulDivisorShift = 0x08;

	pxWatchDogInstance = XScuWdt_LookupConfig( XPAR_SCUWDT_0_DEVICE_ID );
	XScuWdt_CfgInitialize( &xWatchDogInstance, pxWatchDogInstance, pxWatchDogInstance->BaseAddr );

	ulValue = XScuWdt_GetControlReg( &xWatchDogInstance );
	ulValue |= ulMaxDivisor << ulDivisorShift;
	XScuWdt_SetControlReg( &xWatchDogInstance, ulValue );

	XScuWdt_LoadWdt( &xWatchDogInstance, UINT_MAX );
	XScuWdt_SetTimerMode( &xWatchDogInstance );
	XScuWdt_Start( &xWatchDogInstance );
}



