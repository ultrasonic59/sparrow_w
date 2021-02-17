#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "octopuz.h"
///#include "rook_rtos.h"
///#include "printk.h"


///#include "pc_wifi.h"
////#include "list_param.h"
#include "defectoscope.h"
#include "my_misc.h"
///#include "ff.h"
///#include "vers.h"
///#include "drv_flash.h"
////#include "_hdlc.h"
#include "udp_cmd.h"


extern uint8_t g_camera_on;
extern uint8_t contr_connected;
extern uint8_t *tmp_rez_buff1;
extern uint8_t *tmp_rez_buff2;
extern uint8_t *tmp_rez_buff3;

extern uint8_t flg_req_osc;
extern uint8_t *tmp_param_buff;
extern uint8_t buf_rx[];
///extern bt_rez_dat_strobs_t bt_strobs_dat;
///extern bt_rez_dat_strobs_t bt_strobs_dat_a;
extern uint8_t tst_osc_rez[];
///===================================
extern uint8_t prev_rej;
extern uint8_t cur_rej;

alt_dat_req_t g_alt_dat;
///alt_dat_req_t g_stm_dat;

///wifi_stat_t g_UDP_stat;
///wifi_stat_t g_http_stat;
///=================================================

#define VERS          "octopuz_1.0"

///static uint8_t err_file=0;
///char g_tmp_name[MAX_LEN_FILE_NAME];
///===========================================
////uint8_t test_buff[256];
uint8_t tst_offs=0;
#if 0
uint16_t rd_dat_udb(uint16_t addr)
{
uint16_t rez;
if(addr>MAX_KUS_DB+1)
  addr=MAX_KUS_DB+1;
rez=tbl_kus_db[addr];
return rez;
}
void wr_dat_udb(uint16_t addr,uint16_t dat)
{
if(addr>MAX_KUS_DB+1)
  addr=MAX_KUS_DB+1;
tbl_kus_db[addr] = (uint8_t)dat&0xff;
}
uint16_t rd_dat_upow(uint16_t addr)
{
uint16_t rez;
if(addr>NUM_POW+1)
  addr=NUM_POW+1;
rez=tbl_pow[addr];
return rez;
}
void wr_dat_upow(uint16_t addr,uint16_t dat)
{
if(addr>NUM_POW+1)
  addr=NUM_POW+1;
tbl_pow[addr] = (uint8_t)dat&0xff;
}
#endif
///======================================================
#if 1
void	GenerateTestDat(uint8_t *buff,int len)
{
int ii;
for(ii=0;ii<len;ii++)
{
buff[ii]=ii+tst_offs;
}
tst_offs+=11;
}
#endif

///========================================
void set_osc_obuf(uint8_t *obuf,uint16_t len_osc)
{
uint16_t ii;
///uint16_t jj;
///uint16_t tdat;

for(ii=0;ii<len_osc;ii++)
{
///  tdat=0;
///  for(jj=0;jj<NUM_SUMM_OSC;jj++)
///    {
///    tdat+=osc_rez[ii+(jj<<8)];
///    }
///  *(obuf+ii)=(uint8_t)(tdat/NUM_SUMM_OSC);
  *(obuf+ii)=(uint8_t)osc_rez[ii];
}
///memcpy(obuf, osc_rez,len_osc);
}

#if 0
uint16_t   get_dev_id(uint8_t *buf)
{
uint16_t rez;
FRESULT frez;
UINT bytes;
FIL t_fil;
frez=f_open (&t_fil, (const TCHAR*)TMP_NAME,FA_READ);
if(frez!=FR_OK)
  {
  return 0;
  }
frez=f_read(&t_fil,(void*)buf,DEF_LEN_ID,&bytes);
if((frez!=FR_OK)||(bytes!=DEF_LEN_ID))
  rez= 0;
else
  rez= DEF_LEN_ID;
f_close(&t_fil);
return rez;  
}
#endif
///======================================================
///=================================================
extern void set_rec_param(uint8_t* ibuf);
////extern uint16_t put_dat_to_msp(uint8_t *ibuf,uint16_t len);
extern void rd_alt_dat(void);

