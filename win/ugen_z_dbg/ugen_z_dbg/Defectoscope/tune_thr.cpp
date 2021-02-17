#include "ugen_z_dbg.h"
#include "tune_thr.h"
#include "params.h"

//extern CRITICAL_SECTION critsect;
extern amp_tim_t m_amp_tims[];
///extern quint8 g_curr_type_def;
extern RespondAuto::types g_curr_type_def;
extern bool en_set_device_params;
RespondAuto::types conv_def[]={RespondAuto::NoControl,RespondAuto::NoContact,RespondAuto::NoDefect,RespondAuto::Defect};
/*
		NoDefect = 1,
		NoContact = 2,
		Defect = 3
*/
///par_contr_t curr_par_contr;
par_trk_t curr_par_trk;


quint32 g_req_send_dat=0;
////par_trk_t curr_param;
#define LEN_OSC_2	128
quint8	calc_step(quint16 beg_r,quint16 end_r)
{
quint16 tmp;
if(end_r<=beg_r)
	return 1;
tmp=end_r-beg_r;
//tmp*=T_STP;
if(tmp<LEN_OSC_2*2)
	return 1;
return tmp/LEN_OSC_2 - 1;
};

quint16	calc_endr(quint16 beg_r,quint8 step)
{
return beg_r + (step+1)*LEN_OSC_2;
};


int calc_val_pow_Index(unsigned char val)
{
	if(val < 30)
		return 3;
	else if(val < 100)
		return 2;
	else if(val < 197)
		return 1;
	else
		return 0;
}

unsigned char calc_val_pow(int Index)
{
	unsigned char ret;
	switch(Index)
	{
	case 3:
		ret = 3;
		break;
	case 2:
		ret = 60;
		break;
	case 1:
		ret = 140;
		break;
	default:
		ret = 255;
	}
	return ret;
}


///ed_par_trk_t curr_ed_par_trk;

c_tune_thr::c_tune_thr(QWidget *parent)/// :  QThread()
{
int ii;
parent_w=parent;
c_num_osc=0;
///tmp_buff = (quint8 *)malloc(sizeof(par_contr_t)+sizeof(ed_par_trk_t)+4);
tmp_buff = (quint8 *)malloc(sizeof(par_trk_t)+sizeof(ed_par_trk_t)+16);
p_req_timer = new QTimer(this);
connect(p_req_timer, SIGNAL(timeout()), this, SLOT(req_timer_timeout()));

m_changed_param = 0;


////p_req_timer->start(REQ_PERIOD);

///on_thr_obr_kbd=false;
////memset(&pushed_key,0,sizeof(int)*MAX_NUM_PUSHED_KEYS);
///m_row=2;
///m_col=2;
}
void c_tune_thr::on_off_timer(bool on_off)
{
if(on_off)
	p_req_timer->start(REQ_PERIOD);
else
	p_req_timer->stop();

}

c_tune_thr::~c_tune_thr()
{

}
bool c_tune_thr::_draw_osc0()
{
quint32  btmp; 
emit signal_draw_osc0();
return true;
}
bool c_tune_thr::_draw_amps()
{
quint32  btmp; 
emit signal_draw_amps();
return true;
}
///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
#define _mCurrUs m_param.trk_par.takts[m_ed_par_trk.curr_tact].uss[m_ed_par_trk.curr_us]
#define _mCurrNgen m_param.trk_par.takts[m_ed_par_trk.curr_tact].gens[m_ed_par_trk.curr_gen]
#define _mCurrPow m_param.trk_par.pow_gen[m_ed_par_trk.curr_pow]
*/
#define _mCurrUs m_param.takts[m_ed_par_trk.curr_tact].uss[m_ed_par_trk.curr_us]
#define _mCurrNgen m_param.takts[m_ed_par_trk.curr_tact].gens[m_ed_par_trk.curr_gen]
#define _mCurrPow m_param.pow_vol

