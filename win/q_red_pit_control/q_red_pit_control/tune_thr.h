#ifndef _TUNE_THR_H_
#define _TUNE_THR_H_
#include <QtWidgets/QMainWindow>
#include <QThread>
#include <QTimer>
#include "params.h"
#include "dev_interf.h"
#include "dev_cmd.h"

#include <QVector>


class c_tune_thr: public QObject
{
Q_OBJECT

public:
	c_tune_thr(	QObject *parent,
				quint32 *g_changed_param,
				par_contr_t *par_contr,
				QVector<qint16> *imp_ampl);
	virtual ~c_tune_thr();

	bool on_tune_thr;
	quint8 par_trk_buff[ OFFS_PARAM_DAT+sizeof(par_contr_t)+OFFS_FLG_CHNG ];
	quint8 data_buff[MAX_FRAME_LEN];

	c_dev_cmd dev_cmd;

	bool send_params();


	bool is_initing;



protected:


	quint32 *p_changed_param;
	quint32	m_changed_param;

	par_contr_t *p_param;
	par_contr_t m_param;

	QVector<qint16> *p_imp_ampl;



	QTimer req_timer;



	bool send_param();

signals:

	void ConnectionAttempt();
	void ConnectionSetted();

	void signal_draw_all();
	void signal_unsuccesfull();



protected slots:

	void req_timer_timeout();

public slots:

	void on_off_timer(bool on_off);


};
#endif
