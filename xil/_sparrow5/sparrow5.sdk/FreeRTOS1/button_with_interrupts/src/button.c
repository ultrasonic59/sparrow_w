/* Standard includes. */
#include <stdio.h>
#include <limits.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/* Xilinx includes. */
#include "platform.h"
#include "xparameters.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xgpiops.h"

#include "button.h"

/* gic handler declared in main */
extern XScuGic xInterruptController;
/* gpio driver handler */
static XGpioPs xGpio;

/* semaphore handler */
static SemaphoreHandle_t xEventSemaphore = NULL;

/* button interrupt */
static void IntrHandler(void *CallBackRef, int Bank, u32 Status)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	xSemaphoreGiveFromISR( xEventSemaphore, &xHigherPriorityTaskWoken);
}

/* button task */
void button_task(void *pvParameters)
{
	printf("Created button task\n");

    for( ;; )
    {
        /* Block until the semaphore is 'given'. */
        xSemaphoreTake( xEventSemaphore, portMAX_DELAY );

  		printf("Button pressed\n");
    }
}

static void gpio_setupInterrupt(XScuGic *GicInstancePtr, XGpioPs *Gpio, u16 GpioIntrId)
{
	XScuGic_Connect(GicInstancePtr, GpioIntrId,
			(Xil_ExceptionHandler)XGpioPs_IntrHandler,
			(void *)Gpio);

	/* Set the interrupt type for the Button GPIO */
	XGpioPs_SetIntrTypePin(Gpio, PB_PIN, XGPIOPS_IRQ_TYPE_EDGE_RISING);
	/* Set the handler for gpio interrupts. */
	XGpioPs_SetCallbackHandler(Gpio, (void *)Gpio, IntrHandler);
	/* Enable the interrupts for the Button GPIO. */
	XGpioPs_IntrEnablePin(Gpio, PB_PIN);
	/* Enable the interrupt for the GPIO device. */
	XScuGic_Enable(GicInstancePtr, GpioIntrId);
}

int button_init(void)
{
	XGpioPs_Config *pxConfigPtr;

	/* Initialise the GPIO driver.
	 * the XGpio variable is the GPIO driver handler */
	pxConfigPtr = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_DEVICE_ID);
	XGpioPs_CfgInitialize(&xGpio, pxConfigPtr, pxConfigPtr->BaseAddr);

	/* Push button set direction and enable output */
	XGpioPs_SetDirectionPin(&xGpio, PB_PIN, 0);

	/* enable the gpio interrupts */
	gpio_setupInterrupt(&xInterruptController, &xGpio, XPS_GPIO_INT_ID);

    /* Create the semaphore used by the FreeRTOS tick hook function and the
    event semaphore task. */
    vSemaphoreCreateBinary( xEventSemaphore );

    return 0;
}
