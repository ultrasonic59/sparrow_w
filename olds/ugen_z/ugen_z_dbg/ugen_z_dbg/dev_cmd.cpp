#include "dev_cmd.h"
#include "dev_interf.h"
#include "udp_cmd.h"

#include <qmutex.h>

extern quint8 dev_state;
extern void addfcs16( unsigned char *cp, int len );



extern QMutex CommandMutex;
extern QMutex DataMutex;

extern volatile int CommandMutexCounter;
extern volatile int DataMutexCounter;

extern QMutex CommandMutex_sent;
extern QMutex DataMutex_sent;

////quint8 g_type_rsv_dat;

////		SetEvent(p_hdlc->ev_rsv_ack);

////++++++++++++++++++++++++++++++++++++++++++++++++++
c_dev_cmd::c_dev_cmd(QWidget *parent) ///:  QThread()
{
int ii;
parent_w=parent;
in_buf = (quint8*)malloc(MAX_FRAME_LEN );
snd_buff	= (quint8*)malloc(MAX_FRAME_LEN);
t_snd_buff1 = (quint8*)malloc(MAX_FRAME_LEN);
memset(t_snd_buff1,0,MAX_FRAME_LEN);	
t_snd_buff2 = (quint8*)malloc(MAX_FRAME_LEN);
memset(t_snd_buff2,0,MAX_FRAME_LEN);	
///connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
///connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
///connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
///connect(socket, SIGNAL(readyRead(void)), this, SLOT(udp_ready_dat(void)));

unsuccesfull_conn = 0;
old_unsuccesfull_conn = 0;

max_unsuccesfull_conn = 3;

}

c_dev_cmd::~c_dev_cmd()
{
free(in_buf);
free(snd_buff);
free(t_snd_buff1);
free(t_snd_buff2);
}



#if 0
void c_dev_interf::write(const char *data,quint64 len)
{
////socket->writeDatagram(data,len, QHostAddress(host_addr), port);
}
#endif
///==============================
int	c_dev_cmd::udp_send_ack_to(quint8 ipar)
{
int num_send=0;
quint8 *p_tmp	 =t_snd_buff1;
*p_tmp++  = CMD_ACK;
num_send++;
*p_tmp++  = ipar;
num_send++;
return udp_send_frame(t_snd_buff1, num_send);
}
quint16	c_dev_cmd::udp_send_frame(quint8 *data, int len)
{
///================================
addfcs16(data,len);				// Добавить циклический код
memcpy(snd_buff,data, len+2);
frame_out_len = len+2;
return len+2;
}

quint16	c_dev_cmd::udp_send_to(quint8 type_send,quint8 type_dat)
{
int res=0;
quint16 len_dat;
int num_send=0;
quint8 *p_tmp1;
quint8 *p_tmp	 = t_snd_buff1;
if(type_send == SEND_ACK)
	{
	res=udp_send_ack_to(type_dat);
	}
else if(type_send == SEND_REQ_DAT)
	{
	*p_tmp++  = CMD_PUT_REQ_DAT;
	num_send++;
	*p_tmp++  = type_dat;
	num_send++;
	p_tmp1 = p_tmp+2;  
	len_dat = udp_get_req_dat(type_dat,p_tmp1);
	*p_tmp++  = len_dat&0xff;
	num_send++;
	*p_tmp++  = (len_dat>>8)&0xff;
	num_send++;
	if(len_dat > UDP_MAX_FR_LEN)
		len_dat = UDP_MAX_FR_LEN;
	num_send += len_dat;
	res=udp_send_frame(t_snd_buff1, num_send);
	}
else if(type_send == SEND_DAT)
	{
	num_send = snd_buff2_len;
	memcpy(p_tmp,t_snd_buff2,num_send);
	res=udp_send_frame(t_snd_buff1, num_send);
	}
else if(type_send == SEND_REQ)
	{
	num_send = snd_buff2_len;
	memcpy(p_tmp,t_snd_buff2,num_send);
	res=udp_send_frame(t_snd_buff1, num_send);
	}
return res;
}
quint16	c_dev_cmd::udp_get_req_dat(quint8 num_req,quint8 *buf)
{
quint16 rez=0;
return rez;
}
int	c_dev_cmd::udp_get_rez(void *obuf)
{
quint16 num_bytes=g_resv_dat.num_bytes;
if(num_bytes>MAX_FRAME_LEN)
	num_bytes=MAX_FRAME_LEN;
memcpy(obuf,g_resv_dat.buff,num_bytes);
return num_bytes;
}

