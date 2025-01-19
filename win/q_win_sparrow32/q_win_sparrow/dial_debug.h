#ifndef _DIAL_DEBUG_H_
#define _DIAL_DEBUG_H_

#include <QCloseEvent>
#include <QtWidgets/QtWidgets>

#include "ui_dial_debug.h"
#include "dev_interf.h"

namespace Ui {
    class Dial_debug;
}

#define XIL_REJ		0
#define DAC_REJ		1
#define ADC_REJ		2
///#define CONTR_REJ	3

//#define TEST_REJ	4



class DialDebug : public QDialog
{
Q_OBJECT

protected:
	Ui::Dial_debug ui;
	virtual void closeEvent( QCloseEvent * event );

public:
	DialDebug(QWidget *parent= 0);
	virtual ~DialDebug();

signals:
	void req_rd_xil(xil_dat_req_t*);
	void req_wr_xil(xil_dat_req_t*);
	void req_rd_dac(dac_spi_req_t*);
	void req_wr_dac(dac_spi_req_t*);
	void req_wr_adc(adc_spi_req_t*);
	void req_rd_adc(adc_spi_req_t*);


//	void req_rd_test(alt_dat_req_t*);



private slots:
	void  butt_rd();
	void  butt_wr();

	void  req_data_rdy(xil_dat_req_t*);
	void  req_data_rdy(dac_spi_req_t*);
	void  req_data_rdy(adc_spi_req_t*);

private: 
	xil_dat_req_t  xil_dat_req;
	xil_dat_req_t  xil_dat;
	dac_spi_req_t dac_spi_req;
	adc_spi_req_t adc_spi_req;

};

#endif // DIAL_ED_PAR_H
