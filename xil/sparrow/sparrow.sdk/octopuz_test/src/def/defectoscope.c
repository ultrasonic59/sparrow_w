#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "octopuz.h"
///#include "rook_rtos.h"
#include "defectoscope.h"
///#include "my_respond.h"
////#include "my_platform.h"
#include "drv_utrk.h"
///#include "list_param.h"

extern uint8_t ready_dat;	

extern void get_amps_tims(void);

extern int test_send(void);
extern void tst_pin(void);
///extern void on_pow_sd(uint8_t on_off);


circ_buffer_t cb_rezult;
////len_rez_dat_t curr_len_rez_dat;
uint8_t tmp_rez_buff[8192];

uint8_t prev_rej=0;
uint8_t cur_rej=0;

uint8_t flg_req_osc=0;
//// uint8_t tst_osc_a[DEF_LEN_OSC];

///uint8_t osc_rez[DEF_LEN_OSC*NUM_SUMM_OSC];
uint8_t osc_rez[DEF_LEN_OSC];
par_contr_t curr_par_contr;
////par_trk_t curr_par_trk;
#if 0
void set_osc(uint8_t *i_osc,uint16_t *o_osc)
{
uint16_t ii;
for(ii=0;ii<DEF_LEN_OSC;ii++)
{
*(o_osc+ii) = *(i_osc+ii);
}
}
void add_osc(uint8_t *i_osc,uint16_t *o_osc)
{
uint16_t ii;
for(ii=0;ii<DEF_LEN_OSC;ii++)
{
*(o_osc+ii) += *(i_osc+ii);
}
}
void div_osc(uint16_t *i_osc,uint8_t *o_osc)
{
uint16_t ii;
for(ii=0;ii<DEF_LEN_OSC;ii++)
{
*(o_osc+ii) = *(i_osc+ii)/NUM_SUMM_OSC;
}
}

void filtr_osc(uint8_t *c_osc,uint8_t *p_osc,uint8_t *o_osc)
{
uint16_t ii;
for(ii=0;ii<DEF_LEN_OSC;ii++)
{
*(o_osc+ii) = (*(c_osc+ii)+*(p_osc+ii))/2;
*(p_osc+ii) = *(c_osc+ii);
}
  
}
#endif
///=====================================================
#define NUM_CNT_DAT 8
int add_amp_tim_data(uint8_t* in_buff,uint8_t* out_buff,uint8_t flg_first)
{
int ii;
///u16 tmp;
///u16 tmp1;
///u32 tmpt;
///u32 tmpt1;
///u8 t_num_izm;
amp_dat_chan_t *p_amp_dat_o=(amp_dat_chan_t *)out_buff;
amp_dat_chan_t *p_amp_dat_i=(amp_dat_chan_t *)in_buff;
if(flg_first)
	{
	for(ii=0;ii<NUM_STRB_REZ;ii++)
		{
		p_amp_dat_o->amps[ii]=p_amp_dat_i->amps[ii];
		p_amp_dat_o->tims[ii]=p_amp_dat_i->tims[ii];
		}
	}
else
	{
	for(ii=0;ii<NUM_STRB_REZ;ii++)
		{
		if(p_amp_dat_o->amps[ii]<p_amp_dat_i->amps[ii])
			p_amp_dat_o->amps[ii]=p_amp_dat_i->amps[ii];
		if(p_amp_dat_o->tims[ii]>p_amp_dat_i->tims[ii])
			p_amp_dat_o->tims[ii]=p_amp_dat_i->tims[ii];
		}
	}
return 0;
}
#if 0
int add_razv_a_data(uint8_t* in_buff,uint8_t* out_buff,uint8_t flg_first)
{
int ii;
razv_a_dat_chan_t *p_razv_a_dat_i=(razv_a_dat_chan_t *)in_buff;
razv_a_dat_chan_t *p_razv_a_dat_o=(razv_a_dat_chan_t *)out_buff;
if(flg_first)
	{
	for(ii=0;ii<MAX_NUM_STP_R_A;ii++)
		{
		p_razv_a_dat_o->amps[ii]=p_razv_a_dat_i->amps[ii];
		}
	}
else
	{
	for(ii=0;ii<MAX_NUM_STP_R_A;ii++)
		{
		if(p_razv_a_dat_o->amps[ii] < p_razv_a_dat_i->amps[ii])
			{
			p_razv_a_dat_o->amps[ii]=p_razv_a_dat_i->amps[ii];
			}
		}
	}
return 0;
}

