#include <string.h>
#include "octopuz.h"
#include "octopuz_types.h"
////#include "board_cnf.h"
///#include "drv_spi.h"
#include "drv_utrk.h"
///#include "my_platform.h"
#include "defectoscope.h"
///#include "platform_peripheral.h"
///#include "platform_mcu_peripheral.h"

///extern void hw_spi1_set_data_size(uint16_t ds);
uint8_t ready_dat=0;	
#if 0
uint8_t tbl_kus_db[MAX_KUS_DB+1]={10,20,26,30,35,38  ///0...5 +
	,41,43,48,50,53,55					///6..11
	,58,60,62,65,68,70					///12..17
	,73,75,77,80,83,85					///18..23
	,88,91,93,96,99,101		///24..29
	,104,106,110,113,116,118		///30..35
	,121,123,126,129,131,134		///36..41
	,136,138,142,145,147,150		///42..47
	,152,154,157,159,164,166		///48..53
	,169,171,174,177,180,182		///54..59
	,184,186,189,191,195,197		///60..65
	,200,202,205,208,210,212		///66..71
	,216,218,221,224,227,229		///72..77
	,231,233,235,237,240,242		///78..83
        ,245,247,250,252,255			///84..88									///84
};
#endif
///=================================================
const uint8_t tbl_kus[256]={
0,1,2,3,4,5,6,7,8,9,10,11,13,15,17,19,//0..19
21,23,25,27,29,31,	///20-32
33,35,37,40,43,46,
49,52,55,58,61,64,
67,70,73,76,79,82,
85,88,91,94,97,100,
103,105,108,111,114,117,
122,126,130,134,138,142,
146,150,154,158,162,166,
170,174,178,183,187,192,
197,202,207,212,217,222,
222,222,222,222,222,222,
222,222,222,222,222,222,
222,222,222,222,222,222,
222,222,222,222,222,222,
};


////uint8_t tbl_pow[NUM_POW+1]={255,60,3,3};
///===========================================
us_dat_rez_t cur_uss_data[NUM_USS];
///===========================================
void add_new_dat(uint8_t nus,void *i_us_dat)
{
memcpy(&cur_uss_data[nus],i_us_dat,sizeof(us_dat_rez_t));
}

uint16_t t_ptr[MAX_NUM_WORD_DAT_TACT*NUM_TACTS];
#define CUR_NUM_IZM curr_par_trk.izm_par.num_izm
#define CUR_NUM_TACTS curr_par_trk.izm_par.num_tacts
#define CUR_REJ_US(t,u) curr_par_trk.takts[t].uss[u].rej

////#define NUM_WORD_DAT_TACT(r) (r==TD_TOL_REZ) ? MAX_LEN_FFT:MAX_LEN_STROBS_DAT*CUR_NUM_IZM;

void get_amps_tims(void)
{
int ii;
int jj;
int kk;
int tt;
int num_word_tact;
uint16_t htmp=0;
uint8_t curr_rej;
if(CUR_NUM_TACTS>NUM_TACTS)
	CUR_NUM_TACTS=NUM_TACTS;
for(kk=0;kk<NUM_USS;kk++)
	{
	jj=0;
	for(tt=0;tt<CUR_NUM_TACTS;tt++)
		{
///		curr_rej=CUR_REJ_US(tt,kk);
		curr_rej=0;
		if(curr_rej!=TD_TOL_REZ)
			{
			for(ii=0;ii < MAX_LEN_STROBS_DAT*CUR_NUM_IZM;ii++)
				{
				if(tt==1)
					jj++;
				htmp=rd_dat_trk(ADDR_UU_REZ_CHAN(kk)+((tt&0x7)<<8)+ii);
				*(t_ptr+sizeof(us_dat_tact_n_izm_t)*tt/sizeof(uint16_t)+ii )=htmp;
				}
			}
		else
			{
			;
			}
		}
	add_new_dat(kk,t_ptr);

	}
////	add_fft_dat();

}

///=========== ugen =====================
void set_spd_spi_pow(uint8_t i_div)
{
///spi1_mutex_lock(); 
wr_dat_trk(ADDR_SPD_PRDAC,i_div);
///spi1_mutex_unlock(); 
}

