#include "tune_thr.h"
#include "params.h"


c_tune_thr::c_tune_thr(	
				QObject *parent,
				quint32 *g_changed_param,
				par_contr_t *par_contr,
				QVector<qint16> *imp_ampl,
				osc_struct_t *osc_struct,
				quint16 *ampl_val
				) :
		QObject(parent),
		on_tune_thr(false),

		par_trk_buff( new quint8[ OFFS_PARAM_DAT+sizeof(par_contr_t)+OFFS_FLG_CHNG ] ),
		data_buff( new quint8 [MAX_FRAME_LEN] ),

		dev_cmd(this),

		p_changed_param(g_changed_param),
		m_changed_param(0),
		p_param(par_contr),
		m_param(),

		p_imp_ampl(imp_ampl),
		p_osc_struct(osc_struct),
		p_ampl_val(ampl_val),

		req_timer(this)
{
	memset(data_buff, 0, MAX_FRAME_LEN * sizeof(quint8));
	connect(&req_timer, SIGNAL(timeout()), this, SLOT(req_timer_timeout()));
}



void c_tune_thr::on_off_timer(bool on_off)
{
	if(on_off)
		req_timer.start(dev_cmd.GetRequestPeriod());
	else
		req_timer.stop();
}

c_tune_thr::~c_tune_thr()
{
	p_imp_ampl = nullptr;

	delete[] par_trk_buff;
	delete[] data_buff;
}

bool c_tune_thr::send_param()
{
	quint16 tnum_bytes=0;
	quint16 t_offs=0;




	memcpy(par_trk_buff+OFFS_FLG_CHNG, &m_changed_param, sizeof(quint32));
	t_offs=OFFS_PARAM_DAT;
	tnum_bytes=OFFS_PARAM_DAT;

		
	par_sent_t &m_sent_par = m_param.sent_par;


	if(m_changed_param&CHNG_TIMP_LEN)
	{
		memcpy(par_trk_buff+t_offs, &m_sent_par.Timp_len, sizeof(quint16));
		t_offs+=sizeof(quint16);
		tnum_bytes+=sizeof(quint16);
		m_changed_param&=~CHNG_TIMP_LEN;
	}
	if(m_changed_param&CHNG_TIMP_OFFSET)
	{
		memcpy(par_trk_buff+t_offs, &m_sent_par.Timp_offset, sizeof(quint16));
		t_offs+=sizeof(quint16);
		tnum_bytes+=sizeof(quint16);
		m_changed_param&=~CHNG_TIMP_OFFSET;
	}
	if(m_changed_param&CHNG_TCYCLE)
	{
		memcpy(par_trk_buff+t_offs, &m_sent_par.Tcycle, sizeof(quint16));
		t_offs+=sizeof(quint16);
		tnum_bytes+=sizeof(quint16);
		m_changed_param&=~CHNG_TCYCLE;
	}
	if(m_changed_param&CHNG_TDEF)
	{
		memcpy(par_trk_buff+t_offs, &m_sent_par.Tdef, sizeof(quint16));
		t_offs+=sizeof(quint16);
		tnum_bytes+=sizeof(quint16);
		m_changed_param&=~CHNG_TDEF;
	}
	if(m_changed_param&CHNG_IMP_POINTS)
	{
		memcpy(par_trk_buff+t_offs, &m_sent_par.Timp_len, sizeof(quint16));
		t_offs+=sizeof(quint16);
		tnum_bytes+=sizeof(quint16);

		memcpy(par_trk_buff+t_offs, p_imp_ampl->data(), sizeof(qint16)*m_sent_par.Timp_len);
		t_offs+=sizeof(qint16)*m_sent_par.Timp_len;
		tnum_bytes+=sizeof(qint16)*m_sent_par.Timp_len;
		m_changed_param&=~CHNG_IMP_POINTS;
	}
	if(m_changed_param&CHNG_KUS)
	{
		memcpy(par_trk_buff+t_offs, &m_sent_par.kus, sizeof(quint16));
		t_offs+=sizeof(quint16);
		tnum_bytes+=sizeof(quint16);
		m_changed_param&=~CHNG_KUS;
	}
	if(m_changed_param&CHNG_BEG_OSC)
	{
		memcpy(par_trk_buff+t_offs, &m_sent_par.beg_osc, sizeof(quint16));
		t_offs+=sizeof(quint16);
		tnum_bytes+=sizeof(quint16);
		m_changed_param&=~CHNG_BEG_OSC;
	}
	if(m_changed_param&CHNG_STEP_OSC)
	{
		memcpy(par_trk_buff+t_offs, &m_sent_par.step_osc, sizeof(quint16));
		t_offs+=sizeof(quint16);
		tnum_bytes+=sizeof(quint16);
		m_changed_param&=~CHNG_STEP_OSC;
	}
	if(m_changed_param&CHNG_ATTENUATOR)
	{
		memcpy(par_trk_buff+t_offs, &m_sent_par.attenuator, sizeof(quint8));
		t_offs+=sizeof(quint8);
		tnum_bytes+=sizeof(quint8);
		m_changed_param&=~CHNG_ATTENUATOR;
	}


	quint16 tnum_bytes1;
	tnum_bytes1=tnum_bytes+OFFS_FLG_CHNG;
	memcpy(par_trk_buff,&tnum_bytes,sizeof(quint16));


	return dev_cmd.dev_put_param(par_trk_buff,tnum_bytes);
}



void c_tune_thr::req_timer_timeout()
{
	if(on_tune_thr)
	{
		req_timer.stop();


		if(is_initing)
		{
			if(send_params())
			{
				is_initing = false;

				thread()->msleep(100);

				if(on_tune_thr)
					emit ConnectionSetted();
			}
			else if(on_tune_thr)
				emit ConnectionAttempt();
		}
		else
		{



			if(dev_cmd.dev_get_osc( data_buff ))
			{
				memcpy(p_osc_struct, data_buff, sizeof(osc_struct_t));
				emit signal_draw_osc();
			}
			else if(on_tune_thr)
				emit signal_unsuccesfull();



			if(dev_cmd.dev_get_ampl(p_ampl_val))
				emit signal_new_ampl();
			else if(on_tune_thr)
				emit signal_unsuccesfull();





			if(*p_changed_param && on_tune_thr)	// обновление передаваемых настроек
			{
				m_changed_param |= *p_changed_param;
				memcpy(&m_param, p_param, sizeof(par_contr_t));
				*p_changed_param = 0;
			}

			if(m_changed_param && on_tune_thr)
			{
				if(send_param())
					m_changed_param=0;
				else if(on_tune_thr)
					emit signal_unsuccesfull();
			}
		}



		if(on_tune_thr)
			req_timer.start(dev_cmd.GetRequestPeriod());
	}
}



bool c_tune_thr::send_params()
{
	bool result = true;

	m_changed_param = CHNG_TIMP_LEN | CHNG_TIMP_OFFSET | CHNG_TCYCLE | CHNG_TDEF | CHNG_IMP_POINTS | CHNG_BEG_OSC | CHNG_STEP_OSC | CHNG_KUS | CHNG_ATTENUATOR;
	result = send_param();

	return result;
}

