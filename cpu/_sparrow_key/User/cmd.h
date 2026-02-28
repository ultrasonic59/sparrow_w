#ifndef _CMD_H_
#define _CMD_H_
////==============================
///#define CMD_SET_LED .6=10x31
///#define CMD_SET_INP 0x32
///#define CMD_RD_ADC  0x33
///.6 =1 set led  .4-.0 leds
///.6=0, .5=1 set input
///.6=0,.5=0,4.=1 read adc
#define BIT_CMD_SET_ALL (0x1<<7) /// .5 inp,.4-.0 led
#define BIT_CMD_SET_ASD (0x1<<6)
#define BIT_CMD_SET_INP (0x1<<5)
////#define BIT_CMD_RD_ADC  (0x1<<4)
#define LED4_BIT  (0x1<<4)

#define MASK_ASD        0x0f
#define MASK_INP        0x1
///==============================
extern uint8_t  obr_cmd(uint8_t cmd);
////============================
#endif ////_CMD_H_