void set_pow_vol(uint8_t num,uint8_t i_vol)
{
#if 0
uint8_t tmp;
if(i_vol==PC_POW_LOW)
  tmp=tbl_pow[0];
else if(i_vol==PC_POW_MID) 
  tmp=tbl_pow[1];
else
  tmp=tbl_pow[2];
///spi1_mutex_lock(); 
wr_dat_addr_m(ADDR_POW_VOL,tmp);
#endif
wr_dat_trk(ADDR_POW_GEN+(0x1<<(num&0x1)),i_vol);
_printk("\r\n=== set_pow_vol : [%x:%x[%x]",num
 		,i_vol,ADDR_POW_GEN+(0x1<<(num&0x1)));


///spi1_mutex_unlock(); 
}
void set_all_pow(void)
{
int ii;
for(ii=0;ii<NUM_POW_GENS;ii++)
	{
	set_pow_vol(ii,curr_par_trk.pow_gen[ii].val);
	}
}
void set_num_izm(uint8_t i_num)
{
///spi1_mutex_lock();
if(i_num>MAX_NUM_IZM)
	i_num=MAX_NUM_IZM;
if(i_num!=0)
	i_num--;
wr_dat_trk(ADDR_NUM_IZM,i_num);
///spi1_mutex_unlock();

}
void set_max_tact(uint8_t i_tact)
{
///spi1_mutex_lock(); 
if(i_tact>MAX_TACT)
	i_tact=MAX_TACT;
if(i_tact!=0)
	i_tact--;
wr_dat_trk(ADDR_MAX_TACT,i_tact);
///spi1_mutex_unlock(); 

}

void set_num_summ(uint8_t i_izm)
{
///spi1_mutex_lock();
wr_dat_trk(ADDR_NUM_SUMM,i_izm);
///spi1_mutex_unlock();
}
#if 0
void set_max_izm(uint8_t i_izm)
{
///spi1_mutex_lock(); 
wr_dat_trk(ADDR_MAX_IZM,i_izm);
///spi1_mutex_unlock(); 
}
#endif
void on_trk_irq(uint8_t i_onoff)
{
uint16_t tmp;
tmp= rd_dat_trk(ADDR_UG_CONF);
if(i_onoff&0x1)
	tmp|= B_ON_IRQ;
else
	tmp&= ~B_ON_IRQ;
wr_dat_trk(ADDR_UG_CONF,tmp);
}
///======================================
void psk_trk(void)
{
wr_dat_trk(ADDR_UG_PSK,0x1);
}
void clr_ug_irq(void)
{
wr_dat_trk(ADDR_UG_CLR_IRQ,0x1);
}

uint8_t check_ready_dat(void)
{
uint8_t rez=0;
uint16_t tmp;
tmp= rd_dat_trk(ADDR_UG_UPR);
///tmp= rd_dat_addr_m(ADDR_RD_C0);
///ADDR_RD_C0
if(tmp&0x1)
	{
	ready_dat=0;
	rez=0;
	}
else
	{
	ready_dat=1;
	rez=1;
	}
return rez;
}
void tst_pin(void)
{
#if 0
SPI1_R_W_GPIO->BSRRH= SPI1_R_W_PIN;
uDelay(1);
SPI1_R_W_GPIO->BSRRL= SPI1_R_W_PIN;
uDelay(1);
SPI1_A_D_GPIO->BSRRH= SPI1_A_D_PIN;
uDelay(1);
SPI1_A_D_GPIO->BSRRL= SPI1_A_D_PIN;
uDelay(1);
#endif
}

void trk_irq(void *par)
{
///rt_interrupt_enter();
////EXTI->PR|=(0x1<<1); ///
////EXTI->PR|=(0x1<<UTRK_IRQ_NPIN); ///
ready_dat=1;	
////rt_interrupt_leave();
	
}
////extern platform_result_t platform_gpio_irq_enable( const platform_gpio_t* gpio, platform_gpio_irq_trigger_t trigger, platform_gpio_irq_callback_t handler, void* arg );

