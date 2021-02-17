#include "dial_debug.h"

DialDebug::DialDebug(QWidget *parent):
    QDialog(parent, Qt::Window),
    ui()

{
	ui.setupUi(this);
	ui.comboBox_rej->addItem("XIL",XIL_REJ);
	ui.comboBox_rej->addItem("DAC",DAC_REJ);
	ui.comboBox_rej->addItem("ADC",ADC_REJ);
///	ui.comboBox_rej->addItem("CONTR",CONTR_REJ);

//	ui.comboBox_rej->addItem("TEST",TEST_REJ);
}



void DialDebug::closeEvent( QCloseEvent * event )
{
	QDialog::closeEvent(event);
}


DialDebug::~DialDebug()
{

}
void  DialDebug::butt_rd()
{
switch(ui.comboBox_rej->currentIndex())
	{
	case XIL_REJ:
		xil_dat_req.addr=ui.lineEdit_addr->text().toInt(0,16);
		xil_dat_req.nbytes=ui.lineEdit_count->text().toInt(0,16);
		emit req_rd_xil(&xil_dat_req);
		break;
	case DAC_REJ:
		dac_spi_req.addr=ui.lineEdit_addr->text().toInt(0,16);
///		dac_spi_req.nbytes=ui.lineEdit_count->text().toInt(0,16);
		emit req_rd_dac(&dac_spi_req);
		break;
	case ADC_REJ:
		adc_spi_req.addr=ui.lineEdit_addr->text().toInt(0,16);
///		dac_spi_req.nbytes=ui.lineEdit_count->text().toInt(0,16);
		emit req_rd_adc(&adc_spi_req);
		break;

/*	case TEST_REJ:
		emit req_rd_test(&alt_dat_req);
		break;*/

	default:;
	}

}
void  DialDebug::butt_wr()
{


	switch(ui.comboBox_rej->currentIndex())
	{
	case XIL_REJ:
		xil_dat_req.addr=ui.lineEdit_addr->text().toInt(0,16);
		xil_dat_req.nbytes=1;
		xil_dat_req.data[0]=ui.lineEdit_wr_dat->text().toInt(0,16);
		emit req_wr_xil(&xil_dat_req);
		break;

	case ADC_REJ:
		adc_spi_req.addr=ui.lineEdit_addr->text().toInt(0,16);
		adc_spi_req.data=ui.lineEdit_wr_dat->text().toInt(0,16);
		emit req_wr_adc(&adc_spi_req);
		break;

	case DAC_REJ:
		dac_spi_req.addr=ui.lineEdit_addr->text().toInt(0,16);
		dac_spi_req.data=ui.lineEdit_wr_dat->text().toInt(0,16);
		emit req_wr_dac(&dac_spi_req);
		break;
#if 0
	case CONTR_REJ:
		alt_dat_req.nbytes=ui.lineEdit_count->text().toInt(0,16);
		emit req_wr_contr(&alt_dat_req);
		break;
#endif
	default:;
	}


}
void  DialDebug::req_data_rdy(xil_dat_req_t* p_dat)
{
	QString tstr;
	int ii;
	if(p_dat->nbytes==0)
	p_dat->nbytes=1;
	else if(p_dat->nbytes>MAX_XIL_DATA_LEN)
		p_dat->nbytes=MAX_XIL_DATA_LEN;
	for(ii=0;ii<p_dat->nbytes;ii++)
	{
		tstr.sprintf("\naddr=%x->%x",p_dat->addr+ii,p_dat->data[ii]);

		ui.textEdit_rd_dat->append(tstr);
	}

	QTextCursor c = ui.textEdit_rd_dat->textCursor();
	c.movePosition(QTextCursor::End);
	ui.textEdit_rd_dat->setTextCursor(c);
}

void  DialDebug::req_data_rdy(dac_spi_req_t* p_dat)
{
	QString tstr;
	tstr.sprintf("\naddr=%x->%x",p_dat->addr,p_dat->data);
	ui.textEdit_rd_dat->append(tstr);

	QTextCursor c = ui.textEdit_rd_dat->textCursor();
	c.movePosition(QTextCursor::End);
	ui.textEdit_rd_dat->setTextCursor(c);
}
void  DialDebug::req_data_rdy(adc_spi_req_t* p_dat)
{
	QString tstr;
	tstr.sprintf("\naddr=%x->%x",p_dat->addr,p_dat->data);
	ui.textEdit_rd_dat->append(tstr);

	QTextCursor c = ui.textEdit_rd_dat->textCursor();
	c.movePosition(QTextCursor::End);
	ui.textEdit_rd_dat->setTextCursor(c);
}
