#include "dev_cmd.h"


c_dev_cmd::c_dev_cmd(QObject *parent) : 
		c_base_dev_cmd(parent)
{
}

c_dev_cmd::~c_dev_cmd()
{
}



bool c_dev_cmd::dev_get_osc(quint8 *btmp)
{
	return send_and_wait(CMD_REQ_DAT, REQ_GET_OSC, btmp);
}


bool c_dev_cmd::dev_put_param(quint8 *iparam, quint16 num_bytes)
{
	return send_and_wait(CMD_PUT_DAT, REQ_PUT_PAR, iparam, num_bytes);
}



/*
bool c_dev_cmd::dev_rd_par_dev(quint8 *btmp)
{
	return send_and_wait(CMD_REQ_DAT, NUM_RD_PAR_DEV_CMD, btmp);
}

bool c_dev_cmd::dev_wr_par_dev(quint16 ipar)
{
	quint16 htmp=ipar;
	return send_and_wait(CMD_PUT_DAT, NUM_WR_PAR_DEV_CMD, reinterpret_cast<quint8 *>(&htmp), sizeof(quint16));
}
*/
