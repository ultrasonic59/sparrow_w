#ifndef _MY_MISC_H_
#define _MY_MISC_H_
///============================================
#include <stdint.h>
#include <stdio.h>

extern void uDelay (const uint32_t usec);

#define _printk xil_printf
#define msleep vTaskDelay

#define USB_OFF 0
#define USB_ON 1

extern void put_tst1(int on_off); ///27 pin
extern void put_tst2(int on_off); ///27 pin
extern void put_tst3(int on_off); ///27 pin
extern void put_tst4(int on_off); ///27 pin
extern void put_tst5(int on_off); ///27 pin
extern uint8_t conv_bat_led(uint16_t i_adc);
///extern void msleep(uint32_t ms);
extern int kbhit(void);
extern void on_off_usb_thr(uint8_t on_off);
///===============================================
#endif
