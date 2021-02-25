#include "q_win_sparrow.h"
#include <QTimer>


void q_win_sparrow::saveSettings()
{
	QSettings settings( QCoreApplication::applicationDirPath()+"//red_pit_control.ini",
					   QSettings::IniFormat);
	settings.setValue("mainWindowGeometry", saveGeometry());
	settings.setValue("plot_arr_length", plot_arr_length);


	settings.setValue("ip_addr", dev_obj.ip_addr);
	 

	const par_contr_t &par_contr = dev_obj.curr_par_contr;
	const par_sent_t &sent_par = par_contr.sent_par;

	settings.setValue("Timp_len", sent_par.Timp_len);
	settings.setValue("Timp_offset", sent_par.Timp_offset);
	settings.setValue("Tcycle", sent_par.Tcycle);
	settings.setValue("Tdef", sent_par.Tdef);
	settings.setValue("kus", sent_par.kus);
	settings.setValue("beg_osc", sent_par.beg_osc);
	settings.setValue("step_osc", sent_par.step_osc);
	settings.setValue("attenuator", sent_par.attenuator);


	settings.setValue("num_periods", par_contr.num_periods);

	settings.setValue("Aimp", par_contr.Aimp);
	settings.setValue("dev_frequency", par_contr.dev_frequency);
	settings.setValue("gaus_enable", par_contr.gaus_enable);


	QVariant variant;
	variant.setValue<QList<int>>(ui.splitter_main->sizes());
	settings.setValue("Splitter_main", variant );
}