///=================================================
int pc_set_rec_dat(uint8_t cmd,void *in_buf)
{
uint16_t n_bytes;
uint8_t t_byte;
uint16_t num_bytes;
uint8_t *bbuf=(uint8_t*)in_buf;
int rez=0;
////return 0;
switch(cmd)
	{
	case NUM_SET_REJ:
		prev_rej=cur_rej;
		t_byte = *((uint8_t*)(in_buf)+2);
////	if(t_byte==REJ_MENU)
		cur_rej = t_byte;
////	else if(cur_rej==REJ_MENU)
////		cur_rej = t_byte;

	///???+++++++++++++++++++++++++++++++++++++
/////        fprintf(stderr,"\n cur_rej =%x\n",cur_rej);
	///???+++++++++++++++++++++++++++++++++++++
		break;
	 case NUM_WR_N_PAR_CMD:
		set_rec_param(in_buf);
		break;
#if 0
	case NUM_SET_CNGEN:		/// 
		curr_ed_par_trk.curr_gen = *((uint8_t*)(in_buf)+2);
		break;	
	case NUM_SET_CNUS:			/// 
		curr_ed_par_trk.curr_us = *((uint8_t*)(in_buf)+2);
///==========================================
		set_osc0_us_view(curr_ed_par_trk.curr_us);
///==========================================
		break;	
#endif
#if 0
	case REQ_SET_CUR_NUM:
            {
            uint16_t num_par;
            memcpy(&num_par,bbuf+2,sizeof(uint16_t));
            cur_num_par=num_par;
            save_sys_par();
            load_par_num(num_par);
 ////           flg_need_save_par=1;
            put_all_par_trk();
          }
		break;
#endif
	case NUM_SEND_XIL_DAT:
#if 1
		{
		alt_dat_req_t t_alt_dat;
		num_bytes=(*(bbuf+0))|(*(bbuf+1)<<8);
		n_bytes=MIN(num_bytes,sizeof(alt_dat_req_t));
		memcpy(&t_alt_dat,bbuf+2,n_bytes);
 		wr_dat_trk(t_alt_dat.addr ,t_alt_dat.data[0]);
 		_printk("\n\r NUM_SEND_XIL_DAT[%x:%x]",t_alt_dat.addr,t_alt_dat.data[0]);

/// 		wr_dat_trk(t_alt_dat.addr ,t_alt_dat.data[0]);

		}
#endif
		break;
	case NUM_SEND_XIL_REQ:
#if 1
		{
		num_bytes=(*(bbuf+0))|(*(bbuf+1)<<8);
		n_bytes=MIN(num_bytes,sizeof(alt_dat_req_t));
		memcpy(&g_alt_dat,bbuf+2,n_bytes);
		rd_alt_dat();
///		g_alt_dat.data[0]=rd_dat_trk(g_alt_dat.addr);
 		printf("\n\r NUM_SEND_XIL_REQ[%x:%x]",g_alt_dat.addr,g_alt_dat.data[0]);

////		g_alt_dat.data[0]=rd_dat_trk(g_alt_dat.addr);
		}
#endif
		break;
	case NUM_SEND_UDB_DAT:
#if 0
		{
		alt_dat_req_t t_alt_dat;
		num_bytes=(*(bbuf+0))|(*(bbuf+1)<<8);
		n_bytes=MIN(num_bytes,sizeof(alt_dat_req_t));
		memcpy(&t_alt_dat,bbuf+2,n_bytes);
 		wr_dat_udb(t_alt_dat.addr ,t_alt_dat.data[0]);
		}
#endif
		break;
	case NUM_SEND_UDB_REQ:
#if 0
		{
		num_bytes=(*(bbuf+0))|(*(bbuf+1)<<8);
		n_bytes=MIN(num_bytes,sizeof(alt_dat_req_t));
		memcpy(&g_alt_dat,bbuf+2,n_bytes);
 		g_alt_dat.data[0]=rd_dat_udb(g_alt_dat.addr);
		}
#endif
		break;
	case NUM_SEND_UPOW_DAT:
#if 0
		{
		alt_dat_req_t t_alt_dat;
		num_bytes=(*(bbuf+0))|(*(bbuf+1)<<8);
		n_bytes=MIN(num_bytes,sizeof(alt_dat_req_t));
		memcpy(&t_alt_dat,bbuf+2,n_bytes);
 		wr_dat_upow(t_alt_dat.addr ,t_alt_dat.data[0]);
		}
#endif
		break;
	case NUM_SEND_UPOW_REQ:
#if 0
		{
		num_bytes=(*(bbuf+0))|(*(bbuf+1)<<8);
		n_bytes=MIN(num_bytes,sizeof(alt_dat_req_t));
		memcpy(&g_alt_dat,bbuf+2,n_bytes);
 		g_alt_dat.data[0]=rd_dat_upow(g_alt_dat.addr);
		}
#endif
		break;
 	}
return rez;
}
int32_t get_curr_coord(void)
{
return 0;///curr_coord;
}

uint8_t  get_curr_conc(void)
{
return 0;///curr_conc;
}
extern us_dat_rez_t cur_uss_data[];

