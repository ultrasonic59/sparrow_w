
#include "my_misc.h"

const uint8_t m_bat_leds[NUM_STP_LEDS]={0x0,0x1,0x3,0x7,0xf};
uint8_t curr_asd=0;
void vdelay(uint32_t del)
{
///uint32_t cycles = 960 * ms ;
uint32_t cycles = 3 * del ;
do
  {
  asm("nop");
  cycles--;
  }
while(cycles > 0);
}
void delay_ms(uint32_t ms)
{
///uint32_t cycles = 960 * ms ;
uint32_t cycles = 540 * ms ;
do
  {
  asm("nop");
  cycles--;
  }
while(cycles > 0);
}
uint16_t vibro_time=0;
////===============================================
void put_leds(uint8_t idat)
{
set_led0(idat);
idat >>= 1;
set_led1(idat);
idat >>= 1;
set_led2(idat);
idat >>= 1;
set_led3(idat);
}
void set_nled(uint8_t idat)
{
if(idat&(0x1<<0))
  set_led0(0x1);
else
  set_led0(0x0);
if(idat&(0x1<<1))
  set_led1(0x1);
else
  set_led1(0x0);
if(idat&(0x1<<2))
  set_led2(0x1);
else
  set_led2(0x0);
if(idat&(0x1<<3))
  set_led3(0x1);
else
  set_led3(0x0);
if(idat&(0x1<<4))
  set_led4(0x1);
else
  set_led4(0x0);
}

void _set_nled(uint8_t idat)
{
if(idat&(0x1<<0))
  set_led0(0x1);
else
  set_led0(0x0);
if(idat&(0x1<<1))
  set_led1(0x1);
else
  set_led1(0x0);
if(idat&(0x1<<2))
  set_led2(0x1);
else
  set_led2(0x0);
if(idat&(0x1<<3))
  set_led3(0x1);
else
  set_led3(0x0);
}

void put_bat_leds(uint8_t idat)
{
if(idat>NUM_STP_LEDS-1)
  idat=NUM_STP_LEDS-1;
put_leds(m_bat_leds[idat]);
}
/*
uint8_t conv_adc_dat_led(uint16_t  adc_dat)
{
uint8_t rez=0;
long ltmp;
ltmp=adc_dat;
ltmp *= 3;
ltmp >>= 1;
ltmp-=MIN_BAT_VAL;
if(ltmp<0)
  ltmp=0;
ltmp/=BAT_STP;
rez=(uint8_t)ltmp;
return rez;
}
*/
uint8_t conv_adc_dat_led(uint16_t  adc_dat)
{
uint8_t rez=0;
if(adc_dat>BAT_VAL3)
    rez=4;
else if(adc_dat>BAT_VAL2)
    rez=3;
else if (adc_dat>BAT_VAL1)
    rez=2;
else if(adc_dat>BAT_VAL0)
    rez=1;
return rez;
}

void _put_adc_bat_leds(uint16_t  adc_dat)
{
uint8_t bat;
bat=conv_adc_dat_led(adc_dat);
put_bat_leds(bat);
}
////===============================================
void put_adc_bat_leds(void)
{
uint8_t bat;
bat=conv_adc_dat_led(cur_adc_dat);
put_bat_leds(bat);
}
void put_asd_leds(void)
{
_set_nled(curr_asd&0xf);
}
///=========================================
int c_error=0;
uint8_t char_to_hex(char idat)
{
uint8_t rez=0;
if((idat>='0')&&(idat<='9'))
  rez=idat-'0';
else if((idat>='A')&&(idat<='F'))
  rez=idat-'A'+0xa ;
else if((idat>='a')&&(idat<='f'))
  rez=idat-'a'+0xa ;
else
  c_error=-1;
return rez;
}
uint8_t vallid_char(char idat)
{
if((idat>='0')&&(idat<='9'))
  return 1;
else if((idat>='A')&&(idat<='F'))
  return 1;
else if((idat>='a')&&(idat<='f'))
  return 1;
else
  return 0;
}
uint8_t str_to_byte(char *tstr)
{
uint8_t rez=0;
///c_error=0;
rez= char_to_hex(*tstr);
rez<<=4;
rez|= char_to_hex(*tstr+2);
return rez;
}
uint16_t str_to_half(char *tstr)
{
uint16_t rez=0;
rez= str_to_byte(tstr);
rez<<=8;
rez|= str_to_byte(tstr+2);
return rez;
}
uint32_t str_to_word(char *tstr)
{
uint32_t rez=0;
rez= str_to_half(tstr);
rez<<=16;
rez|= str_to_half(tstr+4);
return rez;
}
char nibl_to_char(uint8_t idat)
{
char rez;
if( (idat&0xf)>9 )
  rez=((idat&0xf)-0xa +'A');
else
  rez = (idat&0xf)+'0';
return rez;
}
///=================================
void hex_byte_to_str(uint8_t idat, char *ostr)
{
if(ostr==0)
    return;
*(ostr+1) =nibl_to_char(idat&0xf);
idat>>=4;
*(ostr+0)=nibl_to_char(idat&0xf);
}
#define NUM_NIBBL_HALF 4
void hex_half_to_str(uint16_t idat, char *ostr)
{
uint8_t ii;
char ctmp;
if(ostr==0)
    return;
for(ii=0;ii<NUM_NIBBL_HALF;ii++)
  {
   ctmp =nibl_to_char(idat&0xf);
 ///// *(ostr+NUM_NIBBL_HALF-1-ii) =nibl_to_char(idat&0xf);
   *(ostr+NUM_NIBBL_HALF-1-ii) =ctmp;
 idat>>=4;
  }
}
#define NUM_NIBBL_WORD 8
void hex_word_to_str(uint32_t idat, char *ostr)
{
uint8_t ii;
if(ostr==NULL)
    return;
for(ii=0;ii<NUM_NIBBL_WORD;ii++)
  {
  *(ostr+NUM_NIBBL_HALF-1-ii) =nibl_to_char(idat&0xf);
  idat>>=4;
  }
}
