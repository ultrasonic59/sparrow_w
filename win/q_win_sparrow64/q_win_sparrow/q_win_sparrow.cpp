#include "q_win_sparrow.h"
#include <QTimer>


void q_win_sparrow::saveSettings()
{
	QSettings settings( QCoreApplication::applicationDirPath()+"//red_pit_control.ini",
					   QSettings::IniFormat);
	settings.setValue("mainWindowGeometry", saveGeometry());
	settings.setValue("plot_arr_length", plot_arr_length);


	settings.setValue("ip_addr", device_cmd.ip_addr);
	 

	const par_contr_t &par_contr = device_cmd.curr_par_contr;
	const par_sent_t &sent_par = par_contr.sent_par;

	settings.setValue("Timp_len", sent_par.Timp_len);
	settings.setValue("Timp_offset", sent_par.Timp_offset);
	settings.setValue("Tcycle", sent_par.Tcycle);
	settings.setValue("Tdef", sent_par.Tdef);
	settings.setValue("kus", sent_par.kus);
	settings.setValue("offs", sent_par.offs);
///	settings.setValue("step_osc", sent_par.step_osc);
	settings.setValue("attenuator", sent_par.attenuator);

	settings.setValue("num_periods", par_contr.num_periods);

	settings.setValue("Aimp", par_contr.Aimp);
	settings.setValue("dev_frequency", par_contr.dev_frequency);
	settings.setValue("gaus_enable", par_contr.gaus_enable);
	settings.setValue("dds_enable", par_contr.dds_enable);
	settings.setValue("auto_enable", par_contr.auto_enable);

}


void q_win_sparrow::loadSettings()
{
	QSettings settings( QCoreApplication::applicationDirPath()+"//red_pit_control.ini",
					   QSettings::IniFormat);
	restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
	plot_arr_length = settings.value("plot_arr_length", plot_arr_length).toInt();


	device_cmd.ip_addr = settings.value("ip_addr", "192.168.1.117").toString();


	par_contr_t &par_contr = device_cmd.curr_par_contr;
	par_sent_t &sent_par = par_contr.sent_par;

	sent_par.Timp_len = settings.value("Timp_len", sent_par.Timp_len).toInt();
	sent_par.Timp_offset = settings.value("Timp_offset", sent_par.Timp_offset).toInt();
	sent_par.Tcycle = settings.value("Tcycle", sent_par.Tcycle).toInt();
	sent_par.Tdef = settings.value("Tdef", sent_par.Tdef).toInt();
	sent_par.kus = settings.value("kus", sent_par.kus).toInt();
	sent_par.offs = settings.value("offs", sent_par.offs).toInt();
///	sent_par.step_osc = settings.value("step_osc", sent_par.step_osc).toInt();
	sent_par.attenuator = settings.value("attenuator", sent_par.attenuator).toInt();

	par_contr.num_periods = settings.value("num_periods", par_contr.num_periods).toInt();

	par_contr.Aimp = settings.value("Aimp", par_contr.Aimp).toInt();
	par_contr.dev_frequency = settings.value("dev_frequency", par_contr.dev_frequency).toFloat();
	par_contr.gaus_enable = settings.value("gaus_enable", par_contr.gaus_enable).toBool();
	par_contr.dds_enable = settings.value("dds_enable", par_contr.dds_enable).toBool();
	par_contr.auto_enable = settings.value("auto_enable", par_contr.auto_enable).toBool();

}

