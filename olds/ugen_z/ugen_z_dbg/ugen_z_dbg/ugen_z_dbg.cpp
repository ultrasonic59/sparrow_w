#include "ugen_z_dbg.h"
#include "dial_load_save_par.h"
extern quint8	calc_step(quint16 beg_r,quint16 end_r);
extern quint16	calc_endr(quint16 beg_r,quint8 step);

extern int calc_val_pow_Index(unsigned char val);
extern unsigned char calc_val_pow(int Index);

#include "QWaitCondition"

#include "trans.h"


extern RespondAuto::types g_curr_type_def;

extern QMutex CommandMutex;
extern QMutex DataMutex;

extern volatile int CommandMutexCounter;
extern volatile int DataMutexCounter;

extern QMutex CommandMutex_sent;
extern QMutex DataMutex_sent;


quint32 g_changed_param=0;
bool en_set_device_params = false;
ed_par_trk_t curr_ed_par_trk;
ugen_par_t g_ugen_par;

bool en_Http = false;








void ugen_z_dbg::saveSettings(void)
{
QSettings settings( QCoreApplication::applicationDirPath()+"//ugen_z_dbg.ini",
                   QSettings::IniFormat);
settings.setValue("mainWindowGeometry", saveGeometry());

settings.setValue("param_dir", ParamDirName);

settings.setValue("curr_chan", curr_ed_par_trk.curr_chan);
settings.setValue("curr_tact", curr_ed_par_trk.curr_tact);
settings.setValue("curr_begr", curr_ed_par_trk.curr_begr);
settings.setValue("curr_endr", curr_ed_par_trk.curr_endr);
settings.setValue("curr_step_r", curr_ed_par_trk.curr_step_r);
settings.setValue("curr_us", curr_ed_par_trk.curr_us);
settings.setValue("curr_pvrch", curr_ed_par_trk.curr_pvrch);
///settings.setValue("curr_tv", curr_ed_par_trk.curr_tv);
settings.setValue("curr_strob", curr_ed_par_trk.curr_strob);
settings.setValue("rej_osc", curr_ed_par_trk.rej_osc);

settings.setValue("curr_gen", curr_ed_par_trk.curr_gen);
settings.setValue("curr_pow", curr_ed_par_trk.curr_pow);
settings.setValue("curr_nparam", curr_ed_par_trk.curr_nparam);
settings.setValue("ip_addr", ip_addr);
settings.setValue("port", port);

settings.setValue("delay_sign", g_ugen_par.delay_sign);
settings.setValue("len_sign", g_ugen_par.len_sign);
settings.setValue("pre", g_ugen_par.pre);
settings.setValue("rng", g_ugen_par.rng);
settings.setValue("len_def", g_ugen_par.len_def);

///settings.setValue("last_file_name_curs", LastFileNameCurs);
///settings.setValue("last_file_name_izm", LastFileNameIzm);
///settings.setValue("LastFileNameVideo", LastFileNameVideo);
///.toInt();
///ip_addr=ui->lineEdit_IP->text();
////ip_addr=ui_osc.lineEditServer->text();



settings.setValue("row_number", OscProperties.row_number);
settings.setValue("column_number", OscProperties.column_number);

settings.setValue("BackgroundColor", OscProperties.BackgroundColor.rgb());
settings.setValue("OscillogramColor", OscProperties.OscillogramColor.rgb());
settings.setValue("CellColor", OscProperties.CellColor.rgb());
settings.setValue("TextColor", OscProperties.TextColor.rgb());
settings.setValue("StrobColor", OscProperties.StrobsColor[0].rgb());
settings.setValue("Strob2Color", OscProperties.StrobsColor[1].rgb());
settings.setValue("VRChColor", OscProperties.VRChColor.rgb());
settings.setValue("SepLineColor", OscProperties.SepLineColor.rgb());
settings.setValue("BaseDefColor", OscProperties.BaseDefColor.rgb());
settings.setValue("NoContactColor", OscProperties.NoContactColor.rgb());
settings.setValue("DefectColor", OscProperties.DefectColor.rgb());


settings.setValue("spectr_first_el", spectr_first_el);
settings.setValue("spectr_length", spectr_length);


}
void ugen_z_dbg::loadSettings(void)
{
QSettings settings( QCoreApplication::applicationDirPath()+"//ugen_z_dbg.ini",
                   QSettings::IniFormat);
restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
ParamDirName=settings.value("param_dir","D: ").toString();

curr_ed_par_trk.curr_chan=settings.value("curr_chan",DEF_NUM_CHAN).toInt();
curr_ed_par_trk.curr_tact=settings.value("curr_tact",DEF_NUM_TACT).toInt();
curr_ed_par_trk.curr_begr=settings.value("curr_begr",DEF_BEGR).toInt();
curr_ed_par_trk.curr_step_r=settings.value("curr_step_r",2).toInt();
curr_ed_par_trk.curr_endr=calc_endr(curr_ed_par_trk.curr_begr,curr_ed_par_trk.curr_step_r);

curr_ed_par_trk.curr_us=settings.value("curr_us",DEF_NUM_US).toInt();
curr_ed_par_trk.curr_pvrch=settings.value("curr_pvrch",DEF_PVRCH).toInt();
///curr_ed_par_trk.curr_tv=settings.value("curr_tv",DEF_TV).toInt();
curr_ed_par_trk.curr_strob=settings.value("curr_strob",DEF_NUM_STRB).toInt();
curr_ed_par_trk.rej_osc=settings.value("rej_osc",1).toInt();
curr_ed_par_trk.curr_gen=settings.value("curr_gen",DEF_NUM_GEN).toInt();
curr_ed_par_trk.curr_pow=settings.value("curr_pow",DEF_NUM_POW).toInt();
curr_ed_par_trk.curr_nparam=settings.value("curr_nparam",DEF_NUM_PARAM).toInt();

g_ugen_par.delay_sign=settings.value("delay_sign",DEF_DELAY_SYGN).toInt();
g_ugen_par.len_sign=settings.value("len_sign",DEF_LEN_SYGN).toInt();
g_ugen_par.pre=settings.value("pre",DEF_PRESCALE).toInt();
g_ugen_par.rng=settings.value("rng",DEF_RNG).toInt();
g_ugen_par.len_def=settings.value("len_def",DEF_LEN_DEF).toInt();

ip_addr=settings.value("ip_addr","192.168.1.79").toString();
port=settings.value("port","2000").toInt();



OscProperties.row_number = settings.value("row_number", OscProperties.row_number).toInt();
OscProperties.column_number = settings.value("column_number", OscProperties.column_number).toInt();

OscProperties.BackgroundColor.setRgb( settings.value("BackgroundColor", OscProperties.BackgroundColor.rgb()).toInt() );
OscProperties.OscillogramColor.setRgb( settings.value("OscillogramColor", OscProperties.OscillogramColor.rgb()).toInt() );
OscProperties.CellColor.setRgb( settings.value("CellColor", OscProperties.CellColor.rgb()).toInt() );
OscProperties.TextColor.setRgb( settings.value("TextColor", OscProperties.TextColor.rgb()).toInt() );
OscProperties.StrobsColor[0].setRgb( settings.value("StrobColor", OscProperties.StrobsColor[0].rgb()).toInt() );
OscProperties.StrobsColor[1].setRgb( settings.value("Strob2Color", OscProperties.StrobsColor[1].rgb()).toInt() );
OscProperties.VRChColor.setRgb( settings.value("VRChColor", OscProperties.VRChColor.rgb()).toInt() );
OscProperties.SepLineColor.setRgb( settings.value("SepLineColor", OscProperties.SepLineColor.rgb()).toInt() );
OscProperties.BaseDefColor.setRgb( settings.value("BaseDefColor", OscProperties.BaseDefColor.rgb()).toInt() );
OscProperties.NoContactColor.setRgb( settings.value("NoContactColor", OscProperties.NoContactColor.rgb()).toInt() );
OscProperties.DefectColor.setRgb( settings.value("DefectColor", OscProperties.DefectColor.rgb()).toInt() );



spectr_first_el=settings.value("spectr_first_el",spectr_first_el).toInt();
spectr_length=settings.value("spectr_length",spectr_length).toInt();
}
///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
///#define _CurrUs curr_param.takts[curr_ed_par_trk.curr_tact].uss[curr_ed_par_trk.curr_us]
///#define _CurrNgen curr_param.takts[curr_ed_par_trk.curr_tact].gens[curr_ed_par_trk.curr_gen]
///#define _CurrPow curr_param.pow_gen[curr_ed_par_trk.curr_pow]
///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ugen_z_dbg::ugen_z_dbg(QWidget *parent)
	: QMainWindow(parent),
	ui_osc(),
	old_resp(),
	new_resp(),
	RespTrans(),
	t_buff(nullptr),
	OscProperties(),
	OscPlotter(&RespTrans, &OscProperties, &old_resp),
	auto_frame(400),
	auto_frame_len_old(0),
	auto_frame_len_new(0),
	AutoPlotter(&auto_frame, &auto_frame_len_new, &auto_frame_len_old, &OscProperties),

	SpectrWidget(nullptr),
	SpectrPlotter(&OscProperties),

	viewProp(nullptr),
	resetAutoArrReq(false),
	NoConnect(false),
	curr_unsuccesfull_conn(1),

	on_ed_nus(false),
	on_ed_ntact(false),
	spectr_mode(false),


	spectr_first_el(0),
	spectr_length(256),
	spectr_buff_len(256),

	spectr_constant(128)