void init_trk_irq(void)
{
#if 0
platform_gpio_t  t_platform_gpio;
t_platform_gpio.pin_number=UTRK_IRQ_NPIN;
t_platform_gpio.port=UTRK_IRQ_GPIO;
///gpio_irq_enable (GPIOI, UTRK_IRQ_NPIN,IRQ_FALLING_EDGE,trk_irq, NULL);
platform_gpio_irq_enable (&t_platform_gpio,IRQ_TRIGGER_FALLING_EDGE,trk_irq, NULL);
#endif
}
int utrk_init(void)
{
////int ret = RT_EOK;
int ret = 0;
int ii;
////hw_spi1_set_br(DEF_UTRK_SPI_BR);

////wr_dat_addr_m(ADDR_CONF_ADC,NORM_OP);
///return ret;


set_spd_spi_pow(DEF_SPD_PSPI);
////set_pow_vol(DEF_POW_VOL);
set_adc_clk();

///set_max_tact(DEF_MAX_TACT);
set_max_tact(DEF_NUM_TACT);   ///for otlad
set_num_summ(DEF_NUM_SUMM);
set_num_izm(DEF_NUM_IZM);
#if 0
///===== for otlad ====================
for(ii=0;ii<MAX_TACT+1;ii++)
{
set_on_gens_tact(ii,0x1<<ii);
curr_par_trk.takts[ii].len_hipow=DEF_LEN_HIPOW;
set_len_hipow_tact(ii,curr_par_trk.takts[ii].len_hipow);
curr_par_trk.takts[ii].len_tact=DEF_LEN_TACT;
set_len_per_tact(ii,curr_par_trk.takts[ii].len_tact);
}
#endif
init_def_par_trk();
put_all_par_trk();

///=========================================
init_trk_irq();
on_trk_irq(0x1);
///=========================================
return ret;
}
///======================================================
///#define _SPI_SET_UG_ADDR(x) _spi_set_addr(ADDR_UG_PAR+x+((i_tact&MASK_TACT)<<SH_TACT_UG))
///#define _SPI_SET_UU_ADDR(x) _spi_set_addr(ADDR_UU_PAR+x+((i_tact&MASK_TACT)<<SH_TACT_UU))
///wr_dat_addr_m(ADDR_UU_PAR_CHAN(i_num&MASK_NUS)+OFFS_KUS,i_dat*3);

void put_strobs_tact(uint8_t i_tact,uint8_t inus,uint8_t nstrob)
{
////printk( KERN_INFO "\n=== [mod]put_strobs_tact====[%x:%x:%x]\n",i_takt,nus,nstrob);
///spi1_mutex_lock();
////set_utrk_spi_br();
switch(nstrob)
	{
	case 0:
		wr_dat_trk(ADDR_UU_PAR_CHAN(inus&MASK_NUS,i_tact)+OFFS_POR0
				,(uint16_t)(curr_par_trk.takts[i_tact].uss[inus].strb[0].por));
		wr_dat_trk(ADDR_UU_PAR_CHAN(inus&MASK_NUS,i_tact)+OFFS_BSTR0
				,(uint16_t)(curr_par_trk.takts[i_tact].uss[inus].strb[0].beg));
		wr_dat_trk(ADDR_UU_PAR_CHAN(inus&MASK_NUS,i_tact)+OFFS_LSTR0
				,(uint16_t)(curr_par_trk.takts[i_tact].uss[inus].strb[0].len));
		break;
	case 1:
		wr_dat_trk(ADDR_UU_PAR_CHAN(inus&MASK_NUS,i_tact)+OFFS_POR1
		,(uint16_t)(curr_par_trk.takts[i_tact].uss[inus].strb[1].por));
		wr_dat_trk(ADDR_UU_PAR_CHAN(inus&MASK_NUS,i_tact)+OFFS_BSTR1
		,(uint16_t)(curr_par_trk.takts[i_tact].uss[inus].strb[1].beg));
		wr_dat_trk(ADDR_UU_PAR_CHAN(inus&MASK_NUS,i_tact)+OFFS_LSTR1
		,(uint16_t)(curr_par_trk.takts[i_tact].uss[inus].strb[1].len));
		break;
	case 2:
		wr_dat_trk(ADDR_UU_PAR_CHAN(inus&MASK_NUS,i_tact)+OFFS_POR2
		,(uint16_t)(curr_par_trk.takts[i_tact].uss[inus].strb[2].por));
		wr_dat_trk(ADDR_UU_PAR_CHAN(inus&MASK_NUS,i_tact)+OFFS_BSTR2
		,(uint16_t)(curr_par_trk.takts[i_tact].uss[inus].strb[2].beg));
		wr_dat_trk(ADDR_UU_PAR_CHAN(inus&MASK_NUS,i_tact)+OFFS_LSTR2
		,(uint16_t)(curr_par_trk.takts[i_tact].uss[inus].strb[2].len));
		break;
	case 3:
		wr_dat_trk(ADDR_UU_PAR_CHAN(inus&MASK_NUS,i_tact)+OFFS_POR3
		,(uint16_t)(curr_par_trk.takts[i_tact].uss[inus].strb[3].por));
		wr_dat_trk(ADDR_UU_PAR_CHAN(inus&MASK_NUS,i_tact)+OFFS_BSTR3
		,(uint16_t)(curr_par_trk.takts[i_tact].uss[inus].strb[3].beg));
		wr_dat_trk(ADDR_UU_PAR_CHAN(inus&MASK_NUS,i_tact)+OFFS_LSTR3
		,(uint16_t)(curr_par_trk.takts[i_tact].uss[inus].strb[3].len));
		break;
	default:
		break;
	}
///spi1_mutex_unlock();

}

