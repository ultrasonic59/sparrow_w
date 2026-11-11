/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
///========================================
#include <stdio.h>
#include "xparameters.h"
#include "xil_cache.h"
#include "xscugic.h"
#include "xil_exception.h"
////#include "scugic_header.h"
#include "xdevcfg.h"
///#include "devcfg_header.h"
#include "xdmaps.h"
///#include "dmaps_header.h"
#include "xqspips.h"
///#include "qspips_header.h"
///#include "xtmrctr.h"
///#include "tmrctr_header.h"
///#include "tmrctr_intr_header.h"
#include "xscutimer.h"
///#include "scutimer_header.h"
#include "xscuwdt.h"
///#include "scuwdt_header.h"
#include "xspips.h"
///#include "spips_header.h"
#include "xuartps.h"
///#include "uartps_header.h"
///#include "uartps_intr_header.h"
///#include "xgpio.h"
#include "xgpiops.h"
#include "axi_udef.h"
///===============================================
// Parameter definitions
///#define INTC_DEVICE_ID 		XPAR_PS7_SCUGIC_0_DEVICE_ID
///#define BTNS_DEVICE_ID		XPAR_AXI_GPIO_0_DEVICE_ID
///#define INTC_GPIO_INTERRUPT_ID XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR

///#define BTN_INT 			XGPIO_IR_CH1_MASK

////XGpio  BTNInst;
XScuGic INTCInst;
//////static int btn_value;
//----------------------------------------------------
// PROTOTYPE FUNCTIONS
//----------------------------------------------------
XStatus AXI_UDEF_Reg_SelfTest(void * baseaddr_p);

///static void BTN_Intr_Handler(void *baseaddr_p);
///static int InterruptSystemSetup(XScuGic *XScuGicInstancePtr);
///static int IntcInitFunction(u16 DeviceId, XGpio *GpioInstancePtr);
///===============================
#define TST_PIN 0
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

///================================
//----------------------------------------------------
// INTERRUPT HANDLER FUNCTIONS
// - called by the timer, button interrupt, performs
// - LED flashing
//----------------------------------------------------
#if 0
void BTN_Intr_Handler(void *InstancePtr) {
	// Disable GPIO interrupts
	XGpio_InterruptDisable(&BTNInst, BTN_INT);
	// Ignore additional button presses
	if ((XGpio_InterruptGetStatus(&BTNInst) & BTN_INT) != BTN_INT) {
		return;
	}

	btn_value = XGpio_DiscreteRead(&BTNInst, 1);
	// Increment counter based on button value
	// Reset if centre button pressed
///===========================
	put_tst_pin(btn_value)	;
	///===========================
#if 0
	if (btn_value != 1)
		led_data = led_data + btn_value;
	else
		led_data = 0;

	XGpio_DiscreteWrite(&LEDInst, 1, led_data);
#endif
	(void) XGpio_InterruptClear(&BTNInst, BTN_INT);
	// Enable GPIO interrupts
	XGpio_InterruptEnable(&BTNInst, BTN_INT);
}

//----------------------------------------------------
// INITIAL SETUP FUNCTIONS
//----------------------------------------------------

int InterruptSystemSetup(XScuGic *XScuGicInstancePtr) {
	// Enable interrupt
	XGpio_InterruptEnable(&BTNInst, BTN_INT);
	XGpio_InterruptGlobalEnable(&BTNInst);

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler) XScuGic_InterruptHandler,
			XScuGicInstancePtr);
	Xil_ExceptionEnable();

	return XST_SUCCESS;

}