bool c_dev_cmd::udp_send_dat(quint8 icmd,quint8 *idat,quint16 len)
{
////QString tstr;
int ii;
DWORD dwRes=0;	///
int num_send=0;
bool res=false;
quint8 *p_tmp	 =  t_snd_buff2;
if(len==0)
	return FALSE;
*p_tmp++  = CMD_PUT_DAT;
num_send++;
*p_tmp++  = icmd;
num_send++;
*p_tmp++  = len&0xff;
num_send++;
*p_tmp++  = (len>>8)&0xff;
num_send++;
memcpy(p_tmp,idat,len);
num_send += len;
snd_buff2_len=num_send;
type_dat=icmd;
num_send=udp_send_to(SEND_DAT,icmd);

CommandMutex_sent.lock();

QByteArray tba; 
tba.append( (const char*) snd_buff, num_send);
emit signal_write_udp_cmd(tba);

CommandMutex.lock();
CommandMutexCounter++;

/*
for(ii=0;ii<num_send;ii++)
{
tstr.sprintf("[%x:%x]",ii,snd_buff[ii]);
qDebug()<<tstr;
}
*/


if(!CommandMutex.tryLock(MAX_TIMEOUT_RSV))
	{
	dwRes = WAIT_TIMEOUT;
	unsuccesfull_conn++;
	old_unsuccesfull_conn = unsuccesfull_conn;
	}
else
	{
	dwRes = WAIT_OBJECT_0;
	unsuccesfull_conn = 0;
	CommandMutexCounter++;
	}

if(CommandMutexCounter > 0)
{
	CommandMutexCounter--;
	CommandMutex.unlock();
}

if(unsuccesfull_conn > max_unsuccesfull_conn)
	emit NoConnect();


switch(dwRes)
	{
    case WAIT_OBJECT_0:
 		if(g_resv_dat.type==icmd)
           res = true;
		else
			res=false;
        break;
     case WAIT_TIMEOUT:
 		res=false;
         break;
	default:
 		res=false;
         break;
       }

/*
CommandMutex_sent.lock();
CommandMutex_sent.unlock();
*/

return res;
}
bool c_dev_cmd::udp_req_dat_from(quint8 icmd,quint16 offs,void *obuf)
{
DWORD dwRes=0;	///
///u8 res_byte;
///u16 htmp;
///static u8 ns=0;
int num_send=0;
bool res=false;
quint8 *p_tmp	 = t_snd_buff2;
*p_tmp++  = CMD_REQ_DAT;
num_send++;
*p_tmp++  = icmd;
num_send++;
*p_tmp++  = offs&0xff;
num_send++;
*p_tmp++  = (offs>>8)&0xff;
num_send++;
snd_buff2_len=num_send;
t_rsv_buff=obuf;
num_send=udp_send_to(SEND_REQ,icmd);


DataMutex_sent.lock();

QByteArray tba; 
tba.append( (const char*) snd_buff, num_send);
emit signal_write_udp_dat(tba);

DataMutex.lock();
DataMutex_sent.lock();
DataMutex_sent.unlock();

//tba.clear();
DataMutexCounter++;

if(!DataMutex.tryLock(MAX_TIMEOUT_RSV))
	{
		dwRes = WAIT_TIMEOUT;
		unsuccesfull_conn++;
		old_unsuccesfull_conn = unsuccesfull_conn;
	}	
else
	{
		dwRes = WAIT_OBJECT_0;
		unsuccesfull_conn = 0;
		DataMutexCounter++;
	}


if(DataMutexCounter > 0)
{
	DataMutexCounter--;
	DataMutex.unlock();
}


if(unsuccesfull_conn > max_unsuccesfull_conn)
	emit NoConnect();


//dwRes = ::WaitForSingleObject(ev_rsv_dat, TIMEOUT_RSV);

//DWORD error1 = GetLastError();

switch(dwRes)
	{
    case WAIT_OBJECT_0:
//// 		ResetEvent(ev_rsv_dat);
		if(g_resv_dat.type==icmd)
			{
			udp_get_rez(obuf);
			res = true;
			}
		else
			res=false;
        break;
     case WAIT_TIMEOUT:
 		res=false;
         break;
       }
/*
DataMutex_sent.lock();
DataMutex_sent.unlock();
*/

return res;
}

void c_dev_cmd::ResetConnection()
{
	unsuccesfull_conn = 0;
	old_unsuccesfull_conn = 0;
}