int _add_new_rez(uint8_t* in_buff,uint8_t* out_buff,uint8_t flg_first)
{
int ii;
int offs_i=0;
int offs_o=0;
for(ii=0;ii<CUR_NUM_CHAN;ii++)
	{
	if(CUR_ON_US_CHAN&(0x1<<ii))
	{
		if(CUR_FL_REJ_CHAN&(0x1<<ii))
		{
			add_razv_a_data(in_buff+offs_i,out_buff+offs_o,flg_first);
			offs_i+=sizeof(razv_a_dat_chan_t);
			offs_o+=sizeof(razv_a_dat_chan_t);
		}
		else
		{
			add_amp_tim_data(in_buff+offs_i,out_buff+offs_o,flg_first);
			offs_i+=sizeof(amp_dat_chan_t);
			offs_o+=sizeof(amp_dat_chan_t);
		}
	}
	}
return offs_o;
}

void add_new_rez(uint8_t *buff)
{
static int cnt_dat=0;
if(cnt_dat==0)
	_add_new_rez(buff,tmp_rez_buff,1);
else
	_add_new_rez(buff,tmp_rez_buff,1);
cnt_dat++;
if(cnt_dat>=NUM_CNT_DAT)
	{
	cnt_dat=0;
	cb_push(&cb_rezult,tmp_rez_buff);
	}
}

#endif

///==========================================
uint8_t get_osc(void)
{
uint8_t rez;
///static uint8_t cnt_osc=0;
///int ii;
///int tmp;
///uint16_t htmp;
////uint16_t htmp1;
///uint8_t t_num_us;
///uint8_t t_ots;
////if(curr_tact==ed_par_trk.curr_tact)
///if(curr_par_trk.takts[ed_par_trk.curr_tact].uss[ed_par_trk.curr_us].on_us )
////rd_dat_blok_m(ADDR_OSC_AV,DEF_LEN_OSC/2,(uint16_t *)(osc_rez));
////return 1;
if(1)
{
switch(curr_osc_par.rej_osc)
	{
	case VREJ_RADIO:
          rd_dat_blok_trk(ADDR_OSC_A,DEF_LEN_OSC/2,(uint16_t *)osc_rez);
          rez=1;
          break;
	case VREJ_VIDEO:
         rd_dat_blok_trk(ADDR_OSC_AV,DEF_LEN_OSC/2,(uint16_t *)(osc_rez));
 ///               filtr_osc(t_osc_a,prev_osc_a,osc_a);
          rez=1;
          break;
	}
	}
else
	{
	memset(osc_rez,0,DEF_LEN_OSC);
	}
return rez;
}
///extern void xmit_spi(uint8_t dat);
///extern bt_rez_dat_strobs_t bt_strobs_dat;
///extern bt_rez_dat_strobs_t bt_strobs_dat_a;
////extern uint16_t put_dat_to_msp(uint8_t *ibuf,uint16_t len);


extern int utrk_init(void);
#if 0
void init_tst_osc_buff(void)
{
  int ii;
  for(ii=0;ii<DEF_LEN_OSC;ii++)
  {
   tst_osc_a[ii]=(ii&0xf)+'A'; 
  }
}
#endif
uint8_t on_flg_izl=0;
///====================================
void defectoscope_thread_entry(void *p)
{
uint16_t tcnt=0;
printf("\r\n***defectoscope_thread_entry***");
///t_par_buff = (uint8_t *)malloc(sizeof(par_trk_t)+20);
t_par_buff = (uint8_t *)pvPortMalloc(sizeof(par_trk_t)+20);
if(t_par_buff == NULL )
  printf("t_par_buff: NO MEM !!!\n");
else 
  tmp_param_buff=t_par_buff+sizeof(uint32_t);
utrk_init();
load_cur_par();	
///put_all_par_trk();
clr_ug_irq();

psk_trk();
flg_req_osc=1;
///=================
ready_dat=0;
///=================
#if 0
while(1)
{
get_osc();
msleep(200);
}
#endif
while(1)
{
set_changed_param(tmp_param_buff);
///check_ready_dat();
if(ready_dat)
///if(1)
{
#if 1
////???if(flg_req_osc)
  {
  if(get_osc())
    flg_req_osc=0;
  }
get_amps_tims();
#endif
psk_trk();
///  }
ready_dat=0;
///msleep(20);		///???

}
else
{
msleep(20);
///msleep(10);  
tcnt++;
if(tcnt>5)
	{
	tcnt=0;	
///====================================
///	  if(get_osc())
///	    flg_req_osc=0;

///====================================
////        if(on_flg_izl)
          psk_trk();
	}	
}
///msleep(20);  
msleep(5);  

	}
}

