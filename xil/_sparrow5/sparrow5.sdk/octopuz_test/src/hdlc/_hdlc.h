#ifndef __HDLC_H_
#define __HDLC_H_ 

#include "octopuz_types.h"

//=====================

#ifndef TRUE 
#define	TRUE 0x1
#endif
#ifndef FALSE
#define	FALSE 0
#endif

#ifndef NULL
#define NULL 0                   
#endif
//=====================


///========================================
#define PPP_FRAME	0x7E
#define PPP_ESCAPE	0x7D
#define PPP_ESCAPE_BIT	0x20

///========== put to goat_cpu ====================

#define CMD_PUT_DAT  			0x3	
#define CMD_REQ_DAT  			0x4	
#define CMD_PUT_REQ_DAT  	    0x5
////#define CMD_PUT_CMD  			0x6

///#define CMD_PUT_BUF  			0x5	
///#define CMD_PRG_FLASH  		0x6	
///#define CMD_VER_FLASH  		0x7	
////#define CMD_PRG_PAR  			0x8	
///#define CMD_PUT_CMD  			0x9	

///#define CMD_REQ_STAT  		0xa		

///========== get from dev ====================
///#define CMD_GET_DAT  			0x84	
#define CMD_ACK  					0x85
#define CMD_REZ  					0x86
///===================================
#if 1
#define SEND_DAT			(0x1<<0)
#define SEND_ACK			(0x1<<1)
#define SEND_REQ_DAT			(0x1<<2)
#define SEND_REQ			(0x1<<3)
#define SEND_REZ			(0x1<<4)
#endif

/*
#ifndef MAX_FRAME_DATA
	#define MAX_FRAME_DATA	1000	////8192		// Макс. длина I-поля кадра
#endif
#ifndef MAX_FRAME_LEN
	#define MAX_FRAME_LEN	(MAX_FRAME_DATA+sizeof(hdlc_hdr_t)+2)// Макс. длина кадра (C+I+FCS)
#endif
*/
#define HDLC1_MAX_LEN_DATA		512		//	4096				/// 0x2000 Макс. длина I-поля кадра
#define HDLC1_MAX_FRAME_DATA	1024	//	8192				/// 0x2000 Макс. длина I-поля кадра
///#define HDLC1_MASK_FRAME_DATA	HDLC1_MAX_FRAME_DATA-1	/// 0x1fff
#define HDLC1_MAX_FRAME_LEN	(HDLC1_MAX_FRAME_DATA+2)// Макс. длина кадра (C+I+FCS)

///========================================
#define TIMEOUT_ACK	100
#define TIMEOUT_SEND	10
#define TIMEOUT_RSV	500
///========================================

///==================================
typedef struct hdlc_stat_s_
{
int		state;
int		phase;
uint16_t	frame_in_pos  ;				// Позиция для записи следующего принятого байта
uint8_t 	*in_buf;				// Буфер принимаемого в данный момент кадра
uint8_t 	*obr_buff;				//
uint16_t len_obr_dat;
uint8_t  in_esc_byte;				// Флаг принятия ESC-байта
///uint8_t  addr_res;
///uint8_t  addr_snd;
uint8_t 	*snd_buff;				//
uint8_t 	*t_snd_buff1;				//
uint8_t 	*t_snd_buff2;				//
uint16_t	frame_out_len;
uint16_t	offs_snd_buf;
void *t_rsv_buff;
uint16_t	snd_buff2_len;
QueueHandle_t ev_rsv_ack;
QueueHandle_t ev_rsv_dat;
QueueHandle_t ev_rsv_frame;
QueueHandle_t ev_snd_frame;
///xQueueHandle ua_rsv_dat;
QueueHandle_t ua_snd_dat;
void_fn_u8 fn_send_byte;
int_fn_u8_pvoid fn_set_rec_dat;
///extern int hdlc1_set_rec_dat(uint8_t cmd,void *in_buf);
//// int_fn_uint8_t_pvoid fn_get_rez;
///extern int hdlc1_get_rez(uint8_t type_rez,void *obuf);
u16_fn_u8_u16_pu8 fn_get_req_dat;
u8_fn_pvoid_pu8_u16 fn_send_frame;

///extern uint16_t hdlc1_get_req_dat(uint8_t num_req,uint8_t *buf);

int bytes_sent;
	int bytes_received;
///	int bytes_over_received;
	int	frames_sent;
	int	frames_received;
///	int	err_len;
///	int err_fcs;
///	int	err_unsup;
///	int	err_timeouts;
///	int	err_order;
///	int	err_event;
////	int wrn_hdlc_channel_timeouts;
}hdlc_stat_t;
///================================
////extern int need_send;
extern int checkfcs16(unsigned char *cp, register int len );
extern void hdlc1_init(hdlc_stat_t *p_hdlc);
extern uint8_t hdlc1_send_iframe_s(hdlc_stat_t *p_hdlc,uint8_t *data, int len);
extern uint8_t hdlc1_send_iframe_ns(hdlc_stat_t *p_hdlc,uint8_t *data, int len);
										
///extern int req_dat_goat(uint8_t icmd,uint16_t offs,void *obuf);
///extern BOOL send_dat_goat(uint8_t icmd,uint8_t *idat,uint16_t len);
extern int hdlc1_on_bytein(hdlc_stat_t *p_hdlc, uint8_t in_bt );
extern uint8_t hdlc1_send_dat(hdlc_stat_t *p_hdlc, uint8_t icmd, uint8_t *idat, uint16_t len);
extern void obr_hdlc1_cmd(hdlc_stat_t *p_hdlc);
extern void hdlc1_obr_frame(void *pdata);
extern void hdlc1_snd_task(void *pdata);

typedef struct
{
	uint16_t type_send;
	uint16_t len;
	uint8_t *p_data;
} t_frametx;

#endif ////__HDLC_H_
