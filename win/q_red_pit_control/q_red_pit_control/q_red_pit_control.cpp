#include "q_red_pit_control.h"
#include <QTimer>


void q_red_pit_control::saveSettings()
{
	QSettings settings( QCoreApplication::applicationDirPath()+"//red_pit_control.ini",
					   QSettings::IniFormat);
	settings.setValue("mainWindowGeometry", saveGeometry());
	settings.setValue("plot_arr_length", plot_arr_length);


	settings.setValue("ip_addr", device_CMD.ip_addr);
	 

	const par_contr_t &par_contr = device_CMD.curr_par_contr;
	const par_sent_t &sent_par = par_contr.sent_par;

	settings.setValue("Timp_len", sent_par.Timp_len);
	settings.setValue("Timp_offset", sent_par.Timp_offset);
	settings.setValue("Tcycle", sent_par.Tcycle);
	settings.setValue("Tdef", sent_par.Tdef);
	settings.setValue("pre", sent_par.pre);


	settings.setValue("num_periods", par_contr.num_periods);

	settings.setValue("Aimp", par_contr.Aimp);
	settings.setValue("dev_frequency", par_contr.dev_frequency);
	settings.setValue("gaus_enable", par_contr.gaus_enable);


}


void q_red_pit_control::loadSettings()
{
	QSettings settings( QCoreApplication::applicationDirPath()+"//red_pit_control.ini",
					   QSettings::IniFormat);
	restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
	plot_arr_length = settings.value("plot_arr_length", plot_arr_length).toInt();


	device_CMD.ip_addr = settings.value("ip_addr", "192.168.1.117").toString();


	par_contr_t &par_contr = device_CMD.curr_par_contr;
	par_sent_t &sent_par = par_contr.sent_par;

	sent_par.Timp_len = settings.value("Timp_len", sent_par.Timp_len).toInt();
	sent_par.Timp_offset = settings.value("Timp_offset", sent_par.Timp_offset).toInt();
	sent_par.Tcycle = settings.value("Tcycle", sent_par.Tcycle).toInt();
	sent_par.Tdef = settings.value("Tdef", sent_par.Tdef).toInt();
	sent_par.pre = settings.value("pre", sent_par.Tdef).toInt();


	par_contr.num_periods = settings.value("num_periods", par_contr.num_periods).toInt();

	par_contr.Aimp = settings.value("Aimp", par_contr.Aimp).toInt();
	par_contr.dev_frequency = settings.value("dev_frequency", par_contr.dev_frequency).toFloat();
	par_contr.gaus_enable = settings.value("gaus_enable", par_contr.gaus_enable).toBool();


}




q_red_pit_control::q_red_pit_control(QWidget *parent) :
    QMainWindow(parent),
    ui(),

	plot_arr_length(DEF_LENGTH),
	plot_array(plot_arr_length),

	dev_freq(COEF_DEV_FREQ),		// 1.0

    osc_prop(),
    plotter(this, &osc_prop),
	device_CMD(this)
{
    ui.setupUi(this);

    connect(ui.pushButton_start_stop, SIGNAL(clicked()), this, SLOT(OnStartStop()));


    plotter.ConnectToWidget(ui.widget_plot);
    ui.widget_plot->SetPlotter(&plotter);


	Params::SetDefaultPar(device_CMD.curr_par_contr);

	loadSettings();
	device_CMD.ApplyImpAmlToPar();
	RecalculateImpulse();


	ui.lineEdit_ip->setText(device_CMD.ip_addr);



	ui.ed_osc_length->set_num_dig(NUM_DIG_QUINT16);
	ui.ed_osc_length->set_data(reinterpret_cast<unsigned short*>(&plot_arr_length));
	ui.ed_osc_length->set_min_max(MIN_LENGTH, MAX_LENGTH);
	ui.ed_osc_length->show_par();
	connect(ui.ed_osc_length, SIGNAL(param_changed()), this, SLOT(osc_length_changed()));



	par_contr_t *p_par_contr = &device_CMD.curr_par_contr;
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

	ui.ed_pre->set_num_dig(NUM_DIG_QUINT16);
	ui.ed_pre->set_data(reinterpret_cast<unsigned short*>(&p_sent_par->pre));
	ui.ed_pre->set_min_max(MIN_TDEF, MAX_TDEF);
	ui.ed_pre->show_par();
	connect(ui.ed_pre, SIGNAL(param_changed()), this, SLOT(pre_changed()));

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


	plot_array.fill(0);
	plot_array.resize(plot_arr_length);
	ImpulseToPlot();



	QTimer::singleShot(10, this, SLOT(InitPlot()));
}