void q_win_sparrow::loadSettings()
{
	QSettings settings( QCoreApplication::applicationDirPath()+"//red_pit_control.ini",
					   QSettings::IniFormat);
	restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
	plot_arr_length = settings.value("plot_arr_length", plot_arr_length).toInt();


	dev_obj.ip_addr = settings.value("ip_addr", "192.168.1.117").toString();


	par_contr_t &par_contr = dev_obj.curr_par_contr;
	par_sent_t &sent_par = par_contr.sent_par;

	sent_par.Timp_len = settings.value("Timp_len", sent_par.Timp_len).toInt();
	sent_par.Timp_offset = settings.value("Timp_offset", sent_par.Timp_offset).toInt();
	sent_par.Tcycle = settings.value("Tcycle", sent_par.Tcycle).toInt();
	sent_par.Tdef = settings.value("Tdef", sent_par.Tdef).toInt();
	sent_par.kus = settings.value("kus", sent_par.kus).toInt();
	sent_par.beg_osc = settings.value("beg_osc", sent_par.beg_osc).toInt();
	sent_par.step_osc = settings.value("step_osc", sent_par.step_osc).toInt();
	sent_par.attenuator = settings.value("attenuator", sent_par.attenuator).toInt();


	par_contr.num_periods = settings.value("num_periods", par_contr.num_periods).toInt();

	par_contr.Aimp = settings.value("Aimp", par_contr.Aimp).toInt();
	par_contr.dev_frequency = settings.value("dev_frequency", par_contr.dev_frequency).toFloat();
	par_contr.gaus_enable = settings.value("gaus_enable", par_contr.gaus_enable).toBool();


	QList<int> default_splitter_size;
	default_splitter_size << 200 << 200;
	QVariant default_variant;
	default_variant.setValue<QList<int>>(default_splitter_size);
	ui.splitter_main->setSizes( settings.value("Splitter_main",  default_variant).value<QList<int>>() );

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
	plotter_osc(this, &osc_prop),
	dev_obj(this)
{
	qRegisterMetaTypeStreamOperators< QList<int> >("QList<int>");
    ui.setupUi(this);

    connect(ui.pushButton_start_stop, SIGNAL(clicked()), this, SLOT(OnStartStop()));


    plotter.ConnectToWidget(ui.widget_setted);
    ui.widget_setted->SetPlotter(&plotter);


    plotter_osc.ConnectToWidget(ui.widget_osc);
    ui.widget_osc->SetPlotter(&plotter_osc);



	Params::SetDefaultPar(dev_obj.curr_par_contr);

	loadSettings();
	dev_obj.ApplyImpAmlToPar();
	RecalculateImpulse();


	ui.lineEdit_ip->setText(dev_obj.ip_addr);
////=================================================
	ui.comboBox_Dac_Rej->addItem("DDS",DAC_DDS);
	ui.comboBox_Dac_Rej->addItem("RAM",DAC_RAM);
	ui.comboBox_Dac_Rej->addItem("TST1",DAC_TST1);
	connect(ui.comboBox_Dac_Rej, SIGNAL(currentIndexChanged(int)), this, SLOT(dac_rej_changed()));

	ui.comboBox_Dac_Sync->addItem("No Sync",DAC_NO_SYNC);
	ui.comboBox_Dac_Sync->addItem("Sync",DAC_SYNC);
	ui.comboBox_Dac_Sync->addItem("Ext psk",DAC_EXT_PSK);
	connect(ui.comboBox_Dac_Sync, SIGNAL(currentIndexChanged(int)), this, SLOT(dac_rej_changed()));

	ui.comboBox_Front->addItem("RE",DAC_SYNC_RE);
	ui.comboBox_Front->addItem("FE",DAC_SYNC_FE);
	connect(ui.comboBox_Front, SIGNAL(currentIndexChanged(int)), this, SLOT(dac_rej_changed()));

///=============================================



	//ui.pushButt_debug->hide();
	connect(ui.pushButt_debug, SIGNAL(clicked()), this, SLOT(butt_debug()));

	connect(this, SIGNAL(put_xil_dat_dial(xil_dat_req_t*)), &dial_dbg, SLOT(req_data_rdy(xil_dat_req_t*)));
	connect(this, SIGNAL(put_dac_dat_dial(dac_spi_req_t*)), &dial_dbg, SLOT(req_data_rdy(dac_spi_req_t*)));
	connect(this, SIGNAL(put_adc_dat_dial(adc_spi_req_t*)), &dial_dbg, SLOT(req_data_rdy(adc_spi_req_t*)));




	connect(&dial_dbg, SIGNAL(req_rd_xil(xil_dat_req_t*)), this, SLOT(slot_rd_xil_dat(xil_dat_req_t*)));
	connect(&dial_dbg, SIGNAL(req_wr_xil(xil_dat_req_t*)), this, SLOT(slot_wr_xil_dat(xil_dat_req_t*)));
	connect(&dial_dbg, SIGNAL(req_rd_dac(dac_spi_req_t*)), this, SLOT(slot_rd_dac_dat(dac_spi_req_t*)));
	connect(&dial_dbg, SIGNAL(req_wr_dac(dac_spi_req_t*)), this, SLOT(slot_wr_dac_dat(dac_spi_req_t*)));
	connect(&dial_dbg, SIGNAL(req_wr_adc(adc_spi_req_t*)), this, SLOT(slot_wr_adc_dat(adc_spi_req_t*)));
	connect(&dial_dbg, SIGNAL(req_rd_adc(adc_spi_req_t*)), this, SLOT(slot_rd_adc_dat(adc_spi_req_t*)));

	

	ui.ed_osc_length->set_num_dig(NUM_DIG_QUINT16);
	ui.ed_osc_length->set_data(reinterpret_cast<unsigned short*>(&plot_arr_length));
	ui.ed_osc_length->set_min_max(MIN_LENGTH, MAX_LENGTH);
	ui.ed_osc_length->show_par();
	connect(ui.ed_osc_length, SIGNAL(param_changed()), this, SLOT(osc_length_changed()));



	par_contr_t *p_par_contr = &dev_obj.curr_par_contr;
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



	ui.ed_kus->set_num_dig(NUM_DIG_QUINT16);
	ui.ed_kus->set_data(reinterpret_cast<unsigned short*>(&p_sent_par->kus));
	ui.ed_kus->set_min_max(MIN_KUS, MAX_KUS);
	ui.ed_kus->show_par();
	connect(ui.ed_kus, SIGNAL(param_changed()), this, SLOT(kus_changed()));



	ui.ed_beg_osc->set_num_dig(NUM_DIG_QUINT16);
	ui.ed_beg_osc->set_data(reinterpret_cast<unsigned short*>(&p_sent_par->beg_osc));
	ui.ed_beg_osc->set_min_max(MIN_BEG_OSC, MAX_BEG_OSC);
	ui.ed_beg_osc->show_par();
	connect(ui.ed_beg_osc, SIGNAL(param_changed()), this, SLOT(beg_osc_changed()));

	ui.ed_step_osc->set_num_dig(NUM_DIG_QUINT16);
	ui.ed_step_osc->set_data(reinterpret_cast<unsigned short*>(&p_sent_par->step_osc));
	ui.ed_step_osc->set_min_max(MIN_STEP_OSC, MAX_STEP_OSC);
	ui.ed_step_osc->show_par();
	connect(ui.ed_t_def, SIGNAL(param_changed()), this, SLOT(step_osc_changed()));







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


	ui.checkBox_attenuation_lev0->setChecked(p_sent_par->attenuator & FLAG_ATTENUAT_LEV_0);
	connect(ui.checkBox_attenuation_lev0, SIGNAL(clicked()), this, SLOT(AttenLev0Clicked()));

	ui.checkBox_attenuation_lev1->setChecked(p_sent_par->attenuator & FLAG_ATTENUAT_LEV_1);
	connect(ui.checkBox_attenuation_lev1, SIGNAL(clicked()), this, SLOT(AttenLev1Clicked()));


	plot_array.fill(0);
	plot_array.resize(plot_arr_length);
	ImpulseToPlot();



	QTimer::singleShot(10, this, SLOT(InitPlot()));
}

q_win_sparrow::~q_win_sparrow()
{
	dev_obj.ip_addr = ui.lineEdit_ip->text();
	saveSettings();
	plot_array.clear();
}


void q_win_sparrow::InitPlot()
{
	plotter.PlotRespond(plot_array.data(), plot_arr_length);
}



void q_win_sparrow::OnStartStop()
{
	if(dev_obj.IsAttached())
	{
		dev_obj.StopDevice();
		ui.pushButton_start_stop->setChecked(false);
		ui.pushButton_start_stop->setText(tr("\320\237\321\203\321\201\320\272"));
		ui.label_con_state->setText(tr("\320\222\320\253\320\232\320\233"));
	}
	else
	{
		dev_obj.ip_addr = ui.lineEdit_ip->text();
		dev_obj.SetupDevice();
		ui.pushButton_start_stop->setChecked(true);
		ui.pushButton_start_stop->setText(tr("\320\241\321\202\320\276\320\277"));
		ui.label_con_state->setText(tr("\320\241\320\236\320\225\320\224"));
	}
}

void q_win_sparrow::SetConnStatus()
{
}

void q_win_sparrow::EndInitConnection()		// ???
{
	ui.label_con_state->setText(tr("\320\222\320\232\320\233"));
}

void q_win_sparrow::NoConnection()
{
	dev_obj.StopDevice();

	ui.pushButton_start_stop->setChecked(false);
	ui.pushButton_start_stop->setText(tr("\320\237\321\203\321\201\320\272"));
	ui.label_con_state->setText(tr("\320\222\320\253\320\232\320\233"));
}


void q_win_sparrow::slot_draw_osc()
{
	plotter_osc.PlotRespond(&dev_obj.osc_struct);
}

void q_win_sparrow::slot_new_ampl()
{
	QString ampl_str;
	ampl_str.sprintf("%x",dev_obj.ampl_val);

	ui.label_ampl_val->setText(ampl_str);
}




void q_win_sparrow::osc_length_changed()
{
	plot_array.resize(plot_arr_length);
	ImpulseToPlot();
	plotter.PlotRespond(plot_array.data(), plot_arr_length);
}


void q_win_sparrow::Timp_len_changed()
{
	dev_obj.ApplyImpAmlToPar();
	RecalculateImpulse();
	ImpulseToPlot();

	plotter.PlotRespond(plot_array.data(), plot_arr_length);


	dev_obj.g_changed_param |= CHNG_TIMP_LEN | CHNG_IMP_POINTS;
}

void q_win_sparrow::Timp_offset_changed()
{
	ImpulseToPlot();
	plotter.PlotRespond(plot_array.data(), plot_arr_length);

	dev_obj.g_changed_param |= CHNG_TIMP_OFFSET;
}

void q_win_sparrow::Tcycle_changed()
{
	dev_obj.g_changed_param |= CHNG_TCYCLE;
}

void q_win_sparrow::Tdef_changed()
{
	dev_obj.g_changed_param |= CHNG_TDEF;
}

void q_win_sparrow::kus_changed()
{
	dev_obj.g_changed_param |= CHNG_KUS;
}

void q_win_sparrow::beg_osc_changed()
{
	dev_obj.g_changed_param |= CHNG_BEG_OSC;
}

void q_win_sparrow::step_osc_changed()
{
	dev_obj.g_changed_param |= CHNG_STEP_OSC;
}

void q_win_sparrow::dac_rej_changed()
{
quint16 tmp;
par_contr_t &par_contr = dev_obj.curr_par_contr;
///par_contr.sent_par.rej &= ~0x7;
switch(ui.comboBox_Dac_Rej->currentIndex())
	{
	case DAC_RAM:
		par_contr.rej_dac=DAC_REJ_RAM;
		break;
	case DAC_TST1:
		par_contr.rej_dac=DAC_REJ_TST1;
		break;
	default:
		par_contr.rej_dac=DAC_REJ_DDS;
		break;
	}
switch(ui.comboBox_Dac_Sync->currentIndex())
	{
	case DAC_SYNC:
		par_contr.rej_sync_dac=DAC_SYNC;
		break;
	case DAC_EXT_PSK:
		par_contr.rej_sync_dac=DAC_EXT_PSK;
		break;
	default:
		par_contr.rej_sync_dac=DAC_NO_SYNC;
		break;
	}
switch(ui.comboBox_Front->currentIndex())
	{
	case DAC_SYNC_FE:
		par_contr.rej_ext_sync=DAC_SYNC_FE;
		break;
	default:
		par_contr.rej_ext_sync=DAC_SYNC_RE;
		break;
	}

tmp= (par_contr.rej_dac&0x7)+ ((par_contr.rej_sync_dac&0x7)<<3) + ((par_contr.rej_ext_sync&0x3)<<5);
par_contr.sent_par.rej=tmp;
dev_obj.g_changed_param |= CHNG_DAC_REJ;
}


void q_win_sparrow::NumPeriods_changed()
{
	par_contr_t &par_contr = dev_obj.curr_par_contr;


	float curr_period_len = COEF_PERIOD_TRANSF / par_contr.dev_frequency;
	curr_period_len *= par_contr.num_periods;


	par_contr.sent_par.Timp_len = curr_period_len;		// число тиков после изменения
	dev_obj.ApplyImpAmlToPar();

	ui.ed_t_imp_len->show_par();


	RecalculateImpulse();
	ImpulseToPlot();

	plotter.PlotRespond(plot_array.data(), plot_arr_length);


	dev_obj.g_changed_param |= CHNG_TIMP_LEN | CHNG_IMP_POINTS;
}


void q_win_sparrow::Aimp_changed()
{
	RecalculateImpulse();
	ImpulseToPlot();
	plotter.PlotRespond(plot_array.data(), plot_arr_length);

	dev_obj.g_changed_param |= CHNG_IMP_POINTS;
}


void q_win_sparrow::DevFreq_changed()
{
	par_contr_t &par_contr = dev_obj.curr_par_contr;

	par_contr.dev_frequency = static_cast<float>(dev_freq) / COEF_DEV_FREQ;
#if 0
	float curr_period_len = COEF_PERIOD_TRANSF / par_contr.dev_frequency;
	curr_period_len *= par_contr.num_periods;


	par_contr.sent_par.Timp_len = curr_period_len;		// число тиков после изменения
	dev_obj.ApplyImpAmlToPar();

	ui.ed_t_imp_len->show_par();


	RecalculateImpulse();
	ImpulseToPlot();

	plotter.PlotRespond(plot_array.data(), plot_arr_length);


	dev_obj.g_changed_param |= CHNG_TIMP_LEN | CHNG_IMP_POINTS;
#endif
	par_contr.sent_par.freq=(quint32)(par_contr.dev_frequency*10000);
	dev_obj.g_changed_param |= CHNG_FREQ;

}



void q_win_sparrow::AttenLev0Clicked()
{
	par_sent_t &sent_par = dev_obj.curr_par_contr.sent_par;
	sent_par.attenuator = (sent_par.attenuator & ~FLAG_ATTENUAT_LEV_0) | (ui.checkBox_attenuation_lev0->isChecked() ? FLAG_ATTENUAT_LEV_0 : 0);

	dev_obj.g_changed_param |= CHNG_ATTENUATOR;
}

void q_win_sparrow::AttenLev1Clicked()
{
	par_sent_t &sent_par = dev_obj.curr_par_contr.sent_par;
	sent_par.attenuator = (sent_par.attenuator & ~FLAG_ATTENUAT_LEV_1) | (ui.checkBox_attenuation_lev1->isChecked() ? FLAG_ATTENUAT_LEV_1 : 0);

	dev_obj.g_changed_param |= CHNG_ATTENUATOR;
}



void q_win_sparrow::GaussClicked()
{
	dev_obj.curr_par_contr.gaus_enable = ui.checkBox_gauss->isChecked();

	RecalculateImpulse();
	ImpulseToPlot();
	plotter.PlotRespond(plot_array.data(), plot_arr_length);

	dev_obj.g_changed_param |= CHNG_IMP_POINTS;
}





void q_win_sparrow::RecalculateImpulse()
{
	const par_contr_t &par_contr = dev_obj.curr_par_contr;
	const par_sent_t &sent_par = par_contr.sent_par;

	double k = 4.0*log(2.0*par_contr.Aimp)/(static_cast<double>(sent_par.Timp_len)*sent_par.Timp_len);

	// y = A * e^(-k*(x - len/2)^2)

	int len_div2 = sent_par.Timp_len/2;
	for(quint16 i = 0; i < sent_par.Timp_len; i++)
	{
		if(par_contr.gaus_enable)
			dev_obj.imp_ampl[i] = par_contr.Aimp * exp(-k*(i - len_div2)*(i - len_div2)) * sin(2* M_PI*i*par_contr.dev_frequency/COEF_PERIOD_TRANSF);
		else
			dev_obj.imp_ampl[i] = par_contr.Aimp * sin(2* M_PI*i*par_contr.dev_frequency/COEF_PERIOD_TRANSF);
	}

}




void q_win_sparrow::ImpulseToPlot()
{
	const par_contr_t &par_contr = dev_obj.curr_par_contr;
	const par_sent_t &sent_par = par_contr.sent_par;

	qint16 *plot_arr = plot_array.data();

	if(plot_arr_length > (sent_par.Timp_offset + sent_par.Timp_len))
	{
		memset(plot_arr, 0, sizeof(qint16)*sent_par.Timp_offset);
		memcpy(plot_arr + sent_par.Timp_offset, dev_obj.imp_ampl.data(), sizeof(qint16)*sent_par.Timp_len);
		memset(plot_arr + sent_par.Timp_offset + sent_par.Timp_len, 0, sizeof(qint16)*(plot_arr_length - sent_par.Timp_offset - sent_par.Timp_len) );
	}
	else if(plot_arr_length > sent_par.Timp_offset)
	{
		memset(plot_arr, 0, sizeof(qint16)*sent_par.Timp_offset);
		memcpy(plot_arr + sent_par.Timp_offset, dev_obj.imp_ampl.data(), sizeof(qint16)*(plot_arr_length - sent_par.Timp_offset));
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
	if(!dev_obj.IsAttached())
		return;

	dev_obj.UpdateDevice(false);
	xil_dat_req_t xil_req;
	xil_req.addr=odat->addr;
	xil_req.nbytes=odat->nbytes;
	if(dev_obj.p_tune_thr->dev_cmd.dev_put_req_xil(&xil_req))
	{
		if(dev_obj.p_tune_thr->dev_cmd.dev_get_xil(odat))
			emit put_xil_dat_dial(odat);
	}
	
	dev_obj.UpdateDevice(true);
}

void q_win_sparrow::slot_wr_xil_dat(xil_dat_req_t* idat)
{
	if(!dev_obj.IsAttached())
		return;


	dev_obj.UpdateDevice(false );
	dev_obj.p_tune_thr->dev_cmd.dev_put_xil(idat);
	dev_obj.UpdateDevice(true );
}

void q_win_sparrow::slot_rd_dac_dat(dac_spi_req_t* odat)
{
	if(!dev_obj.IsAttached())
		return;

	dev_obj.UpdateDevice(false );
	dac_spi_req_t dac_req;
	dac_req.addr=odat->addr;
	///alt_req.nbytes=odat->nbytes;
	if(dev_obj.p_tune_thr->dev_cmd.dev_put_req_dac(&dac_req))
	{
		if(dev_obj.p_tune_thr->dev_cmd.dev_get_dac(odat))
			emit put_dac_dat_dial(odat);
	}
	dev_obj.UpdateDevice(true );
}

void q_win_sparrow::slot_wr_dac_dat(dac_spi_req_t* idat)
{
	if(!dev_obj.IsAttached())
		return;

	dev_obj.UpdateDevice(false );
	dev_obj.p_tune_thr->dev_cmd.dev_put_dac(idat);
	dev_obj.UpdateDevice(true );
}

void q_win_sparrow::slot_wr_adc_dat(adc_spi_req_t* idat)
{
	if(!dev_obj.IsAttached())
		return;

	dev_obj.UpdateDevice(false );
	dev_obj.p_tune_thr->dev_cmd.dev_put_adc(idat);
	dev_obj.UpdateDevice(true );
}

void q_win_sparrow::slot_rd_adc_dat(adc_spi_req_t* odat)
{
	if(!dev_obj.IsAttached())
		return;

	dev_obj.UpdateDevice(false );
	adc_spi_req_t adc_req;
	adc_req.addr=odat->addr;
	///alt_req.nbytes=odat->nbytes;
	if(dev_obj.p_tune_thr->dev_cmd.dev_put_req_adc(&adc_req))
	{
		if(dev_obj.p_tune_thr->dev_cmd.dev_get_adc(odat))
			emit put_adc_dat_dial(odat);
	}
	dev_obj.UpdateDevice(true );
}