q_win_sparrow::q_win_sparrow(QWidget *parent) :
    QMainWindow(parent),
    ui(),
	plot_arr_length(DEF_LENGTH),
	plot_array(plot_arr_length),

	dev_freq(COEF_DEV_FREQ),		// 1.0
	dial_dbg(this),
    osc_prop(),
    plotter(this, &osc_prop),
	device_cmd(this,odata)
{
	qRegisterMetaTypeStreamOperators< QList<int> >("QList<int>");
	qRegisterMetaType<xil_dat_req_t>("xil_dat_req_t");

    ui.setupUi(this);

    connect(ui.pushButton_start_stop, SIGNAL(clicked()), this, SLOT(OnStartStop()));

    plotter.ConnectToWidget(ui.widget_setted);
    ui.widget_setted->SetPlotter(&plotter);

	Params::SetDefaultPar(device_cmd.curr_par_contr);

	loadSettings();
	device_cmd.ApplyImpAmlToPar();
	RecalculateImpulse();
	ui.lineEdit_ip->setText(device_cmd.ip_addr);

	//ui.pushButt_debug->hide();
	connect(ui.pushButt_debug, SIGNAL(clicked()), this, SLOT(butt_debug()));
	connect(this, SIGNAL(put_xil_dat_dial(xil_dat_req_t*)), &dial_dbg, SLOT(req_data_rdy(xil_dat_req_t*)));
	connect(this, SIGNAL(put_dac_dat_dial(xil_dat_req_t*)), &dial_dbg, SLOT(req_data_rdy(xil_dat_req_t*)));

	connect(&dial_dbg, SIGNAL(req_rd_xil(xil_dat_req_t*)), this, SLOT(slot_rd_xil_dat(xil_dat_req_t*)));
	connect(&dial_dbg, SIGNAL(req_wr_xil(xil_dat_req_t)), this, SLOT(slot_wr_xil_dat(xil_dat_req_t)));
	connect(&dial_dbg, SIGNAL(req_rd_dac(xil_dat_req_t*)), this, SLOT(slot_rd_dac_dat(xil_dat_req_t*)));
	connect(&dial_dbg, SIGNAL(req_wr_dac(xil_dat_req_t)), this, SLOT(slot_wr_dac_dat(xil_dat_req_t)));
	connect(this, SIGNAL(put_str_dial(char*)), &dial_dbg, SLOT(req_str_rdy(char*)));

	ui.ed_osc_length->set_num_dig(NUM_DIG_QUINT16);
	ui.ed_osc_length->set_data(reinterpret_cast<unsigned short*>(&plot_arr_length));
	ui.ed_osc_length->set_min_max(MIN_LENGTH, MAX_LENGTH);
	ui.ed_osc_length->show_par();
	connect(ui.ed_osc_length, SIGNAL(param_changed()), this, SLOT(osc_length_changed()));

	par_contr_t *p_par_contr = &device_cmd.curr_par_contr;
	par_sent_t *p_sent_par = &p_par_contr->sent_par;

	ui.ed_t_imp_len->set_num_dig(NUM_DIG_QUINT16);
	ui.ed_t_imp_len->set_data(reinterpret_cast<unsigned short*>(&p_sent_par->Timp_len));
	ui.ed_t_imp_len->set_min_max(MIN_TIMP_LEN, MAX_TIMP_LEN);
	ui.ed_t_imp_len->show_par();
	connect(ui.ed_t_imp_len, SIGNAL(param_changed()), this, SLOT(Timp_len_changed()));

	ui.ed_t_imp_offset->set_num_dig(NUM_DIG_QUINT16);
	ui.ed_t_imp_offset->set_data(reinterpret_cast<unsigned short*>(&p_sent_par->Timp_offset));
	ui.ed_t_imp_offset->set_min_max(MIN_TIMP_OFFSET, MAX_TIMP_OFFSET);
	ui.ed_t_imp_offset->show_par();

	connect(ui.ed_t_imp_offset, SIGNAL(param_changed()), this, SLOT(Timp_offset_changed()));

	ui.ed_t_cycle->set_num_dig(NUM_DIG_QUINT16);
	ui.ed_t_cycle->set_data(reinterpret_cast<unsigned short*>(&p_sent_par->Tcycle));
	ui.ed_t_cycle->set_min_max(MIN_TCYCLE, MAX_TCYCLE);
	ui.ed_t_cycle->show_par();
	connect(ui.ed_t_cycle, SIGNAL(param_changed()), this, SLOT(Tcycle_changed()));

	ui.ed_t_def->set_num_dig(NUM_DIG_QUINT16);
	ui.ed_t_def->set_data(reinterpret_cast<unsigned short*>(&p_sent_par->Tdef));
	ui.ed_t_def->set_min_max(MIN_TDEF, MAX_TDEF);
	ui.ed_t_def->show_par();
	connect(ui.ed_t_def, SIGNAL(param_changed()), this, SLOT(Tdef_changed()));

	ui.ed_a_imp->set_num_dig(NUM_DIG_AIMP);
	ui.ed_a_imp->set_data(reinterpret_cast<unsigned short*>(&p_par_contr->Aimp));
	ui.ed_a_imp->set_min_max(MIN_AIMP, MAX_AIMP);
	ui.ed_a_imp->show_par();
	connect(ui.ed_a_imp, SIGNAL(param_changed()), this, SLOT(Aimp_changed()));

	ui.ed_num_periods->set_num_dig(NUM_DIG_QUINT16);
	ui.ed_num_periods->set_data(reinterpret_cast<unsigned short*>(&p_par_contr->num_periods));
	ui.ed_num_periods->set_min_max(MIN_NUM_PER, MAX_NUM_PER);
	ui.ed_num_periods->show_par();
	connect(ui.ed_num_periods, SIGNAL(param_changed()), this, SLOT(NumPeriods_changed()));

	dev_freq = COEF_DEV_FREQ * p_par_contr->dev_frequency;

	ui.ed_f_imp->set_num_dig(NUM_DIG_DEV_FREQ);
	ui.ed_f_imp->set_coeff(COEF_DEV_FREQ);
	ui.ed_f_imp->set_data(reinterpret_cast<unsigned long*>(&dev_freq));
	ui.ed_f_imp->set_min_max(MIN_DEV_FREQ, MAX_DEV_FREQ);
	ui.ed_f_imp->show_par();
	connect(ui.ed_f_imp, SIGNAL(param_changed()), this, SLOT(DevFreq_changed()));

	ui.checkBox_gauss->setChecked(p_par_contr->gaus_enable);
	connect(ui.checkBox_gauss, SIGNAL(clicked()), this, SLOT(GaussClicked()));

	ui.ed_kus->set_num_dig(NUM_DIG_KUS);
	ui.ed_kus->set_data(reinterpret_cast<unsigned short*>(&p_sent_par->kus));
	ui.ed_kus->set_min_max(MIN_KUS, MAX_KUS);
	ui.ed_kus->show_par();
	connect(ui.ed_kus, SIGNAL(param_changed()), this, SLOT(kus_changed()));

	ui.ed_offs->set_num_dig(NUM_DIG_OFFS);
	ui.ed_offs->set_data(reinterpret_cast<unsigned short*>(&p_sent_par->offs));
	ui.ed_offs->set_min_max(MIN_OFFS, MAX_OFFS);
	ui.ed_offs->show_par();
	connect(ui.ed_offs, SIGNAL(param_changed()), this, SLOT(offs_changed()));

	ui.checkBox_dds->setChecked(p_par_contr->dds_enable);
	connect(ui.checkBox_dds, SIGNAL(clicked()), this, SLOT(dds_clicked()));


	plot_array.fill(0);
	plot_array.resize(plot_arr_length);
	ImpulseToPlot();
	QTimer::singleShot(10, this, SLOT(InitPlot()));
}