int IntcInitFunction(u16 DeviceId, XGpio *GpioInstancePtr) {
	XScuGic_Config *IntcConfig;
	int status;

	// Interrupt controller initialisation
	IntcConfig = XScuGic_LookupConfig(DeviceId);
	status = XScuGic_CfgInitialize(&INTCInst, IntcConfig,
			IntcConfig->CpuBaseAddress);
	if (status != XST_SUCCESS)
		return XST_FAILURE;

	// Call to interrupt setup
	status = InterruptSystemSetup(&INTCInst);
	if (status != XST_SUCCESS)
		return XST_FAILURE;

	// Connect GPIO interrupt to handler
	status = XScuGic_Connect(&INTCInst, INTC_GPIO_INTERRUPT_ID,
			(Xil_ExceptionHandler) BTN_Intr_Handler, (void *) GpioInstancePtr);
	if (status != XST_SUCCESS)
		return XST_FAILURE;

	// Enable GPIO interrupts interrupt
	XGpio_InterruptEnable(GpioInstancePtr, 1);
	XGpio_InterruptGlobalEnable(GpioInstancePtr);

	// Enable GPIO and timer interrupts in the controller
	XScuGic_Enable(&INTCInst, INTC_GPIO_INTERRUPT_ID);

	return XST_SUCCESS;
}
#endif
volatile uint16_t *udef_base_addr;
int main() {
///	int status;
	char t_str[256];
////	int xx=1;
uint16_t xx=0;
uint32_t yy=0;
udef_base_addr=(uint16_t*)XPAR_AXI_UDEF_0_S_AXI_BASEADDR;
////	uint32_t *udef_base_addr=(uint32_t*)XPAR_AXI_UDEF_0_S_AXI_BASEADDR;
	init_platform();
	init_mio_gpio() ;
	print("Hello World\n\r");
///	status= AXI_UDEF_Reg_SelfTest((void *) XPAR_AXI_UDEF_0_S_AXI_BASEADDR);
////	status= AXI_UDEF_Reg_SelfTest((void *) udef_base_addr);

#if 1
	for(;;)
	{
		put_tst_pin(xx)	;
///		xx=*udef_base_addr;

		*(udef_base_addr+0x0)=xx;
		xx++;
////		*((uint32_t*)((uint32_t)udef_base_addr+0x1))=xx;
		*(udef_base_addr+0x1)=xx;
		xx++;
#if 1
		*(udef_base_addr+0x2)=xx;
		xx++;
		*(udef_base_addr+0x3)=xx;
		xx++;
////		*(udef_base_addr+0x14)=xx;
///		xx++;
#endif
#if 1
		yy=*(udef_base_addr);
		sprintf(t_str,"\n\r 0[%x]",yy);
		print((const char8 *)t_str);
		yy=*(udef_base_addr+1);
		sprintf(t_str,"\n\r 1[%x]",yy);
		print((const char8 *)t_str);
		yy=*(udef_base_addr+2);
		sprintf(t_str,"\n\r 2[%x]",yy);
		print((const char8 *)t_str);
		yy=*(udef_base_addr+3);
		sprintf(t_str,"\n\r 3[%x]",yy);
		print((const char8 *)t_str);
#endif
		vdelay(1000);
	}
#endif
///======================================
	//----------------------------------------------------
	// INITIALIZE THE PERIPHERALS & SET DIRECTIONS OF GPIO
	//----------------------------------------------------
	// Initialise LEDs
#if 0
	status = XGpio_Initialize(&LEDInst, LEDS_DEVICE_ID);
	if (status != XST_SUCCESS)
		return XST_FAILURE;
#endif
	// Initialise Push Buttons
////	status = XGpio_Initialize(&BTNInst, BTNS_DEVICE_ID);
////	if (status != XST_SUCCESS)
///		return XST_FAILURE;

	// Set LEDs direction to outputs
////	XGpio_SetDataDirection(&LEDInst, 1, 0x00);

	// Set all buttons direction to inputs
////	XGpio_SetDataDirection(&BTNInst, 1, 0xFF);
///=========================================
#if 1
	for(;;)
		{
////		xx = XGpio_DiscreteRead(&BTNInst, 1);
		// Increment counter based on button value
		// Reset if centre button pressed
		put_tst_pin(xx)	;
		}
#endif
	///======================================
#if 0
	// Initialize interrupt controller
	status = IntcInitFunction(INTC_DEVICE_ID, &BTNInst);
	if (status != XST_SUCCESS)
		return XST_FAILURE;
#endif
	while (1)
		;
	return 0;
}


///==============================================
#if 0
int main()
{
    init_platform();

    print("Hello World\n\r");

    cleanup_platform();
    return 0;
}
#endif
