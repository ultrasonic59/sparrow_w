#include "pc_udp.h"
#include "device_cmd.h"

static quint8 CurRej=0;

void set_cur_rej(quint8 rej)
{
	CurRej=rej;
}
quint8 get_cur_rej(void)
{
return CurRej;
}

pc_udp::pc_udp(int *pport,QString &_ip_addr, quint8* o_data
	          , bool* end_cmd, quint32* g_changed_param,quint8* last_cmd_good, udp_stat_t* udp_stat)
	: 
	pUdpSendRes(nullptr), attached(false)
	,ip_addr(_ip_addr)
	,p_odata(o_data)
	, p_end_cmd(end_cmd)
	, pg_changed_param(g_changed_param)
	, p_last_cmd_good(last_cmd_good)
	,p_udp_stat(udp_stat)
	,req_cnt(0)
	,m_changed_param(0)
///	,m_cmd(0)
////	,m_cmd_val(0)
	,initing(false)
	, stop_def_rej(false)
///	,prev_unsuccesfull_conn(-1)
////	,unsuccesfull_conn(0)
	,timeout_resive(DEF_TIMEOUT_RSV)///,	//	// в мс
	,par_trk_buff( new quint8[ OFFS_PARAM_DAT+sizeof(par_contr_t)+OFFS_FLG_CHNG ] )
	,frames_sent(0)

////	p_port(pport),ip_addr(_ip_addr)
{
pUdpSendRes= new udp_send_res(p_udp_stat);
p_udp_send_res_thr = new QThread();

pUdpSendRes->moveToThread(p_udp_send_res_thr);

connect(this, SIGNAL(s_send_udp_dat(char*, int)), pUdpSendRes, SLOT(slot_send_udp_dat(char*, int)));
connect(pUdpSendRes, SIGNAL(s_res_dat()), this, SLOT(slot_res_udp_dat()));
connect(this, SIGNAL(s_set_host(QString, int)), pUdpSendRes, SLOT(slot_set_host(QString, int)));
connect(this, SIGNAL(s_Start(bool)), pUdpSendRes, SLOT(sl_Start(bool)));

p_udp_send_res_thr->start();

}

pc_udp::~pc_udp()
{
}
void pc_udp::udp_pc_init(void)
{
	/*
p_udp_stat->in_buf = (quint8*) malloc(MAX_FRAME_LEN);
p_udp_stat->snd_buff	= (quint8*)malloc(MAX_FRAME_LEN);
p_udp_stat->t_snd_buff1	= (quint8*)malloc(MAX_FRAME_LEN);
p_udp_stat->t_snd_buff2	= (quint8*)malloc(MAX_FRAME_LEN);
*/
}


bool pc_udp::dev_get_cur_rej(quint8 *btmp)
{
	return send_and_wait_get(CMD_REQ_DAT, NUM_REQ_REJ, btmp);
}
bool pc_udp::dev_set_cur_rej(quint8 rej)
{
	qDebug() << "Rej: " << rej;

return send_and_wait_put(CMD_PUT_DAT, NUM_SET_REJ, &rej,1);

}

void pc_udp::sl_dev_set_cur_rej(quint8 rej)
{
	dev_set_cur_rej(rej);
}

bool pc_udp::dev_put_param(quint8 *iparam, quint16 num_bytes)
{
	return _send_and_wait(CMD_PUT_DAT, REQ_PUT_PAR, reinterpret_cast<quint8 *>(iparam), num_bytes);
}

bool pc_udp::dev_get_version_str(quint8 *btmp)
{
	return send_and_wait_get(CMD_REQ_DAT, NUM_REQ_GET_VERS, btmp);
}