q_win_sparrow::~q_win_sparrow()
{
///	device_cmd.ip_addr = ui.lineEdit_ip->text();
	saveSettings();
	plot_array.clear();
}


void q_win_sparrow::InitPlot()
{
	plotter.PlotRespond(plot_array.data(), plot_arr_length);
}

void q_win_sparrow::OnStartStop()
{
	if(device_cmd.IsAttached())
	{

		device_cmd.StartStop(false);
		ui.pushButton_start_stop->setChecked(false);
		ui.pushButton_start_stop->setText(QString::fromLocal8Bit("Пуск"));
		ui.label_con_state->setText(QString::fromLocal8Bit("Не соед."));
		ui.label_con_state->setStyleSheet("");
	}
	else
	{
		device_cmd.ip_addr = ui.lineEdit_ip->text();
		device_cmd.StartStop(true);

		device_cmd.SetupDevice();
		ui.pushButton_start_stop->setChecked(true);
		ui.pushButton_start_stop->setText(QString::fromLocal8Bit("Стоп"));
		ui.label_con_state->setText(QString::fromLocal8Bit("Соед."));
		ui.label_con_state->setStyleSheet("background-color: green;");
	}
}

void q_win_sparrow::SetConnStatus()
{
}

void q_win_sparrow::EndInitConnection()		// ???
{
	ui.label_con_state->setText(tr("vvv5\320\222\320\232\320\233"));
}

