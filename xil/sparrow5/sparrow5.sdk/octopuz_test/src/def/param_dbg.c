#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "octopuz.h"
#include "defectoscope.h"
///#include "my_respond.h"

#include "xyz.h"
///#include "ff.h"
///#include "minini.h"

extern uint8_t tbl_kus[];
extern void put_strobs_tact(uint8_t i_tact,uint8_t inus,uint8_t nstrob);

/// volatile int s_list_param_curr_num=0;

uint32_t g_changed_param=0;
uint8_t *t_par_buff;
uint8_t *tmp_param_buff;
static uint16_t param_num_bytes;
///================================================
#define UG_ADDR(x,tact) (ADDR_UG_PAR+x+((tact&MASK_TACT)<<SH_TACT_UG))
////#define UU_ADDR(x) (OFFS_UU_PAR+x+((i_tact&MASK_TACT)<<SH_TACT_UU))
///#define SPI_UG_ADDR(x) (ADDR_UG_PAR+x+((i_tact&MASK_TACT)<<SH_TACT_UG))
///#define SPI_UU_ADDR(x) (ADDR_UU_PAR+x+((i_tact&MASK_TACT)<<SH_TACT_UU))

void set_on_gens_tact(uint8_t i_tact,uint16_t i_dat)
{
wr_dat_trk(UG_ADDR(OFFS_ON_GEN,i_tact),i_dat);
_printk( "\r\n=== set_on_gens_tact : [%x:%x:[%x]",i_tact,i_dat,UG_ADDR(OFFS_ON_GEN,i_tact));

}

void set_len_per_tact(uint8_t i_tact,uint16_t i_dat)
{
wr_dat_trk(UG_ADDR(OFFS_LEN_TACT,i_tact),i_dat);
_printk( "\r\n=== set_len_per_tact : [%x:%x:[%x]",i_tact,i_dat,UG_ADDR(OFFS_LEN_TACT,i_tact));

}
void set_len_hipow_tact(uint8_t i_tact,uint16_t i_dat)
{
wr_dat_trk(UG_ADDR(OFFS_HI_POW,i_tact),i_dat);
}
uint16_t get_len_hipow_tact(uint8_t i_tact)
{
return rd_dat_trk(UG_ADDR(OFFS_HI_POW,i_tact));
}

void set_len_imp_tact(uint8_t i_tact,uint8_t i_num,uint8_t i_dat)
{
if(i_num>NUM_GENS-1)
	i_num=NUM_GENS-1;
///SPI_SET_UG_ADDR(OFFS_LEN_IMP0G+i_num);
///spi_set_dat(i_dat);
wr_dat_trk(UG_ADDR(OFFS_LEN_IMP0G+i_num,i_tact),i_dat);
////_printk("\r\n=== set_len_imp_tact [%x:%x:%x:[%x]",i_tact,i_num,i_dat,UG_ADDR(OFFS_LEN_IMP0G+i_num));

}

uint8_t get_len_imp_tact(uint8_t i_tact,uint8_t i_num)
{
if(i_num>NUM_GENS-1)
	i_num=NUM_GENS-1;
///SPI_SET_UG_ADDR(OFFS_LEN_IMP0G+i_num);
////return (uint8_t)(spi_get_dat()&0xff);
return  (uint8_t)(rd_dat_trk(UG_ADDR(OFFS_HI_POW,i_tact))&0xff);
}
///========== u us ========================
#define DEF_CLK_CNF_ADC		0xf
void set_adc_clk(void)
{
wr_dat_trk(ADDR_CNF_ADC,DEF_CLK_CNF_ADC);
}
uint8_t tbl_mux_a[NUM_MUX_US]=
{
0x80,0x40,0x20,0x10
};
uint8_t tbl_mux_b[NUM_MUX_US]=
{
0x8,0x4,0x2,0x1
};

void set_mux_tact(uint8_t i_tact,uint8_t i_mux_a,uint8_t i_mux_b)
{
uint8_t mux_ab;
if(i_mux_a>=NUM_MUX_US)
	i_mux_a=NUM_MUX_US-1;
i_mux_b-=NUM_MUX_US;
if(i_mux_b>=NUM_MUX_US)
	i_mux_b=NUM_MUX_US-1;
mux_ab=tbl_mux_a[i_mux_a]|tbl_mux_b[i_mux_b];
wr_dat_trk(UG_ADDR(OFFS_MUX_PREV,i_tact),mux_ab);
_printk("\r\n***set_mux_tact[%x:%x:%x:%x]",i_tact,i_mux_a,i_mux_b,mux_ab);

}

