#ifndef Q_WIN_TEMPO_H
#define Q_WIN_TEMPO_H

#include <QCloseEvent>
#include <QtWidgets/QtWidgets>
#include <QPluginLoader>

#include <QtWidgets/QMainWindow>
///#include <QtNetwork/QUdpSocket>
#include <QSettings>
#include <QColor>
#include "ui_ugen_z_dbg.h"
#include "OscillogramPlotter.h"
#include "AutoAmplPlotter.h"
#include "RespondAutoCollector.h"
#include "dial_par.h"
#include "params.h"
#include "dev_interf.h"
#include "udp_cmd.h"
#include "tune_thr.h"
#include "dev_cmd.h"
#include "ViewProperties.h"
#include "dial_debug.h"

#include "TwoDimBuffer.h"
#include "SpectrPlotter.h"

//extern HANDLE ev_rsv_ack;
//extern HANDLE ev_rsv_dat;

#define NUS_BG_COLOR "rgb(250, 250, 250)"
#define NUS_ON_COLOR "rgb(0, 255, 0)"
#define NUS_OFF_COLOR "rgb(0, 0, 0)"

#define NGEN_BG_COLOR "rgb(250, 250, 250)"
#define NGEN_ON_COLOR "rgb(250, 0, 0)"
#define NGEN_OFF_COLOR "rgb(0, 0, 0)"
#define DEF_PORT 2000



class ugen_z_dbg : public QMainWindow
{
	Q_OBJECT

public:
	ugen_z_dbg(QWidget *parent = 0);
	~ugen_z_dbg();

	Ui::ugen_z_dbgUI ui_osc;

private:

	Respond old_resp;
	Respond new_resp;							// заполнять осциллограммой
	RespondTrans RespTrans;

	OscillogramProperties OscProperties;
	OscillogramPlotter OscPlotter;

	RespondAuto auto_resp;						// заполнять амплитудами каналов

	bool resetAutoArrReq;
	RespondAutoCollector auto_collector;

	RespondFrame auto_frame;
	int auto_frame_len_old;
	int auto_frame_len_new;
	AutoAmplPlotter AutoPlotter;


	PlotterWidget *SpectrWidget;
	SpectrPlotter SpectrPlotter;


	ViewProperties *viewProp;

	quint8 *t_buff;

	bool NoConnect;

	int curr_unsuccesfull_conn;

	void UpdateOscPlotter();
	void ProcessAutoRespond();
	void UpdateAutoPlotter();

	void SetConnStatus();

	inline void CheckAndSetStrob(int strob);

signals:
	void bind(void);
	void signal_on_off_timer(bool );
	void put_alt_dat_dial(alt_dat_req_t*);
	void OpenMutex();

private slots:
	void slot_draw_osc0(void);
	void slot_draw_amps(void);



///	void BegOscIntermedChange(int beg);
///	void BegOscFinalChange();
///	void EndOscIntermedChange(int end);
///	void EndOscFinalChange();
	void SetStrobIntermedPos(int strob, int begin, int porog);
	void SetStrobFinalPos();
	void SetStrobIntermedLeft(int strob, int begin, int len);
	void SetStrobFinalLeft();
	void SetStrobIntermedRight(int strob, int len);
	void SetStrobFinalRight();


public slots:
	void NewWindowParameters();
	void CloseWindowParameters();
	void ResetPlot();
	void on_test_butt();
	void on_butt_par();
	void EndInitConnection();
	void on_butt_rd_par_dev();
	void on_butt_wr_par_dev();

private:
	DialPar *dial_par;

private:
	QString ip_addr;
	void saveSettings(void);
	void loadSettings(void);
	QString ParamDirName;
	int port;

private:

	quint8 spectr_first_el;
	quint16 spectr_length;

	inline void calculate_spectr_len();

	TwoDimBuffer<double, quint16> buffer_forSpectr;

	double spectr_arr[1024];

	quint8 spectr_buff_beg;
	quint16 spectr_buff_len;


	quint8 spectr_constant;



///	ed_par_trk_t curr_ed_par_trk;
///    QUdpSocket *socket;

private slots:
 ///   void socketConnected();
 ///   void socketError(QAbstractSocket::SocketError);
 ///   void socketStateChanged(QAbstractSocket::SocketState);
////	void udp_ready_dat();
	void slot_us_on_off();
	void slot_gen_on_off();

	void changeColors();


	void req_timer_timeout();
	void on_butt_save_par();
	void on_butt_load_par();
	void rej_changed(int);
	void	chan_changed();
	void	tact_changed();
	void	nus_changed();
	void	kus_changed();
	void	pre_changed();
	void	rng_changed();
	void	len_def_changed();

////	void	endr_changed();
///	void	endr_result_changed();

	void NoConnection();
	void ResetConnection();

///	void	tt_changed();
////	void	tv_changed();
	void	nstrob_changed();
	void	bstrob_changed();
	void	lstrob_changed();
	void	por_changed();
	void	numpv_changed();
	void	vron_changed(int);
	void	vtime_changed();
	void	vkus_changed();
	void	ngen_changed();
	void	leni_changed();
	void	npow_changed(int);
	void	vpow_changed(int);


	void	spectr_bstrob_changed();
	void	spectr_lstrob_changed();

////	void setDeviceParams(bool);
	void setFlagHttp(bool );



	void setSpectrOn(bool);

private:
	void init_all_ed_params();
	void set_chan_ed_params(void);
	void set_tact_ed_params(quint8 num_tact);
	void set_us_ed_params(quint8 num_us);
	void set_gen_ed_params(quint8 num_gen);
	void set_pow_ed_params(quint8 num_pow);
	void set_strob_ed_params(quint8 num_us);
	void set_vrch_ed_params(void);

	bool check_tact_chan(quint8 num_chan);
	bool check_ngen_tact();
	void show_on_gens(void);
	void show_on_uss(void);

	bool load_params(void);
	bool save_params(void);
	bool load_from_file_trk_params(int num);
	bool save_to_file_trk_params(int num);

	void dev_param_edit(bool);
private:
	bool on_ed_nus;
	bool on_ed_ntact;


	bool spectr_mode;


////	void draw_all_osc_parm(void);
	void show_all_par(void);

protected:
///	virtual void	keyPressEvent(QKeyEvent * e);

public:
////	bool udp_req_dat_from(quint8 icmd,quint16 offs,void *obuf);
////	bool udp_send_dat(quint8 icmd,quint8 *idat,quint16 len);
	c_udp_cmd *p_udp_cmd;
	c_dev_cmd *p_dev_cmd;
	c_tune_thr *p_tune_thr;
private:
	DialDebug *dial_dbg;
private slots:
	void on_butt_debug();
	void on_butt_test1();
	void slot_rd_xil_dat(alt_dat_req_t* odat);
	void slot_wr_xil_dat(alt_dat_req_t* idat);
	void slot_rd_stm_dat(alt_dat_req_t* odat);
	void slot_wr_stm_dat(alt_dat_req_t* idat);
	void slot_rd_msp_dat(QByteArray odat);
	void slot_wr_msp_dat(QByteArray idat);
	void slot_rd_udb(alt_dat_req_t* odat);
	void slot_wr_udb(alt_dat_req_t* idat);
	void slot_rd_upow(alt_dat_req_t* odat);
	void slot_wr_upow(alt_dat_req_t* idat);
public slots:
	void draw_all_osc_parm(void);

///	QTimer *p_req_timer;
};
#define REQ_PERIOD 55			///500		//55	///2000
#define RESET_PERIOD 2000


#endif // Q_WIN_TEMPO_H