///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool c_tune_thr::send_param(void)
{
ugen_z_dbg *p_ugen_z_dbg=(ugen_z_dbg *)parent_w;
quint16 tnum_bytes=0;
quint16 t_offs=0;

memcpy(tmp_buff+OFFS_FLG_CHNG,&m_changed_param,sizeof(quint32));
{
QString tstr;
tstr.sprintf("[%x]",m_changed_param);
qDebug()<<tstr;
}
t_offs=OFFS_PARAM_DAT;
tnum_bytes=OFFS_PARAM_DAT;
if(m_changed_param&CHNG_ALL)
{
memcpy(tmp_buff+t_offs,&m_ed_par_trk,sizeof(ed_par_trk_t));
t_offs+=sizeof(ed_par_trk_t);
tnum_bytes+=sizeof(ed_par_trk_t);
///memcpy(tmp_buff+t_offs,&m_param,sizeof(par_contr_t));
memcpy(tmp_buff+t_offs,&m_param,sizeof(par_trk_t));
///t_offs+=sizeof(par_contr_t);
///tnum_bytes+=sizeof(par_contr_t);
t_offs+=sizeof(par_trk_t);
tnum_bytes+=sizeof(par_trk_t);
m_changed_param=0;
}
else
{
if(m_changed_param&CHNG_CHAN)
	{
	memcpy(tmp_buff+t_offs,&m_ed_par_trk.curr_chan,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_CHAN;
	}
#if 0
if(m_changed_param&CHNG_TACT)
	{
	memcpy(tmp_buff+t_offs,&m_ed_par_trk.curr_tact,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_TACT;
	}
#endif
if(m_changed_param&CHNG_NUS)
	{
	memcpy(tmp_buff+t_offs,&m_ed_par_trk.curr_us,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_NUS;
	}
if(m_changed_param&CHNG_ONUS)
	{
	memcpy(tmp_buff+t_offs,&_mCurrUs.on_us,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8); 
	m_changed_param&=~CHNG_ONUS;
	}
if(m_changed_param&CHNG_KUS)
	{
	memcpy(tmp_buff+t_offs,&_mCurrUs.kus1,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_KUS;
	}
#if 0
if(m_changed_param&CHNG_BEGR)
	{
	memcpy(tmp_buff+t_offs,&m_ed_par_trk.curr_begr,sizeof(quint16));
	t_offs+=sizeof(quint16);
	tnum_bytes+=sizeof(quint16);
	m_changed_param&=~CHNG_BEGR;
	}
if(m_changed_param&CHNG_ENDR)
	{
	quint8 t_stp;
	memcpy(tmp_buff+t_offs,&m_ed_par_trk.curr_endr,sizeof(quint16));
	t_offs+=sizeof(quint16);
	tnum_bytes+=sizeof(quint16);
	memcpy(tmp_buff+t_offs,&m_ed_par_trk.curr_step_r,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_ENDR;
	}
if(m_changed_param&CHNG_TT)
	{
	memcpy(tmp_buff+t_offs,&_mCurrUs.taut,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	qDebug()<< "tt"<< *(tmp_buff+6);
	m_changed_param&=~CHNG_TT;
	}
if(m_changed_param&CHNG_TV)
	{
	memcpy(tmp_buff+t_offs,&_mCurrUs.tauv,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_TV;
	}
#endif
if(m_changed_param&CHNG_NSTRB)
	{
	memcpy(tmp_buff+t_offs,&m_ed_par_trk.curr_strob,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_NSTRB;
	}
if(m_changed_param&CHNG_BSTRB)
	{
	memcpy(tmp_buff+t_offs,&_mCurrUs.strb[m_ed_par_trk.curr_strob].beg,sizeof(quint16));
	t_offs+=sizeof(quint16);
	tnum_bytes+=sizeof(quint16);
	m_changed_param&=~CHNG_BSTRB;
	}
if(m_changed_param&CHNG_LSTRB)
	{
	memcpy(tmp_buff+t_offs,&_mCurrUs.strb[m_ed_par_trk.curr_strob].len,sizeof(quint16));
	t_offs+=sizeof(quint16);
	tnum_bytes+=sizeof(quint16);
	m_changed_param&=~CHNG_LSTRB;
	}
if(m_changed_param&CHNG_POR)
	{
	memcpy(tmp_buff+t_offs,&_mCurrUs.strb[m_ed_par_trk.curr_strob].por,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_POR;
	}
#if 0
if(m_changed_param&CHNG_NUMPV)
	{
	memcpy(tmp_buff+t_offs,&m_ed_par_trk.curr_pvrch,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_NUMPV;
	}
if(m_changed_param&CHNG_VRON)
	{
	memcpy(tmp_buff+t_offs,&_mCurrUs.vrch.on_vrch,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_VRON;
	}
if(m_changed_param&CHNG_VTIME)
	{
	memcpy(tmp_buff+t_offs,&_mCurrUs.vrch.time[m_ed_par_trk.curr_pvrch],sizeof(quint16));
	t_offs+=sizeof(quint16);
	tnum_bytes+=sizeof(quint16);
	m_changed_param&=~CHNG_VTIME;
	}
if(m_changed_param&CHNG_VKUS)
	{
	memcpy(tmp_buff+t_offs,&_mCurrUs.vrch.kus[m_ed_par_trk.curr_pvrch],sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_VKUS;
	}
if(m_changed_param&CHNG_NGEN)
	{
	memcpy(tmp_buff+t_offs,&m_ed_par_trk.curr_gen,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_NGEN;
	}
if(m_changed_param&CHNG_ONGEN)
	{
	memcpy(tmp_buff+t_offs,&_mCurrNgen.on_gen,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_ONGEN;
	}
if(m_changed_param&CHNG_LIMP)
	{
	memcpy(tmp_buff+t_offs,&_mCurrNgen.len_imp,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_LIMP;
	}
#endif
#if 0
if(m_changed_param&CHNG_NPOW)
	{
	memcpy(tmp_buff+t_offs,&m_ed_par_trk.curr_pow,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_NPOW;
	}
if(m_changed_param&CHNG_PVAL)
	{
////	memcpy(tmp_buff+t_offs,&_mCurrPow.val,sizeof(quint8));
	memcpy(tmp_buff+t_offs,&_mCurrPow,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_PVAL;
	}
if(m_changed_param&CHNG_REJ_OSC)
	{
	memcpy(tmp_buff+t_offs,&m_ed_par_trk.rej_osc,sizeof(quint8));
	t_offs+=sizeof(quint8);
	tnum_bytes+=sizeof(quint8);
	m_changed_param&=~CHNG_REJ_OSC;
	}
#endif
#if 0
if(m_changed_param&CHNG_IZM)
	{
	memcpy(tmp_buff+t_offs,&m_param.izm_par,sizeof(par_izm_t));
	t_offs+=sizeof(par_izm_t);
	tnum_bytes+=sizeof(par_izm_t);
	m_changed_param&=~CHNG_IZM;
	}
#endif
}
quint16 tnum_bytes1;
tnum_bytes1=tnum_bytes+OFFS_FLG_CHNG;
memcpy(tmp_buff,&tnum_bytes,sizeof(quint16));

p_ugen_z_dbg->p_dev_cmd->udp_put_param(tmp_buff,tnum_bytes);
return true;
}

bool c_tune_thr::obr_req_send_dat(void)
{
		ugen_z_dbg *p_ugen_z_dbg=(ugen_z_dbg *)parent_w;
		if(g_req_send_dat&REQ_XIL_DAT)
		{
		p_ugen_z_dbg->p_dev_cmd->udp_put_req_xil(&g_alt_dat_req);
		g_req_send_dat&=~REQ_XIL_DAT;
		}
		if(g_req_send_dat&GET_XIL_DAT)
		{
		p_ugen_z_dbg->p_dev_cmd->udp_get_xil(&g_alt_dat);
		g_req_send_dat&=~GET_XIL_DAT;
		}
		if(g_req_send_dat&PUT_XIL_DAT)
		{
		g_req_send_dat&=~PUT_XIL_DAT;
		}
		return true;
		}
		#if (DBG_NO_DEV==1)
		void c_tune_thr::get_tst_osc(quint8 *btmp)
		{
		static quint8 t_offs=0;
		int ii;
		for(ii=0;ii<256;ii++)
		{
		btmp[ii]=ii+t_offs;
		}
		t_offs+=7;
		}
		void c_tune_thr::get_tst_amps(amp_tim_t *t_amp_tims)
		{
		static quint8 t_offs_amps=0;
		t_amp_tims[0].amp[0]=t_offs_amps;
		t_amp_tims[0].amp[1]=0xff-t_offs_amps;
		t_offs_amps++;
		t_amp_tims[1].amp[0]=t_offs_amps;
		t_amp_tims[1].amp[1]=0xff-t_offs_amps;
		t_offs_amps++;
		t_amp_tims[2].amp[0]=t_offs_amps;
		t_amp_tims[2].amp[1]=0xff-t_offs_amps;
		t_offs_amps++;
		t_amp_tims[3].amp[0]=t_offs_amps;
		t_amp_tims[3].amp[1]=0xff-t_offs_amps;
		t_offs_amps++;
		t_amp_tims[4].amp[0]=t_offs_amps;
		t_amp_tims[4].amp[1]=0xff-t_offs_amps;
		t_offs_amps++;
		t_amp_tims[5].amp[0]=t_offs_amps;
		t_amp_tims[5].amp[1]=0xff-t_offs_amps;
		t_offs_amps++;
		t_amp_tims[6].amp[0]=t_offs_amps;
		t_amp_tims[6].amp[1]=0xff-t_offs_amps;
		t_offs_amps++;
		t_amp_tims[7].amp[0]=t_offs_amps;
		t_amp_tims[7].amp[1]=0xff-t_offs_amps;

		t_offs_amps++;
		}
		#endif
		amp_tim_t t_amp_tims[NUM_CHANS+1];///amp_tim +type_defcct

void c_tune_thr::req_timer_timeout()
{
	if(on_tune_thr)
	{
		int ii;
		bool get_osc_rez;
		bool get_amps_rez;
		////amp_tim_t t_amp_tims[NUM_CHANS+1];///amp_tim +type_defcct
		#if (DBG_NO_DEV==1)
		get_tst_osc(buffOscs[0]);
		get_osc_rez=_draw_osc0();
		get_tst_amps(t_amp_tims);
		memcpy(m_amp_tims,t_amp_tims,sizeof(amp_tim_t)*NUM_CHANS);
		get_amps_rez=_draw_amps();

		#else
		ugen_z_dbg *p_ugen_z_dbg=(ugen_z_dbg *)parent_w;

		on_off_timer(false);
		p_ugen_z_dbg->p_dev_cmd->udp_get_osc0(buffOscs[0]);
		get_osc_rez=_draw_osc0();
////		p_ugen_z_dbg->p_dev_cmd->udp_get_amps((quint8*)t_amp_tims);
//////		memcpy(m_amp_tims,t_amp_tims,sizeof(amp_tim_t)*NUM_CHANS);
/////		quint8 tdef=t_amp_tims[NUM_CHANS].amp[0];
/////		g_curr_type_def= conv_def[tdef];

		////g_curr_type_def&=0x7;
		get_amps_rez=_draw_amps();
		/*
		EnterCriticalSection(&critsect);
		*/

		if(g_changed_param)
			{
			if(en_set_device_params)
				m_changed_param=g_changed_param;
#if 0
			else
				m_changed_param=g_changed_param & (CHNG_CHAN|CHNG_TACT|CHNG_NUS
						|CHNG_BEGR|CHNG_ENDR|CHNG_NSTRB
						|CHNG_NUMPV|CHNG_NGEN|CHNG_NPOW
						|CHNG_REJ_OSC);

#endif

			memcpy(&m_param,&curr_par_trk,sizeof(par_trk_t));
			memcpy(&m_ed_par_trk,&curr_ed_par_trk,sizeof(ed_par_trk_t));
			g_changed_param=0;
			}
		else
			m_changed_param=0;
		/*
		LeaveCriticalSection(&critsect);
		*/
		if(m_changed_param)
		{
		send_param();
		}
		#endif

		#if 0
		if(g_req_send_dat)
		{
		obr_req_send_dat();
		}
		#endif

	}


	on_off_timer(true);

#if 0

	switch(c_num_osc&0x3)
		{
		case 1:
	////		get_osc_rez=udp_get_amp_tim();
				break;
		default:
		///				get_osc_rez=t_dlg->udp_get_osc0();
						break;
					}
					if(get_osc_rez)
						{
		////				curr_conn_state=TRUE;
						}
					else
						{
		///				curr_conn_state=FALSE;
						}
					c_num_osc++;
					if(c_num_osc>CMAX_NUM_OSC)
						c_num_osc=0;
#endif
}

///===============================================