{
	ui_osc.setupUi(this);

	

	buffer_forSpectr.SetArraySize(256, 2);


	ui_osc.OscPlotter->SetPosTester(&OscPlotter);
	OscPlotter.ConnectToWidget(ui_osc.OscPlotter);

	ui_osc.AutoPlotter->SetOscPlotter(&AutoPlotter);
	AutoPlotter.ConnectToWidget(ui_osc.AutoPlotter);


	SpectrWidget = new PlotterWidget(ui_osc.splitter);
	SpectrWidget->setObjectName(QStringLiteral("SpectrPlotter"));
	ui_osc.splitter->addWidget(SpectrWidget);


	SpectrWidget->SetOscPlotter(&SpectrPlotter);
	SpectrPlotter.ConnectToWidget(SpectrWidget);

	SpectrWidget->hide();


	dev_param_edit(en_set_device_params);

	
	ui_osc.label_noConn->hide();

	connect(ui_osc.pushButton_test, SIGNAL(clicked()), this, SLOT(on_test_butt()));
	connect(ui_osc.pushButton_par, SIGNAL(clicked()), this, SLOT(on_butt_par()));
	connect(ui_osc.pushButt_load_par, SIGNAL(clicked()), this, SLOT(on_butt_load_par()));
	connect(ui_osc.pushButt_save_par, SIGNAL(clicked()), this, SLOT(on_butt_save_par()));
	connect(ui_osc.comboBox_rej, SIGNAL(currentIndexChanged(int)), this, SLOT(rej_changed(int)));
///	connect(ui_osc.checkBox_vrch, SIGNAL(stateChanged(int)), this, SLOT(vron_changed(int)));
	connect(ui_osc.pushButton_test1, SIGNAL(clicked()), this, SLOT(on_butt_test1()));
	connect(ui_osc.pushButt__rd_par, SIGNAL(clicked()), this, SLOT(on_butt_rd_par_dev()));
	connect(ui_osc.pushButt_wr_par, SIGNAL(clicked()), this, SLOT(on_butt_wr_par_dev()));
	connect(ui_osc.pushButt_debug, SIGNAL(clicked()), this, SLOT(on_butt_debug()));
////	connect(ui_osc.checkBox_devSet, SIGNAL(clicked(bool)), this, SLOT(setDeviceParams(bool)));
	connect(ui_osc.checkBox_http, SIGNAL(clicked(bool)), this, SLOT(setFlagHttp(bool)));


//*******************************************************
	
	port =DEF_PORT;
	loadSettings();
	calculate_spectr_len();
	dial_par = new DialPar(this);

///socket = new QUdpSocket(this);
///connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
///connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
///connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
///connect(socket, SIGNAL(readyRead()), this, SLOT(udp_ready_dat()));
///socket->bind();

///	connect(ui_osc.ed_nus, SIGNAL(on_off()), this, SLOT(slot_us_on_off()));
///	connect(ui_osc.ed_ngen, SIGNAL(on_off()), this, SLOT(slot_gen_on_off()));

///++++++++++++++++++++++++++++++++++++
	QThread *thr_udp = new QThread;
	p_udp_cmd= new c_udp_cmd(this);
	p_udp_cmd->moveToThread(thr_udp);
	p_udp_cmd->on_thr_obr_udp_cmd=true;

	QThread *thr_dev_cmd = new QThread;
	p_dev_cmd= new c_dev_cmd(this);
	p_dev_cmd->moveToThread(thr_dev_cmd);

	connect(p_dev_cmd, SIGNAL(signal_write_udp_cmd(QByteArray)),p_udp_cmd, SLOT(write_udp_cmd(QByteArray)),Qt::QueuedConnection);
	connect(p_dev_cmd, SIGNAL(signal_write_udp_dat(QByteArray)),p_udp_cmd, SLOT(write_udp_dat(QByteArray)),Qt::QueuedConnection);

	connect(p_dev_cmd, SIGNAL(NoConnect()),this, SLOT(NoConnection()));
	connect(this, SIGNAL(bind(void)),p_udp_cmd, SLOT(bind(void)));


	p_tune_thr= new c_tune_thr(this);
	p_tune_thr->moveToThread(thr_dev_cmd);
///my_cmd_http= new QCmdHttp;
///connect(this, SIGNAL(req_set_dat(QString ,QString *)),my_cmd_http, SLOT(slot_set_dat(QString ,QString *)));
////connect(my_cmd_http, SIGNAL(data_resv(QString)), this, SLOT(data_ans(QString)));
///connect(my_obr_cmd, SIGNAL(req_set_dat(QString ,QString *)),my_cmd_http, SLOT(slot_set_dat(QString ,QString *)));
////connect(my_obr_cmd, SIGNAL(req_get_img(QString *,QImage *)),my_cmd_http, SLOT(slot_req_image(QString *,QImage *)));

	connect(p_tune_thr, SIGNAL(signal_draw_osc0(void)),this, SLOT(slot_draw_osc0(void)));
	connect(p_tune_thr, SIGNAL(signal_draw_amps(void)),this, SLOT(slot_draw_amps(void)));
////connect(this, SIGNAL(change_param(quint32)),this, SLOT(slot_change_param(quint32)));

	connect(this, SIGNAL(signal_on_off_timer(bool )),p_tune_thr, SLOT(on_off_timer(bool )));

	thr_udp->start();
	thr_dev_cmd->start();

	p_tune_thr->on_tune_thr=true;

////=============================================================
////p_udp_cmd->bind();
///p_udp_cmd->set_host(host_addr,2000);
	ui_osc.pushButt_load_par->setShortcut(QKeySequence(tr("ALT+L")));

	ui_osc.comboBox_rej->setCurrentIndex(1);

///++++++++++++++++++++++++++++++
	on_ed_nus=false;
	on_ed_ntact=false;
///++++++++++++++++++++++++++++++



	ui_osc.label_bstr_sp->setVisible(false);
	ui_osc.ed_bstr_sp->setVisible(false);
	ui_osc.label_lstr_sp->setVisible(false);
	ui_osc.ed_lstr_sp->setVisible(false);
	ui_osc.ed_spectr_const->setVisible(false);


	connect(ui_osc.checkBox_spectr, SIGNAL(clicked(bool)), this, SLOT(setSpectrOn(bool)));





//+++++++++++++++++++++++++++++++
load_params();
init_all_ed_params();
draw_all_osc_parm();
///++++++++++++++++++++++++++++++
dial_dbg=new DialDebug(this);
connect(this, SIGNAL(put_alt_dat_dial(alt_dat_req_t*)), dial_dbg, SLOT(req_data_rdy(alt_dat_req_t*)));
connect(dial_dbg, SIGNAL(req_rd_xil(alt_dat_req_t*)), this, SLOT(slot_rd_xil_dat(alt_dat_req_t*)));
connect(dial_dbg, SIGNAL(req_wr_xil(alt_dat_req_t*)), this, SLOT(slot_wr_xil_dat(alt_dat_req_t*)));
connect(dial_dbg, SIGNAL(req_rd_stm(alt_dat_req_t*)), this, SLOT(slot_rd_stm_dat(alt_dat_req_t*)));
connect(dial_dbg, SIGNAL(req_wr_stm(alt_dat_req_t*)), this, SLOT(slot_wr_stm_dat(alt_dat_req_t*)));
connect(dial_dbg, SIGNAL(req_rd_msp(QByteArray*)), this, SLOT(slot_rd_msp_dat(QByteArray)));
connect(dial_dbg, SIGNAL(req_wr_msp(QByteArray)), this, SLOT(slot_wr_msp_dat(QByteArray)));
connect(dial_dbg, SIGNAL(req_rd_udb(alt_dat_req_t*)), this, SLOT(slot_rd_udb(alt_dat_req_t*)));
connect(dial_dbg, SIGNAL(req_wr_udb(alt_dat_req_t*)), this, SLOT(slot_wr_udb(alt_dat_req_t*)));
connect(dial_dbg, SIGNAL(req_rd_upow(alt_dat_req_t*)), this, SLOT(slot_rd_upow(alt_dat_req_t*)));
connect(dial_dbg, SIGNAL(req_wr_upow(alt_dat_req_t*)), this, SLOT(slot_wr_upow(alt_dat_req_t*)));

connect(dial_dbg, SIGNAL(signal_send_udp_test_dats(QByteArray)),p_udp_cmd, SLOT(write_udp_dats(QByteArray)),Qt::QueuedConnection);

connect(dial_par->ui->pushButton_colors, SIGNAL(clicked()), this, SLOT(changeColors()));

///ui_osc.checkBox_vrch->setChecked(false);
///ui_osc.ed_timev->hide();
///ui_osc.ed_kusv->hide();
///ui_osc.ed_np_vrch->hide();
///ui_osc.label_npointv->hide();
///ui_osc.label_kusv->hide();
///ui_osc.label_vtime->hide();



///ui_osc.checkBox_vrch->hide();
ui_osc.pushButton_test1->hide();
///???=================================
////ui_osc.pushButt_debug->hide();
///???=================================


////connect(&OscPlotter, SIGNAL(BegOscIntermedChange(int)), this, SLOT(BegOscIntermedChange(int)));
////connect(&OscPlotter, SIGNAL(BegOscFinalChange()), this, SLOT(BegOscFinalChange()));
///connect(&OscPlotter, SIGNAL(EndOscIntermedChange(int)), this, SLOT(EndOscIntermedChange(int)));
////connect(&OscPlotter, SIGNAL(EndOscFinalChange()), this, SLOT(EndOscFinalChange()));
connect(&OscPlotter, SIGNAL(SetStrobIntermedPos(int, int, int)), this, SLOT(SetStrobIntermedPos(int, int, int)));
connect(&OscPlotter, SIGNAL(SetStrobFinalPos()), this, SLOT(SetStrobFinalPos()));
connect(&OscPlotter, SIGNAL(SetStrobIntermedLeft(int, int, int)), this, SLOT(SetStrobIntermedLeft(int, int, int)));
connect(&OscPlotter, SIGNAL(SetStrobFinalLeft()), this, SLOT(SetStrobFinalLeft()));
connect(&OscPlotter, SIGNAL(SetStrobIntermedRight(int, int)), this, SLOT(SetStrobIntermedRight(int, int)));
connect(&OscPlotter, SIGNAL(SetStrobFinalRight()), this, SLOT(SetStrobFinalRight()));


en_set_device_params = true;
dev_param_edit(en_set_device_params);



emit bind();

}

