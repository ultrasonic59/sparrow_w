/**
 *	led.c
 *
 */
#include "xgpiops.h"

#include "led.h"

#define partstNUM_LEDS			( 1 )
#define partstDIRECTION_OUTPUT	( 1 )
#define partstOUTPUT_ENABLED	( 1 )
#define partstLED_OUTPUT		( 47 )

static XGpioPs xGpio;

void led_init( void )
{
	XGpioPs_Config *pxConfigPtr;
	int xStatus;

	/* Initialise the GPIO driver. */
	pxConfigPtr = XGpioPs_LookupConfig( XPAR_XGPIOPS_0_DEVICE_ID );
	xStatus = XGpioPs_CfgInitialize( &xGpio, pxConfigPtr, pxConfigPtr->BaseAddr );

	/* Enable outputs and set low. */
	XGpioPs_SetDirectionPin( &xGpio, partstLED_OUTPUT, partstDIRECTION_OUTPUT );
	XGpioPs_SetOutputEnablePin( &xGpio, partstLED_OUTPUT, partstOUTPUT_ENABLED );
	XGpioPs_WritePin( &xGpio, partstLED_OUTPUT, 0x0 );
}

void led_set(int xValue)
{
	XGpioPs_WritePin( &xGpio, partstLED_OUTPUT, xValue );
}

void led_toggle(void)
{
	int xLEDState;

	xLEDState = XGpioPs_ReadPin( &xGpio, partstLED_OUTPUT );
	XGpioPs_WritePin( &xGpio, partstLED_OUTPUT, !xLEDState );
}