bool pc_udp::_send_and_wait(const quint8 type, const quint8 cmd, quint8 *in_dat
	, quint16 in_len, quint8 *out_dat, quint16 *out_len)
{
quint16 olen =0;
quint8 btmp;
///if (!attached)
///   return false;
olen =udp_send_to(type,cmd,in_dat,in_len);
el_timer.start();

p_udp_stat->reseive_wait = true;
emit s_send_udp_dat((char*)p_udp_stat->snd_buff,olen);

while(p_udp_stat->reseive_wait)
	{
	int	time_count = el_timer.elapsed();

	//qDebug() << "Receive wait time: " << time_count << "milliseconds";
	if(time_count>=timeout_resive)
		{
		qDebug() << "Receive timeout!!!!: " << time_count << "milliseconds";
		emit s_set_conn_stat(false);
		*p_end_cmd = true;
		*p_last_cmd_good = 0;
		return false;
		}
	}
if(out_dat)
	{
	memcpy(out_dat, p_udp_stat->in_buf+sizeof(hdlc_head_t), p_udp_stat->len_dat_bytes);
	}

if(out_len != nullptr)
	*out_len = p_udp_stat->len_dat_bytes;
emit s_set_conn_stat(true);
*p_end_cmd = true;
*p_last_cmd_good = p_udp_stat->ans_cmd;

return true;
}
/*
void pc_udp::sl_set_out_a(void* buf)
{
	p_odat = buf;
}
*/
bool pc_udp::send_and_wait_get(const quint8 type, const quint8 cmd, quint8 *out_dat,quint16 *res_len)
{
return _send_and_wait(type, cmd, nullptr, 0, out_dat, res_len);	// 
}
bool pc_udp::send_and_wait_put(const quint8 type, const quint8 cmd, quint8* in_dat, quint16 len)
{
	return _send_and_wait(type, cmd, in_dat, len, nullptr, 0);	// 
}

///==================================
quint16 pc_udp::udp_set_frame(quint8 *data, int len)
{
addfcs16((char*)data,len);				///
memcpy(p_udp_stat->snd_buff,data,len +2);
 
return len +2;
}
int g_flg_req_dat=0;
quint8 dev_state=0;

quint16	pc_udp::pc_get_req_dat(quint8 num_req,quint8 *buf)
{
quint16 rez=0;
switch(num_req)
	{
	case NUM_REQ_STAT:
		*buf=dev_state;
		rez=1;
		break;
	case NUM_REQ_REJ:
		*buf=get_cur_rej();
		rez=1;
		break;
	case NUM_REQ_XIL_DAT:
		break;

	break;
	case NUM_REQ_GET_VERS:
		{
		char t_buf[256];
	////	rez=get_vers(t_buf);
	////	memcpy(buf, &rez,sizeof(quint16));
	////	memcpy(buf+sizeof(quint16), t_buf,rez);
		rez+=sizeof(quint16);
		}
		break;
	default:
		rez=0;
		break;
	}
return rez;
}
quint16	pc_udp::udp_send_req_to(quint8 ipar)
{
////static quint16 tst_offs=0;
quint8 p_tmp[8];
p_udp_stat->req_cmd=ipar;
///ResDat.req_cmd=ipar;
p_tmp[0]  = CMD_REQ_DAT;
p_tmp[1] = ipar;
p_tmp[2] = 0;///offs
p_tmp[3] = 0;///offs
return udp_set_frame(p_tmp, 4);
}


quint16	pc_udp::udp_send_ack_to(quint8 ipar)
{
quint8 p_tmp[8];
p_tmp[0]  = CMD_ACK;
p_tmp[1] = ipar;
return udp_set_frame(p_tmp, 2);
}

