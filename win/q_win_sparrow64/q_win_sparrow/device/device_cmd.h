#ifndef __DEVICE_CMD_H__
#define __DEVICE_CMD_H__

#include <QObject>
#include <QThread>
#include <QTimer>

#include "pc_udp.h"
///#include <QElapsedTimer>


#define TIMEOUT_RSV		300
#define	DEF_TIMER_DT 15	///10


class c_device_cmd : public QObject
{
Q_OBJECT
public:
	c_device_cmd(QObject *parent);
	virtual ~c_device_cmd();
	pc_udp *p_pc_udp;
	int tim_dt;	// в мс
	quint32 g_changed_param;
	par_contr_t curr_par_contr;

	bool is_connected()
	{
		return dev_connected;
	}

protected:
	QThread *p_thr_udp;
	bool attached;
	bool dev_connected;
	bool prev_connected;
	bool new_conn;
	xil_dat_req_t rdy_dat;
	bool data_is_rdy;
public:
	QString ip_addr;		///
	int port;				///
	bool  end_cmd;
	quint8  last_cmd_good;
	udp_stat_t udp_stat;

///	volatile bool attached;
///	c_base_cmd *curr_cmd;
///	sent_dat_t *p_send_dat;
///	char *p_char_send_dat;
///	QMutex SendMutex;
///	resv_dat_t *p_resv_dat;
	///int unsuccesfull_conn;	
	///int max_unsuccesfull_conn;

///	QElapsedTimer el_timer;

///	inline void gen_send_dat(const quint8 type, const quint8 cmd, const quint8 *in_dat, quint16 len);	// генерирование отправляемых данных
///	bool send_and_wait(const quint8 type, const quint8 cmd, const quint8 *in_dat, quint16 len, quint8 *out_dat = nullptr);
///	bool send_and_wait(const quint8 type, const quint8 cmd, quint8 *out_dat);

public:

///	void AttachToCMD(c_base_cmd *base_cmd);
///	void DetachFromCMD();

	///void ResetConnCount();
	////int GetUnSuccesfullCount() const;

	bool IsAttached() const;

///	int time_count;


///	int GetRequestPeriod() const;
public:

	bool dev_put_param(quint8 *iparam, quint16 num_bytes);

	bool dev_put_req_xil(xil_dat_req_t *ireq);
	bool dev_get_xil(xil_dat_req_t *odat);
	bool dev_put_xil(xil_dat_req_t *ireq);
	bool dev_put_req_dac(dac_spi_req_t *ireq);
	bool dev_get_dac(dac_spi_req_t *odat);
	bool dev_put_dac(dac_spi_req_t *ireq);
	void ApplyImpAmlToPar();
	void StopDevice();
	void SetupDevice();


signals:

///	void signal_write();
///	void NoConnection();

///	void signal_start();
///	void signal_stop();
};

#endif // __DEVICE_CMD_H__