ugen_z_dbg::~ugen_z_dbg()
{
	if(viewProp != nullptr)
		delete viewProp;

	saveSettings();

	p_tune_thr->on_tune_thr = false;


	if(CommandMutexCounter > 0)
	{
		CommandMutexCounter--;
		CommandMutex.unlock();
	}

	if(DataMutexCounter > 0)
	{
		DataMutexCounter--;
		DataMutex.unlock();
	}

	CommandMutex_sent.tryLock();
	CommandMutex_sent.unlock();

	DataMutex_sent.tryLock();
	DataMutex_sent.unlock();


	if(t_buff != nullptr)
		free(t_buff);

	Sleep(MAX_TIMEOUT_RSV);

}

void ugen_z_dbg::NoConnection()
{
	if(NoConnect)
		return;

	p_tune_thr->on_tune_thr = false;

	ui_osc.pushButton_test->hide();
	ui_osc.label_noConn->show();

	ui_osc.label_noConn->setStyleSheet("background-color: rgb(255, 50, 50); color: rgb(255, 255, 255)");
	ui_osc.label_noConn->setText(tr("\320\235\320\265\321\202 \321\201\320\262\321\217\320\267\320\270!"));

	NoConnect = true;


	QTimer::singleShot(RESET_PERIOD, this, SLOT(ResetConnection()));
}

void ugen_z_dbg::ResetConnection()
{
	g_changed_param = 0;

	NoConnect = false;
	p_tune_thr->on_tune_thr = true;
}




void ugen_z_dbg::changeColors()
{

	if(viewProp != NULL)
		CloseWindowParameters();

	viewProp = new ViewProperties(ui_osc.centralWidget, this, &OscProperties);
	viewProp->show();

}


void ugen_z_dbg::NewWindowParameters()
{
	if(viewProp == NULL)
		return;

	delete viewProp;
	viewProp = NULL;
}

void ugen_z_dbg::CloseWindowParameters()
{
	if(viewProp == NULL)
		return;

	OscProperties = viewProp->OscPropTemp;

	ResetPlot();

	delete viewProp;
	viewProp = NULL;
}


void ugen_z_dbg::ResetPlot()
{
	OscPlotter.ResetPlot();

	AutoPlotter.PaintBackground();
	AutoPlotter.PlotOldRespond(false);
	AutoPlotter.UpdatePlot();
}

void ugen_z_dbg::UpdateOscPlotter()
{
	OscPlotter.PlotRespond(new_resp, old_resp);
	old_resp = new_resp;
}
void ugen_z_dbg::setFlagHttp(bool newSet)
{
	en_Http = newSet;
////	dev_param_edit(en_set_device_params);
}

#if 0
void ugen_z_dbg::setDeviceParams(bool newSet)
{
	en_set_device_params = newSet;
	dev_param_edit(en_set_device_params);
}
#endif

void ugen_z_dbg::dev_param_edit(bool edit)
{
	ui_osc.ed_nus->setEnabled(edit);
///	ui_osc.ed_tt->setEnabled(edit);
///	ui_osc.ed_tv->setEnabled(edit);
	ui_osc.ed_por->setEnabled(edit);
	ui_osc.ed_bstr->setEnabled(edit);
	ui_osc.ed_lstr->setEnabled(edit);
	ui_osc.ed_kus->setEnabled(edit);
///	ui_osc.checkBox_vrch->setEnabled(edit);
///	ui_osc.ed_timev->setEnabled(edit);
///	ui_osc.ed_kusv->setEnabled(edit);
////	ui_osc.ed_len_imp->setEnabled(edit);
////	ui_osc.comboBox_val_pow->setEnabled(edit);
}



void ugen_z_dbg::on_butt_par()
{
////	dial_par->set_rele(curr_par_contr.rele_par.ena_rele);
////	dial_par->set_rele_len(curr_par_contr.rele_par.rele_len);
	dial_par->set_ip_addr(ip_addr);
///	dial_par->set_chans(curr_param.cn_list.cn_info);
	dial_par->set_on_ed_us(on_ed_nus);
	dial_par->set_on_ed_tact(on_ed_ntact);


	dial_par->EnableParams(en_set_device_params);


	dial_par->show();
	if (dial_par->exec() == QDialog::Accepted) 
		{
		dial_par->get_ip_addr(&ip_addr);
////		dial_par->get_rele(&(curr_par_contr.rele_par.ena_rele));
////		dial_par->get_rele_len(&(curr_par_contr.rele_par.rele_len));
////		dial_par->get_chans(curr_param.cn_list.cn_info);
		dial_par->get_on_ed_us(&on_ed_nus);
////		dial_par->get_on_ed_tact(&on_ed_ntact);
///		ui_osc.ed_tact->set_ena_edit(on_ed_ntact);
		ui_osc.ed_nus->set_ena_edit(on_ed_nus);
		saveSettings();

		if(NoConnect)
			exit(0);

		set_chan_ed_params();

////???		g_changed_param |= CHNG_RELE;

		}
}


void ugen_z_dbg::ProcessAutoRespond(void)
{
	if(NoConnect)
		return;

	if(!curr_ed_par_trk.rej_osc && spectr_mode)
		return;


	if(resetAutoArrReq)
	{
		resetAutoArrReq = false;
		auto_collector.SetSize(0);
	}
	else
	{
		if(auto_collector.PushRespondAuto(auto_resp))
			UpdateAutoPlotter();
		else
			auto_frame.clear();
	}
}


void ugen_z_dbg::UpdateAutoPlotter()
{
	AutoPlotter.PlotOldRespond(true);

	if(auto_frame_len_new != auto_frame_len_old)
	{
		auto_frame_len_old = auto_frame_len_new;
		auto_frame.SetNewMaxSize(auto_frame_len_old, false);
	}

	quint64 frame_beg = 15;
	quint64 max_size = 0;

	qint64 temp_size = auto_collector.GetSize() - 15;

	if(temp_size > 0)
	{
		if(temp_size  > auto_frame_len_old )
		{
			frame_beg += temp_size - auto_frame_len_old;
			max_size = auto_frame_len_old;
		}
		else
			max_size = temp_size;

		auto_frame.CopyFromArray(auto_collector.GetFrameArray() + frame_beg, max_size, false);
		AutoPlotter.PlotNewRespond();
	}
}


///+++++++++++++++++++++++++++++++++++++++++++++++++++
void ugen_z_dbg::slot_draw_osc0(void)
{
	memcpy(new_resp.osc,buffOscs[0],LEN_OSC);

	if(spectr_mode)
	{
		buffer_forSpectr.ClearDim(0);
		buffer_forSpectr.ClearDim(1);

		for(int i = 0; i < spectr_length; i++)
			buffer_forSpectr.val_array[spectr_buff_beg+i][0] = static_cast<double>(buffOscs[0][spectr_first_el+i]) - spectr_constant;
	

/*		for(int i = 0; i < spectr_length; i++)
			buffer_forSpectr.val_array[spectr_buff_beg+i][0] = 1 + sin(0.1*i);
*/

	}

	UpdateOscPlotter();




	if(spectr_mode)
	{
		


		fft_dif(buffer_forSpectr.GetValDest(), spectr_buff_len);
		bitrev_permute(buffer_forSpectr.GetValDest(), spectr_buff_len);

		for(int i = 1; i < spectr_buff_len/2; i++)
		{
			buffer_forSpectr.val_array[i][0] += buffer_forSpectr.val_array[spectr_buff_len - i][0];	
			buffer_forSpectr.val_array[i][1] -= buffer_forSpectr.val_array[spectr_buff_len - i][1];
		}


		double max_val = 0;


		for(int i = 0; i < spectr_buff_len/2; i++)
		{
			spectr_arr[i] = sqrt(buffer_forSpectr.val_array[i][0]*buffer_forSpectr.val_array[i][0] +  buffer_forSpectr.val_array[i][1]*buffer_forSpectr.val_array[i][1]) / spectr_buff_len ;

			if(spectr_arr[i] > max_val)
				max_val = spectr_arr[i];
		}





		if(max_val == 0)
			max_val = 1;



		SpectrPlotter.ProtSpectr(spectr_buff_len/2, spectr_arr, max_val);


	}
}
///+++++++++++++++++++++++++++++++++++++++++++++++++++

void ugen_z_dbg::draw_all_osc_parm(void)
{
new_resp.begr=curr_ed_par_trk.curr_begr;
new_resp.strb[0].beg= _CurrUs.strb[0].beg;
new_resp.strb[0].len= _CurrUs.strb[0].len;
new_resp.strb[0].por= _CurrUs.strb[0].por;
new_resp.step_r = curr_ed_par_trk.curr_step_r;
new_resp.kUs= _CurrUs.kus1;
new_resp.strb[1].beg= _CurrUs.strb[1].beg;
new_resp.strb[1].len= _CurrUs.strb[1].len;
new_resp.strb[1].por= _CurrUs.strb[1].por;
new_resp.num_sel_strb=curr_ed_par_trk.curr_strob;


new_resp.video_mode = curr_ed_par_trk.rej_osc ;

///if()
UpdateOscPlotter();
SetConnStatus();
}
////+++++++++++++++++++++++++++++++++++++++++++++++++++++

extern amp_tim_t m_amp_tims[NUM_CHANS];

