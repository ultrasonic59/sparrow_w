#include "udp_cmd.h"
#include "ugen_z_dbg.h"
#include <qmutex.h>


QMutex CommandMutex;
QMutex DataMutex;

volatile int CommandMutexCounter = 0;
volatile int DataMutexCounter = 0;

QMutex CommandMutex_sent;
QMutex DataMutex_sent;


///extern quint8 g_type_rsv_dat;
extern int checkfcs16(unsigned char *cp, register int len );

resv_dat_t g_resv_dat;

c_udp_cmd::c_udp_cmd(QWidget *parent) ///:  QThread()
{
int ii;
socket = new QUdpSocket(this);
parent_w=parent;
in_buf = (quint8*)malloc(MAX_FRAME_LEN );
snd_buff	= (quint8*)malloc(MAX_FRAME_LEN);
t_snd_buff1 = (quint8*)malloc(MAX_FRAME_LEN);
memset(t_snd_buff1,0,MAX_FRAME_LEN);	

t_snd_buff2 = (quint8*)malloc(MAX_FRAME_LEN);
memset(t_snd_buff2,0,MAX_FRAME_LEN);	
///connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
///connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
connect(socket, SIGNAL(readyRead(void)), this, SLOT(udp_ready_dat(void)));
is_host_init=false;

}

c_udp_cmd::~c_udp_cmd()
{

free(in_buf);
free(snd_buff);
free(t_snd_buff1);
free(t_snd_buff2);
}
void c_udp_cmd::bind()
{
socket->bind();
}
void c_udp_cmd::write_udp_cmd(QByteArray tba)
{
if(is_host_init)
	socket->writeDatagram(tba, QHostAddress(host_addr), port);

CommandMutex_sent.unlock();
}

void c_udp_cmd::write_udp_dat(QByteArray tba)
{
if(is_host_init)
	socket->writeDatagram(tba, QHostAddress(host_addr), port);

DataMutex_sent.unlock();
}
void c_udp_cmd::write_udp_dats(QByteArray tba)
{
if(is_host_init)
	socket->writeDatagram(tba, QHostAddress(host_addr), port);

////DataMutex_sent.unlock();
}

/*void c_udp_cmd::write_udp(void *data,int len)
{
///int ii;
///QString tstr;
const char *tdata=(const char *)data;
if(is_host_init)
	socket->writeDatagram(tdata,len, QHostAddress(host_addr), port);

CommandMutex_sent.unlock();

}*/

/*
void c_udp_cmd::write_udp(CmdReq req)
{
int ii;
QString tstr;
if(is_host_init)
	socket->writeDatagram(req.snd_buff,req.num_send, QHostAddress(host_addr), port);
}
*/
void c_udp_cmd::set_host(QString addr,int iport)
{
host_addr=addr;
port=iport;
is_host_init=true;
}
int c_udp_cmd::udp_check_framein(quint8 *i_buff,quint16 len)
{
int res = 1;
if(len < 3 )
	{
	res = -1;
	}
if( !checkfcs16(i_buff, len - 2 ))
	{
	res = -2;
	}
return res;
}
int c_udp_cmd::udp_set_rec_dat(void *in_buf)
{
///int rez;
quint8 *bbuf= (quint8 *)in_buf;
quint16 num_bytes;
num_bytes= (*(bbuf))|(*(bbuf+1)<<8);
if(num_bytes>MAX_FRAME_LEN)
	num_bytes=MAX_FRAME_LEN;
memcpy(g_resv_dat.buff,bbuf+2,num_bytes);
g_resv_dat.num_bytes=num_bytes;
return num_bytes;
}


int c_udp_cmd::udp_parse_frame(quint8 *in_buf,quint16 len)
{
int rez =0;
quint8 btmp;
///u16 htmp;
switch(*in_buf)
	{
	case CMD_ACK:

		btmp = *(in_buf+1);
/////		p_hdlc->type_dat=btmp;
		g_resv_dat.type=btmp;

		if(CommandMutexCounter > 0)
		{
			CommandMutexCounter--;
			CommandMutex.unlock();
		}

		//SetEvent(ev_rsv_ack);
		break;	
	case CMD_PUT_DAT:
		btmp = *(in_buf+1);
///???		udp_set_rec_dat(btmp,(void *)((quint32)in_buf+2));
////		p_hdlc->type_send=SEND_ACK;
////		p_hdlc->type_dat=btmp;
////		SetEvent(g_udp_pc.ev_snd_frame);
////		SetEvent(g_ev_snd_frame);
		break;
	case CMD_PUT_REQ_DAT:

		btmp = *(in_buf+1);
		udp_set_rec_dat((void *)((quint32)in_buf+2));
///???		g_udp_pc.type_dat=btmp;
		g_resv_dat.type=btmp;

		if(DataMutexCounter > 0)
		{
			DataMutexCounter--;
			DataMutex.unlock();
		}

		//SetEvent(ev_rsv_dat);
		break;
	case CMD_REQ_DAT:
		btmp = *(in_buf+1);
///???		g_udp_pc.offs_snd_buf = *(in_buf+2);
///???		g_udp_pc.offs_snd_buf |= (*(in_buf+3))<<8;
////		p_hdlc->type_send=SEND_REQ_DAT;
////		p_hdlc->type_dat=btmp;

////		SetEvent(g_udp_pc.ev_snd_frame);
		rez =0;
		break;
	}
return rez;
}

void c_udp_cmd::obr_udp_cmd(quint8 *i_buff,quint16 len)
{
int res;
////u8 ii;
if((res= udp_check_framein(i_buff,len))>0 )	// Проверить правильность входящего кадра
	{
///==================================
	udp_parse_frame(i_buff,len);
	}
///else
///	{
///	;
///	}
}

void c_udp_cmd::udp_ready_dat(void)
{
while (socket->hasPendingDatagrams()) 
{
QByteArray datagram;
datagram.resize(socket->pendingDatagramSize());
QHostAddress sender;
quint16 senderPort;
socket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);
obr_udp_cmd((quint8 *)datagram.data(),datagram.size());
///qDebug()<<"senderPort"<<senderPort;
}
}
void c_udp_cmd::socketError(QAbstractSocket::SocketError err)
{
 ///   ui_osc.labelStatus->setText(QString("Error: %1").arg(err));
  ///  ui_osc.pushButton_test->setEnabled(true);
qDebug()<<"socketError"<< QString("Error: %1").arg(err);
}

///==============================



///============================