void set_kus_tact(uint8_t i_tact,uint8_t i_num,uint8_t i_dat)
{
if(i_num >= NUM_USS)
	i_num=NUM_USS-1;
///else
///	i_num=0;
///wr_dat_addr_m(ADDR_UU_PAR_CHAN(i_num&MASK_NUS)+OFFS_KUS,tbl_kus_db[i_dat]);
wr_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_KUS,i_dat*3);
_printk("\n\r set_kus_tact[%x:%x:%x:%x:%x",i_tact,i_num,i_dat
		,ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_KUS
		,i_dat*3);
}
void off_us_tact(uint8_t i_tact,uint8_t i_num)
{
if(i_num>NUM_USS-1)
	i_num=NUM_USS-1;
///wr_dat_addr_m(UU_ADDR(OFFS_KUS+((i_num&MASK_NUS)<<SH_NUS)),0);
wr_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_KUS,0);
}
uint8_t get_kus_tact(uint8_t i_tact,uint8_t i_num)
{
if(i_num>NUM_USS-1)
	i_num=NUM_USS-1;
///SPI_SET_UU_ADDR(OFFS_KUS+((i_num&MASK_NUS)<<SH_NUS));
////return (uint8_t)(spi_get_dat()&0xff);
return  (uint8_t)(rd_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_KUS));
}
void set_flgs1_tact(uint8_t i_tact,uint8_t i_num,uint16_t i_dat)
{
if(i_num>NUM_USS-1)
	i_num=NUM_USS-1;
///SPI_SET_UU_ADDR(OFFS_FLGS1+((i_num&MASK_NUS)<<SH_NUS));
///spi_set_dat(i_dat);
///wr_dat_addr_m(UU_ADDR(OFFS_FLGS1+((i_num&MASK_NUS)<<SH_NUS)),i_dat);
wr_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_FLGS1,i_dat);
}
uint16_t get_flgs1_tact(uint8_t i_tact,uint8_t i_num)
{
if(i_num>NUM_USS-1)
	i_num=NUM_USS-1;
///SPI_SET_UU_ADDR(OFFS_FLGS1+((i_num&MASK_NUS)<<SH_NUS));
///return spi_get_dat();
return  rd_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_FLGS1);

}
void set_flgs2_tact(uint8_t i_tact,uint8_t i_num,uint16_t i_dat)
{
if(i_num>NUM_USS-1)
  i_num=NUM_USS-1;
wr_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_FLGS2,i_dat);
}
uint16_t get_flgs2_tact(uint8_t i_tact,uint8_t i_num)
{
if(i_num>NUM_USS-1)
	i_num=NUM_USS-1;
///SPI_SET_UU_ADDR(OFFS_FLGS2+((i_num&MASK_NUS)<<SH_NUS));
///return spi_get_dat();
return  rd_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_FLGS2);
}

void put_gens_par_tact(uint8_t i_takt)
{
uint8_t ii;
uint16_t htmp=0;
i_takt&=MASK_TACT;
for(ii=0;ii<NUM_GENS;ii++)
{
if(curr_par_trk.takts[i_takt].gens[ii].on_gen)
	{
	htmp |= 0x1<<ii;
	set_len_imp_tact(i_takt,ii,curr_par_trk.takts[i_takt].gens[ii].len_imp);
	}
}
set_on_gens_tact(i_takt,htmp);
set_len_per_tact(i_takt,curr_par_trk.takts[i_takt].len_tact);
///======================================
if(curr_par_trk.takts[i_takt].len_hipow<MIN_LEN_HIPOW)
 curr_par_trk.takts[i_takt].len_hipow=MIN_LEN_HIPOW;
///curr_par_trk.takts[i_takt].len_hipow=DEF_LEN_HIPOW;
///==================================================
set_len_hipow_tact(i_takt,curr_par_trk.takts[i_takt].len_hipow);
}
void check_set_param(void)
{
////memcpy(&curr_param,&rec_param,sizeof(par_trk_t));
}
///================================
void set_dpoint_vrch(par_vrch_t *i_points,uint8_t i_tact,uint8_t i_num)
{
uint8_t ii;
///uint8_t num_points;
int16_t dkus;
int16_t vpoint;
int16_t dt;
if(i_num>NUM_USS-1)
  i_num=NUM_USS-1;
////num_points=MAX_NUM_PVRCH;
if(i_points->num_p>MAX_NUM_PVRCH)
  i_points->num_p=MAX_NUM_PVRCH;
for(ii=0;ii<i_points->num_p;ii++)
	{
	if( ii==0 )
          {
          i_points->kus[0]=0;
          dkus = (int16_t)i_points->kus[ii];
          dt	 = (int16_t)i_points->time[ii];
          }
	else	///ii==0
          {
          uint8_t tkus=i_points->kus[ii];
          uint8_t kus_prev=i_points->kus[ii-1];
          if(tkus>MAX_KUS_DB)
              tkus=MAX_KUS_DB;
          if(kus_prev>MAX_KUS_DB)
            kus_prev=MAX_KUS_DB;
          dkus = (int16_t)tbl_kus[tkus] -
                (int16_t)tbl_kus[kus_prev];
          dt	 = (int16_t)i_points->time[ii] -
                (int16_t)i_points->time[ii-1];
			}
			dt  /= VRCH_DELT;
		if( dt <= 0 )
			dt = 1;
		vpoint = (dkus*256)/dt;

///                wr_dat_addr_m(SPI_UU_ADDR(ii*2+((i_num&MASK_NUS)<<SH_NUS)),(uint16_t)dt);
        wr_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+ii*2,(uint16_t)dt);
        wr_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+ii*2+1,(uint16_t)vpoint);
 ////               wr_dat_addr_m(SPI_UU_ADDR(ii*2+1+((i_num&MASK_NUS)<<SH_NUS)),(uint16_t)vpoint);
        ///=================================================
         _printk("\r\n put_v=[[%x:%x:%x][%x:%x][%x:%x][%x:%x]",i_tact,i_num,ii
         			,i_points->kus[ii],i_points->time[ii]
         			,ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+ii*2,dt
 					,ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+ii*2+1,vpoint );
         ///===================================================


	}