void q_win_sparrow::NoConnection()
{
	device_cmd.StartStop(false);

	ui.pushButton_start_stop->setChecked(false);
	ui.pushButton_start_stop->setText(QString::fromLocal8Bit("Пуск"));
	ui.label_con_state->setText(QString::fromLocal8Bit("Выкл"));
}


void q_win_sparrow::osc_length_changed()
{
	plot_array.resize(plot_arr_length);
	ImpulseToPlot();
	plotter.PlotRespond(plot_array.data(), plot_arr_length);
}


void q_win_sparrow::Timp_len_changed()
{
	device_cmd.ApplyImpAmlToPar();
	RecalculateImpulse();
	ImpulseToPlot();

	plotter.PlotRespond(plot_array.data(), plot_arr_length);

	device_cmd.g_changed_param |=  CHNG_IMP_POINTS;
}

void q_win_sparrow::Timp_offset_changed()
{
	ImpulseToPlot();
	plotter.PlotRespond(plot_array.data(), plot_arr_length);

	device_cmd.g_changed_param |= CHNG_TIMP_OFFSET;
}

void q_win_sparrow::Tcycle_changed()
{
	device_cmd.g_changed_param |= CHNG_TCYCLE;
}

void q_win_sparrow::Tdef_changed()
{
	device_cmd.g_changed_param |= CHNG_TDEF;
}


void q_win_sparrow::NumPeriods_changed()
{
	par_contr_t &par_contr = device_cmd.curr_par_contr;
	float curr_period_len = COEF_PERIOD_TRANSF / par_contr.dev_frequency;
	curr_period_len *= par_contr.num_periods;
	par_contr.sent_par.Timp_len = curr_period_len;		// число тиков после изменения
	device_cmd.ApplyImpAmlToPar();
	ui.ed_t_imp_len->show_par();
	RecalculateImpulse();
	ImpulseToPlot();
	plotter.PlotRespond(plot_array.data(), plot_arr_length);
	device_cmd.g_changed_param |=  CHNG_IMP_POINTS;
}


void q_win_sparrow::Aimp_changed()
{
	RecalculateImpulse();
	ImpulseToPlot();
	plotter.PlotRespond(plot_array.data(), plot_arr_length);
	device_cmd.g_changed_param |= CHNG_IMP_POINTS;
}
void q_win_sparrow::kus_changed()
{
	device_cmd.g_changed_param |= CHNG_KUS;
}
void q_win_sparrow::offs_changed()
{
	device_cmd.g_changed_param |= CHNG_OFFS;
}
void q_win_sparrow::dds_clicked()
{
	device_cmd.curr_par_contr.dds_enable = ui.checkBox_dds->isChecked();

///	RecalculateImpulse();
///	ImpulseToPlot();
///	plotter.PlotRespond(plot_array.data(), plot_arr_length);

	device_cmd.g_changed_param |= CHNG_ON_DDS;
}


void q_win_sparrow::DevFreq_changed()
{
	par_contr_t &par_contr = device_cmd.curr_par_contr;

	par_contr.dev_frequency = static_cast<float>(dev_freq) / COEF_DEV_FREQ;
	float curr_period_len = COEF_PERIOD_TRANSF / par_contr.dev_frequency;
	curr_period_len *= par_contr.num_periods;
	par_contr.sent_par.Timp_len = curr_period_len;		// число тиков после изменения
	device_cmd.ApplyImpAmlToPar();

	ui.ed_t_imp_len->show_par();
	RecalculateImpulse();
	ImpulseToPlot();
	plotter.PlotRespond(plot_array.data(), plot_arr_length);
	device_cmd.g_changed_param |=  CHNG_IMP_POINTS;
}


void q_win_sparrow::GaussClicked()
{
	device_cmd.curr_par_contr.gaus_enable = ui.checkBox_gauss->isChecked();

	RecalculateImpulse();
	ImpulseToPlot();
	plotter.PlotRespond(plot_array.data(), plot_arr_length);

	device_cmd.g_changed_param |= CHNG_IMP_POINTS;
}