void ugen_z_dbg::slot_draw_amps(void)
{
///int ii;
////quint8 tbuf_amps[NUM_CHANS*NUM_STROBS_OBR];
///amp_tim_t t_amp_tims[NUM_CHANS];
///p_dev_cmd->udp_get_amps((quint8*)t_amp_tims);
///memcpy(m_amp_tims,t_amp_tims,sizeof(amp_tim_t)*NUM_CHANS);

auto_resp.type_def = g_curr_type_def;///RespondAuto::Defect;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

for (int ii = 0;ii<NUM_CHANS;ii++)
{
////	auto_resp.us_on[ii] = curr_param.takts[(curr_param.cn_list.cn_info[ii].ntact)].uss[(curr_param.cn_list.cn_info[ii].nus)].on_us;
	auto_resp.ampl[ii]=m_amp_tims[ii].amp[0];
	auto_resp.ampl2[ii]=m_amp_tims[ii].amp[1];
}

ProcessAutoRespond();
SetConnStatus();
}
void ugen_z_dbg::on_butt_test1()
{
	static int tmp=0;
	int tmp1;
	p_dev_cmd->udp_set_cur_rej(tmp);
	qDebug()<<"tmp"<<tmp;
	tmp++;
	tmp&=0x7;
	p_dev_cmd->udp_get_cur_rej((quint8*)&tmp1);
	qDebug()<<"tmp1"<<tmp1;
}
void ugen_z_dbg::on_test_butt()
{
	///QByteArray tba="ui_osc.lineEditTestStr->text()";
	int tmp=0;

	t_buff=(quint8 *)malloc(sizeof(par_trk_t)+sizeof(ed_par_trk_t)+8000);



	p_udp_cmd->set_host(ip_addr,port);

	////par_contr_t t_param_contr;

	emit signal_on_off_timer(false );
	///p_dev_cmd->udp_rd_par_dev((quint8 *)&t_param_contr);
	///memcpy(&curr_par_contr, &t_param_contr,sizeof(par_contr_t));

	//p_dev_cmd->udp_rd_par_dev(t_buff);
	////memcpy(&curr_ed_par_trk, t_buff,sizeof(ed_par_trk_t));
	/*memcpy(&curr_par_contr, t_buff,sizeof(par_contr_t));



	g_changed_param = CHNG_CHAN|CHNG_TACT|CHNG_NUS
					|CHNG_BEGR|CHNG_ENDR|CHNG_NSTRB
					|CHNG_NUMPV|CHNG_NGEN|CHNG_NPOW
					|CHNG_REJ_OSC;

	*/
	QTimer::singleShot(REQ_PERIOD, this, SLOT(EndInitConnection()));

}



void ugen_z_dbg::EndInitConnection()
{
	if(!p_dev_cmd->udp_rd_par_dev(t_buff) || NoConnect)
	{
		QTimer::singleShot(REQ_PERIOD, this, SLOT(EndInitConnection()));
		return;
	}

	emit signal_on_off_timer(true );

	memcpy(&curr_par_trk, t_buff,sizeof(par_trk_t));
#if 0
g_changed_param = CHNG_CHAN|CHNG_TACT|CHNG_NUS
				|CHNG_BEGR|CHNG_ENDR|CHNG_NSTRB
				|CHNG_NUMPV|CHNG_NGEN|CHNG_NPOW
				|CHNG_REJ_OSC;
#endif
	show_all_par();
	draw_all_osc_parm();
	show_on_gens();
	show_on_uss();

	set_us_ed_params(curr_ed_par_trk.curr_us);
	set_gen_ed_params(curr_ed_par_trk.curr_gen);
	ui_osc.pushButton_test->hide();
	ui_osc.label_noConn->show();
}




void ugen_z_dbg::SetConnStatus(void)
{
	if(NoConnect)
		return;

	if(curr_unsuccesfull_conn != p_dev_cmd->old_unsuccesfull_conn)
	{
		curr_unsuccesfull_conn = p_dev_cmd->old_unsuccesfull_conn;

		if(curr_unsuccesfull_conn == 0)
		{
			ui_osc.label_noConn->setStyleSheet("background-color: rgb(50, 255, 50); color: rgb(255, 255, 255)");
			ui_osc.label_noConn->setText( "" );
		}
		else
		{

			ui_osc.label_noConn->setStyleSheet("background-color: rgb(255, 50, 50); color: rgb(255, 255, 255)");
			ui_osc.label_noConn->setText(  QString::number(curr_unsuccesfull_conn,10) );
		}

		p_dev_cmd->old_unsuccesfull_conn = 0;
	}
}




void ugen_z_dbg::show_on_gens(void)
{
QString tstr="";
QString tstr1;
int ii;
for(ii=0;ii<NUM_GENS;ii++)
{
if(curr_param.takts[curr_ed_par_trk.curr_tact]
		.gens[ii].on_gen)
		{
		tstr1.sprintf("G%d ",(ii+1));
		tstr+=tstr1;
		}
}
ui_osc.label_gens->setStyleSheet("background-color: rgb(250, 250, 250); color: rgb(250, 0, 0)");
ui_osc.label_gens->setText(tstr);
}
void ugen_z_dbg::show_on_uss(void)
{
	QString tstr="";
	QString tstr1;
	int ii;
	for(ii=0;ii<NUM_USS;ii++)
	{
		if(curr_param.takts[curr_ed_par_trk.curr_tact]
				.uss[ii].on_us)
				{
				tstr1.sprintf("U%d ",(ii+1));
				tstr+=tstr1;
				}
	}
	ui_osc.label_uss->setStyleSheet("background-color: rgb(250, 250, 250); color: rgb(0, 250, 0)");
	ui_osc.label_uss->setText(tstr);
}

void ugen_z_dbg::set_chan_ed_params(void)
{
#if 0
if(curr_param.cn_list.chan_n==0)
	curr_param.cn_list.chan_n=1;
if(curr_ed_par_trk.curr_chan>=curr_param.cn_list.chan_n)
	curr_ed_par_trk.curr_chan=curr_param.cn_list.chan_n-1;
if(!check_tact_chan(curr_ed_par_trk.curr_chan))
	set_tact_ed_params(curr_ed_par_trk.curr_tact);
///curr_ed_par_trk.curr_tact=curr_param.cn_list.cn_info[num_chan].ntact;
curr_ed_par_trk.curr_us=curr_param.cn_list.cn_info[curr_ed_par_trk.curr_chan].nus;
#endif
set_us_ed_params(curr_ed_par_trk.curr_us);
}
bool ugen_z_dbg::check_tact_chan(quint8 num_chan)
{
////if(curr_ed_par_trk.curr_tact==curr_param.cn_list.cn_info[num_chan].ntact)
	return true;
////else
////curr_ed_par_trk.curr_tact=curr_param.cn_list.cn_info[num_chan].ntact;
////return false;
}

bool ugen_z_dbg::check_ngen_tact()
{
if(_CurrNgen.on_gen)
	return true;
else
{
int ii;
for(ii=0;ii<NUM_GENS;ii++)
	{
	if(curr_param.takts[curr_ed_par_trk.curr_tact]
		.gens[ii].on_gen)
		{
		curr_ed_par_trk.curr_gen=ii;
		return true;
		}
	}
return false;
}
}
void ugen_z_dbg::set_tact_ed_params(quint8 num_tact)
{
#if 0
ui_osc.ed_tact->show_par();

set_us_ed_params(curr_ed_par_trk.curr_us);
////if(!check_ngen_tact())
check_ngen_tact();
set_gen_ed_params(curr_ed_par_trk.curr_gen);
////curr_param.cn_list.cn_info[curr_ed_par_trk.curr_chan].ntact=curr_ed_par_trk.curr_tact;
show_on_gens();
show_on_uss();
#endif
}
void ugen_z_dbg::set_vrch_ed_params(void)
{
#if 0
if(_CurrUs.vrch.on_vrch)
	{
	ui_osc.checkBox_vrch->setChecked(true);
	ui_osc.ed_timev->set_data((unsigned short*)&(_CurrUs.vrch.time[curr_ed_par_trk.curr_pvrch]));
	ui_osc.ed_kusv->set_data((unsigned char*)&(_CurrUs.vrch.kus[curr_ed_par_trk.curr_pvrch]));
	ui_osc.ed_kusv->show();
	ui_osc.ed_np_vrch->show();
	ui_osc.ed_timev->show();
	ui_osc.label_npointv->show();
	ui_osc.label_kusv->show();
	ui_osc.label_vtime->show();
	}
else
	{
	ui_osc.checkBox_vrch->setChecked(false);
	ui_osc.ed_timev->hide();
	ui_osc.ed_kusv->hide();
	ui_osc.ed_np_vrch->hide();
	ui_osc.label_npointv->hide();
	ui_osc.label_kusv->hide();
	ui_osc.label_vtime->hide();
	}
#endif
}

void ugen_z_dbg::set_us_ed_params(quint8 num_us)
{
if(_CurrUs.on_us)
	ui_osc.ed_nus->set_colors(NUS_BG_COLOR,NUS_ON_COLOR);
else
	ui_osc.ed_nus->set_colors(NUS_BG_COLOR,NUS_OFF_COLOR);
ui_osc.ed_nus->show_par();
ui_osc.ed_kus->set_data((unsigned char*)&_CurrUs.kus1);
ui_osc.ed_kus->show_par();
set_strob_ed_params(curr_ed_par_trk.curr_strob);
////ui_osc.ed_tt->set_data((unsigned char*)&_CurrUs.taut);
///ui_osc.ed_tt->show_par();
///ui_osc.ed_tv->set_data((unsigned char*)&_CurrUs.tauv);
////ui_osc.ed_tv->show_par();
set_vrch_ed_params();
}

void ugen_z_dbg::set_strob_ed_params(quint8 num_str)
{
////ui_osc.ed_nstrob->show_par();
ui_osc.ed_bstr->set_data((unsigned short*)&_CurrUs.strb[curr_ed_par_trk.curr_strob].beg);
ui_osc.ed_bstr->show_par();
ui_osc.ed_lstr->set_data((unsigned short*)&_CurrUs.strb[curr_ed_par_trk.curr_strob].len);
ui_osc.ed_lstr->show_par();
ui_osc.ed_por->set_data((unsigned char*)&_CurrUs.strb[curr_ed_par_trk.curr_strob].por);
////ui_osc.ed_por->show_par();


new_resp.strb[0] = _CurrUs.strb[0];
new_resp.strb[1] = _CurrUs.strb[1];
}