if(ii<MAX_NUM_PVRCH)
	{
///        wr_dat_addr_m(SPI_UU_ADDR(ii*2+((i_num&MASK_NUS)<<SH_NUS)),0);
///        wr_dat_addr_m(SPI_UU_ADDR(ii*2+1+((i_num&MASK_NUS)<<SH_NUS)),(uint16_t)vpoint);
        wr_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+ii*2,0);
        wr_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+ii*2+1,(uint16_t)vpoint);
	}
}

void vrch_set_par_us(uint8_t i_tact,uint8_t i_num)
{
uint16_t htmp=0;
if(i_num>NUM_USS-1)
	i_num=NUM_USS-1;
////htmp=rd_dat_addr_m(SPI_UU_ADDR(OFFS_FLGS1+((i_num&MASK_NUS)<<SH_NUS)));
htmp=rd_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_FLGS1);
if(curr_par_trk.takts[i_tact].uss[i_num].vrch.on_vrch)
	{
	set_dpoint_vrch(&curr_par_trk.takts[i_tact].uss[i_num].vrch
	,i_tact,i_num);
	htmp |= (0x1<<OFFS_B_ON_VRCH);
////        wr_dat_addr_m(SPI_UU_ADDR(OFFS_FLGS1+((i_num&MASK_NUS)<<SH_NUS)),htmp);
        wr_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_FLGS1,htmp);


	}
else
	{
	htmp &= ~(0x1<<OFFS_B_ON_VRCH);
///        wr_dat_addr_m(SPI_UU_ADDR(OFFS_FLGS1+((i_num&MASK_NUS)<<SH_NUS)),htmp);
        wr_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_FLGS1,htmp);

	}

}
void put_onus_tact(uint8_t i_tact,uint8_t i_num,uint8_t onus)
{
uint16_t tmp=(onus&0x1)<<OFFS_B_ON_US;
uint16_t tmp1;
///SPI_SET_UU_ADDR(OFFS_FLGS1+((i_num&MASK_NUS)<<SH_NUS));
///tmp1=spi_get_dat();
tmp1=rd_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_FLGS1);

tmp1&= ~(0x1<<OFFS_B_ON_US);
tmp|= tmp1;
////spi_set_dat(tmp);
wr_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_FLGS1,tmp);
}
void put_tt_tact(uint8_t i_tact,uint8_t i_num,uint8_t tt)
{
uint16_t htmp;
////SPI_SET_UU_ADDR(OFFS_FLGS2+((i_num&MASK_NUS)<<SH_NUS));
///htmp=spi_get_dat();
htmp=rd_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_FLGS2);
htmp &= ~(0x7<<4);
htmp |= ((tt&0x7)<<4);
///spi_set_dat(htmp);
wr_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_FLGS2,htmp);

////printk( KERN_INFO "\n=== [mod]put_tt_tact????====[%x:%x:%x]\n",i_tact,nus,htmp);
}
void put_tv_tact(uint8_t i_tact,uint8_t i_num,uint8_t tv)
{
uint16_t htmp;
///SPI_SET_UU_ADDR(OFFS_FLGS2+((i_num&MASK_NUS)<<SH_NUS));
///htmp=spi_get_dat();
htmp=rd_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_FLGS2);
htmp &= ~(0xf);
htmp |= (tv&0xf);
////spi_set_dat(htmp);
wr_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_FLGS2,htmp);
}
void put_vron_tact(uint8_t i_tact,uint8_t i_num,uint8_t vron)
{
uint16_t tmp=(vron&0x1)<<OFFS_B_ON_VRCH;
uint16_t tmp1;

tmp1=rd_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_FLGS1);
tmp1&= ~(0x1<<OFFS_B_ON_VRCH);
tmp|= tmp1;
_printk("\r\n***put_vron_tact[%x:%x:%x:%x:%x][%x]",i_tact,i_num,vron
		,tmp,tmp1,ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_FLGS1);

wr_dat_trk(ADDR_UU_PAR_CHAN(i_num&MASK_NUS,i_tact)+OFFS_FLGS1,tmp);

}


void put_all_strobs_tact(uint8_t i_tact,uint8_t inus)
{
int ii;
for(ii=0;ii<MAX_NUM_STRB;ii++)
  {
  put_strobs_tact(i_tact,inus,ii);  
  }
}

