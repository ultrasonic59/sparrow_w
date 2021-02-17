#ifndef _UDP_CMD_H_
#define _UDP_CMD_H_
#include <QThread>
#include <QtNetwork/QUdpSocket>
#include "cmdreq.h"

///=====================================================
#define MAX_FRAME_LEN 8192


//Q_DECLARE_METATYPE(CmdReq)



typedef struct resv_dat_s
{
quint8 type;
int num_bytes;
quint8 buff[MAX_FRAME_LEN];
}resv_dat_t;
extern resv_dat_t g_resv_dat;

class c_udp_cmd: public QObject////public QThreadpublic QThread 
{
 Q_OBJECT;
public:
	c_udp_cmd(QWidget *parent = 0);
	~c_udp_cmd();
	bool on_thr_obr_udp_cmd;
	QWidget *parent_w;
//// protected:
private:
    QUdpSocket *socket;
	QString host_addr;
	int port;

public slots:
	void socketError(QAbstractSocket::SocketError err);
////	void udp_req_dat_from(quint8 ,quint16 ,void*);
	void udp_ready_dat(void);
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
	quint16 num_snd_dat;

	

protected:
////	quint16	udp_send_to(quint8 type_send,quint8 type_dat);
///	int	udp_send_ack_to(quint8 ipar);
////	quint16	udp_send_frame(quint8 *data, int len);
////	quint16	udp_get_req_dat(quint8 num_req,quint8 *buf);
	int udp_set_rec_dat(void *in_buf);
public :
	void obr_udp_cmd(quint8 *i_buff,quint16 len);
	bool udp_send_dat(quint8 icmd,quint8 *idat,quint16 len);
	void set_host(QString addr,int iport);
public slots:
	void write_udp_cmd(QByteArray tba);
	void write_udp_dat(QByteArray tba);
	void write_udp_dats(QByteArray tba);
///	void write_udp(void *data,int len);
////	void write_udp(CmdReq);
	void bind(void);
private:
	int udp_check_framein(quint8 *i_buff,quint16 len);
	int udp_parse_frame(quint8 *in_buf,quint16 len);
	bool is_host_init;
///public :
///	bool udp_set_cur_rej(quint8 irej);


};
#endif