void ugen_z_dbg::set_gen_ed_params(quint8 num_gen)
{
#if 0
if(_CurrNgen.on_gen)
{
	ui_osc.ed_ngen->set_colors(NGEN_BG_COLOR,NGEN_ON_COLOR);
}
else
{
	ui_osc.ed_ngen->set_colors(NGEN_BG_COLOR,NGEN_OFF_COLOR);
}
ui_osc.ed_ngen->show_par();
ui_osc.ed_len_imp->set_data((unsigned char*)&_CurrNgen.len_imp);
ui_osc.ed_len_imp->show_par();
#endif
}
void ugen_z_dbg::set_pow_ed_params(quint8 num_pow)
{
#if 0
curr_ed_par_trk.curr_pow = num_pow;
ui_osc.comboBox_npow->setCurrentIndex(curr_ed_par_trk.curr_pow);
ui_osc.comboBox_val_pow->setCurrentIndex( calc_val_pow_Index(_CurrPow.val) );
#endif
/*ui_osc.ed_npow->show_par();
ui_osc.ed_val_pow->set_data((unsigned char*)&_CurrPow.val); 
ui_osc.ed_val_pow->show_par();
*/
}

void ugen_z_dbg::init_all_ed_params()
{
//++++ num_chan ++++++
ui_osc.ed_chan->set_num_dig(NUM_DIG_NCHAN);
ui_osc.ed_chan->set_data((unsigned char*)&curr_ed_par_trk.curr_chan);
ui_osc.ed_chan->set_min_max(MIN_NUM_CHAN,MAX_NUM_CHAN);
ui_osc.ed_chan->set_ena_enter_on_off(false);
ui_osc.ed_chan->set_shift(1);
ui_osc.ed_chan->show_par();
connect(ui_osc.ed_chan, SIGNAL(param_changed()), this, SLOT(chan_changed()));
//++++ num_tact ++++++
#if 0
ui_osc.ed_tact->set_num_dig(NUM_DIG_NTACT);
ui_osc.ed_tact->set_data((unsigned char*)&curr_ed_par_trk.curr_tact);
ui_osc.ed_tact->set_min_max(MIN_NUM_TACT,MAX_NUM_TACT);
ui_osc.ed_tact->set_ena_enter_on_off(false);
ui_osc.ed_tact->set_ena_edit(on_ed_ntact);
ui_osc.ed_tact->set_shift(1);
ui_osc.ed_tact->show_par();
connect(ui_osc.ed_tact, SIGNAL(param_changed()), this, SLOT(tact_changed()));
#endif
//++++ pre ++++++
ui_osc.ed_pre->set_num_dig(NUM_DIG_BEGR);
ui_osc.ed_pre->set_coeff(COEF_BEGR);
ui_osc.ed_pre->set_data((unsigned short*)&g_ugen_par.pre);
ui_osc.ed_pre->set_inc_step(INC_PRE);
ui_osc.ed_pre->set_min_max(MIN_BEGR,MAX_BEGR);
ui_osc.ed_pre->set_ena_enter_on_off(false);
ui_osc.ed_pre->show_par();
connect(ui_osc.ed_pre, SIGNAL(param_changed()), this, SLOT(pre_changed()));
//++++ rng ++++++
ui_osc.ed_rng->set_num_dig(NUM_DIG_BEGR);
ui_osc.ed_rng->set_coeff(COEF_BEGR);
ui_osc.ed_rng->set_data((unsigned short*)&g_ugen_par.rng);
ui_osc.ed_rng->set_inc_step(INC_RNG);
ui_osc.ed_rng->set_min_max(MIN_BEGR,MAX_BEGR);
ui_osc.ed_rng->set_ena_enter_on_off(false);
ui_osc.ed_rng->show_par();
connect(ui_osc.ed_pre, SIGNAL(param_changed()), this, SLOT(rng_changed()));
//++++ len_def ++++++
ui_osc.ed_len_def->set_num_dig(NUM_DIG_BEGR);
ui_osc.ed_len_def->set_coeff(COEF_BEGR);
ui_osc.ed_len_def->set_data((unsigned short*)&g_ugen_par.rng);
ui_osc.ed_len_def->set_inc_step(INC_LEN_DEF);
ui_osc.ed_len_def->set_min_max(MIN_BEGR,MAX_BEGR);
ui_osc.ed_len_def->set_ena_enter_on_off(false);
ui_osc.ed_len_def->show_par();
connect(ui_osc.ed_len_def, SIGNAL(param_changed()), this, SLOT(len_def_changed()));
//++++ bstrob ++++++
ui_osc.ed_bstr->set_num_dig(NUM_DIG_BSTR);
ui_osc.ed_bstr->set_coeff(10);
ui_osc.ed_bstr->set_data((unsigned short*)&g_ugen_par.delay_sign);
ui_osc.ed_bstr->set_min_max(MIN_BSTR,MAX_BSTR);
ui_osc.ed_bstr->set_ena_enter_on_off(false);
ui_osc.ed_bstr->show_par();
connect(ui_osc.ed_bstr, SIGNAL(param_changed()), this, SLOT(bstrob_changed()));
//++++ lstrob ++++++
ui_osc.ed_lstr->set_num_dig(NUM_DIG_LSTR);
ui_osc.ed_lstr->set_coeff(10);
ui_osc.ed_lstr->set_data((unsigned short*)&g_ugen_par.len_sign);
ui_osc.ed_lstr->set_min_max(MIN_LSTR,MAX_LSTR);
ui_osc.ed_lstr->set_ena_enter_on_off(false);
ui_osc.ed_lstr->show_par();
connect(ui_osc.ed_lstr, SIGNAL(param_changed()), this, SLOT(lstrob_changed()));

//++++ endr ++++++
///ui_osc.ed_endr->set_num_dig(NUM_DIG_ENDR);
///ui_osc.ed_endr->set_coeff(COEF_ENDR);
///ui_osc.ed_endr->set_data((unsigned char*)&curr_ed_par_trk.curr_step_r);
///ui_osc.ed_endr->set_inc_step(INC_STEPR);
///ui_osc.ed_endr->set_min_max(MIN_STEPR,MAX_STEPR);
///ui_osc.ed_endr->show_par(curr_ed_par_trk.curr_endr);
///connect(ui_osc.ed_endr, SIGNAL(param_changed()), this, SLOT(endr_changed()));
///connect(ui_osc.ed_endr, SIGNAL(result_changed()), this, SLOT(endr_result_changed()));
//++++ num_us ++++++
ui_osc.ed_nus->set_num_dig(NUM_DIG_NUS);
ui_osc.ed_nus->set_data((unsigned char*)&curr_ed_par_trk.curr_us);
ui_osc.ed_nus->set_min_max(MIN_NUM_US,MAX_NUM_US);
ui_osc.ed_nus->set_ena_enter_on_off(true);
ui_osc.ed_nus->set_ena_edit(on_ed_nus);
ui_osc.ed_nus->set_shift(1);
ui_osc.ed_nus->show_par();
connect(ui_osc.ed_nus, SIGNAL(param_changed()), this, SLOT(nus_changed()));
//++++ kus ++++++
ui_osc.ed_kus->set_num_dig(NUM_DIG_KUS);
ui_osc.ed_kus->set_data((unsigned char*)&_CurrUs.kus1);
ui_osc.ed_kus->set_min_max(MIN_KUS,MAX_KUS);
ui_osc.ed_kus->set_ena_enter_on_off(false);
ui_osc.ed_kus->show_par();
connect(ui_osc.ed_kus, SIGNAL(param_changed()), this, SLOT(kus_changed()));
//++++ tt ++++++
///ui_osc.ed_tt->set_num_dig(NUM_DIG_TT);
///ui_osc.ed_tt->set_data((unsigned char*)&_CurrUs.taut);
///ui_osc.ed_tt->set_min_max(MIN_TT,MAX_TT);
///ui_osc.ed_tt->set_ena_enter_on_off(false);
///ui_osc.ed_tt->show_par();
///connect(ui_osc.ed_tt, SIGNAL(param_changed()), this, SLOT(tt_changed()));
//++++ tv ++++++
///ui_osc.ed_tv->set_num_dig(NUM_DIG_TV);
///ui_osc.ed_tv->set_data((unsigned char*)&_CurrUs.tauv);
///ui_osc.ed_tv->set_min_max(MIN_TV,MAX_TV);
///ui_osc.ed_tv->set_ena_enter_on_off(false);
///ui_osc.ed_tv->show_par();
///connect(ui_osc.ed_tv, SIGNAL(param_changed()), this, SLOT(tv_changed()));
//++++ nstrob ++++++
#if 0
ui_osc.ed_nstrob->set_num_dig(NUM_DIG_NSTROB);
ui_osc.ed_nstrob->set_data((unsigned char*)&curr_ed_par_trk.curr_strob);
ui_osc.ed_nstrob->set_min_max(MIN_NSTROB,MAX_NSTROB);
ui_osc.ed_nstrob->set_ena_enter_on_off(true);
ui_osc.ed_nstrob->set_shift(1);
ui_osc.ed_nstrob->show_par();
connect(ui_osc.ed_nstrob, SIGNAL(param_changed()), this, SLOT(nstrob_changed()));
#endif
//++++ por ++++++
ui_osc.ed_por->set_num_dig(NUM_DIG_POR);
ui_osc.ed_por->set_data((unsigned char*)&_CurrUs.strb[curr_ed_par_trk.curr_strob].por);
ui_osc.ed_por->set_min_max(MIN_POR,MAX_POR);
ui_osc.ed_por->set_ena_enter_on_off(false);
ui_osc.ed_por->show_par();
connect(ui_osc.ed_por, SIGNAL(param_changed()), this, SLOT(por_changed()));


ui_osc.ed_bstr_sp->set_num_dig(3);
ui_osc.ed_bstr_sp->set_data(&spectr_first_el);
ui_osc.ed_bstr_sp->set_min_max(0, (256 - spectr_length));
ui_osc.ed_bstr_sp->show_par();
connect(ui_osc.ed_bstr_sp, SIGNAL(param_changed()), this, SLOT(spectr_bstrob_changed()));




ui_osc.ed_lstr_sp->set_num_dig(3);
ui_osc.ed_lstr_sp->set_data(&spectr_length);
ui_osc.ed_lstr_sp->set_min_max(8, (256 - spectr_first_el));
ui_osc.ed_lstr_sp->show_par();
connect(ui_osc.ed_lstr_sp, SIGNAL(param_changed()), this, SLOT(spectr_lstrob_changed()));


ui_osc.ed_spectr_const->set_num_dig(3);
ui_osc.ed_spectr_const->set_data(&spectr_constant);
ui_osc.ed_spectr_const->set_min_max(0, 255);
ui_osc.ed_spectr_const->show_par();


#if 0

//++++ ngen ++++++
ui_osc.ed_ngen->set_num_dig(NUM_DIG_NGEN);
ui_osc.ed_ngen->set_data((unsigned char*)&curr_ed_par_trk.curr_gen);
ui_osc.ed_ngen->set_min_max(MIN_NUM_GEN,MAX_NUM_GEN);
ui_osc.ed_ngen->set_ena_enter_on_off(true);
ui_osc.ed_ngen->set_shift(1);
ui_osc.ed_ngen->show_par();
connect(ui_osc.ed_ngen, SIGNAL(param_changed()), this, SLOT(ngen_changed()));
//++++ len imp ++++++
ui_osc.ed_len_imp->set_num_dig(NUM_DIG_LEN_IMP);
ui_osc.ed_len_imp->set_data((unsigned char*)&_CurrNgen.len_imp);
ui_osc.ed_len_imp->set_min_max(MIN_LEN_IMP,MAX_LEN_IMP);
ui_osc.ed_len_imp->set_ena_enter_on_off(false);
ui_osc.ed_len_imp->show_par();
connect(ui_osc.ed_len_imp, SIGNAL(param_changed()), this, SLOT(leni_changed()));
//++++ npow ++++++
ui_osc.comboBox_npow->setCurrentIndex(curr_ed_par_trk.curr_pow);
connect(ui_osc.comboBox_npow, SIGNAL(currentIndexChanged(int)), this, SLOT(npow_changed(int)));

//++++ vpow ++++++
ui_osc.comboBox_val_pow->setCurrentIndex( calc_val_pow_Index(_CurrPow.val) );
connect(ui_osc.comboBox_val_pow, SIGNAL(currentIndexChanged(int)), this, SLOT(vpow_changed(int)));


/*ui_osc.ed_val_pow->set_num_dig(NUM_DIG_VPOW);
ui_osc.ed_val_pow->set_data((unsigned char*)&_CurrPow.val); 
ui_osc.ed_val_pow->set_min_max(MIN_VPOW,MAX_VPOW);
ui_osc.ed_val_pow->set_ena_enter_on_off(false);
ui_osc.ed_val_pow->show_par();
connect(ui_osc.ed_val_pow, SIGNAL(param_changed()), this, SLOT(vpow_changed()));

*/


//++++ npov ++++++
ui_osc.ed_np_vrch->set_num_dig(NUM_DIG_NPVRCH);
ui_osc.ed_np_vrch->set_data((unsigned char*)&curr_ed_par_trk.curr_pvrch);
ui_osc.ed_np_vrch->set_min_max(MIN_NPVRCH,MAX_NPVRCH);
ui_osc.ed_np_vrch->set_ena_enter_on_off(true);
ui_osc.ed_np_vrch->show_par();
connect(ui_osc.ed_np_vrch, SIGNAL(param_changed()), this, SLOT(numpv_changed()));
//++++ tvrch ++++++
ui_osc.ed_timev->set_num_dig(NUM_DIG_TVRCH);
ui_osc.ed_timev->set_coeff(10);
ui_osc.ed_timev->set_data((unsigned short*)&_CurrUs.vrch.time[curr_ed_par_trk.curr_pvrch]);
ui_osc.ed_timev->set_min_max(MIN_TVRCH,MAX_TVRCH);
ui_osc.ed_timev->set_ena_enter_on_off(false);
ui_osc.ed_timev->show_par();
connect(ui_osc.ed_timev, SIGNAL(param_changed()), this, SLOT(vtime_changed()));
//++++ kvrch ++++++
ui_osc.ed_kusv->set_num_dig(NUM_DIG_KVRCH);
ui_osc.ed_kusv->set_data((unsigned char*)&_CurrUs.vrch.kus[curr_ed_par_trk.curr_pvrch]);
ui_osc.ed_kusv->set_min_max(MIN_KVRCH,MAX_KVRCH);
ui_osc.ed_kusv->set_ena_enter_on_off(false);
ui_osc.ed_kusv->show_par();
connect(ui_osc.ed_kusv, SIGNAL(param_changed()), this, SLOT(vkus_changed()));

#endif
///ui_osc.ed_begr
}