///==========================================

void put_uss_par_tact(uint8_t i_tact)
{
uint8_t ii;
uint16_t htmp=0;
i_tact&=MASK_TACT;
for(ii=0;ii<NUM_USS;ii++)
{
htmp=rd_dat_trk(ADDR_UU_PAR_CHAN(ii&MASK_NUS,i_tact)+OFFS_FLGS1);
htmp &= ~(0x1<<OFFS_B_ON_US);
if(curr_par_trk.takts[i_tact].uss[ii].on_us )
	{
	uint16_t tmp;
	tmp=curr_par_trk.takts[i_tact].uss[ii].kus;

	set_kus_tact(i_tact,ii,tmp);
    put_all_strobs_tact(i_tact,ii);
	htmp |= (0x1<<OFFS_B_ON_US);
    wr_dat_trk(ADDR_UU_PAR_CHAN(ii&MASK_NUS,i_tact)+OFFS_FLGS1,htmp);
    htmp=rd_dat_trk(ADDR_UU_PAR_CHAN(ii&MASK_NUS,i_tact)+OFFS_FLGS2);
    curr_par_trk.takts[i_tact].uss[ii].detpar=2;
    curr_par_trk.takts[i_tact].uss[ii].tauv=7;
    curr_par_trk.takts[i_tact].uss[ii].taut=3;
	htmp &= ~(0x37f);
	htmp |= (uint8_t)(curr_par_trk.takts[i_tact].uss[ii].tauv&0xf);
	htmp |= (uint8_t)((curr_par_trk.takts[i_tact].uss[ii].taut&0x7)<<4);
	htmp |= (uint16_t)((curr_par_trk.takts[i_tact].uss[ii].detpar&0x3)<<8);
    ///=========================================
        
        _printk("\r\n***put_uss_par_tact[%x:%x:%x:%x:%x][%x]",i_tact,ii,htmp
                ,curr_par_trk.takts[i_tact].uss[ii].tauv&0xf
                  ,(curr_par_trk.takts[i_tact].uss[ii].taut&0x7)
				  ,curr_par_trk.takts[i_tact].uss[ii].kus);

        ///========================================
    wr_dat_trk(ADDR_UU_PAR_CHAN(ii&MASK_NUS,i_tact)+OFFS_FLGS2,htmp);
	vrch_set_par_us(i_tact,ii);
	}
else
	{
////	set_kus_tact(i_tact,ii,0);
 ////   off_us_tact(i_tact,ii);
    wr_dat_trk(ADDR_UU_PAR_CHAN(ii&MASK_NUS,i_tact)+OFFS_FLGS1,htmp);
	}
}
set_mux_tact(i_tact,curr_par_trk.takts[i_tact].uss[0].num_pus
		,curr_par_trk.takts[i_tact].uss[1].num_pus);

}

void put_uss_gens_par_tact(uint8_t i_tact)
{
put_gens_par_tact(i_tact);
put_uss_par_tact(i_tact);
}
void set_osc0_tact_view(uint8_t inum_tact)
{
if(inum_tact>NUM_TACTS)
	inum_tact=NUM_TACTS;
wr_dat_trk(ADDR_NTACT_OSC_A,inum_tact);
}

void set_osc0_us_view(uint8_t inum_us)
{
if(inum_us >= NUM_USS)
	inum_us=NUM_USS-1;

wr_dat_trk(ADDR_NUS_OSC_A,inum_us);
set_begr_osc_a(curr_osc_par.curr_begr);
set_stp_osc_a(curr_osc_par.curr_step_r);
set_osc0_tact_view(curr_osc_par.curr_tact);
}
void set_begr_osc_a(uint16_t idat)
{
if(idat==0)
  idat=1;
wr_dat_trk(ADDR_BEG_RZV_A,idat<<2);
}
//====================================
void set_stp_osc_a(uint8_t idat)
{
wr_dat_trk(ADDR_OSC_STP_A,idat);
}
void set_delay_strb(uint8_t i_tact,uint8_t inus,uint16_t dela)
{
	uint8_t nus=	inus&(NUM_USS-1);
/*
iowrite16(OFFS_RAM_PAR+(i_tact<<6)+ (nus<<SH_NUS)
		+OFFS_DELAY, weasel_trk_base+OFFS_UUU_ADDR);
iowrite16((u16)(dela), weasel_trk_base+OFFS_UUU_DAT);
*/
}

void set_delay_vrch(uint8_t i_tact,uint8_t inus,uint16_t dela)
{
	uint8_t nus=	inus&(NUM_USS-1);
/*
iowrite16(OFFS_RAM_PAR+(i_tact<<6)+ (nus<<SH_NUS)
		+OFFS_DELAY_V, weasel_trk_base+OFFS_UUU_ADDR);
iowrite16((u16)(dela), weasel_trk_base+OFFS_UUU_DAT);
*/
}

