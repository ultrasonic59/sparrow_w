#ifndef _DEV_INTERF_H_
#define _DEV_INTERF_H_
///=====================================================

#define UDP_MAX_FR_LEN 1000
#if 0
typedef struct udp_stat_s_
{
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
}udp_stat_t;
#endif
#define MAX_ALT_DATA_LEN 64

typedef struct _adc_spi_req_s
{
quint8 addr;
quint8 resv;///nbytes;
quint16 data;///[MAX_SPI_ADC_LEN];
}adc_spi_req_t;
typedef struct _alt_dat_req_s
{
quint16 addr;
quint16 nbytes;
quint16 data[MAX_ALT_DATA_LEN];
}alt_dat_req_t;
extern alt_dat_req_t g_alt_dat_req;
extern alt_dat_req_t g_alt_dat;

typedef struct _cpu_dat_req_s
{
quint32 addr;
quint32 nbytes;
quint32 *data;///[MAX_SPI_ADC_LEN];
}cpu_dat_req_t;

#define NUM_STROBS_OBR 2
typedef struct amp_tim_s
{
unsigned char  amp[NUM_STROBS_OBR];
unsigned short time;
}amp_tim_t;
///==== zamer for send =======
typedef struct head_snd_zamer_s
{
quint32 num_zamer;
quint8	new_zamer;
quint8	type;
quint16 size_dat;
quint8	rzv[4];
}head_snd_zamer_t;
typedef struct snd_zamer_s
{
head_snd_zamer_t head_snd_zamer;
void *data;
}snd_zamer_t;

#define NO_NEW_ZAMER	0
#define NEW_ZAMER		1

#define TYPE_NO_ARC		0
#define TYPE_ARC_MLZO	1
#define TYPE_ARC_LZMA	2
#define ERROR_NUM_ZAMER	0xffffffff

////extern udp_stat_t g_udp_pc;

#define CMD_ACK  					0x85

#define CMD_PUT_DAT  			0x3	
#define CMD_REQ_DAT  			0x4	
#define CMD_PUT_REQ_DAT  		0x5	
#define CMD_PUT_BUF  			0x6	

#define SEND_DAT					(0x1<<0)
#define SEND_ACK					(0x1<<1)
#define SEND_REQ_DAT				(0x1<<2)
#define SEND_REQ					(0x1<<3)
///++++++++++++++++++++++++++++++++++++++++++++++++
#define NUM_REQ_CONTRL_STAT  		0x5		/// 
#define NUM_REQ_ONCN  					0x6		/// 

#define NUM_REQ_STAT						0x8		/// 

#define NUM_REQ_REJ							0x9		/// 

#define NUM_SET_REJ						0xA		/// 
///+++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if 0

#define NUM_SET_PAR						0xB		/// 
#define NUM_PUT_CONTRL_CMD				0xC		/// 
#define NUM_SET_BUFF_CMD				0x10		/// 
///#define NUM_REQ_FDATE					0x11		/// 
////#define NUM_REQ_FTIME					0x12		/// 
#define NUM_REQ_CLR_ERR					0x13		/// 

#define NUM_SET_PAR_POW					0x19		
#define NUM_REQ_GET_AMPS_TIMS			0x1a		
#define NUM_REQ_GET_CONTR_STAT			0x1b		
#define NUM_SET_MARKER					0x1c		

////#define NUM_SET_CNGEN					0x1d		/// 
///#define NUM_SET_CNUS					0x1e		/// 
///#define NUM_SET_CNTACT					0x1f		/// curr takt
#define NUM_WR_BUFF_CMD					0x22		
///#define NUM_WR_ALL_PAR_CMD				0x23		
///#define NUM_SET_PAR_US					0x24		
///#define NUM_GET_PAR_US					0x25		
///#define NUM_SET_PAR_GEN					0x26		
///#define NUM_GET_PAR_GEN					0x27		
///#define NUM_SET_PAR_OSC					0x28		
///#define NUM_GET_PAR_OSC					0x29		
#define NUM_REQ_START_CONTR			0x2e	
#define NUM_REQ_STOP_CONTR			0x2f	
#define NUM_SET_MAX_TACT				0x30	
////#define NUM_REQ_GET_ZAMER				0x31
#endif
///++++++++++++++++++++++++++++++++++++++++++++++++
#define NUM_REQ_GET_AMPS				0x2a		
#define NUM_REQ_GET_TIMS				0x2b		
#define NUM_REQ_GET_OSC0				0x2c		
#define NUM_REQ_GET_OSC1				0x2d		
#define NUM_WR_N_PAR_CMD				0x32		
#define NUM_RD_PAR_DEV_CMD				0x33		
#define NUM_WR_PAR_DEV_CMD				0x34		

///#define NUM_SEND_SPI_ADC_DAT	0x90
///#define NUM_REQ_SPI_ADC_DAT		0x91
///#define NUM_SEND_SPI_ADC_REQ	0x92

#define NUM_SEND_XIL_DAT		0x93
#define NUM_REQ_XIL_DAT			0x94
#define NUM_SEND_XIL_REQ		0x95

////#define NUM_SEND_MOD_STR		0x96
////#define NUM_REQ_MOD_STR			0x97

#define NUM_SEND_STM_DAT		0xA0
#define NUM_REQ_STM_DAT			0xA1
#define NUM_SEND_STM_REQ		0xA2

#define NUM_SEND_MSP_DAT		0xA4
#define NUM_REQ_MSP_DAT			0xA5
#define NUM_SEND_MSP_REQ		0xA6

#define NUM_SEND_UDB_DAT		0xA7
#define NUM_REQ_UDB_DAT			0xA8
#define NUM_SEND_UDB_REQ		0xA9

#define NUM_SEND_UPOW_DAT		0xAA
#define NUM_REQ_UPOW_DAT		0xAB
#define NUM_SEND_UPOW_REQ		0xAC

#define MAX_REC_DAT				8192
///+++++++++++++++++++++++++++++++++
extern quint8 *buffOscs[];
///+++++++++++++++++++++++++++++++++
#define REQ_XIL_DAT	(0x1<<0)
#define GET_XIL_DAT	(0x1<<1)
#define PUT_XIL_DAT	(0x1<<2)

///extern int checkfcs16(unsigned char *cp, register int len );
////extern void addfcs16( unsigned char *cp, int len );
////extern quint16	_udp_send_to(quint8 type_send,quint8 type_dat);




///=====================================================
#endif