void ugen_z_dbg::slot_gen_on_off()
{
#if 0
if(!en_set_device_params)
	return;

if(_CurrNgen.on_gen)
	{
	_CurrNgen.on_gen=OFF;
	ui_osc.ed_ngen->set_colors(NGEN_BG_COLOR,NGEN_OFF_COLOR);
	}
else
	{
	_CurrNgen.on_gen=ON;
	ui_osc.ed_ngen->set_colors(NGEN_BG_COLOR,NGEN_ON_COLOR);
	}

ui_osc.ed_ngen->show_par();
show_on_gens();
g_changed_param |= CHNG_ONGEN;
qDebug() << "slot_gens_on_off";
#endif
}

void ugen_z_dbg::slot_us_on_off()
{
#if 0
if(!en_set_device_params)
	return;

if(_CurrUs.on_us)
	{
	_CurrUs.on_us=OFF;
	ui_osc.ed_nus->set_colors(NUS_BG_COLOR,NUS_OFF_COLOR);
	}
else
	{
	_CurrUs.on_us=ON;
	ui_osc.ed_nus->set_colors(NUS_BG_COLOR,NUS_ON_COLOR);
	}
ui_osc.ed_nus->show_par();
show_on_uss();
g_changed_param |= CHNG_ONUS;
qDebug() << "slot_us_on_off";
#endif
}

void ugen_z_dbg::req_timer_timeout()
{
qDebug() << "req_timer_timeout";
}
 ///QCoreApplication::applicationDirPath()+"//qwin_vic.ini"
///+++++++++++++++++++++++++++++++++++++++++++++++++++
bool ugen_z_dbg::load_from_file_trk_params(int num)
{
QFile load_file;
quint64 len;
par_trk_t t_param;
QString tstr1;
///QString tstr=QCoreApplication::applicationDirPath();
///tstr1.sprintf("/params/trk_params%d.ini",num);
QString tstr=ParamDirName;
tstr1.sprintf("/trk_params%d.ini",num);
tstr += tstr1;
load_file.setFileName(tstr);
if(!load_file.open(QIODevice::ReadOnly))
	return false;
len=load_file.read((char*)&t_param,sizeof(par_trk_t));
load_file.close();
if(len==sizeof(par_trk_t))
	{
	memcpy(&curr_param,&t_param,sizeof(par_trk_t));
	return true;
	}
return false;
}
bool ugen_z_dbg::save_to_file_trk_params(int num)
{
QFile save_file;
QString tstr1;
////QString tstr=QCoreApplication::applicationDirPath();

QString tstr=ParamDirName;
tstr1.sprintf("/trk_params%d.ini",num);
tstr += tstr1;
save_file.setFileName(tstr);
if(!save_file.open(QIODevice::WriteOnly))
	return false;
save_file.write((const char*)&curr_param,sizeof(par_trk_t));
save_file.close();
return true;
}

bool ugen_z_dbg::load_params(void)
{
if(!load_from_file_trk_params(curr_ed_par_trk.curr_nparam))
	{
/////????	memcpy(&curr_par_trk,&def_param_contr,sizeof(par_trk_t));
	}
return true;
}
bool ugen_z_dbg::save_params(void)
{
return save_to_file_trk_params(curr_ed_par_trk.curr_nparam);
}
///+++++++++++++++++++++++++++++++++++++++++++++++++++
void ugen_z_dbg::on_butt_save_par()
{
DialLoadSavePar *dial_ld_save= new DialLoadSavePar;
dial_ld_save->set_butt_text(tr("\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214"));
dial_ld_save->set_path(ParamDirName);
dial_ld_save->set_num_par(curr_ed_par_trk.curr_nparam);
dial_ld_save->show();
if (dial_ld_save->exec() == QDialog::Accepted) 
	{
	dial_ld_save->get_path(&ParamDirName);
	dial_ld_save->get_num_par((int*)&curr_ed_par_trk.curr_nparam);
	saveSettings();
	save_params();
	}
////else
////	qDebug()<<"on_butt_load_par().no";
delete dial_ld_save;
}
void ugen_z_dbg::show_all_par(void)
{
ui_osc.ed_chan->show_par();
///ui_osc.ed_tact->show_par();
ui_osc.ed_pre->show_par();

////ui_osc.ed_endr->show_par(curr_ed_par_trk.curr_endr);

ui_osc.ed_nus->show_par();
ui_osc.ed_kus->show_par();
///ui_osc.ed_tt->show_par();
///ui_osc.ed_tv->show_par();
///ui_osc.ed_nstrob->show_par();
ui_osc.ed_por->show_par();
ui_osc.ed_bstr->show_par();
ui_osc.ed_lstr->show_par();
///ui_osc.ed_ngen->show_par();
///ui_osc.ed_len_imp->show_par();


///ui_osc.comboBox_npow->setCurrentIndex(curr_ed_par_trk.curr_pow);
////ui_osc.comboBox_val_pow->setCurrentIndex(calc_val_pow_Index(_CurrPow.val));

/*
ui_osc.ed_val_pow->show_par();
*/
}