void q_win_sparrow::RecalculateImpulse()
{
	const par_contr_t &par_contr = device_cmd.curr_par_contr;
	const par_sent_t &sent_par = par_contr.sent_par;

	double k = 4.0*log(2.0*par_contr.Aimp)/(static_cast<double>(sent_par.Timp_len)*sent_par.Timp_len);

	// y = A * e^(-k*(x - len/2)^2)

	int len_div2 = sent_par.Timp_len/2;
	
	for(quint16 i = 0; i < sent_par.Timp_len; i++)
	{
		if(par_contr.gaus_enable)
			device_cmd.imp_ampl[i] = par_contr.Aimp * exp(-k*(i - len_div2)*(i - len_div2)) *  sin(2* M_PI*i*par_contr.dev_frequency/COEF_PERIOD_TRANSF);
		else
			device_cmd.imp_ampl[i] = par_contr.Aimp * sin(2* M_PI*i*par_contr.dev_frequency/COEF_PERIOD_TRANSF);
	}

}
///#define AMP_OFFS 0x2000

void q_win_sparrow::ImpulseToPlot()
{
	const par_contr_t &par_contr = device_cmd.curr_par_contr;
	const par_sent_t &sent_par = par_contr.sent_par;

	qint16 *plot_arr = plot_array.data();

	if(plot_arr_length > (sent_par.Timp_offset + sent_par.Timp_len))
	{
		memset(plot_arr, 0, sizeof(qint16)*sent_par.Timp_offset);
		memcpy(plot_arr + sent_par.Timp_offset, device_cmd.imp_ampl.data(), sizeof(qint16)*sent_par.Timp_len);
		memset(plot_arr + sent_par.Timp_offset + sent_par.Timp_len, 0, sizeof(qint16)*(plot_arr_length - sent_par.Timp_offset - sent_par.Timp_len) );
	}
	else if(plot_arr_length > sent_par.Timp_offset)
	{
		memset(plot_arr, 0, sizeof(qint16)*sent_par.Timp_offset);
		memcpy(plot_arr + sent_par.Timp_offset, device_cmd.imp_ampl.data(), sizeof(qint16)*(plot_arr_length - sent_par.Timp_offset));
	}
	else
		memset(plot_arr, 0, sizeof(qint16)*plot_arr_length);
}

void q_win_sparrow::butt_debug()
{
	dial_dbg.show();
}

void q_win_sparrow::slot_rd_xil_dat(xil_dat_req_t* odat)
{
	if(!device_cmd.IsAttached())
		return;

///	device_cmd.UpdateDevice(false);
	xil_dat_req_t xil_req;
	xil_req.addr=odat->addr;
	xil_req.nbytes=odat->nbytes;
	if (device_cmd.dbg_get_xil(xil_req, odat))
	{
		emit put_xil_dat_dial(odat);
	}
	else
	{
		emit put_str_dial((char*)"eroor dat ");
	}
	/*
	dev_obj.UpdateDevice(true);
	*/
}

void q_win_sparrow::slot_wr_xil_dat(xil_dat_req_t idat)
{
	if(!device_cmd.IsAttached())
		return;


///	dev_obj.UpdateDevice(false );
	device_cmd.dbg_put_xil(idat);
///	dev_obj.UpdateDevice(true );
}

void q_win_sparrow::slot_rd_dac_dat(xil_dat_req_t* odat)
{
	if(!device_cmd.IsAttached())
		return;

///	dev_obj.UpdateDevice(false );
	xil_dat_req_t dac_req;
	dac_req = *odat;

	if (device_cmd.dbg_get_dac(dac_req, odat))
	{
		emit put_dac_dat_dial(odat);
	}
	else
	{
		emit put_str_dial((char*)"eroor dat ");
	}
///	dev_obj.UpdateDevice(true );
}
void q_win_sparrow::slot_wr_dac_dat(xil_dat_req_t idat)
{
	if(!device_cmd.IsAttached())
		return;
///	dev_obj.UpdateDevice(false );
	device_cmd.dbg_put_dac(idat);
///	dev_obj.UpdateDevice(true );
}



