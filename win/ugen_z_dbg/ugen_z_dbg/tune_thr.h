#ifndef _TUNE_THR_H_
#define _TUNE_THR_H_
#include <QtWidgets/QMainWindow>
#include <QThread>
#include "params.h"
#include "dev_interf.h"
#include "debugs_par.h"

#define CMAX_NUM_OSC 4
///=====================================================
class c_tune_thr: public QObject////public QThread 
{
 Q_OBJECT;
public:
	c_tune_thr(QWidget *parent = 0);
	~c_tune_thr();
	bool on_tune_thr;
	QWidget *parent_w;

///	 void SetCommNumb(int *comm_num);

 ///protected:
///	 int *comm_number;

////	virtual void	run();

///	QString str_ans;
////	void put_cmd1(QString str_cmd,int val,void_fn_void fn=0);
///	void SetAngle();
///public:
///	void do_work(int type,QString cmd, int val);
////	int pushed_key[MAX_NUM_PUSHED_KEYS];
///public slots:
///	void put_key(char key);
///	void slot_key_changed(int *);
///signals:
////	void req_get_img(QString *,QImage *img);
////	void req_set_dat(QString ,QString *);

////	void put_cmd_req(cmd_req_t);

////	void angleChange(void);
signals:
	void signal_draw_osc0(void);
	void signal_draw_amps(void);
////	void _udp_req_dat_from(quint8,quint16,void*);
private:
	int c_num_osc;
	bool _draw_osc0();
	bool _draw_amps();
	quint32	m_changed_param;
////	par_contr_t m_param;
	par_trk_t m_param;
	ed_par_trk_t m_ed_par_trk;
	quint8 *tmp_buff;
	bool send_param(void);
	bool obr_req_send_dat(void);
	QTimer *p_req_timer;

public slots:
	void req_timer_timeout();
	void on_off_timer(bool on_off);
#if (DBG_NO_DEV==1)
	void get_tst_osc(quint8 *btmp);
	void get_tst_amps(amp_tim_t *t_amp_tims);
#endif

};
#endif
