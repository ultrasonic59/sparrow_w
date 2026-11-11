#ifndef __BT_CMD_H_
#define __BT_CMD_H_ 
///========================================
#define BT_CMD_PUT						0x3		/// 
#define BT_CMD_REQ						0x4		/// 

#define BT_CMD_MIN_NUM			        0x2c	

#define BT_GET_OSC0				0x2c		

#define BT_PUT_PAR				0x32	
#define BT_GET_ALL_PAR				0x33	
#define BT_GET_ALL_PAR1			        0x34	
#define BT_GET_NUM_PARAMS			0x35	
#define BT_GET_NEXT_PARAM_NAME			0x36	
#define BT_SET_PARAM_NAME			0x37	
#define BT_SET_CURR_PAR_NUM			0x38	

#define BT_CMD_MAX_NUM			        0x38	


/*
#define NUM_SET_CNTACT					0x1f		/// curr takt


#define NUM_REQ_GET_AMPS				0x2a
#define NUM_REQ_GET_OSC0				0x2c		

#define NUM_WR_N_PAR_CMD				0x32

#define NUM_SEND_XIL_DAT		0x93
#define NUM_REQ_XIL_DAT			0x94
#define NUM_SEND_XIL_REQ		0x95

#define NUM_SEND_STM_DAT		0xA0
#define NUM_REQ_STM_DAT			0xA1
#define NUM_SEND_STM_REQ		0xA2

#define NUM_SEND_MSP_DAT		0xA4
#define NUM_REQ_MSP_DAT			0xA5
#define NUM_SEND_MSP_REQ		0xA6
*/

extern uint16_t bt_set_rec_param(void);
extern uint16_t bt_set_param_name(void);
extern uint16_t bt_get_all_par(void);
extern uint16_t bt_get_all_par1(void);
extern uint16_t bt_get_num_params(void);
extern uint16_t bt_get_next_param_name(void);
extern uint16_t bt_set_curr_par_num(void);

#endif///__UDP_CMD_H_
