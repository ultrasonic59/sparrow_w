#ifndef DEV_CMD_H
#define DEV_CMD_H

#include <base_dev_cmd.h>
#include "dev_interf.h"

class c_dev_cmd : public c_base_dev_cmd
{
public:

	c_dev_cmd(QObject *parent);
	virtual ~c_dev_cmd();


	bool dev_get_osc(quint8 *btmp);
	bool dev_get_ampl(quint16 *p_val);

	bool dev_put_param(quint8 *iparam, quint16 num_bytes);


/*	bool dev_rd_par_dev(quint8 *btmp);							// ???
	bool dev_wr_par_dev(quint16 ipar);
*/

	bool dev_put_req_xil(xil_dat_req_t *ireq);
	bool dev_get_xil(xil_dat_req_t *odat);
	bool dev_put_xil(xil_dat_req_t *ireq);
	bool dev_put_req_dac(dac_spi_req_t *ireq);
	bool dev_get_dac(dac_spi_req_t *odat);
	bool dev_put_dac(dac_spi_req_t *ireq);
	bool dev_put_req_adc(adc_spi_req_t *ireq);
	bool dev_get_adc(adc_spi_req_t *odat);
	bool dev_put_adc(adc_spi_req_t *ireq);
	


	
};

#endif // DEV_CMD_H