///======================================================
///======================================================
uint16_t rd_dat_trk(uint16_t addr)
{
uint16_t rez;
uint16_t *udef_addr=(uint16_t*)XPAR_M_AXI_GP0_BASEADDR;
///spi1_mutex_lock();
rez= *(udef_addr+addr);
///printf("\n\r rd_dat_addr_m[%x:%x]",rez,addr);
///spi1_mutex_unlock();
return rez;
}
void wr_dat_trk(uint16_t addr,uint16_t dat)
{
volatile uint16_t *udef_addr=(uint16_t*)XPAR_M_AXI_GP0_BASEADDR;
*(udef_addr+addr)=dat;

}
///======================================================
uint32_t rd_dat32_trk(uint16_t addr)
{
uint32_t rez;
uint16_t tmp;
uint16_t *udef_addr=(uint16_t*)XPAR_M_AXI_GP0_BASEADDR;

///spi1_mutex_lock();
tmp= *(udef_addr+addr);
rez= *(udef_addr+addr+2);
rez<<=16;
rez|=tmp;
///spi1_mutex_unlock();
return rez;
}

void rd_dat_blok_trk(uint16_t addr,uint16_t len,uint16_t *obuf)
{
uint16_t ii=0;
uint16_t t_dat;
uint16_t *udef_addr=(uint16_t*)XPAR_M_AXI_GP0_BASEADDR;
///spi1_mutex_lock();
////len=64;
for(ii=0;ii<len;ii++)
	{
	t_dat=*(udef_addr+addr+ii);
	*(obuf+ii)=t_dat;
	}

///spi1_mutex_unlock();
}
extern alt_dat_req_t g_alt_dat;

void rd_alt_dat(void)
{
if(g_alt_dat.nbytes>MAX_ALT_DAT_LEN)
	g_alt_dat.nbytes=MAX_ALT_DAT_LEN;
rd_dat_blok_trk(g_alt_dat.addr,g_alt_dat.nbytes,g_alt_dat.data)	;
/// 		g_alt_dat.data[0]=rd_dat_trk(g_alt_dat.addr);

}

void rd_osc_(uint16_t addr_osc,uint16_t len,uint8_t *obuf)
{
uint16_t ii;
uint16_t *t_buf=(uint16_t*)obuf;
uint16_t t_dat;
uint16_t *udef_addr=(uint16_t*)XPAR_M_AXI_GP0_BASEADDR;
///spi1_mutex_lock();
for(ii=0;ii<(len/2);ii++)
	{
	t_dat=*(udef_addr+addr_osc+ii);
	*(t_buf+ii)=t_dat;
	}
///spi1_mutex_unlock();
}

void wr_dat32_trk(uint16_t addr,uint32_t dat)
{
uint16_t *udef_addr=(uint16_t*)XPAR_M_AXI_GP0_BASEADDR;
///	 spi1_mutex_lock();
*(udef_addr+addr)=dat&0xffff;
*(udef_addr+addr+2)=(dat>>16)&0xffff;
///spi1_mutex_unlock();
}
void wr_dat_blok_trk(uint16_t addr,uint16_t len,uint16_t *ibuf)
{
uint16_t ii;
uint16_t *udef_addr=(uint16_t*)XPAR_M_AXI_GP0_BASEADDR;
///spi1_mutex_lock();
for(ii=0;ii<len;ii++)
	{
	*(udef_addr+addr+ii) = *(ibuf+ii);
	}
///spi1_mutex_unlock(); 
}

