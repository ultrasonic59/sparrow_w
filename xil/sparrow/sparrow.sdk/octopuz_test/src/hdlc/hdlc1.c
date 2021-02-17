#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "_hdlc.h"
#include "my_misc.h"
////#include "printk.h"
#include "ringbuffer.h"

extern void addfcs16(unsigned char *cp,int len );
///============================
/*
xQueueHandle ev_rsv_ack;
xQueueHandle ev_rsv_dat;
xQueueHandle ev_rsv_frame;
xQueueHandle ev_snd_frame;
///xQueueHandle ua_rsv_dat;
xQueueHandle ua_snd_dat;
*/
///============================
///==================================
////#define HDLC1_OBR_BUF_LENGTH_MAX   8192
///#define hdlc1_obr_buff g_hdlc_st.obr_buff
///#define hdlc1_tbuff1_send g_hdlc_st.t_snd_buff1
///#define hdlc1_tbuff2_send g_hdlc_st.t_snd_buff2

///==============================
void hdlc1_error( char const *err )
{
puts(err);
}

///==============================
void hdlc1_init(hdlc_stat_t *p_hdlc)
{
p_hdlc->frame_in_pos  =0;	
p_hdlc->in_esc_byte		=0;	
p_hdlc->frame_out_len	= 0;
p_hdlc->in_buf = (uint8_t*) pvPortMalloc( HDLC1_MAX_FRAME_LEN );
if( p_hdlc->in_buf == NULL ) 
	hdlc1_error("hdlc_frames_init(): NO MEM\n");

p_hdlc->snd_buff	= (uint8_t*)pvPortMalloc(HDLC1_MAX_FRAME_LEN);
if(p_hdlc->snd_buff == NULL )
	hdlc1_error("hdlc1_init(): NO MEM FOR SND_BUF\n");
p_hdlc->obr_buff	= (uint8_t*)pvPortMalloc(HDLC1_MAX_FRAME_LEN);
if( p_hdlc->obr_buff == NULL )
	hdlc1_error("hdlc1_init(): NO MEM FOR OBR_TMPBUF\n");
	
p_hdlc->t_snd_buff1 = (uint8_t*)pvPortMalloc(HDLC1_MAX_FRAME_LEN);
if(p_hdlc->t_snd_buff1 == NULL)
	{
	hdlc1_error("hdlc1_init(): NO MEM FOR hdlc1_tbuff_send\n");
	return ;	
	}
memset(p_hdlc->t_snd_buff1,0,HDLC1_MAX_FRAME_LEN);	
p_hdlc->t_snd_buff2 = (uint8_t*)pvPortMalloc(HDLC1_MAX_FRAME_LEN);
if(p_hdlc->t_snd_buff2 == NULL)
	{
	hdlc1_error("hdlc1_init(): NO MEM FOR hdlc1_tbuff_send\n");
	return ;	
	}


}

///======================================
///	Обработка ошибки длины входящего кадра
#if 0
void hdlc1_error_frame_len(hdlc_stat_t *p_hdlc)
{
p_hdlc->err_len++;
}
#endif
///volatile uint8_t tst_buf[512];
///int ptr_tst_buf=0;
///========================================================
uint8_t hdlc1_send_iframe(hdlc_stat_t *p_hdlc,uint8_t *data, int len)
{
////frame_t fr;
  int len_o=0;
uint16_t ii;
uint8_t bt;
uint8_t rez=TRUE;
////hdlc_hdr_t *hdr = (hdlc_hdr_t *)p_hdlc->snd_buff;
///================================
///snd_fr.data = g_hdlc_st.snd_buff;
memcpy(p_hdlc->snd_buff, data, len );
p_hdlc->frame_out_len=len ;
////snd_fr.len =len + sizeof(hdlc_hdr_t);
///================================
////put_tst1(1);
////_printk("\n\r ***hdlc1_send_iframe [%x] ***",len);
///================================
addfcs16(p_hdlc->snd_buff,p_hdlc->frame_out_len);				// Добавить циклический код
p_hdlc->frame_out_len += 2;
p_hdlc->fn_send_byte(PPP_FRAME);
for(ii=0;ii<p_hdlc->frame_out_len;ii++)
		{
		bt = p_hdlc->snd_buff[ii];
		if( bt == PPP_FRAME || bt == PPP_ESCAPE )			// Замена "FLAG" или "ESC" на ESC-последовательность
			{
			p_hdlc->fn_send_byte(PPP_ESCAPE);
		len_o++;
			p_hdlc->fn_send_byte((bt ^ PPP_ESCAPE_BIT));
			len_o++;
			}
		else
			{
			p_hdlc->fn_send_byte(bt);
			len_o++;
			}
		}
p_hdlc->fn_send_byte(PPP_FRAME);
///put_tst1(0);

////rez = hdlc1_send_frame(send_byte_ua0,&snd_fr);

return rez;
}