///============================
bool c_dev_cmd::udp_set_cur_rej(quint8 irej)
{
quint8 btmp=irej;
return udp_send_dat(NUM_SET_REJ,&btmp,1);
}
///============================
bool c_dev_cmd::udp_get_cur_rej(quint8 *btmp)
{
///u8 btmp;
return udp_req_dat_from(NUM_REQ_REJ,0,(void*)btmp);
}
bool c_dev_cmd::udp_get_osc0(quint8 *btmp)
{
///u8 btmp;
return udp_req_dat_from(NUM_REQ_GET_OSC0,0,(void*)btmp);
}
bool c_dev_cmd::udp_get_amps(quint8 *btmp)
{
///u8 btmp;
return udp_req_dat_from(NUM_REQ_GET_AMPS,0,(void*)btmp);
}
bool c_dev_cmd::udp_put_param(quint8 *iparam,quint16 num_bytes)
{
return udp_send_dat(NUM_WR_N_PAR_CMD,iparam,num_bytes);
}
bool c_dev_cmd::udp_put_req_xil(alt_dat_req_t *ireq)
{
return udp_send_dat(NUM_SEND_XIL_REQ,(quint8*)ireq,sizeof(alt_dat_req_t));
}
bool c_dev_cmd::udp_get_xil(alt_dat_req_t *odat)
{
return udp_req_dat_from(NUM_REQ_XIL_DAT,0,(void*)odat);
}
bool c_dev_cmd::udp_put_xil(alt_dat_req_t *ireq)
{
return udp_send_dat(NUM_SEND_XIL_DAT,(quint8*)ireq,sizeof(alt_dat_req_t));
}

bool c_dev_cmd::udp_put_req_stm(alt_dat_req_t *ireq)
{
return udp_send_dat(NUM_SEND_STM_REQ,(quint8*)ireq,sizeof(alt_dat_req_t));
}
bool c_dev_cmd::udp_get_stm(alt_dat_req_t *odat)
{
return udp_req_dat_from(NUM_REQ_STM_DAT,0,(void*)odat);
}
bool c_dev_cmd::udp_put_stm(alt_dat_req_t *ireq)
{
return udp_send_dat(NUM_SEND_STM_DAT,(quint8*)ireq,sizeof(alt_dat_req_t));
}

bool c_dev_cmd::udp_put_req_msp(quint8 *ireq,quint16 len)
{
return udp_send_dat(NUM_SEND_MSP_REQ,(quint8*)ireq,len);
}
bool c_dev_cmd::udp_get_msp(quint8 *odat)
{
return udp_req_dat_from(NUM_REQ_MSP_DAT,0,(void*)odat);
}
bool c_dev_cmd::udp_put_msp(quint8 *ireq,quint16 len)
{
return udp_send_dat(NUM_SEND_MSP_DAT,(quint8*)ireq,len);
}

bool c_dev_cmd::udp_put_req_udb(alt_dat_req_t *ireq)
{
return udp_send_dat(NUM_SEND_UDB_REQ,(quint8*)ireq,sizeof(alt_dat_req_t));
}
bool c_dev_cmd::udp_get_udb(alt_dat_req_t *odat)
{
return udp_req_dat_from(NUM_REQ_UDB_DAT,0,(void*)odat);
}
bool c_dev_cmd::udp_put_udb(alt_dat_req_t *ireq)
{
return udp_send_dat(NUM_SEND_UDB_DAT,(quint8*)ireq,sizeof(alt_dat_req_t));
}

bool c_dev_cmd::udp_put_req_upow(alt_dat_req_t *ireq)
{
return udp_send_dat(NUM_SEND_UPOW_REQ,(quint8*)ireq,sizeof(alt_dat_req_t));
}
bool c_dev_cmd::udp_get_upow(alt_dat_req_t *odat)
{
return udp_req_dat_from(NUM_REQ_UPOW_DAT,0,(void*)odat);
}
bool c_dev_cmd::udp_put_upow(alt_dat_req_t *ireq)
{
return udp_send_dat(NUM_SEND_UPOW_DAT,(quint8*)ireq,sizeof(alt_dat_req_t));
}

bool c_dev_cmd::udp_rd_par_dev(quint8 *btmp)
{
return udp_req_dat_from(NUM_RD_PAR_DEV_CMD,0,(void*)btmp);
}

bool c_dev_cmd::udp_wr_par_dev(quint16 ipar)
{
quint16 htmp=ipar;
return udp_send_dat(NUM_WR_PAR_DEV_CMD,(quint8*)&htmp,sizeof(quint16));
}

///+++++++++++++++++++++++++++++++++++++++++++++++++++

///+++++++++++++++++++++++++++++++++++++++++++++++++++