void put_all_par_trk(void)
{
int ii;
_printk( "\n\r= put_all_par_trk====\n\r");

set_num_izm(curr_par_trk.izm_par.num_izm);
set_max_tact(curr_par_trk.izm_par.num_tacts );

for(ii=0;ii<NUM_TACTS;ii++)
{
put_uss_gens_par_tact(ii);
}
set_osc0_us_view(curr_osc_par.curr_us);
set_all_pow();

for(ii=0;ii<NUM_TACTS;ii++)
{
curr_par_trk.takts[ii].len_hipow=_DEF_LEN_HIPOW;
}

}

///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////extern struct rt_ringbuffer cmd_rb;

#define _curr_tact curr_osc_par.curr_tact
#define _curr_strb curr_osc_par.curr_strob
#define _curr_us curr_osc_par.curr_us
#define _curr_pvrch curr_osc_par.curr_pvrch
#define _mCurrUs curr_par_trk.takts[_curr_tact].uss[_curr_us]
#define _mCurrNgen curr_par_trk.takts[_curr_tact].gens[curr_par_contr.ngen]
////#define _mCurrPow mod_curr_param.pow_gen[ed_par_trk.curr_pow]
//+++++++++++++++++++++++++++++++++++
///par_contr_t curr_par_contr;
void set_changed_param(uint8_t *ibuff)
{
uint16_t tmp;
uint16_t t_offs=0;
////debug_out("\n=== set_changed_param : %lx\n", g_changed_param);
if(g_changed_param &(uint32_t)CHNG_ALL)
	{
_printk( "\n\r=== CHNG_ALL : %lx", g_changed_param);
///	memcpy(&ed_par_trk,ibuff+t_offs,sizeof(ed_par_trk_t));
////	t_offs+=sizeof(ed_par_trk_t);
	memcpy(&curr_par_contr,ibuff+t_offs,sizeof(par_contr_t));
	check_set_param();
///	t_offs+=sizeof(par_trk_t);
	put_all_par_trk();
////	flg_need_save_par=1;
///	save_cur_par();	
	g_changed_param=0;
	}
else if(g_changed_param)
	{
/*
	if(C&CHNG_CHAN)
		{
		memcpy(&ed_par_trk.curr_chan,tmp_par_buff+t_offs,sizeof(uint8_t));
		put_chan_trk();
		t_offs+=sizeof(uint8_t);
///		printk( KERN_INFO "\n=== [mod]CHNG_CHAN : %x", ed_par_trk.curr_chan);
		g_changed_param&=~CHNG_CHAN;
		}
		*/
	_printk("\r\n=== g_changed_param : [%x]",g_changed_param);

	if(g_changed_param&CHNG_TACT)
		{
		memcpy(&curr_osc_par.curr_tact,ibuff+t_offs,sizeof(uint8_t));
		t_offs+=sizeof(uint8_t);
		set_osc0_tact_view(curr_osc_par.curr_tact);
///		printk( KERN_INFO "\n=== [mod]CHNG_TACT : %x", ed_par_trk.curr_tact);
		g_changed_param&=~CHNG_TACT;
		}
	if(g_changed_param&CHNG_NUS)
		{
		memcpy(&curr_osc_par.curr_us,ibuff+t_offs,sizeof(uint8_t));
		t_offs+=sizeof(uint8_t);
		memcpy(&curr_osc_par.num_inp,ibuff+t_offs,sizeof(uint8_t));
		t_offs+=sizeof(uint8_t);
		curr_par_trk.takts[curr_osc_par.curr_tact].uss[curr_osc_par.curr_us].num_pus=curr_osc_par.num_inp;
		set_osc0_us_view(curr_osc_par.curr_us);

		_printk("\r\n=== CHNG_NUS : [%x][%x(%x)][%x][%x]\n"
			,curr_osc_par.curr_tact,curr_osc_par.curr_us,curr_osc_par.num_inp,_mCurrUs.on_us,_mCurrUs.kus);
                ///====================================
                put_uss_par_tact(curr_osc_par.curr_tact);
////                set_begr_osc_a(ed_par_trk.curr_begr);
////		set_stp_osc_a(ed_par_trk.curr_step_r);	///????
                ///====================================
        _printk("\r\n=== END CHNG_NUS : [%x][%x(%x)][%x][%x][%x:%x]\n"
    			,curr_osc_par.curr_tact,curr_osc_par.curr_us,curr_osc_par.num_inp,_mCurrUs.on_us,_mCurrUs.kus
				,curr_par_trk.takts[curr_osc_par.curr_tact].uss[0]
				,curr_par_trk.takts[curr_osc_par.curr_tact].uss[1]
				);


		g_changed_param&=~CHNG_NUS;
		}
	if(g_changed_param&CHNG_ONUS)
		{
		memcpy(&_mCurrUs.on_us,ibuff+t_offs,sizeof(uint8_t));
		check_set_param();
		tmp=_mCurrUs.on_us;
		put_onus_tact(_curr_tact,_curr_us,tmp);

////		put_uss_par_tact(ed_par_trk.curr_tact);
///		put_onoff_us();
		t_offs+=sizeof(uint8_t);
		_printk( "\n\r=== CHNG_ONUS : [%x:%x:%x:%x]",_curr_tact,_mCurrUs.on_us
		,curr_par_trk.takts[curr_osc_par.curr_tact].uss[0]
		,curr_par_trk.takts[curr_osc_par.curr_tact].uss[1]												);
////		_printk("\r\n=== CHNG_ONUS : [%x][%x][%x][%x]\n",tmp
////			,curr_par_trk.dop_par.curr_tact,curr_par_trk.dop_par.curr_us,_mCurrUs.on_us);
		g_changed_param&=~CHNG_ONUS;
		}
	if(g_changed_param&CHNG_KUS)
		{
		memcpy(&_mCurrUs.kus,ibuff+t_offs,sizeof(uint8_t));
		check_set_param();
		tmp=_mCurrUs.kus;
		set_kus_tact(curr_osc_par.curr_tact,curr_osc_par.curr_us,tmp);
		_printk("\r\n=== CHNG_KUS : [%x][%x][%x]\n",tmp
			,curr_osc_par.curr_tact,curr_osc_par.curr_us);
		t_offs+=sizeof(uint8_t);
		g_changed_param&=~CHNG_KUS;
		}
	if(g_changed_param&CHNG_BEGR)
		{
		memcpy(&curr_osc_par.curr_begr,ibuff+t_offs,sizeof(uint16_t));
////		memcpy(&_mCurrUs.beg_r,ibuff+t_offs,sizeof(uint16_t));
		set_begr_osc_a(curr_osc_par.curr_begr);
		t_offs+=sizeof(uint16_t);
        _printk( "\r\n=> CHNG_BEGR : %x", curr_osc_par.curr_begr);
		g_changed_param&=~CHNG_BEGR;
		}
	if(g_changed_param&CHNG_ENDR)
		{
		memcpy(&curr_osc_par.curr_end_r,ibuff+t_offs,sizeof(uint16_t));
		t_offs+=sizeof(uint16_t);
		memcpy(&curr_osc_par.curr_step_r,ibuff+t_offs,sizeof(uint8_t));
////		memcpy(&_mCurrUs.stp_r,ibuff+t_offs,sizeof(uint8_t));
		set_stp_osc_a(curr_osc_par.curr_step_r);	///????
		t_offs+=sizeof(uint8_t);
		_printk( "\n\r=== CHNG_ENDR : [%x:%x]\n", curr_osc_par.curr_end_r
				,curr_osc_par.curr_step_r);
		g_changed_param&=~CHNG_ENDR;
		}
	if(g_changed_param&CHNG_TT)
		{
		memcpy(&_mCurrUs.taut,ibuff+t_offs,sizeof(uint8_t));
		check_set_param();
		tmp=_mCurrUs.taut;
		put_tt_tact(curr_osc_par.curr_tact,curr_osc_par.curr_us,tmp);
///		put_uss_par_tact(ed_par_trk.curr_tact);
		t_offs+=sizeof(uint8_t);
	///	printk( KERN_INFO "\n=== [mod]CHNG_TT : %x",_mCurrUs.taut);
		g_changed_param&=~CHNG_TT;
		}
	if(g_changed_param&CHNG_TV)
		{
		memcpy(&_mCurrUs.tauv,ibuff+t_offs,sizeof(uint8_t));
		check_set_param();
		tmp=_mCurrUs.tauv;
		put_tv_tact(curr_osc_par.curr_tact,curr_osc_par.curr_us,tmp);
///		put_uss_par_tact(ed_par_trk.curr_tact);
		t_offs+=sizeof(uint8_t);
	///	printk( KERN_INFO "\n=== [mod]CHNG_TV : %x",_mCurrUs.tauv);
		g_changed_param&=~CHNG_TV;
		}
	if(g_changed_param&CHNG_CURSTRB)
		{
		memcpy(&curr_osc_par.curr_strob,ibuff+t_offs,sizeof(uint8_t));
		t_offs+=sizeof(uint8_t);
////		printk( KERN_INFO "\n=== [mod]CHNG_NSTRB : %x", ed_par_trk.curr_strob);
		g_changed_param&=~CHNG_CURSTRB;
		}

	if(g_changed_param&CHNG_BSTRB)
		{
		memcpy((void*)&_mCurrUs.strb[_curr_strb].beg,ibuff+t_offs,sizeof(uint16_t));
		check_set_param();
		put_strobs_tact(curr_osc_par.curr_tact,_curr_us,_curr_strb);
		_printk( "\n\r=== CHNG_BSTRB : [%x:%x:%x]",_curr_us, _curr_strb
				,_mCurrUs.strb[_curr_strb].beg);
		t_offs+=sizeof(uint16_t);
		g_changed_param&=~CHNG_BSTRB;
		}
	if(g_changed_param&CHNG_LSTRB)
		{
		memcpy(&_mCurrUs.strb[_curr_strb].len,ibuff+t_offs,sizeof(uint16_t));
		check_set_param();
		put_strobs_tact(curr_osc_par.curr_tact,_curr_us,_curr_strb);
		_printk( "\n\r=== CHNG_LSTRB : [%x:%x:%x]", _curr_us,_curr_strb
				,_mCurrUs.strb[_curr_strb].len);
		t_offs+=sizeof(uint16_t);
		g_changed_param&=~CHNG_LSTRB;
		}
	if(g_changed_param&CHNG_POR)
		{
		memcpy(&_mCurrUs.strb[_curr_strb].por,ibuff+t_offs,sizeof(uint8_t));
		t_offs+=sizeof(uint8_t);
		_printk("\n\r===CHNG_POR[%x:%x:%x]====",_curr_us,_curr_strb
				,_mCurrUs.strb[_curr_strb].por);
		g_changed_param&=~CHNG_POR;
		}
	if(g_changed_param&CHNG_NUMPV)
		{
		memcpy(&curr_osc_par.curr_pvrch,ibuff+t_offs,sizeof(uint8_t));
		t_offs+=sizeof(uint8_t);
		memcpy(&_mCurrUs.vrch.num_p ,ibuff+t_offs,sizeof(uint8_t));
		t_offs+=sizeof(uint8_t);
///		printk( KERN_INFO "\n=== [mod]CHNG_NUMPV : %x", ed_par_trk.curr_pvrch);
		g_changed_param&=~CHNG_NUMPV;
		}
	if(g_changed_param&CHNG_VRON)
		{
		memcpy(&_mCurrUs.vrch.on_vrch,ibuff+t_offs,sizeof(uint8_t));
		check_set_param();
		tmp=_mCurrUs.vrch.on_vrch;
		put_vron_tact(curr_osc_par.curr_tact,curr_osc_par.curr_us,tmp);
///		put_uss_par_tact(ed_par_trk.curr_tact);
		t_offs+=sizeof(uint8_t);
////		printk( KERN_INFO "\n=== [mod]CHNG_VRON : %x", _mCurrUs.vrch.on_vrch);
		g_changed_param&=~CHNG_VRON;
		}
#if 1
	if(g_changed_param&CHNG_VTIME)  ///curr point vrch
		{
		memcpy((void*)&_mCurrUs.vrch.time[_curr_pvrch],ibuff+t_offs,sizeof(uint16_t));
		t_offs+=sizeof(uint16_t);
		check_set_param();
		vrch_set_par_us(_curr_tact,_curr_us);
////		put_uss_par_tact(ed_par_trk.curr_tact);
		_printk("\r\n=== CHNG_VTIME : [%x:%x:%x][%x]\n"
			,_curr_tact,_curr_us,_curr_pvrch,_mCurrUs.vrch.time[_curr_pvrch]);
		g_changed_param&=~CHNG_VTIME;
		}
	if(g_changed_param&CHNG_VKUS)  ///
		{
		memcpy((void*)&_mCurrUs.vrch.kus[_curr_pvrch],ibuff+t_offs,sizeof(uint8_t));
		t_offs+=sizeof(uint8_t);
		check_set_param();
		vrch_set_par_us(_curr_tact,_curr_us);
		_printk("\r\n=== CHNG_VKUS : [%x:%x:%x][%x]\n"
			,_curr_tact,_curr_us,_curr_pvrch,_mCurrUs.vrch.kus[_curr_pvrch]);
		g_changed_param&=~CHNG_VKUS;
		}
#endif
	uint8_t npow;		///2
	uint8_t ngen;			///8

	if(g_changed_param&CHNG_CGEN)
		{
		memcpy(&curr_par_contr.ngen,ibuff+t_offs,sizeof(uint8_t));
		t_offs+=sizeof(uint8_t);
		g_changed_param&=~CHNG_CGEN;
		}
	if(g_changed_param&CHNG_ONGEN)
		{
		memcpy(&_mCurrNgen.on_gen,ibuff+t_offs,sizeof(uint8_t));
		check_set_param();
		put_gens_par_tact(curr_osc_par.curr_tact);
		t_offs+=sizeof(uint8_t);
        _printk( "\r\n=== CHNG_ONGEN : [%x:%x:%x]",_mCurrNgen.on_gen
        ,curr_osc_par.curr_tact
		,curr_par_contr.ngen);
		g_changed_param&=~CHNG_ONGEN;
		}
	if(g_changed_param&CHNG_LIMP)
		{
		memcpy(&_mCurrNgen.len_imp,ibuff+t_offs,sizeof(uint8_t));
		check_set_param();
		set_len_imp_tact(curr_osc_par.curr_tact
				,curr_par_contr.ngen
				,_mCurrNgen.len_imp);

///		put_gens_par_tact(curr_ed_par_trk.curr_tact);
		t_offs+=sizeof(uint8_t);
		_printk("\r\n=== CHNG_LIMP : %x",_mCurrNgen.len_imp);
		g_changed_param&=~CHNG_LIMP;
		}
	if(g_changed_param&CHNG_NPOW)
		{
		memcpy(&curr_par_contr.npow,ibuff+t_offs,sizeof(uint8_t));
		t_offs+=sizeof(uint8_t);
		_printk("\r\n===CHNG_NPOW[%x]====",curr_par_contr.npow);
		g_changed_param&=~CHNG_NPOW;
		}
	if(g_changed_param&CHNG_PVAL)
		{
		memcpy((void*)&curr_par_trk.pow_gen[curr_par_contr.npow].val,ibuff+t_offs,sizeof(uint8_t));
		check_set_param();
		set_pow_vol(curr_par_contr.npow,curr_par_trk.pow_gen[curr_par_contr.npow].val);

		t_offs+=sizeof(uint8_t);
        _printk("\r\n=== CHNG_PVAL : [%x:%x]",curr_par_contr.npow
        		,curr_par_trk.pow_gen[curr_par_contr.npow].val);
		g_changed_param&=~CHNG_PVAL;
		}
	if(g_changed_param&CHNG_REJ_OSC)
		{
		memcpy(&curr_osc_par.rej_osc,ibuff+t_offs,sizeof(uint8_t));
		t_offs+=sizeof(uint8_t);
		_printk("\r\n=== CHNG_REJ_OSC : %x",curr_osc_par.rej_osc);
		g_changed_param&=~CHNG_REJ_OSC;
		}
		
	else
		g_changed_param= 0;
////	flg_need_save_par=1;
////	save_cur_par();	
	}
}
///================================================
par_trk_t dcurr_par_trk;