///==============================
int hdlc1_parse_iframe(hdlc_stat_t *p_hdlc,uint8_t *in_buf,uint16_t len)
{
int rez =0;
uint8_t btmp;
uint32_t wtmp=0;
uint16_t h_rez;
///===============================================
///_printk("\n\r ***hdlc1_parse_iframe [%x] ***",*in_buf);
///return rez;
///===============================================
switch(*in_buf)
	{
	case CMD_ACK:
		btmp = *(in_buf+1);
		xQueueSend(p_hdlc->ev_rsv_ack,&btmp,TIMEOUT_SEND);
		break;	
	case CMD_PUT_DAT:
		btmp = *(in_buf+1);
		p_hdlc->fn_set_rec_dat(btmp,(void *)((uint32_t)in_buf+2));
	///	xQueueSend(ev_rsv_dat,&btmp,TIMEOUT_SEND);
		wtmp=SEND_ACK;
		wtmp |= (btmp<<8);
		xQueueSend(p_hdlc->ev_snd_frame,&wtmp,TIMEOUT_SEND);
		break;
#if 0
	case CMD_PUT_CMD:
		btmp = *(in_buf+1);
		h_rez=p_hdlc->fn_set_rec_dat(btmp,(void *)((uint32_t)in_buf+2));
	///	xQueueSend(ev_rsv_dat,&btmp,TIMEOUT_SEND);
		wtmp=SEND_REZ;
		wtmp |= (btmp<<8);
		wtmp |= (h_rez<<16);
 		xQueueSend(p_hdlc->ev_snd_frame,&wtmp,TIMEOUT_SEND);
		break;
#endif
	case CMD_PUT_REQ_DAT:
		btmp = *(in_buf+1);
		p_hdlc->fn_set_rec_dat(btmp,(void *)((uint32_t)in_buf+2));
 ////???       xQueueSend(p_hdlc->ev_rsv_dat,&btmp,TIMEOUT_SEND);
		wtmp=SEND_ACK;
		wtmp |= (btmp<<8);
////                xQueueSend(p_hdlc->ev_rsv_dat,&btmp,TIMEOUT_SEND);
		xQueueSend(p_hdlc->ev_snd_frame,&wtmp,TIMEOUT_SEND);
		break;
	case CMD_REQ_DAT:
		btmp = *(in_buf+1);
		p_hdlc->offs_snd_buf = *(in_buf+2);
		p_hdlc->offs_snd_buf |= (*(in_buf+3))<<8;
		wtmp = SEND_REQ_DAT;
		wtmp |= (btmp<<8);
///		set_req_rez((req_rez_t *)((uint32_t)in_buf+1) );
////		_printk("\n\r rrr \n\r");
		xQueueSend(p_hdlc->ev_snd_frame,&wtmp,TIMEOUT_SEND);
		rez =0;
////		_printk("\n\r rrr \n\r");
		break;
	}
return rez;
}
///==================================
int hdlc1_check_framein(hdlc_stat_t *p_hdlc)
{
int res = 1;
if(p_hdlc->len_obr_dat < 3 )
	{
////	p_hdlc->err_len++;
	res = -1;
	}
else if( !checkfcs16(p_hdlc->obr_buff, p_hdlc->len_obr_dat - 2 ) )
	{
////	p_hdlc->err_fcs++;
	res = -2;
	}
return res;
}
///=====================================
void hdlc1_parse_framein(hdlc_stat_t *p_hdlc)
{
///int flg_parse;
////hdlc_hdr_t *hdr;
p_hdlc->frames_received++;
hdlc1_parse_iframe(p_hdlc, p_hdlc->obr_buff , p_hdlc->len_obr_dat);
}
///================================
int	hdlc1_send_ack_to(hdlc_stat_t *p_hdlc,uint8_t ipar)
{
int num_send=0;
uint8_t *p_tmp	 =p_hdlc->t_snd_buff1;
*p_tmp++  = CMD_ACK;
num_send++;
*p_tmp++  = ipar;
num_send++;
return hdlc1_send_iframe(p_hdlc,p_hdlc->t_snd_buff1, num_send);
////return 0;
}
int	hdlc1_send_rez_to(hdlc_stat_t *p_hdlc,uint8_t ipar,uint16_t rez)
{
int num_send=0;
uint8_t *p_tmp	 =p_hdlc->t_snd_buff1;
*p_tmp++  = CMD_REZ;
num_send++;
*p_tmp++  = ipar;
num_send++;
*p_tmp++  = rez&0xff;
num_send++;
*p_tmp++  = (rez>>8)&0xff;
num_send++;
return hdlc1_send_iframe(p_hdlc,p_hdlc->t_snd_buff1, num_send);
////return 0;
}

