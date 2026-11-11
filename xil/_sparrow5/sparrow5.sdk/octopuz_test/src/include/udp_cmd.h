#ifndef __UDP_CMD_H_
#define __UDP_CMD_H_ 
///========================================
////#define LEN_OSC			256


#define NUM_REQ_REJ				0x9		///

#define NUM_SET_REJ				0xA		///

#define NUM_SET_CNGEN			0x1d		/// 
#define NUM_SET_CNUS			0x1e		/// 
#define NUM_SET_CNTACT			0x1f		/// curr takt


#define NUM_REQ_GET_AMPS		0x2a
#define NUM_REQ_GET_OSC0		0x2c
#define NUM_REQ_GET_OSC0A		0x2d

////#define REQ_GET_DEV_ID		        0x30
///#define REQ_GET_PAR_NAMES		0x31
#define NUM_WR_N_PAR_CMD		0x32
///==========================================
#define REQ_GET_PAR		        0x33
////#define REQ_GET_ALL_PAR_NEW		0x34
	
///#define REQ_GET_NUM_PAR			0x35
////#define REQ_GET_NEXT_PAR		0x36

///#define REQ_SET_PAR_NAME		0x37
///#define REQ_SET_CUR_NUM			0x38
///#define REQ_SET_PEP_NAME		0x39
///#define REQ_GET_VERS		        0x3A
///#define REQ_PUT_ID		        0x3B
///=========================================
///#define NUM_OPEN_FILE		        0x80
///#define NUM_CLOSE_FILE			0x81
///#define NUM_RD_FILE		        0x82
///#define NUM_WR_FILE		        0x83
///#define NUM_GET_STAT		        0x84
///#define NUM_SEEK_FILE		        0x85
///#define NUM_SET_NAME		        0x86
///#define NUM_GET_ERR		        0x87

///=========================================
#define NUM_SEND_XIL_DAT		0x93
#define NUM_REQ_XIL_DAT			0x94
#define NUM_SEND_XIL_REQ		0x95

///#define NUM_SEND_STM_DAT		0xA0
///#define NUM_REQ_STM_DAT			0xA1
///#define NUM_SEND_STM_REQ		0xA2

///#define NUM_SEND_MSP_DAT		0xA4
///#define NUM_REQ_MSP_DAT			0xA5
///#define NUM_SEND_MSP_REQ		0xA6

#define NUM_SEND_UDB_DAT		0xA7
#define NUM_REQ_UDB_DAT			0xA8
#define NUM_SEND_UDB_REQ		0xA9

#define NUM_SEND_UPOW_DAT		0xAA
#define NUM_REQ_UPOW_DAT		0xAB
#define NUM_SEND_UPOW_REQ		0xAC
///===========================================================
#endif///__UDP_CMD_H_
