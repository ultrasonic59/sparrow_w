#ifndef _DEV_CMD_H_
#define _DEV_CMD_H_
///#include <QThread>
///#include <QtNetwork/QUdpSocket>
#include <QtWidgets/QtWidgets>
#include "dev_interf.h"
#include "cmdreq.h"

#define MAX_FRAME_LEN 8192
#define MAX_TIMEOUT_RSV		500	///4000



///=====================================================
class c_dev_cmd: public QObject////public QThreadpublic QThread 
{
 Q_OBJECT;
public:
	c_dev_cmd(QWidget *parent = 0);
	~c_dev_cmd();
	QWidget *parent_w;

///	 void SetCommNumb(int *comm_num);

/// protected:
signals:
///	void signal_write_udp(void *,int);
	void signal_write_udp_cmd(QByteArray);
	void signal_write_udp_dat(QByteArray);
////	void signal_write_udp_test_dats(QByteArray);

	void NoConnect();

private:
////    QUdpSocket *socket;
///	QString host_addr;
///	int port;

////public slots:
///	void socketError(QAbstractSocket::SocketError err);
///	void udp_req_dat_from(quint8 ,quint16 ,void*);
////	void udp_ready_dat(void);
private:
	quint8 	*in_buf;				// 
	quint16 len_in_dat;
	void *t_rsv_buff;
	quint8 	*snd_buff;				// 
	quint16	frame_out_len;
	quint16	offs_snd_buf;
	quint8 	*t_snd_buff1;				// 
	quint8 	*t_snd_buff2;				// 
	quint16	snd_buff2_len;
	///HANDLE ev_rsv_ack;
	///HANDLE ev_rsv_dat;
	///HANDLE ev_rsv_frame;
	quint8 type_dat;
////	quint8 type_rsv_dat;
	quint16 num_snd_dat;

	int unsuccesfull_conn;
	
	int max_unsuccesfull_conn;

protected:
	bool udp_req_dat_from(quint8 ,quint16 ,void*);
	quint16	udp_send_to(quint8 type_send,quint8 type_dat);
	int	udp_send_ack_to(quint8 ipar);
	quint16	udp_send_frame(quint8 *data, int len);
	quint16	udp_get_req_dat(quint8 num_req,quint8 *buf);
	int	udp_get_rez(void *obuf);

public :
	bool udp_send_dat(quint8 icmd,quint8 *idat,quint16 len);
public :
	int old_unsuccesfull_conn;

	bool udp_set_cur_rej(quint8 irej);
	bool udp_get_cur_rej(quint8 *btmp);
	bool udp_get_osc0(quint8 *btmp);
	bool udp_get_amps(quint8 *btmp);
	bool udp_put_param(quint8 *iparam,quint16 num_bytes);
	bool udp_put_req_xil(alt_dat_req_t *ireq);
	bool udp_get_xil(alt_dat_req_t *odat);
	bool udp_put_xil(alt_dat_req_t *ireq);
	bool udp_rd_par_dev(quint8 *btmp);
	bool udp_wr_par_dev(quint16 ipar);
	bool udp_put_req_stm(alt_dat_req_t *ireq);
	bool udp_get_stm(alt_dat_req_t *odat);
	bool udp_put_stm(alt_dat_req_t *ireq);
	bool udp_put_req_msp(quint8 *ireq,quint16 len);
	bool c_dev_cmd::udp_get_msp(quint8 *odat);
	bool c_dev_cmd::udp_put_msp(quint8 *ireq,quint16 len);
	bool udp_put_req_udb(alt_dat_req_t *ireq);
	bool udp_get_udb(alt_dat_req_t *odat);
	bool udp_put_udb(alt_dat_req_t *ireq);

	bool udp_put_req_upow(alt_dat_req_t *ireq);
	bool udp_get_upow(alt_dat_req_t *odat);
	bool udp_put_upow(alt_dat_req_t *ireq);
	void ResetConnection();

};
///=====================================================
#endif