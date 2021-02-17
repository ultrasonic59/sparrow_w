#include "obj_cmd.h"

c_obj_cmd::c_obj_cmd(QObject *parent)
	:	QObject(parent),
		p_parent(parent),


		p_thr_udp(nullptr),

		p_thr_tune(nullptr),

		p_udp_cmd(nullptr),



		ip_addr(),
		port(2000),
		

		g_changed_param(0),
		curr_par_contr(),
		imp_ampl(),


		p_tune_thr(nullptr)
{

	p_thr_udp = new QThread();

	p_udp_cmd= new c_udp_cmd(0);
	p_udp_cmd->moveToThread(p_thr_udp);


	p_tune_thr = new c_tune_thr( 0,
						&g_changed_param,
						&curr_par_contr,
						&imp_ampl);
	p_thr_tune = new QThread();
	p_tune_thr->moveToThread(p_thr_tune);


	p_thr_udp->start(QThread::HighestPriority);

	p_thr_tune->start(QThread::HighPriority);



	connect(this, SIGNAL(on_off_tune_timer(bool)), p_tune_thr, SLOT(on_off_timer(bool)), Qt::QueuedConnection);

	connect(&p_tune_thr->dev_cmd, SIGNAL(NoConnection()), parent, SLOT(NoConnection()), Qt::QueuedConnection);
	connect(p_tune_thr, SIGNAL(ConnectionAttempt()), parent, SLOT(SetConnStatus()), Qt::QueuedConnection);
	connect(p_tune_thr, SIGNAL(ConnectionSetted()), parent, SLOT(EndInitConnection()), Qt::QueuedConnection);
	connect(p_tune_thr, SIGNAL(signal_draw_all()), parent, SLOT(slot_draw_all()), Qt::QueuedConnection);
	connect(p_tune_thr, SIGNAL(signal_unsuccesfull()), parent, SLOT(slot_unsuccesfull()), Qt::QueuedConnection);

}

c_obj_cmd::~c_obj_cmd()
{
	p_tune_thr->on_tune_thr = false;

	p_tune_thr->dev_cmd.DetachFromCMD();


	p_udp_cmd->deleteLater();
	p_tune_thr->deleteLater();

	p_thr_tune->exit();
	p_thr_udp->exit();

	p_thr_tune->wait();
	delete p_thr_tune;

	p_thr_udp->wait();
	delete p_thr_udp;
}


void c_obj_cmd::SetupDevice()
{
	p_tune_thr->dev_cmd.ResetConnCount();
	g_changed_param = 0;

	
	p_tune_thr->is_initing = true;

	p_udp_cmd->set_host(ip_addr,port);
	p_tune_thr->dev_cmd.AttachToCMD(p_udp_cmd);


	UpdateDevice(true);
}




void c_obj_cmd::StopDevice()
{
	UpdateDevice(false);

	p_tune_thr->dev_cmd.DetachFromCMD();

	if(p_tune_thr->is_initing)
		p_tune_thr->is_initing = false;
}



void c_obj_cmd::UpdateDevice(const bool update)
{
	if(p_tune_thr->on_tune_thr != update)
	{
		p_tune_thr->on_tune_thr = update;		// немедленно отключить работу таймера
		emit on_off_tune_timer(update);			// со временем отключить сам таймер
	}
}


bool c_obj_cmd::IsAttached() const
{
	return p_tune_thr->dev_cmd.IsAttached();
}


void c_obj_cmd::ApplyImpAmlToPar()
{
	imp_ampl.resize(curr_par_contr.sent_par.Timp_len);
}