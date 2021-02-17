#ifndef _DIAL_DEFECTOSCOPE_H_
#define _DIAL_DEFECTOSCOPE_H_

#include <QCloseEvent>
#include <QtWidgets/QtWidgets>
#include <QPluginLoader>

#include "ui_Defectoscope.h"
#include "dev_interf.h"

namespace Ui {
    class Defectoscope;
}

#define XIL_REJ 0
#define STM_REJ 1
#define MSP_REJ 2

class DialDefectoscope : public QDialog
{
Q_OBJECT
private:
///	cn_info_t	cn_info[NUM_CHANS];		// описание каждого (такто)канала тракта

protected:
	Ui::Defectoscope *ui;
	virtual void closeEvent( QCloseEvent * event );

public:
////	    explicit DialPhoto(QWidget *parent = 0);

	DialDefectoscope(QWidget *parent= 0);
	virtual ~DialDefectoscope();
#if 0
signals:
	void req_rd_xil(alt_dat_req_t*);
	void req_wr_xil(alt_dat_req_t*);
	void req_rd_stm(alt_dat_req_t*);
	void req_wr_stm(alt_dat_req_t*);
	void req_rd_msp(QByteArray);
	void req_wr_msp(QByteArray);

	void signal_send_udp_test_dats(QByteArray);

private slots:
	void  on_butt_rd();
	void  on_butt_wr();
	void  on_butt_send();
	void  req_data_rdy(alt_dat_req_t*);
private: 
	alt_dat_req_t  alt_dat_req;
	alt_dat_req_t  alt_dat;
#endif

#if 0
	void  set_chans(cn_info_t *cn_info);
	void  get_chans(cn_info_t *cn_info);
	void  get_ip_addr(QString  *o_addr);
	void  set_ip_addr(QString addr);
	void  get_rele(bool  *on_rele);
	void  set_rele(bool  on_rele);
	void  get_on_ed_us(bool  *on_us);
	void  set_on_ed_us(bool  on_us);
	void  get_on_ed_tact(bool  *on_tact);
	void  set_on_ed_tact(bool  on_tact);
#endif
///private slots:
///	void on_send_butt();
};

#endif // DIAL_DEFECTOSCOPE_H