q_red_pit_control::~q_red_pit_control()
{
	device_CMD.ip_addr = ui.lineEdit_ip->text();
	saveSettings();
	plot_array.clear();
}


void q_red_pit_control::InitPlot()
{
	plotter.PlotRespond(plot_array.data(), plot_arr_length);
}



void q_red_pit_control::OnStartStop()
{
	if(device_CMD.IsAttached())
	{
		device_CMD.StopDevice();
		ui.pushButton_start_stop->setChecked(false);
		ui.pushButton_start_stop->setText(tr("\320\237\321\203\321\201\320\272"));
		ui.label_con_state->setText(tr("\320\222\320\253\320\232\320\233"));
	}
	else
	{
		device_CMD.ip_addr = ui.lineEdit_ip->text();
		device_CMD.SetupDevice();
		ui.pushButton_start_stop->setChecked(true);
		ui.pushButton_start_stop->setText(tr("\320\241\321\202\320\276\320\277"));
		ui.label_con_state->setText(tr("\320\241\320\236\320\225\320\224"));
	}
}

void q_red_pit_control::SetConnStatus()
{
}

void q_red_pit_control::EndInitConnection()		// ???
{
	ui.label_con_state->setText(tr("\320\222\320\232\320\233"));
}

void q_red_pit_control::NoConnection()
{
	device_CMD.StopDevice();

	ui.pushButton_start_stop->setChecked(false);
	ui.pushButton_start_stop->setText(tr("\320\237\321\203\321\201\320\272"));
	ui.label_con_state->setText(tr("\320\222\320\253\320\232\320\233"));
}


void q_red_pit_control::osc_length_changed()
{
	plot_array.resize(plot_arr_length);
	ImpulseToPlot();
	plotter.PlotRespond(plot_array.data(), plot_arr_length);
}


void q_red_pit_control::Timp_len_changed()
{
	device_CMD.ApplyImpAmlToPar();
	RecalculateImpulse();
	ImpulseToPlot();

	plotter.PlotRespond(plot_array.data(), plot_arr_length);


	device_CMD.g_changed_param |= CHNG_TIMP_LEN | CHNG_IMP_POINTS;
}

void q_red_pit_control::Timp_offset_changed()
{
	ImpulseToPlot();
	plotter.PlotRespond(plot_array.data(), plot_arr_length);

	device_CMD.g_changed_param |= CHNG_TIMP_OFFSET;
}

void q_red_pit_control::Tcycle_changed()
{
	device_CMD.g_changed_param |= CHNG_TCYCLE;
}

void q_red_pit_control::Tdef_changed()
{
	device_CMD.g_changed_param |= CHNG_TDEF;
}
void q_red_pit_control::pre_changed()
{
	device_CMD.g_changed_param |= CHNG_PRESC;
}

void q_red_pit_control::NumPeriods_changed()
{
	par_contr_t &par_contr = device_CMD.curr_par_contr;


	float curr_period_len = COEF_PERIOD_TRANSF / par_contr.dev_frequency;
	curr_period_len *= par_contr.num_periods;


	par_contr.sent_par.Timp_len = curr_period_len;		// число тиков после изменения
	device_CMD.ApplyImpAmlToPar();

	ui.ed_t_imp_len->show_par();


	RecalculateImpulse();
	ImpulseToPlot();

	plotter.PlotRespond(plot_array.data(), plot_arr_length);


	device_CMD.g_changed_param |= CHNG_TIMP_LEN | CHNG_IMP_POINTS;
}


