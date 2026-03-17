#include "device_cmd.h"

extern void addfcs16( char *cp, int len );


c_device_cmd::c_device_cmd(QObject *parent): 
		QObject(parent),
	    tim_dt(DEF_TIMER_DT),
	    attached(false),
	    dev_connected(false),
	    prev_connected(false),
	    end_cmd(false),
	    last_cmd_good(0),
	    p_thr_udp(nullptr),
	    p_pc_udp(nullptr),
	    g_changed_param(0),
	    ip_addr(),
	    port(2000)
{
	p_thr_udp = new QThread();
	p_pc_udp = new pc_udp(&port, ip_addr,  &end_cmd, &g_changed_param, &last_cmd_good, &udp_stat);
	p_pc_udp->udp_pc_init();
	p_pc_udp->moveToThread(p_thr_udp);
	p_thr_udp->start();

}

c_device_cmd::~c_device_cmd()
{
	p_thr_udp->quit();
	p_thr_udp->wait();

	delete p_thr_udp;

}

bool c_device_cmd::IsAttached() const
{
	return attached;
}
void c_device_cmd::StopDevice()
{
/*
	UpdateDevice(false);

	p_tune_thr->dev_cmd.DetachFromCMD();

	if (p_tune_thr->is_initing)
		p_tune_thr->is_initing = false;
*/
}
void c_device_cmd::SetupDevice()
{
	g_changed_param = 0;
	/*
	p_tune_thr->is_initing = true;

	p_udp_cmd->set_host(ip_addr, port);
	p_tune_thr->dev_cmd.AttachToCMD(p_udp_cmd);
	UpdateDevice(true);
	*/
}
bool c_device_cmd::dev_put_req_dac(dac_spi_req_t* ireq)
{
///	return send_and_wait(CMD_PUT_DAT, NUM_SEND_SPI_DAC_REQ, reinterpret_cast<quint8*>(ireq), sizeof(dac_spi_req_t));
	return false;
}

bool c_device_cmd::dev_get_dac(dac_spi_req_t* odat)
{
///	return send_and_wait(CMD_REQ_DAT, NUM_REQ_SPI_DAC_DAT, reinterpret_cast<quint8*>(odat));
	return false;

}

bool c_device_cmd::dev_put_dac(dac_spi_req_t* ireq)
{
///	return send_and_wait(CMD_PUT_DAT, NUM_SEND_SPI_DAC_DAT, reinterpret_cast<quint8*>(ireq), sizeof(dac_spi_req_t));
	return false;

}
void c_device_cmd::ApplyImpAmlToPar()
{
///	imp_ampl.resize(curr_par_contr.sent_par.Timp_len);
}