#if 1
int	pc_udp::udp_send_to(quint8 type_send,quint8 type_dat,quint8 *buff,quint16 len)
{
int res=0;
quint16 len_dat=0;
int num_send=0;
quint8 *p_tmp1;
quint8 *p_tmp = p_udp_stat->t_snd_buff1;
if(type_send == CMD_ACK)
	{
	res=udp_send_ack_to(type_dat);
	}
else if(type_send == CMD_REQ_DAT)
	{

//	qDebug() << "CMD_REQ_DAT - Value of type_dat: " << type_dat;
	res=udp_send_req_to(type_dat);
	}
else if(type_send == CMD_PUT_REQ_DAT)
	{
	*p_tmp++  = CMD_PUT_REQ_DAT;
	num_send++;
	*p_tmp++  = type_dat;
	num_send++;
	p_tmp1 = p_tmp+2;  

	// Debugging the value of type_dat
//qDebug() << "Value of type_dat: " << type_dat;

// Debugging the value of p_tmp1
///qDebug() << "Value of p_tmp1: " <<(*p_tmp1);

	len_dat = pc_get_req_dat(type_dat,p_tmp1);

//qDebug() << "Value of len_dat: " << len_dat;
	
	*p_tmp++  = len_dat&0xff;
	num_send++;
	*p_tmp++  = (len_dat>>8)&0xff;
	num_send++;
	if(len_dat > HDLC1_MAX_FRAME_LEN)
		len_dat = HDLC1_MAX_FRAME_LEN;
	num_send += len_dat;
	res=udp_set_frame(p_udp_stat->t_snd_buff1, num_send);
	}
else if(type_send == CMD_PUT_DAT)
	{
	*p_tmp++  = CMD_PUT_DAT;
	num_send++;
	*p_tmp++  = type_dat;
	num_send++;
	if(len > HDLC1_MAX_FRAME_LEN)
		len = HDLC1_MAX_FRAME_LEN;
	p_tmp1 = p_tmp+2;  
	memcpy(p_tmp1,buff,len);
	*p_tmp++  = len&0xff;
	num_send++;
	*p_tmp++  = (len>>8)&0xff;
	num_send++;
	num_send += len;
	///qDebug() << "Value of p_tmp1: " <<(*p_tmp1);
	////if(buff)
	///qDebug() << "buff: " <<*buff;

	res=udp_set_frame(p_udp_stat->t_snd_buff1, num_send);
	}

return res;
}
#endif
/*
void pc_udp::SlotResetConnCount()
{
unsuccesfull_conn = 0;
prev_unsuccesfull_conn=-1;
}
*/
///========================================================================================
///========================================================================================
/*
void pc_udp::slot_put_motor_cmd_stop()
{
motor_struct_t motor_struct;
motor_struct.dir = 0;
motor_struct.speed = 0;
_send_and_wait(CMD_PUT_DAT, CMD_MOTOR_GO, reinterpret_cast<quint8 *>(&motor_struct), sizeof(motor_struct_t));
}
*/
void pc_udp::slot_res_udp_dat()
{
////	reseive_wait=false;
qDebug() << " slot_res_udp_dat " ;
////udp_parse_framein(ResDat.buff,ResDat.len);
}

void pc_udp::SlotSetIp(QString _ip, int iport)
{
emit s_set_host(_ip, iport);
}
/*
void pc_udp::sl_set_run(bool irun)
{
	socket_run = irun;
}
*/