void q_red_pit_control::Aimp_changed()
{
	RecalculateImpulse();
	ImpulseToPlot();
	plotter.PlotRespond(plot_array.data(), plot_arr_length);

	device_CMD.g_changed_param |= CHNG_IMP_POINTS;
}


void q_red_pit_control::DevFreq_changed()
{
	par_contr_t &par_contr = device_CMD.curr_par_contr;

	par_contr.dev_frequency = static_cast<float>(dev_freq) / COEF_DEV_FREQ;
	float curr_period_len = COEF_PERIOD_TRANSF / par_contr.dev_frequency;
	curr_period_len *= par_contr.num_periods;


	par_contr.sent_par.Timp_len = curr_period_len;		// число тиков после изменения
	device_CMD.ApplyImpAmlToPar();

	ui.ed_t_imp_len->show_par();


	RecalculateImpulse();
	ImpulseToPlot();

	plotter.PlotRespond(plot_array.data(), plot_arr_length);


	device_CMD.g_changed_param |= CHNG_TIMP_LEN | CHNG_IMP_POINTS;
}


void q_red_pit_control::GaussClicked()
{
	device_CMD.curr_par_contr.gaus_enable = ui.checkBox_gauss->isChecked();

	RecalculateImpulse();
	ImpulseToPlot();
	plotter.PlotRespond(plot_array.data(), plot_arr_length);

	device_CMD.g_changed_param |= CHNG_IMP_POINTS;
}





void q_red_pit_control::RecalculateImpulse()
{
	const par_contr_t &par_contr = device_CMD.curr_par_contr;
	const par_sent_t &sent_par = par_contr.sent_par;

	double k = 4.0*log(2.0*par_contr.Aimp)/(static_cast<double>(sent_par.Timp_len)*sent_par.Timp_len);

	// y = A * e^(-k*(x - len/2)^2)

	int len_div2 = sent_par.Timp_len/2;
	for(quint16 i = 0; i < sent_par.Timp_len; i++)
	{
		if(par_contr.gaus_enable)
			device_CMD.imp_ampl[i] = par_contr.Aimp * exp(-k*(i - len_div2)*(i - len_div2)) * sin(2* M_PI*i*par_contr.dev_frequency/COEF_PERIOD_TRANSF);
		else
			device_CMD.imp_ampl[i] = par_contr.Aimp * sin(2* M_PI*i*par_contr.dev_frequency/COEF_PERIOD_TRANSF);
	}

}




void q_red_pit_control::ImpulseToPlot()
{
	const par_contr_t &par_contr = device_CMD.curr_par_contr;
	const par_sent_t &sent_par = par_contr.sent_par;

	qint16 *plot_arr = plot_array.data();

	if(plot_arr_length > (sent_par.Timp_offset + sent_par.Timp_len))
	{
		memset(plot_arr, 0, sizeof(qint16)*sent_par.Timp_offset);
		memcpy(plot_arr + sent_par.Timp_offset, device_CMD.imp_ampl.data(), sizeof(qint16)*sent_par.Timp_len);
		memset(plot_arr + sent_par.Timp_offset + sent_par.Timp_len, 0, sizeof(qint16)*(plot_arr_length - sent_par.Timp_offset - sent_par.Timp_len) );
	}
	else if(plot_arr_length > sent_par.Timp_offset)
	{
		memset(plot_arr, 0, sizeof(qint16)*sent_par.Timp_offset);
		memcpy(plot_arr + sent_par.Timp_offset, device_CMD.imp_ampl.data(), sizeof(qint16)*(plot_arr_length - sent_par.Timp_offset));
	}
	else
		memset(plot_arr, 0, sizeof(qint16)*plot_arr_length);
}





