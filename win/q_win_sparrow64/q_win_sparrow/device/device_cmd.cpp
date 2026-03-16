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




