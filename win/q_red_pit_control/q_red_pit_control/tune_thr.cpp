#include "tune_thr.h"
#include "params.h"


c_tune_thr::c_tune_thr(	
				QObject *parent,
				quint32 *g_changed_param,
				par_contr_t *par_contr,
				QVector<qint16> *imp_ampl
				) :
		QObject(parent),
		on_tune_thr(false),

		dev_cmd(this),

		p_changed_param(g_changed_param),
		m_changed_param(0),
		p_param(par_contr),
		m_param(),

		p_imp_ampl(imp_ampl),

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
		memcpy(par_trk_buff+t_offs, &m_sent_par.Timp_len, sizeof(qint16));
		t_offs+=sizeof(quint16);
		memcpy(par_trk_buff+t_offs, p_imp_ampl->data(), sizeof(qint16)*m_sent_par.Timp_len);
		t_offs+=sizeof(qint16)*m_sent_par.Timp_len;
		tnum_bytes+=sizeof(qint16)*m_sent_par.Timp_len+sizeof(quint16);
		m_changed_param&=~CHNG_IMP_POINTS;
	}
	if(m_changed_param&CHNG_PRESC)
	{
		memcpy(par_trk_buff+t_offs, &m_sent_par.pre, sizeof(quint16));
		t_offs+=sizeof(quint16);
		tnum_bytes+=sizeof(quint16);
		m_changed_param&=~CHNG_PRESC;
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

/*			if(dev_cmd.dev_get_osc( data_buff ))			// ???
			{
				memcpy(p_ehogram_struct, data_buff, sizeof(ehogram_struct_t));
				emit signal_draw_all();
			}
			else if(on_tune_thr)
				emit signal_unsuccesfull();
*/

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

	m_changed_param = CHNG_TIMP_LEN | CHNG_TIMP_OFFSET | CHNG_TCYCLE| CHNG_PRESC | CHNG_TDEF | CHNG_IMP_POINTS;
	result = send_param();

	return result;
}