void init_def_par_trk(void)
{
uint8_t ii;
uint8_t jj;
uint8_t kk;
///curr_par_trk.num_gen=	DEF_NUM_GEN;
///curr_par_trk.num_us=DEF_NUM_US;
set_num_summ(DEF_NUM_SUMM);
set_num_izm(DEF_NUM_IZM);

curr_par_trk.izm_par.num_tacts=	DEF_NUM_TACT;
set_max_tact(curr_par_trk.izm_par.num_tacts);   ///
curr_par_trk.izm_par.num_izm=DEF_NUM_IZM;
curr_par_trk.pow_gen[0].val=DEF_POW_VOL;
curr_par_trk.pow_gen[1].val=DEF_POW_VOL;
for(ii=0;ii<curr_par_trk.izm_par.num_tacts;ii++)
		{
		curr_par_trk.takts[ii].len_hipow=MAX_LEN_HIPOW;
		curr_par_trk.takts[ii].len_tact=MAX_LEN_TACT;
		for(jj=0;jj<DEF_NUM_GEN;jj++)
			{
			curr_par_trk.takts[ii].gens[jj].on_gen=0;	
			curr_par_trk.takts[ii].gens[jj].len_imp=DEF_LEN_IMP;	
			}
		for(jj=0;jj<DEF_NUM_US;jj++)
			{
			curr_par_trk.takts[ii].uss[jj].rej =DEF_REJ;
			curr_par_trk.takts[ii].uss[jj].num_pus =0;
			curr_par_trk.takts[ii].uss[jj].on_us=0;	
			curr_par_trk.takts[ii].uss[jj].kus=DEF_KUS;
			curr_par_trk.takts[ii].uss[jj].detpar=DEF_DET_PAR;
			curr_par_trk.takts[ii].uss[jj].taut=DEF_TT;
			curr_par_trk.takts[ii].uss[jj].tauv=DEF_TV;
				
			for(kk=0;kk<MAX_NUM_STRB;kk++)
			{
			curr_par_trk.takts[ii].uss[jj].strb[kk].on_strb=1;
			curr_par_trk.takts[ii].uss[jj].strb[kk].por=DEF_POR1;
			curr_par_trk.takts[ii].uss[jj].strb[kk].offs_type=DEF_TYPE_STR;	///
				
			curr_par_trk.takts[ii].uss[jj].strb[kk].beg=DEF_BSTR1+kk*5;
			curr_par_trk.takts[ii].uss[jj].strb[kk].len=DEF_LSTR1;
			}
			curr_par_trk.takts[ii].uss[jj].vrch.on_vrch=0;
			curr_par_trk.takts[ii].uss[jj].vrch.num_p=DEF_NUM_PV;
			for(kk=0;kk<MAX_NUM_PVRCH;kk++)
				{
				curr_par_trk.takts[ii].uss[jj].vrch.time[kk]=DEF_TIME_VRCH+kk;
				curr_par_trk.takts[ii].uss[jj].vrch.kus[kk]=DEF_KUS_VRCH;
				}
		}
}
}	

int check_par(par_trk_t *p_par_trk)
{
	return 1;
}
int load_cur_par(void)
{
	return 0;
}

///==============================================
void set_rec_param(uint8_t* ibuf)
{
param_num_bytes= (*ibuf)|(*(ibuf+1)<<8);

memcpy(t_par_buff,ibuf+4,param_num_bytes);
memcpy(&g_changed_param,ibuf+4,sizeof(uint32_t));
_printk("\r\n=== param_num_bytes : [%x]",param_num_bytes);

}
///=============================================
