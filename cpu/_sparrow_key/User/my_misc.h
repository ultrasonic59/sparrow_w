#ifndef _MY_MISC_H_
#define _MY_MISC_H_
///==================================================
#include <stdint.h>
#include "../brd/brook_kbd_brd.h"

#define DELT_BLINK     4     ////200
#define MAX_CNT_PUSH    20
#define MAX_CNT_DREB    2 ///5       ///10      ///20

#define MAX_APP_PAYLOAD 16      ////???

#define ADC_IN_VAL     2500    ////mv


#define NUM_STP_LEDS    5
#define MIN_BAT_VAL     3000            ////2700    ////2750    ///mv
#define MAX_BAT_VAL     4200///3700    ///4200    ///mv
#define NUM_BAT_STP     4    ///
#define BAT_STP         ((MAX_BAT_VAL-MIN_BAT_VAL)/NUM_BAT_STP)    ///


#define BAT_VAL0     3400            ////
#define BAT_VAL1     3550            ////
#define BAT_VAL2     3700            ////
#define BAT_VAL3     3870            ////

///========================================
extern volatile uint16_t cur_adc_dat;
///extern uint8_t pushed_key;
extern void delay_ms(uint32_t ms);
extern uint8_t state;

///extern uint8_t pushed_key;
extern volatile unsigned int adc_results[];

extern uint32_t get_curr_ms(void);
extern void obr_butt(void);
///extern void onoff_rst(uint8_t idat);
//extern void on_pow(uint8_t idat);
//extern void on_pow_stm8(uint8_t idat);
extern void put_leds(uint8_t idat);
extern uint8_t get_key(void);
extern void _put_adc_bat_leds(uint16_t  adc_dat);
extern void put_adc_bat_leds();

extern void set_led0(uint8_t idat);
extern void set_led1(uint8_t idat);
extern void set_led2(uint8_t idat);
extern void set_led3(uint8_t idat);
extern void set_led4(uint8_t idat);
extern void set_nled(uint8_t idat);
//extern void set_b_a(uint8_t idat);

extern void init_hw(void);
extern uint8_t rx_rdy(void);
extern uint8_t get_dat(void);
extern void TIM1_init(void);
extern void TIM2_init(void);
extern volatile uint16_t cur_adc_dat;
extern void put_asd_leds(void);
extern uint8_t asd_bat;
extern uint8_t curr_asd;
extern void vdelay(uint32_t del);
///#define onoff_gled set_led0
///extern uint16_t vibro_time;

////extern void  set_vibro(uint16_t itime);

extern int c_error;
extern uint8_t str_to_byte(char *tstr);
extern uint16_t str_to_half(char *tstr);
extern uint32_t str_to_word(char *tstr);
extern void hex_byte_to_str(uint8_t idat, char *ostr);
extern void hex_half_to_str(uint16_t idat, char *ostr);
extern void hex_word_to_str(uint32_t idat, char *ostr);
///extern void onoff_pow_gen(uint8_t idat);
extern uint8_t char_to_hex(char idat);


////=================================================
#endif