///====== from uart isr ============================
int hdlc1_on_bytein(hdlc_stat_t *p_hdlc, uint8_t in_bt )
{
p_hdlc->bytes_received++;
if( in_bt == PPP_FRAME )						// Пришел флаг
	{
	if(p_hdlc->frame_in_pos > 2 )				// Если между двумя флагами были данные:
		{
		memcpy(p_hdlc->obr_buff,p_hdlc->in_buf,p_hdlc->frame_in_pos);
		p_hdlc->len_obr_dat=p_hdlc->frame_in_pos;
		p_hdlc->frame_in_pos = 0;			
		p_hdlc->in_esc_byte	 = 0;
		return 1;
		}
	}
else
	{
	if(p_hdlc->frame_in_pos >= HDLC1_MAX_FRAME_LEN )
		{
////		hdlc1_error_frame_len(p_hdlc);
		printf("\n hdlc1_error_frame_len");
		return -1;
		}
	if(p_hdlc->in_esc_byte )								// Второй байт ESC-последовательности
		{
		p_hdlc->in_buf[p_hdlc->frame_in_pos++] = in_bt ^ PPP_ESCAPE_BIT;
		p_hdlc->in_esc_byte = 0;
		}
	else
		{
		if( in_bt == PPP_ESCAPE)						// Первый байт ESC-последовательности
			{
			p_hdlc->in_esc_byte = TRUE;
			}
		else								// Обычный байт данных
			{
			p_hdlc->in_buf[p_hdlc->frame_in_pos++] = in_bt;	// Запись пришедшего байта в буфер
			}
		}
	return 0;	
	}
return 0;	
}
///==============================
extern uint16_t pppfcs16(register uint16_t fcs, register unsigned char *cp, register int len);
///====================================
void obr_hdlc1_cmd(hdlc_stat_t *p_hdlc)
{
int res;
uint8_t ii;
if((res= hdlc1_check_framein(p_hdlc))>0 )	// Проверить правильность входящего кадра
	{
///==================================
	hdlc1_parse_framein(p_hdlc);
////    _printk("\n\r ***hdlc1_check_framein ok ***");
	}
else
	{
///	_printk("\n\r error framein[%d][%02x]",res,p_hdlc->len_obr_dat);
	if(res==-2)
		{
                  _printk("\n\r[%02x]:",p_hdlc->len_obr_dat);
	for(ii=0;ii<p_hdlc->len_obr_dat;ii++)
		{
		_printk("%02x ",p_hdlc->obr_buff[ii]);
		}
///	printf("%x\r\n ", 0xffff ^ pppfcs16( 0xffff, p_hdlc->obr_buff, p_hdlc->len_obr_dat-2));	// dbg!
	}
	;
	}
}
extern ringbuffer_t rx_msp_buffer;

///====================================
void hdlc1_obr_frame(void *pdata)
{
///uint8_t addr= *((uint8_t*)pdata);
hdlc_stat_t *p_hdlc=(hdlc_stat_t *)pdata;
uint8_t res_byte;
///uint8_t btmp=0x33;
///uint16_t cnt=0;
///_printk("\n\r=== hdlc1_obr_frame===  [%s]",pcTaskGetTaskName(NULL) );
for(;;)
	{
///	  while(ringbuffer_getc(&rx_msp_buffer ,&res_byte)<0);

	xQueueReceive(p_hdlc->ev_rsv_frame,&res_byte,portMAX_DELAY);
	obr_hdlc1_cmd(p_hdlc);
////	_printk("[%0.2x]",res_byte);
#if 0
	  while(ringbuffer_getc(&rx_msp_buffer ,&res_byte))
	  	  {
		  _printk("[%0.2x]",res_byte);
	  	  }
#endif
	}
}