int set_tst_amp_tim(uint8_t *buf)
{
	int uu;
	int tt;
	int ss;
static	int t_coord=0;
int rez;
amp_buff_head_t t_amp_buff_head;
amp_respond_t t_amp_respond;
t_amp_buff_head.count=1;
t_amp_buff_head.staus=0xa5;
for(tt=0;tt<NUM_TACT_REZ;tt++)
	{
	for(uu=0;uu<NUM_USS_REZ;uu++)
	{
		for(ss=0;ss<NUM_STRB_REZ;ss++)
		{
			t_amp_respond.coord=t_coord;
/*
			t_amp_respond.ampl.ampl_tact[tt].ampl_us[uu].ampl_str[ss].ampl=(ss+1)+((uu+1)<<4)+((tt+1)<<6);
			t_amp_respond.ampl.ampl_tact[tt].ampl_us[uu].ampl_str[ss].time=(ss+1)+((uu+1)<<8)+((tt+1)<<12);
			*/

			t_amp_respond.ampl.ampl_tact[tt].ampl_us[uu].ampl_str[ss].ampl=
					cur_uss_data[uu].us_dat_tact[tt].one_izm[0].amps[ss];
			t_amp_respond.ampl.ampl_tact[tt].ampl_us[uu].ampl_str[ss].time=
					cur_uss_data[uu].us_dat_tact[tt].one_izm[0].tims[ss];

		}
	}
	}
memcpy(buf, &t_amp_buff_head,sizeof(amp_buff_head_t));
rez=sizeof(amp_buff_head_t);
memcpy(buf+rez, &t_amp_respond,sizeof(amp_respond_t));

t_coord++;
return sizeof(amp_respond_t)+ sizeof(amp_buff_head_t);
}
///==================================================
int set_req_rez_buf(uint8_t *buf)
{
int len=0;
len= set_tst_amp_tim(buf);
////int ii;
	/*
static	uint32_t test_coord=0;
int cnt=0;
static int prev_cnt=0;
///int offs;
rez_head_t *p_rez_head=(rez_head_t*)buf;
p_rez_head->data_rdy=((g_camera_on&0x1)<<2)|((contr_connected&0x1)<<1);
p_rez_head->conceviks=get_curr_conc();
////p_rez_head->coord=test_coord;
p_rez_head->coord=get_curr_coord();
len=sizeof(rez_head_t);
test_coord++;
cnt=cb_get_count(&cb_rezult);
if(cnt>0)
	{
	p_rez_head->data_rdy|=0x1;
	if(prev_cnt!=cnt)
		{
		prev_cnt=cnt;
///		fprintf(stderr,"\n=====set_req_rez_buf[%d]=====",cnt);
///		fprintf(stderr,"=====data_rdy[%x:%x]=====",p_rez_head->data_rdy,*buf);
		}
	cb_pop(&cb_rezult, tmp_rez_buff2);
	}
memcpy(buf+len,tmp_rez_buff2,CUR_LEN_CHAN_DAT);
len+=CUR_LEN_CHAN_DAT;
*/
return len;
}
///==========================================================
///=====================================================
uint16_t	pc_get_req_dat(uint8_t num_req,uint16_t offs,uint8_t *buf)
{
uint16_t rez=0;
switch(num_req)
	{
	case NUM_REQ_REJ:
		*buf=cur_rej;
		rez=1;
		break;
	case NUM_REQ_GET_AMPS:
			rez=set_req_rez_buf(buf);
///			printf("\nNUM_REQ_GET_AMPS[%d]\n",rez);
			break;

	case NUM_REQ_GET_OSC0:
////		GenerateTestDat(osc_rez, DEF_LEN_OSC);
		set_osc_obuf(buf, DEF_LEN_OSC);
///=============================================================
////		GenerateTestDat(buf, DEF_LEN_OSC);
///=============================================================
		flg_req_osc=1;
		rez=DEF_LEN_OSC;
		break;
	case NUM_REQ_XIL_DAT:

   ///             _printk("x");
		memcpy(buf,&g_alt_dat,sizeof(alt_dat_req_t));
		rez=sizeof(alt_dat_req_t);
		break;
 	case NUM_REQ_UDB_DAT:
#if 0
 ///               _printk("x");
		memcpy(buf,&g_alt_dat,sizeof(alt_dat_req_t));
		rez=sizeof(alt_dat_req_t);
#endif
		rez=0;
		break;
 	case NUM_REQ_UPOW_DAT:
#if 0
 		memcpy(buf,&g_alt_dat,sizeof(alt_dat_req_t));
		rez=sizeof(alt_dat_req_t);
#endif
		rez=0;
		break;
	default:
 ////         _printk("z[%x]!",num_req);
          rez=0;
          break;

	}
return rez;
}

///==============================================