void ugen_z_dbg::on_butt_load_par()
{
DialLoadSavePar *dial_ld_save= new DialLoadSavePar;
dial_ld_save->set_butt_text(tr("\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214"));
dial_ld_save->set_path(ParamDirName);
dial_ld_save->set_num_par(curr_ed_par_trk.curr_nparam);
dial_ld_save->show();
if (dial_ld_save->exec() == QDialog::Accepted) 
	{
	dial_ld_save->get_path(&ParamDirName);
	dial_ld_save->get_num_par((int*)&curr_ed_par_trk.curr_nparam);
	saveSettings();
	load_params();
	show_all_par();
	draw_all_osc_parm();
	}
////else
////	qDebug()<<"on_butt_load_par().no";
delete dial_ld_save;
}
void	ugen_z_dbg::rej_changed(int rej)
{
#if 0
curr_ed_par_trk.rej_osc=rej;
new_resp.video_mode = rej;
g_changed_param |= CHNG_REJ_OSC;



QList<int> splitterSizes;


		if(rej)
		{
			ui_osc.AutoPlotter->show();
			SpectrWidget->hide();
			splitterSizes<< 1 << 1 << 0;
			ui_osc.splitter->setSizes(splitterSizes);
		}
		else
		{
			ui_osc.AutoPlotter->hide();
			SpectrWidget->show();
			splitterSizes<< 1 << 0 << 1;
			ui_osc.splitter->setSizes(splitterSizes);
		}


#endif
}
void	ugen_z_dbg::chan_changed()
{
set_chan_ed_params();
g_changed_param |= CHNG_CHAN;
///g_changed_param |= CHNG_TACT;
g_changed_param |= CHNG_NUS;

///qDebug()<<"chan_changed"<<chan;
}
void	ugen_z_dbg::tact_changed()
{
////curr_ed_par_trk.curr_tact=tact.toInt();
////curr_param.cn_list.cn_info[curr_ed_par_trk.curr_chan].ntact=curr_ed_par_trk.curr_tact;
#if 0
set_tact_ed_params(curr_ed_par_trk.curr_tact);
g_changed_param |= CHNG_TACT;
draw_all_osc_parm();
#endif
///qDebug()<<"tact_changed"<<tact;
}
void	ugen_z_dbg::nus_changed()
{
//curr_ed_par_trk.curr_us=nus.toInt();
////curr_param.cn_list.cn_info[curr_ed_par_trk.curr_chan].nus=curr_ed_par_trk.curr_us;
set_us_ed_params(curr_ed_par_trk.curr_us);
g_changed_param |= CHNG_NUS;

///qDebug()<<"nus_changed"<<nus;
}
void	ugen_z_dbg::kus_changed()
{
g_changed_param |= CHNG_KUS;
///qDebug()<<"nus_changed"<<kus;
}
void	ugen_z_dbg::pre_changed()
{
g_changed_param |= CHNG_PRE;
///new_resp.pre=curr_ed_par_trk.curr_begr ;

///curr_ed_par_trk.curr_endr = calc_endr(curr_ed_par_trk.curr_begr,curr_ed_par_trk.curr_step_r);
///ui_osc.ed_pre->show_par(curr_ed_par_trk.curr_endr);
}
void	ugen_z_dbg::rng_changed()
{
g_changed_param |= CHNG_RNG;
///new_resp.pre=curr_ed_par_trk.curr_begr ;

///curr_ed_par_trk.curr_endr = calc_endr(curr_ed_par_trk.curr_begr,curr_ed_par_trk.curr_step_r);
///ui_osc.ed_pre->show_par(curr_ed_par_trk.curr_endr);
}
void	ugen_z_dbg::len_def_changed()
{
g_changed_param |= CHNG_LEN_DEF;
///new_resp.pre=curr_ed_par_trk.curr_begr ;

///curr_ed_par_trk.curr_endr = calc_endr(curr_ed_par_trk.curr_begr,curr_ed_par_trk.curr_step_r);
///ui_osc.ed_pre->show_par(curr_ed_par_trk.curr_endr);
}

#if 0
void	ugen_z_dbg::endr_changed()
{
curr_ed_par_trk.curr_endr = calc_endr(curr_ed_par_trk.curr_begr,curr_ed_par_trk.curr_step_r);
new_resp.step_r=curr_ed_par_trk.curr_step_r;

ui_osc.ed_endr->show_par(curr_ed_par_trk.curr_endr);

g_changed_param |= CHNG_ENDR;
}

void	ugen_z_dbg::endr_result_changed()
{
curr_ed_par_trk.curr_endr = ui_osc.ed_endr->GetResult();

if(curr_ed_par_trk.curr_endr < MIN_ENDR)
	curr_ed_par_trk.curr_endr = MIN_ENDR;
else if(curr_ed_par_trk.curr_endr > MAX_ENDR)
	curr_ed_par_trk.curr_endr = MAX_ENDR;


curr_ed_par_trk.curr_step_r=calc_step(curr_ed_par_trk.curr_begr,curr_ed_par_trk.curr_endr);
new_resp.step_r=curr_ed_par_trk.curr_step_r;

curr_ed_par_trk.curr_endr = calc_endr(curr_ed_par_trk.curr_begr,curr_ed_par_trk.curr_step_r);
ui_osc.ed_endr->show_par(curr_ed_par_trk.curr_endr);


g_changed_param |= CHNG_ENDR;
}


void	ugen_z_dbg::tt_changed()
{
g_changed_param |= CHNG_TT;

//qDebug()<<"tt_changed"<<_CurrUs.taut;
}
void	ugen_z_dbg::tv_changed()
{
g_changed_param |= CHNG_TV;

//qDebug()<<"tv_changed"<<_CurrUs.tauv;
}
#endif
void	ugen_z_dbg::nstrob_changed()
{
g_changed_param |= CHNG_NSTRB;
new_resp.num_sel_strb=curr_ed_par_trk.curr_strob;
set_strob_ed_params(curr_ed_par_trk.curr_strob);

///qDebug()<<"nstrob_changed"<<nstrob;
}
void	ugen_z_dbg::bstrob_changed()
{
g_changed_param |= CHNG_BSTRB;
new_resp.strb[curr_ed_par_trk.curr_strob].beg= _CurrUs.strb[curr_ed_par_trk.curr_strob].beg;

//emit change_param(g_changed_param);
///qDebug()<<"bstrob_changed"<<bstrob;
}
void	ugen_z_dbg::lstrob_changed()
{
g_changed_param |= CHNG_LSTRB;
new_resp.strb[curr_ed_par_trk.curr_strob].len= _CurrUs.strb[curr_ed_par_trk.curr_strob].len;
//emit change_param(g_changed_param);
////qDebug()<<"lstrob_changed"<<lstrob;
}
void	ugen_z_dbg::por_changed()
{
g_changed_param |= CHNG_POR;
new_resp.strb[curr_ed_par_trk.curr_strob].por= _CurrUs.strb[curr_ed_par_trk.curr_strob].por;
// change_param(g_changed_param);
///qDebug()<<"lstrob_changed"<<por;
}
void	ugen_z_dbg::numpv_changed()
{
////g_changed_param |= CHNG_NUMPV;
}
void	ugen_z_dbg::vron_changed(int vron)
{
#if 0
_CurrUs.vrch.on_vrch=vron;
set_vrch_ed_params();
g_changed_param |= CHNG_VRON;
#endif
//emit change_param(g_changed_param);
//qDebug()<<"vron_changed"<<vron;
}
void	ugen_z_dbg::vtime_changed()
{
///g_changed_param |= CHNG_VTIME;
//emit change_param(g_changed_param);
//qDebug()<<"vtime_changed"<<vtime;
}
void	ugen_z_dbg::vkus_changed()
{
////g_changed_param |= CHNG_VKUS;
//emit change_param(g_changed_param);
////qDebug()<<"vkus_changed"<<vkus;
}
void	ugen_z_dbg::ngen_changed()
{
//quint8 tst=ngen.toInt();
///set_gen_ed_params(curr_ed_par_trk.curr_gen);
///g_changed_param |= CHNG_NGEN;
//emit change_param(g_changed_param);
///qDebug()<<"ngen_changed"<<ngen;
}

void	ugen_z_dbg::leni_changed()
{
///g_changed_param |= CHNG_LIMP;
//emit change_param(g_changed_param);
///qDebug()<<"leni_changed"<<leni;
}

void	ugen_z_dbg::npow_changed(int newVal)
{
#if 0
curr_ed_par_trk.curr_pow = newVal;
set_pow_ed_params(curr_ed_par_trk.curr_pow);
g_changed_param |= CHNG_NPOW;
//emit change_param(g_changed_param);
///qDebug()<<"npow_changed"<<npow;
#endif
}
void	ugen_z_dbg::vpow_changed(int NewVal)
{
#if 0
_CurrPow.val = calc_val_pow(NewVal);
g_changed_param |= CHNG_PVAL;
//emit change_param(g_changed_param);
///qDebug()<<"vpow_changed"<<vpow;
#endif
}

///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void ugen_z_dbg::on_butt_rd_par_dev()
{
par_trk_t t_param_contr;
emit signal_on_off_timer(false );
p_dev_cmd->udp_rd_par_dev((quint8 *)&t_param_contr);
memcpy(&curr_par_trk, &t_param_contr,sizeof(par_trk_t));
emit signal_on_off_timer(true );
show_all_par();
draw_all_osc_parm();


///qDebug()<<"par_contr_t=" << sizeof(par_contr_t)<<"par_trk"<<sizeof(par_trk_t);
///qDebug()<<"par_rele_t=" << sizeof(par_rele_t)<<"cn_list_info_t"<<sizeof(cn_list_info_t);
///qDebug()<<"par_pow_t=" << sizeof(par_pow_t)<<"par_takt_t"<<sizeof(par_takt_t);
///qDebug()<<"par_gen_t=" << sizeof(par_gen_t)<<"par_us_t"<<sizeof(par_us_t);
///qDebug()<<"par_vrch_t=" << sizeof(par_vrch_t)<<"par_strb_t"<<sizeof(par_strb_t);
}