/*
BOOL hdlc1_req_dat_from(hdlc_stat_t *p_hdlc,uint8_t addr,uint8_t icmd,uint16_t offs,void *obuf)
{
uint8_t res_byte;
uint16_t htmp;
static uint8_t ns=0;
int num_send=0;
BOOL res=FALSE;
uint8_t *p_tmp	 = p_hdlc->t_snd_buff2;
*p_tmp++  = CMD_REQ_DAT;
num_send++;
*p_tmp++  = icmd;
num_send++;
*p_tmp++  = offs&0xff;
num_send++;
*p_tmp++  = (offs>>8)&0xff;
num_send++;
p_hdlc->snd_buff2_len=num_send;
p_hdlc->t_rsv_buff=obuf;
htmp=SEND_REQ;
htmp |= (icmd<<8);
ns++;
xQueueSend(p_hdlc->ev_snd_frame,&htmp,TIMEOUT_SEND);
for(;;)
	{
///	if(	xQueuePeek(p_hdlc->ev_rsv_dat,&res_byte,TIMEOUT_RSV)==pdTRUE)
	if(	xQueueReceive(p_hdlc->ev_rsv_dat,&res_byte,TIMEOUT_RSV)==pdTRUE)
		{
		if(res_byte==icmd)
			{
////			xQueueReceive(p_hdlc->ev_rsv_dat,&res_byte,portMAX_DELAY);
			p_hdlc->fn_get_rez(icmd,obuf);
///			htmp=SEND_ACK;
///			htmp |= (icmd<<8);
///			xQueueSend(p_hdlc->ev_snd_frame,&htmp,TIMEOUT_SEND);
			res=TRUE;
			break;
			}
		else
			continue;	
		}
	else
		{
		res=FALSE;
		break;
		}
	}
return res;
}

BOOL hdlc1_send_dat(hdlc_stat_t *p_hdlc,uint8_t icmd,uint8_t *idat,uint16_t len)
{
uint8_t res_byte;
uint16_t htmp;
static uint8_t ns=0;
int num_send=0;
BOOL res=FALSE;
uint8_t *p_tmp	 =  p_hdlc->t_snd_buff2;
if(len==0)
	return FALSE;
/// *odat=0;
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
p_hdlc->snd_buff2_len=num_send;
htmp=SEND_DAT;
htmp |= (icmd<<8);
xQueueSend(p_hdlc->ev_snd_frame,&htmp,TIMEOUT_SEND);
if(	xQueuePeek(p_hdlc->ev_rsv_ack,&res_byte,TIMEOUT_RSV)==pdTRUE)
	{
	if(res_byte==icmd)
		{
		xQueueReceive(p_hdlc->ev_rsv_ack,&res_byte,portMAX_DELAY);
		res=TRUE;
		}
	}
else
	{
	res=FALSE;
	}	
return res;
}
BOOL hdlc1_send_buf(hdlc_stat_t *p_hdlc,uint8_t icmd,uint8_t *idat,uint16_t offs,uint16_t len)
{
uint8_t res_byte;
uint16_t htmp;
static uint8_t ns=0;
int num_send=0;
BOOL res=FALSE;
uint8_t *p_tmp	 =  p_hdlc->t_snd_buff2;
if(len==0)
	return FALSE;
/// *odat=0;
*p_tmp++  = CMD_PUT_DAT;
num_send++;
*p_tmp++  = icmd;
num_send++;
*p_tmp++  = len&0xff;
num_send++;
*p_tmp++  = (len>>8)&0xff;
num_send++;
*p_tmp++  = offs&0xff;
num_send++;
*p_tmp++  = (offs>>8)&0xff;
num_send++;
memcpy(p_tmp,idat,len);
num_send += len;
p_hdlc->snd_buff2_len=num_send;
htmp=SEND_DAT;
htmp |= (icmd<<8);
xQueueSend(p_hdlc->ev_snd_frame,&htmp,TIMEOUT_SEND);
if(	xQueuePeek(p_hdlc->ev_rsv_ack,&res_byte,TIMEOUT_RSV)==pdTRUE)
	{
	if(res_byte==icmd)
		{
		xQueueReceive(p_hdlc->ev_rsv_ack,&res_byte,portMAX_DELAY);
		res=TRUE;
		}
	}
else
	{
	res=FALSE;
	}	
return res;
}
*/
#if 0
uint8_t hdlc1_req_dat_from(hdlc_stat_t *p_hdlc, uint8_t addr, uint8_t icmd, uint16_t offs, void *obuf)
{
	t_frametx frame_tx;
	uint8_t res_byte;
	static uint8_t ns=0;
	uint8_t res=FALSE;
	uint8_t *p_tmp;

	frame_tx.len = 4;
	frame_tx.pdata = (uint8_t*)pvPortMalloc(frame_tx.len * sizeof(uint8_t));

	if (frame_tx.pdata == 0)
		return FALSE;

	p_tmp = frame_tx.pdata;

	*p_tmp++  = CMD_REQ_DAT;
	*p_tmp++  = icmd;
	*p_tmp++  = offs&0xff;
	*p_tmp++  = (offs>>8)&0xff;
	p_hdlc->t_rsv_buff=obuf;		// ???
	ns++;							// ?
	frame_tx.type_send = SEND_REQ;
	frame_tx.type_send |= (icmd<<8);
	if (xQueueSend(p_hdlc->ev_snd_frame, &frame_tx, TIMEOUT_SEND) == pdFAIL)
	{
		vPortFree(frame_tx.pdata);
		return FALSE;
	}

	if(	xQueueReceive(p_hdlc->ev_rsv_dat,&res_byte,TIMEOUT_RSV)==pdTRUE)
	{
		if(res_byte==icmd)
		{
//////?????			p_hdlc->fn_get_rez(icmd,obuf);
			res=TRUE;
		}
		else
			res=FALSE;	
	}
	else
	{
		res=FALSE;
	}
	/*
	for(;;)
	{
		if(	xQueuePeek(p_hdlc->ev_rsv_dat,&res_byte,TIMEOUT_RSV)==pdTRUE)
		///if(	xQueueReceive(p_hdlc->ev_rsv_dat,&res_byte,TIMEOUT_RSV)==pdTRUE)
		{
			if(res_byte==icmd)
			{
				xQueueReceive(p_hdlc->ev_rsv_dat,&res_byte,portMAX_DELAY);
				p_hdlc->fn_get_rez(icmd,obuf);
////			frame_tx.pdata = 0;
////			frame_tx.len = 0;
////			frame_tx.type_send = SEND_ACK;
////			frame_tx.type_send |= (icmd<<8);
////			xQueueSend(p_hdlc->ev_snd_frame,&frame_tx,TIMEOUT_SEND);
				res=TRUE;
				break;
			}
			else
				continue;	
		}
		else
		{
			res=FALSE;
			break;
		}
	}
	*/

	return res;
}
#endif
uint8_t hdlc1_send_dat(hdlc_stat_t *p_hdlc, uint8_t icmd, uint8_t *idat, uint16_t len)
{
uint8_t res_byte;
uint16_t htmp;
uint8_t res=FALSE;
int num_send=0;
uint8_t *p_tmp=p_hdlc->t_snd_buff2;
if (len == 0)
  return FALSE;
*p_tmp++  = CMD_PUT_DAT;
num_send++;
*p_tmp++  = icmd;
num_send++;
*p_tmp++  = len&0xff;
num_send++;
*p_tmp++  = (len>>8)&0xff;
num_send++;
memcpy(p_tmp, idat, len);
num_send += len;
p_hdlc->snd_buff2_len=num_send;
htmp=SEND_DAT;
htmp |= (icmd<<8);
if (xQueueSend(p_hdlc->ev_snd_frame, &htmp, TIMEOUT_SEND) == pdFAIL)
  {
  return FALSE;
  }
if(xQueuePeek(p_hdlc->ev_rsv_ack,&res_byte,TIMEOUT_RSV)==pdTRUE)
  {
  if(res_byte==icmd)
    {
    xQueueReceive(p_hdlc->ev_rsv_ack,&res_byte,portMAX_DELAY);
    res=TRUE;
    }
  }
else
  {
  res=FALSE;
  }	
return res;
}
#if 0
uint8_t hdlc1_send_buf(hdlc_stat_t *p_hdlc, uint8_t icmd, uint8_t *idat, uint16_t offs, uint16_t len)
{
	t_frametx frame_tx;
	uint8_t res_byte;
//	static uint8_t ns=0;
	uint8_t res=FALSE;
	uint8_t *p_tmp;

	if(len==0)
		return FALSE;

	frame_tx.len = 6 + len;
	frame_tx.pdata = (uint8_t*)pvPortMalloc(frame_tx.len * sizeof(uint8_t));

	if (frame_tx.pdata == 0)
		return FALSE;

	p_tmp = frame_tx.pdata;

	*p_tmp++  = CMD_PUT_DAT;
	*p_tmp++  = icmd;
	*p_tmp++  = len&0xff;
	*p_tmp++  = (len>>8)&0xff;
	*p_tmp++  = offs&0xff;
	*p_tmp++  = (offs>>8)&0xff;
	memcpy(p_tmp, idat, len);

	frame_tx.type_send = SEND_DAT;
	frame_tx.type_send |= (icmd<<8);
	if (xQueueSend(p_hdlc->ev_snd_frame, &frame_tx, TIMEOUT_SEND) == pdFAIL)
	{
		vPortFree(frame_tx.pdata);
		return FALSE;
	}

	if (xQueueReceive(p_hdlc->ev_rsv_ack,&res_byte,TIMEOUT_RSV)==pdTRUE)
	{
		if(res_byte==icmd)
			res = TRUE;
		else
			res = FALSE;
	}
	else
	{
		res = FALSE;
	}

	return res;
}
#endif
int	hdlc1_send_to(hdlc_stat_t *p_hdlc,uint8_t type_send,uint8_t type_dat,uint16_t rez)
{
int res=0;
uint16_t len_dat;
int num_send=0;
uint8_t *p_tmp1;
uint8_t *p_tmp	 = p_hdlc->t_snd_buff1;
////_printk("\n\r ***hdlc1_send_to [%x] ***",type_send);
///return res;

if(type_send == SEND_REZ)
	{
	res=hdlc1_send_rez_to(p_hdlc,type_dat,rez);
	}
else if(type_send == SEND_ACK)
	{
	res=hdlc1_send_ack_to(p_hdlc,type_dat);
	}
else if(type_send == SEND_REQ_DAT)
	{
	*p_tmp++  = CMD_PUT_REQ_DAT;
	num_send++;
	*p_tmp++  = type_dat;
	num_send++;
	p_tmp1 = p_tmp+2;  
	len_dat = p_hdlc->fn_get_req_dat(type_dat,p_hdlc->offs_snd_buf, p_tmp1);
	*p_tmp++  = len_dat&0xff;
	num_send++;
	*p_tmp++  = (len_dat>>8)&0xff;
	num_send++;
	if(len_dat > HDLC1_MAX_FRAME_LEN)
		len_dat = HDLC1_MAX_FRAME_LEN;
	num_send += len_dat;
	res=hdlc1_send_iframe(p_hdlc,p_hdlc->t_snd_buff1, num_send);
	}
else if(type_send == SEND_DAT)
	{
	num_send = p_hdlc->snd_buff2_len;
	memcpy(p_tmp,p_hdlc->t_snd_buff2,num_send);
	res=hdlc1_send_iframe(p_hdlc,p_hdlc->t_snd_buff1, num_send);
	}
else if(type_send == SEND_REQ)
	{
	num_send = p_hdlc->snd_buff2_len;
	memcpy(p_tmp,p_hdlc->t_snd_buff2,num_send);
	res=hdlc1_send_iframe(p_hdlc,p_hdlc->t_snd_buff1, num_send);
	}
return res;
}
extern void send_byte_uart0(uint8_t ch);

void hdlc1_snd_task(void *pdata)
{
////t_frametx frame_tx;
	uint8_t tstb=0;
uint32_t type_send;
hdlc_stat_t *p_hdlc=(hdlc_stat_t *)pdata;
////uint8_t addr= *((uint8_t*)pdata);
_printk("\n\r=== hdlc1_snd_task=== [%s]",pcTaskGetTaskName(NULL));
for(;;)
  {
#if 0
///	hdlc_bt_send_byte(tstb);
	send_byte_uart0(tstb);
	tstb++;
	vTaskDelay( 10 );
#endif
#if 1
  if(xQueueReceive(p_hdlc->ev_snd_frame,&type_send,portMAX_DELAY))
  	  {
///	  _printk("\n\r=== hdlc1_send_to");
	  hdlc1_send_to(p_hdlc,type_send&0xff,type_send>>8,type_send>>16);
  	  }
#endif
  }
}
///====================================