bool pc_udp::apply_params()
{
quint8 curr_mode = 0;
bool result = dev_get_cur_rej(&curr_mode);
#if 0
if(result )
	{
	if(curr_mode==REJ_MENU)
		{
		result = dev_set_cur_rej(REJ_TUNE);			// 
		}
		Params::g_changed_param = CHNG_ALL;
		result = send_par_to_dev();/// send_all_par_to_dev();
///		if(result )
////			Params::g_changed_param=0;
	}
#endif
return result;
}
bool pc_udp::send_par_to_dev()
{
bool rez=false;
if (*pg_changed_param) {
	    m_changed_param=*pg_changed_param;
		*pg_changed_param = 0;
	    }
else 
	{
	///	m_changed_param = *pg_changed_param & (CHNG_CHAN | CHNG_TACT | CHNG_NUS
	///		| CHNG_BEGR | CHNG_ENDR | CHNG_NSTRB
	///		| CHNG_NUMPV | CHNG_NGEN | CHNG_NPOW
	///		| CHNG_REJ_OSC | CHNG_ALL);
		*pg_changed_param = 0;

	}
	if (m_changed_param)
	{
	///	memcpy(&m_param, &p_dev_data->curr_par_session.par_dev.par_contr, sizeof(par_contr_t));
///		qDebug() << "rej_osc=" << p_dev_data->curr_par_session.par_dev.par_contr.osc_par.rej_osc << "size=" << sizeof(par_contr_t);
///		qDebug() << "par_trk_t=" << sizeof(par_trk_t) << "cn_info_t=" << sizeof(cn_info_t) << "par_osc_t=" << sizeof(par_osc_t);

	///====================================
///	Params::g_changed_param = 0;
///	if (m_changed_param)
///	{
		if (send_param())
		{
			m_changed_param = 0;
			rez = true;
		}
		else
			rez = false;
	}
///}
////set_changes(Params::g_changed_param);

/////return dev_cmd.dev_put_param(cmd_buffer.get(), cmd_buffer.get_len());
return rez;
}
bool pc_udp::send_all_par_to_dev()
{
bool rez = false;
m_changed_param = *pg_changed_param;
///memcpy(&m_param, &p_dev_data->curr_par_session.par_dev.par_contr, sizeof(par_contr_t));
if (send_param())
   {
	m_changed_param = 0;
	rez = true;
   }
else
 rez = false;

return rez;
}
///===========================================================
#define _mCurrUs	m_param.trk_par.tacts[m_param.osc_par.ntact].uss[m_param.osc_par.ninp/NUM_INPUTS]
#define _mCurrNgen	m_param.trk_par.tacts[m_param.osc_par.ntact].gens[m_param.curr_gen]
#define _mCurrPow	m_param.trk_par.pow_gen[m_param.curr_pow]
///===========================================================
bool pc_udp::send_param()
{
quint32 t_changed_param=0;
quint16 tnum_bytes=0;
quint16 t_offs=0;
if(m_changed_param&CHNG_ALL)
	m_changed_param = CHNG_ALL;
////qDebug() << "rej_osc" << m_param.osc_par.rej_osc<<"par_contr_t" << sizeof(par_contr_t);

if(m_changed_param & CHNG_ALL)
    {
	t_changed_param=m_changed_param;
////	memcpy(par_trk_buff+OFFS_FLG_CHNG, &m_changed_param, sizeof(quint32));
	t_offs=OFFS_PARAM_DAT;
	tnum_bytes=OFFS_PARAM_DAT;
///	memcpy(par_trk_buff+t_offs, &m_param, sizeof(par_contr_t));
	t_offs+=sizeof(par_contr_t);
	tnum_bytes+=sizeof(par_contr_t);
////	qDebug() << "par_contr_t=" <<sizeof(par_contr_t)<<"par_trk_t="<< sizeof(par_trk_t)<<"par_tact_t="<< sizeof(par_tact_t)<<"par_osc_t="<< sizeof(par_osc_t)
///		);
/*
	for (int ii = 0; ii < 8; ii++)
	{
		quint8 t_nus = m_param.cn_info[ii].ninp / NUM_INPUTS;
		qDebug() << "rej us ch" << ii << "tact = " << m_param.cn_info[ii].n_tact << " us = " << t_nus << "rej=" << QString::number(m_param.trk_par.tacts[m_param.cn_info[ii].n_tact].uss[t_nus].us_Rej, 16);
	}
	*/
/*
	qDebug() << "per0" << m_param.trk_par.tacts[0].len_tact <<"per1"<< m_param.trk_par.tacts[1].len_tact
		<<"per2"<< m_param.trk_par.tacts[2].len_tact<< "per3"<< m_param.trk_par.tacts[3].len_tact;
	qDebug() << "hi0" << m_param.trk_par.tacts[0].len_hipow <<"hi1"<< m_param.trk_par.tacts[1].len_hipow
		<<"hi2"<< m_param.trk_par.tacts[2].len_hipow<< "hi3"<< m_param.trk_par.tacts[3].len_hipow;
	qDebug() << "mx_a0" << m_param.trk_par.tacts[0].uss[0].us_input <<"mx_a1"<< m_param.trk_par.tacts[1].uss[0].us_input
		<<"mx_a2"<< m_param.trk_par.tacts[2].uss[0].us_input<< "mx_a3"<< m_param.trk_par.tacts[3].uss[0].us_input;
	qDebug() << "mx_b0" << m_param.trk_par.tacts[0].uss[1].us_input <<"mx_b1"<< m_param.trk_par.tacts[1].uss[1].us_input
		<<"mx_b2"<< m_param.trk_par.tacts[2].uss[1].us_input<< "mx_b3"<< m_param.trk_par.tacts[3].uss[1].us_input;
*/
	}
else
	{
	t_offs=OFFS_PARAM_DAT;
	tnum_bytes=OFFS_PARAM_DAT;
#if 0
	if(m_changed_param&CHNG_TACT)
		{
		memcpy(par_trk_buff+t_offs,&m_param.osc_par.ntact, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_TACT;
		m_changed_param&=~CHNG_TACT;
		}
	if(m_changed_param&CHNG_NUS)
		{
	////	memcpy(par_trk_buff+t_offs, &m_param.osc_par.n_us, sizeof(quint8));
	////	t_offs+=sizeof(quint8);
	////	tnum_bytes+=sizeof(quint8);

		memcpy(par_trk_buff+t_offs, &m_param.osc_par.ninp, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_NUS;
		m_changed_param&=~CHNG_NUS;
		}
	if(m_changed_param&CHNG_ONUS)
		{
		memcpy(par_trk_buff+t_offs,&_mCurrUs.on_us, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8); 
		t_changed_param|= CHNG_ONUS;
		m_changed_param&=~CHNG_ONUS;
		}
	if(m_changed_param&CHNG_KUS)
		{
		memcpy(par_trk_buff+t_offs, &_mCurrUs.kus, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_KUS;
		m_changed_param&=~CHNG_KUS;
		}
	if(m_changed_param&CHNG_BEGR)
		{
		memcpy(par_trk_buff+t_offs, &m_param.osc_par.curr_beg_r, sizeof(quint16));
		t_offs+=sizeof(quint16);
		tnum_bytes+=sizeof(quint16);
		t_changed_param|= CHNG_BEGR;
		m_changed_param&=~CHNG_BEGR;
		}
	if(m_changed_param&CHNG_ENDR)
		{
		quint8 t_stp;
		memcpy(par_trk_buff+t_offs, &m_param.osc_par.curr_end_r, sizeof(quint16));
		t_offs+=sizeof(quint16);
		tnum_bytes+=sizeof(quint16);
		memcpy(par_trk_buff+t_offs, &m_param.osc_par.curr_stp_r, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_ENDR;
		m_changed_param&=~CHNG_ENDR;
		}
	if(m_changed_param&CHNG_TT)
		{
		memcpy(par_trk_buff+t_offs, &_mCurrUs.taut, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_TT;
		m_changed_param&=~CHNG_TT;
		}
	if(m_changed_param&CHNG_TV)
		{
		memcpy(par_trk_buff+t_offs, &_mCurrUs.tauv, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_TV;
		m_changed_param&=~CHNG_TV;
		}
	if(m_changed_param&CHNG_NSTRB)
		{
		memcpy(par_trk_buff+t_offs, &m_param.osc_par.curr_strb, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_NSTRB;
		m_changed_param&=~CHNG_NSTRB;
		}
	if(m_changed_param&CHNG_BSTRB)
		{
		memcpy(par_trk_buff+t_offs, &_mCurrUs.strb[m_param.osc_par.curr_strb].beg, sizeof(quint16));
		t_offs+=sizeof(quint16);
		tnum_bytes+=sizeof(quint16);
		t_changed_param|= CHNG_BSTRB;
		m_changed_param&=~CHNG_BSTRB;
		}
	if(m_changed_param&CHNG_LSTRB)
		{
		memcpy(par_trk_buff+t_offs, &_mCurrUs.strb[m_param.osc_par.curr_strb].len, sizeof(quint16));
		t_offs+=sizeof(quint16);
		tnum_bytes+=sizeof(quint16);
		t_changed_param|= CHNG_LSTRB;
		m_changed_param&=~CHNG_LSTRB;
		}
	if(m_changed_param&CHNG_POR)
		{
		memcpy(par_trk_buff+t_offs, &_mCurrUs.strb[m_param.osc_par.curr_strb].por, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_POR;
		m_changed_param&=~CHNG_POR;
		}
	if(m_changed_param&CHNG_NUMPV)
		{
		memcpy(par_trk_buff+t_offs, &m_param.osc_par.curr_pvrch, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_NUMPV;
		m_changed_param&=~CHNG_NUMPV;
		}
	if(m_changed_param&CHNG_VRON)
		{
		memcpy(par_trk_buff+t_offs, &_mCurrUs.vrch.on_vrch, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_VRON;
		m_changed_param&=~CHNG_VRON;
		}
	if(m_changed_param&CHNG_VTIME)
		{
		memcpy(par_trk_buff+t_offs, &_mCurrUs.vrch.time[m_param.osc_par.curr_pvrch], sizeof(quint16));
		t_offs+=sizeof(quint16);
		tnum_bytes+=sizeof(quint16);
		t_changed_param|= CHNG_VTIME;
		m_changed_param&=~CHNG_VTIME;
		}
	if(m_changed_param&CHNG_VKUS)
		{
		memcpy(par_trk_buff+t_offs, &_mCurrUs.vrch.kus[m_param.osc_par.curr_pvrch], sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_VKUS;
		m_changed_param&=~CHNG_VKUS;
		}
	if(m_changed_param&CHNG_NGEN)
		{
		memcpy(par_trk_buff+t_offs, &m_param.curr_gen, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_NGEN;
		m_changed_param&=~CHNG_NGEN;
		}
	if(m_changed_param&CHNG_ONGEN)
		{

		memcpy(par_trk_buff+t_offs, &_mCurrNgen.on_gen, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_ONGEN;
		m_changed_param&=~CHNG_ONGEN;
		}
	if(m_changed_param&CHNG_LIMP)
		{
		memcpy(par_trk_buff+t_offs, &_mCurrNgen.len_imp, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_LIMP;
		m_changed_param&=~CHNG_LIMP;
		}
	if(m_changed_param&CHNG_NPOW)
		{
		memcpy(par_trk_buff+t_offs, &m_param.curr_pow, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_NPOW;
		m_changed_param&=~CHNG_NPOW;
		}
	if(m_changed_param&CHNG_PVAL)
		{
		memcpy(par_trk_buff+t_offs, &_mCurrPow.val, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_PVAL;
		m_changed_param&=~CHNG_PVAL;
		}
	if(m_changed_param&CHNG_US_REJ)
		{
		memcpy(par_trk_buff+t_offs, &_mCurrUs.us_Rej, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		memcpy(par_trk_buff+t_offs, &_mCurrUs.detpar, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_US_REJ;
		m_changed_param&=~CHNG_US_REJ;
		}
	if(m_changed_param&CHNG_REJ_OSC)
		{
		memcpy(par_trk_buff+t_offs, &m_param.osc_par.rej_osc, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		t_changed_param|= CHNG_REJ_OSC;
		m_changed_param&=~CHNG_REJ_OSC;
		}
#endif
	}
memcpy(par_trk_buff, &tnum_bytes, sizeof(quint16));
memcpy(par_trk_buff+OFFS_FLG_CHNG, &t_changed_param, sizeof(quint32));
t_changed_param=0;
return dev_put_param(par_trk_buff,tnum_bytes);
}
void pc_udp::sl_set_run(bool irun)
{
	socket_run = irun;
}
void pc_udp::obr_cmd(req_cmd_t&req_cmd)
{
	bool res;

	switch (req_cmd.cmd) {
	case NUM_SEND_XIL_DAT:
		dev_put_xil(req_cmd.req);
		break;
	case NUM_SEND_XIL_REQ:
		dev_put_req_xil(req_cmd.req);
		break;
	case NUM_REQ_XIL_DAT:
		dev_get_xil();
		break;
	case NUM_REQ_STAT:
		dev_get_stat();
		break;
	}

}

void pc_udp::req_timer_timeout()
{
	if (!socket_run)
		return;
	req_cmd_t t_req_cmd;
	send_par_to_dev();
	int cnt = req_que.count();
	if (cnt)
	{
		///		qDebug() << "cnt= "<<cnt;
		t_req_cmd = req_que.dequeue();
		obr_cmd(t_req_cmd);
	}

#if 0
//	qDebug() << "                             ++++++++++++++                                ";
//	qDebug() << "------------------------Called req_timer_timeout-------------------------" ;
//	el_timer_2.start();
if (!socket_run)
		return;
if(initing)
	{
	if(apply_params())
		{
		initing = false;
		emit signal_connect_setted();
		}
	}
else
	{
	req_cmd_t t_req_cmd;
	send_par_to_dev();
	int cnt = req_que.count();
///	qDebug() << "cnt= " << cnt;
	if (cnt)
       {
///		qDebug() << "cnt= "<<cnt;
		t_req_cmd = req_que.dequeue();
		obr_cmd(t_req_cmd);
	   }
	bool get_result = true;
	if(*p_tune_mode | stop_def_rej)
		{
		++osc_counter;
		if(osc_counter > max_osc_counter)
			{
			osc_counter = 0;
				/////???if((p_dev_data->curr_par_session.ad_osc_par.mode == OFF) && get_result)
					{
				quint8 t_osc_buf[DEF_LEN_OSC*2];
				get_result = dev_get_osc0(t_osc_buf);
				if(get_result )
					{
					memcpy(p_dev_data->osc_data.buffOsc.osc, t_osc_buf, sizeof(quint8)*DEF_LEN_OSC);
				    emit sign_ready_data(READY_OSC0);
					}
					}
			}
		}
///===========================================================================
		quint16 res_length = 0;
	////	quint8 t_data_buff[MAX_FRAME_LEN];
	////	get_result = dev_cmd.dev_get_amps_stat( t_data_buff, &res_length );			// 
	////	if(get_result)
			{
////			amp_stat_buff_t *amp_stat_buff = reinterpret_cast<amp_stat_buff_t *>(p_dev_data->data_buff);
			////	cur_cnt++;
			amp_stat_buff_t *amp_stat_buff = &p_dev_data->buff_struct;
			get_result = get_amp_stat_dat(amp_stat_buff, &res_length);
			if(get_result)
				{
				emit sign_ready_data(READY_AMPS);
				}
			else
				{
				qDebug() << "[!get_result]" ;
				}
		}
///===========================================================================
	}

//qDebug() << "-----------------------End fuc req_timer, elapsed time:" << el_timer_2.elapsed() << "-------------------";
#endif
}
void pc_udp::SlotSetIniting(bool ini)
{
initing=ini;
}
void pc_udp::timerEvent(QTimerEvent* event)
{
	req_timer_timeout();
////	qDebug() << "Timer ID:" << event->timerId();

}
void pc_udp::start_timer(quint32 dt)
{
	if (dt == 0) {
		///	req_timer->stop();
		killTimer(timer_id);
		qDebug() << "start_timer[0]" << timer_id;
	}
	else {
		timer_id = startTimer(dt);
		qDebug() << "start_timer[dt]" << timer_id;
	}
	////  req_timer->start(dt);


  ///	emit s_Start(start_stop);
}
void pc_udp::sl_start(bool start_stop)
{
	emit s_Start(start_stop);
}



void pc_udp::sl_start_timer(quint32 dt)
{
	start_timer(dt);
	qDebug() << " sl_start_timer " << dt;
#if 0
	if (dt == 0)
		///	req_timer->stop();
		killTimer(1);
	else
		startTimer(50);
	////  req_timer->start(dt);


  ///	emit s_Start(start_stop);
#endif
}
///=========================================================================================================
void pc_udp::dev_get_stat(void)
{
#if 1
	bool rez;
	if (p_odata)
	{
		xil_dat_req_t* odat = (xil_dat_req_t*)p_odata;
		///		rez = send_and_wait_get(CMD_REQ_DAT, NUM_REQ_XIL_DAT, reinterpret_cast<quint8*>(odat));
		rez = send_and_wait_get(CMD_REQ_DAT, NUM_REQ_STAT, reinterpret_cast<quint8*>(p_odata));
		if (rez)
		{
			;
			///	p_dev_data->odata
			////		emit s_ready_xil_dat(odat);
		///		qDebug() << " dev_get_xil " << odat->addr << odat->data[0];
		}
		///		qDebug() << " dev_get_xil " << odat->addr << odat.data[0];
	}
#endif
}

void pc_udp::dev_put_req_xil(xil_dat_req_t ireq)
{
	xil_dat_req_t t_req = ireq;
	_send_and_wait(CMD_PUT_DAT, NUM_SEND_XIL_REQ, reinterpret_cast<quint8*>(&t_req), sizeof(xil_dat_req_t));
////	qDebug() << " dev_put_req_xil ";
}

void pc_udp::dev_get_xil(void)
{
#if 1
	bool rez;
	if (p_odata)
	{
		xil_dat_req_t *odat= (xil_dat_req_t*)p_odata;
///		rez = send_and_wait_get(CMD_REQ_DAT, NUM_REQ_XIL_DAT, reinterpret_cast<quint8*>(odat));
		rez = send_and_wait_get(CMD_REQ_DAT, NUM_REQ_XIL_DAT, reinterpret_cast<quint8*>(p_odata));
		if (rez)
		{
			;
		///	p_dev_data->odata
		////		emit s_ready_xil_dat(odat);
	///		qDebug() << " dev_get_xil " << odat->addr << odat->data[0];
		}
///		qDebug() << " dev_get_xil " << odat->addr << odat.data[0];
	}
#endif
}
void pc_udp::dev_put_xil(xil_dat_req_t ireq)
{
	xil_dat_req_t t_req = ireq;
	_send_and_wait(CMD_PUT_DAT, NUM_SEND_XIL_DAT, reinterpret_cast<quint8*>(&t_req), sizeof(xil_dat_req_t));
}

void pc_udp::sl_put_xil(xil_dat_req_t ireq)
{
	req_cmd_t  t_req;
	t_req.cmd = NUM_SEND_XIL_DAT;
	t_req.req = ireq;
	req_que.enqueue(t_req);

	////xil_dat_req_t t_req = ireq;

	/// _send_and_wait(CMD_PUT_DAT, NUM_SEND_XIL_DAT, reinterpret_cast<quint8 *>(&t_req), sizeof(xil_dat_req_t));
}

void pc_udp::sl_put_req_xil(xil_dat_req_t ireq)
{
	req_cmd_t  t_req;
	t_req.cmd = NUM_SEND_XIL_REQ;
	t_req.req = ireq;
	req_que.enqueue(t_req);
////	qDebug() << " sl_put_req_xil ";
////	int cnt = req_que.count();
////	qDebug() << "cnt0= " << cnt;

	///	xil_dat_req_t t_req = ireq;
	///	_send_and_wait(CMD_PUT_DAT, NUM_SEND_XIL_REQ, reinterpret_cast<quint8*>(&t_req), sizeof(xil_dat_req_t));
}

void pc_udp::sl_get_xil(void)
{
	req_cmd_t  t_req;
	t_req.cmd = NUM_REQ_XIL_DAT;
	///	t_req.req = ireq;
	req_que.enqueue(t_req);
///	qDebug() << " sl_get_xil ";
////	int cnt = req_que.count();
////	qDebug() << "cnt2= " << cnt;

	///	bool rez;
		///	xil_dat_req_t t_req = ireq;
////	xil_dat_req_t odat;

	///	rez = send_and_wait_get(CMD_REQ_DAT, NUM_REQ_XIL_DAT, reinterpret_cast<quint8*>(&odat));
	///	if (rez)
	///		emit s_ready_xil_dat(odat);
}
void pc_udp::sl_set_stop_def_rej(bool on) {
	stop_def_rej = on;
}

void pc_udp::sl_put_dac(dac_spi_req_t ireq)
{
	req_cmd_t  t_req;
	t_req.cmd = NUM_SEND_SPI_DAC_DAT;
	t_req.req.addr = ireq.addr;
	t_req.req.nbytes = 2;
	t_req.req.data[0]= ireq.data&0xff;
	t_req.req.data[1] = (ireq.data>>8) & 0xff;

	req_que.enqueue(t_req);

	////xil_dat_req_t t_req = ireq;

	/// _send_and_wait(CMD_PUT_DAT, NUM_SEND_XIL_DAT, reinterpret_cast<quint8 *>(&t_req), sizeof(xil_dat_req_t));
}
void pc_udp::sl_put_req_dac(dac_spi_req_t ireq) {
	req_cmd_t  t_req;
	t_req.cmd = NUM_REQ_SPI_DAC_DAT;
	t_req.req.addr = ireq.addr;
	t_req.req.nbytes = 2;
	t_req.req.data[0] = ireq.data & 0xff;
	t_req.req.data[1] = (ireq.data >> 8) & 0xff;

	req_que.enqueue(t_req);

}
void pc_udp::sl_get_dac(void)
{
	req_cmd_t  t_req;
	t_req.cmd = NUM_SEND_SPI_DAC_REQ;
	req_que.enqueue(t_req);
}
void pc_udp::sl_get_stat(void)
{
	req_cmd_t  t_req;
	t_req.cmd = NUM_REQ_STAT;
	req_que.enqueue(t_req);
}
