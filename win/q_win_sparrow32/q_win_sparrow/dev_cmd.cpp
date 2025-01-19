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

bool c_dev_cmd::dev_get_ampl(quint16 *p_val)
{
	return send_and_wait(CMD_REQ_DAT, REQ_GET_AMPL, reinterpret_cast<quint8 *>(p_val));
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


bool c_dev_cmd::dev_put_req_xil(xil_dat_req_t *ireq)
{
	return send_and_wait(CMD_PUT_DAT, NUM_SEND_XIL_REQ, reinterpret_cast<quint8 *>(ireq), sizeof(xil_dat_req_t));
}

bool c_dev_cmd::dev_get_xil(xil_dat_req_t *odat)
{
	return send_and_wait(CMD_REQ_DAT, NUM_REQ_XIL_DAT, reinterpret_cast<quint8 *>(odat));
}

bool c_dev_cmd::dev_put_xil(xil_dat_req_t *ireq)
{
	return send_and_wait(CMD_PUT_DAT, NUM_SEND_XIL_DAT, reinterpret_cast<quint8 *>(ireq), sizeof(xil_dat_req_t));
}

bool c_dev_cmd::dev_put_req_dac(dac_spi_req_t *ireq)
{
	return send_and_wait(CMD_PUT_DAT, NUM_SEND_SPI_DAC_REQ, reinterpret_cast<quint8 *>(ireq), sizeof(dac_spi_req_t));
}

bool c_dev_cmd::dev_get_dac(dac_spi_req_t *odat)
{
	return send_and_wait(CMD_REQ_DAT, NUM_REQ_SPI_DAC_DAT, reinterpret_cast<quint8 *>(odat));
}

bool c_dev_cmd::dev_put_dac(dac_spi_req_t *ireq)
{
	return send_and_wait(CMD_PUT_DAT, NUM_SEND_SPI_DAC_DAT, reinterpret_cast<quint8 *>(ireq), sizeof(dac_spi_req_t));
}

bool c_dev_cmd::dev_put_adc(adc_spi_req_t *ireq)
{
	return send_and_wait(CMD_PUT_DAT, NUM_SEND_SPI_ADC_DAT, reinterpret_cast<quint8 *>(ireq), sizeof(adc_spi_req_t));
}

bool c_dev_cmd::dev_put_req_adc(adc_spi_req_t *ireq)
{
	return send_and_wait(CMD_PUT_DAT, NUM_SEND_SPI_ADC_REQ, reinterpret_cast<quint8 *>(ireq), sizeof(adc_spi_req_t));
}

bool c_dev_cmd::dev_get_adc(adc_spi_req_t *odat)
{
	return send_and_wait(CMD_REQ_DAT, NUM_REQ_SPI_ADC_DAT, reinterpret_cast<quint8 *>(odat));
}