void ugen_z_dbg::on_butt_wr_par_dev()
{
	emit signal_on_off_timer(false );
	p_dev_cmd->udp_wr_par_dev(5);

	emit signal_on_off_timer(true );

	qDebug()<<"on_butt_wr_par_dev";

}
void ugen_z_dbg::on_butt_debug()
{
	dial_dbg->show();
}
void ugen_z_dbg::slot_rd_xil_dat(alt_dat_req_t* odat)
{
	emit signal_on_off_timer(false );
	alt_dat_req_t alt_req;
	alt_req.addr=odat->addr;
	alt_req.nbytes=odat->nbytes;
	p_dev_cmd->udp_put_req_xil(&alt_req);
	p_dev_cmd->udp_get_xil(odat);
	emit put_alt_dat_dial(odat);
	emit signal_on_off_timer(true );
}
void ugen_z_dbg::slot_wr_xil_dat(alt_dat_req_t* idat)
{
	emit signal_on_off_timer(false );
	p_dev_cmd->udp_put_xil(idat);
	emit signal_on_off_timer(true );
}
void ugen_z_dbg::slot_rd_stm_dat(alt_dat_req_t* odat)
{
	emit signal_on_off_timer(false );
	alt_dat_req_t alt_req;
	alt_req.addr=odat->addr;
	alt_req.nbytes=odat->nbytes;
	p_dev_cmd->udp_put_req_stm(&alt_req);
	p_dev_cmd->udp_get_stm(odat);
	emit put_alt_dat_dial(odat);
	emit signal_on_off_timer(true );
}
void ugen_z_dbg::slot_wr_stm_dat(alt_dat_req_t* idat)
{
	emit signal_on_off_timer(false );
	p_dev_cmd->udp_put_stm(idat);
	emit signal_on_off_timer(true );
}
void ugen_z_dbg::slot_rd_udb(alt_dat_req_t* odat)
{
	emit signal_on_off_timer(false );
	alt_dat_req_t alt_req;
	alt_req.addr=odat->addr;
	alt_req.nbytes=odat->nbytes;
	p_dev_cmd->udp_put_req_udb(&alt_req);
	p_dev_cmd->udp_get_udb(odat);
	emit put_alt_dat_dial(odat);
	emit signal_on_off_timer(true );
}
void ugen_z_dbg::slot_wr_udb(alt_dat_req_t* idat)
{
	emit signal_on_off_timer(false );
	p_dev_cmd->udp_put_udb(idat);
	emit signal_on_off_timer(true );
}
void ugen_z_dbg::slot_rd_upow(alt_dat_req_t* odat)
{
	emit signal_on_off_timer(false );
	alt_dat_req_t alt_req;
	alt_req.addr=odat->addr;
	alt_req.nbytes=odat->nbytes;
	p_dev_cmd->udp_put_req_udb(&alt_req);
	p_dev_cmd->udp_get_udb(odat);
	emit put_alt_dat_dial(odat);
	emit signal_on_off_timer(true );
}
void ugen_z_dbg::slot_wr_upow(alt_dat_req_t* idat)
{
	emit signal_on_off_timer(false );
	p_dev_cmd->udp_put_udb(idat);
	emit signal_on_off_timer(true );
}

void ugen_z_dbg::slot_rd_msp_dat(QByteArray odat)
{
	emit signal_on_off_timer(false );
///	alt_dat_req_t alt_req;
///	alt_req.addr=odat->addr;
////	alt_req.nbytes=odat->nbytes;
///	p_dev_cmd->udp_put_req_stm(&alt_req);
////	p_dev_cmd->udp_get_stm(odat);
///	emit put_alt_dat_dial(odat);
	emit signal_on_off_timer(true );
}
void ugen_z_dbg::slot_wr_msp_dat(QByteArray idat)
{
quint8 tmpb[64];
quint16 len=idat.size();
memcpy(tmpb, idat.data(), idat.size());//sizeof(quint8) * 32);
tmpb[len] = 0;

	emit signal_on_off_timer(false );
	p_dev_cmd->udp_put_msp(tmpb,len+1);
	emit signal_on_off_timer(true );
}

#if 0
void ugen_z_dbg::BegOscIntermedChange(int beg)
{
	curr_ed_par_trk.curr_begr = beg;
	ui_osc.ed_pre->show_par();
	new_resp.begr=curr_ed_par_trk.curr_begr ;

	curr_ed_par_trk.curr_endr = calc_endr(curr_ed_par_trk.curr_begr,curr_ed_par_trk.curr_step_r);
	ui_osc.ed_endr->show_par(curr_ed_par_trk.curr_endr);

	UpdateOscPlotter();
}

void ugen_z_dbg::BegOscFinalChange()
{
	g_changed_param |= CHNG_BEGR;
}
#endif

#if 0
void ugen_z_dbg::EndOscIntermedChange(int end)
{
	curr_ed_par_trk.curr_endr = end;

	if(curr_ed_par_trk.curr_endr < MIN_ENDR)
		curr_ed_par_trk.curr_endr = MIN_ENDR;
	else if(curr_ed_par_trk.curr_endr > MAX_ENDR)
		curr_ed_par_trk.curr_endr = MAX_ENDR;

	curr_ed_par_trk.curr_step_r=calc_step(curr_ed_par_trk.curr_begr,curr_ed_par_trk.curr_endr);
	new_resp.step_r=curr_ed_par_trk.curr_step_r;

	curr_ed_par_trk.curr_endr = calc_endr(curr_ed_par_trk.curr_begr,curr_ed_par_trk.curr_step_r);
	ui_osc.ed_endr->show_par(curr_ed_par_trk.curr_endr);

	UpdateOscPlotter();
}

void ugen_z_dbg::EndOscFinalChange()
{
	g_changed_param |= CHNG_ENDR;
}

#endif
void ugen_z_dbg::CheckAndSetStrob(int strob)
{
	if(curr_ed_par_trk.curr_strob != strob)
	{
		curr_ed_par_trk.curr_strob = strob;
		new_resp.num_sel_strb=curr_ed_par_trk.curr_strob;
	}
}


void ugen_z_dbg::SetStrobIntermedPos(int strob, int begin, int porog)
{
	CheckAndSetStrob(strob);

	_CurrUs.strb[curr_ed_par_trk.curr_strob].beg = begin;
	_CurrUs.strb[curr_ed_par_trk.curr_strob].por = porog;

	set_strob_ed_params(curr_ed_par_trk.curr_strob);

	UpdateOscPlotter();
}


void ugen_z_dbg::SetStrobFinalPos()
{
	g_changed_param |= CHNG_NSTRB | CHNG_BSTRB | CHNG_POR;
}



void ugen_z_dbg::SetStrobIntermedLeft(int strob, int begin, int len)
{
	CheckAndSetStrob(strob);

	_CurrUs.strb[curr_ed_par_trk.curr_strob].beg = begin;
	_CurrUs.strb[curr_ed_par_trk.curr_strob].len = len;

	set_strob_ed_params(curr_ed_par_trk.curr_strob);

	UpdateOscPlotter();
}

void ugen_z_dbg::SetStrobFinalLeft()
{
	g_changed_param |= CHNG_NSTRB | CHNG_BSTRB | CHNG_LSTRB;
}

void ugen_z_dbg::SetStrobIntermedRight(int strob, int len)
{
	CheckAndSetStrob(strob);

	_CurrUs.strb[curr_ed_par_trk.curr_strob].len = len;
	set_strob_ed_params(curr_ed_par_trk.curr_strob);

	UpdateOscPlotter();
}

void ugen_z_dbg::SetStrobFinalRight()
{
	g_changed_param |= CHNG_NSTRB | CHNG_LSTRB;
}







void ugen_z_dbg::setSpectrOn(bool new_state)
{
	if(new_state != spectr_mode)
	{
		spectr_mode = new_state;

		ui_osc.label_bstr_sp->setVisible(spectr_mode);
		ui_osc.ed_bstr_sp->setVisible(spectr_mode);
		ui_osc.label_lstr_sp->setVisible(spectr_mode);
		ui_osc.ed_lstr_sp->setVisible(spectr_mode);

		ui_osc.ed_spectr_const->setVisible(spectr_mode);


	}

	new_resp.spectr_mode = spectr_mode;

	if(spectr_mode)
	{
		new_resp.spectr_first_el = spectr_first_el;
		new_resp.spectr_length = spectr_length;
	}

}




void ugen_z_dbg::spectr_bstrob_changed()
{
	ui_osc.ed_lstr_sp->set_min_max(8, (256 - spectr_first_el));

	if(spectr_length > (256 - spectr_first_el))
	{
		spectr_length = 256 - spectr_first_el;
		ui_osc.ed_lstr_sp->show_par();
	}

	new_resp.spectr_first_el = spectr_first_el;
	new_resp.spectr_length = spectr_length;

	calculate_spectr_len();
}

void ugen_z_dbg::spectr_lstrob_changed()
{
	ui_osc.ed_bstr_sp->set_min_max(0, (256 - spectr_length));
	
	if(spectr_first_el > (256 - spectr_length))
	{
		spectr_first_el = 256 - spectr_length;
		ui_osc.ed_bstr_sp->show_par();
	}

	new_resp.spectr_first_el = spectr_first_el;
	new_resp.spectr_length = spectr_length;


	calculate_spectr_len();

}


void ugen_z_dbg::calculate_spectr_len()
{
	float curr_2_pow = log((double)spectr_length) / log(2.0);
	if(curr_2_pow > (int)curr_2_pow)
	{
		curr_2_pow = (int)curr_2_pow + 1.0;
		spectr_buff_len = pow(2, curr_2_pow);
		spectr_buff_beg = (spectr_buff_len - spectr_length) / 2;
	}
	else
	{
		spectr_buff_len = spectr_length;
		spectr_buff_beg = 0;
	}

}





