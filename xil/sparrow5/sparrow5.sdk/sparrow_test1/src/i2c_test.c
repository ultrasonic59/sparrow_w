//mcp4725 output example
#if 0
#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//include uart
#define UART_BAUD_RATE 2400
#include "uart/uart.h"

//include adc
#include "adc/adc.h"

//set the test to run
#define TESTMODE_SINE 1
#define TESTMODE_VOLT 2
#define TESTMODE TESTMODE_SINE

#if TESTMODE == TESTMODE_SINE
unsigned char sine256[]  = {
 127,130,133,136,139,143,146,149,152,155,158,161,164,167,170,173,176,178,181,184,187,190,192,195,198,200,203,205,208,210,212,215,217,219,221,223,225,227,229,231,233,234,236,238,239,240,
 242,243,244,245,247,248,249,249,250,251,252,252,253,253,253,254,254,254,254,254,254,254,253,253,253,252,252,251,250,249,249,248,247,245,244,243,242,240,239,238,236,234,233,231,229,227,
 225,223, 221,219,217,215,212,210,208,205,203,200,198,195,192,190,187,184,181,178,176,173,170,167,164,161,158,155,152,149,146,143,139,136,133,130,127,124,121,118,115,111,108,105,102,
 99,96,93,90,87,84,81,78,76,73,70,67,64,62,59,56,54,51,49,46,44,42,39,37,35,33,31,29,27,25,23,21,20,18,16,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,
 10,11,12,14,15,16,18,20,21,23,25,27,29,31,33,35,37,39,42,44,46,49,51,54,56,59,62,64,67,70,73,76,78,81,84,87,90,93,96,99,102,105,108,111,115,118,121,124
};
#endif

#include "mcp4725/mcp4725.h"

int i2c_main(void) {
    //init uart
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));

	//set the chip address
	uint8_t address = 0x00;

	//init adc
	adc_init();

	sei();

	//init mcp4725
	mcp4725_init(address, 0);

#if TESTMODE == TESTMODE_SINE
	//get adc ref voltage
	double adcvref = acd_getrealvref();
	//get dac ref voltage
	double dacvref = adc_getvoltage(adc_read(0), adcvref);

	for(;;) {
		//map sine to max voltage
		for(uint8_t i=0; i<256; i++) {
			uint8_t sinval = sine256[i];
			double sinvolt = (sinval - 0) * (dacvref - 0) / (255 - 0) + 0;
			mcp4725_setvoltagefast(address, sinvolt, dacvref);
		}
	}
#endif

#if TESTMODE == TESTMODE_VOLT
	char printbuff[100];

    for(;;) {
    	//get adc ref voltage
    	double adcvref = acd_getrealvref();
    	//get dac ref voltage
    	double dacvref = adc_getvoltage(adc_read(0), adcvref);

    	//get dac ref voltage
		dtostrf(dacvref, 3, 3, printbuff);
		uart_puts("DAC VREF "); uart_puts(printbuff); uart_puts("\r\n");

		double volt = 0.8;
		//set voltage output
		mcp4725_setvoltage(address, volt, dacvref, 1);

    	_delay_ms(50);

    	//get dac voltage
    	double dacread = adc_getvoltage(adc_read(1), adcvref);
		dtostrf(volt, 3, 3, printbuff);
		uart_puts("DAC WANT "); uart_puts(printbuff); uart_puts("\r\n");
		dtostrf(dacread, 3, 3, printbuff);
		uart_puts("DAC READ "); uart_puts(printbuff); uart_puts("\r\n");

		_delay_ms(100);

		uart_puts("\r\n");

		_delay_ms(1000);
    }
#endif
}
#endif
