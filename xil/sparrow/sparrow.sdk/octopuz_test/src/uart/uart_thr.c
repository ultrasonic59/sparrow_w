#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
///#include "mico_rtos.h"
#include "octopuz.h"
#include "my_misc.h"
///#include "mx_wnet.h"
#include "types.h"
#include "octopuz_types.h"
#include "ringbuffer.h"
///#include "drv_bt.h"
///#include "bt_cmd.h"
///#include "list_param.h"
///#include "link_msp.h"
#include "_hdlc.h"
#include "udp_cmd.h"
#include "drv_utrk.h"

extern 	uint32_t g_changed_param;
extern uint8_t *tmp_param_buff;
///extern sys_par_t sys_par;
////extern par_trk_t curr_par_trk;
extern int send_dat_bt(uint8_t *idat,int len);
extern uint8_t flg_req_osc;
extern uint8_t osc_rez[];
extern int uart_bt_rx_rdy(void);
extern int uart_bt_read(uint8_t* o_ch, int cnt);
extern uint16_t curr_bat;
///===========================
hdlc_stat_t g_hdlc_bt;
void init_hdlc_bt(void);
///bt_rez_dat_strobs_t bt_strobs_dat;
////bt_rez_dat_strobs_t bt_strobs_dat_a;

extern void on_int_tx_uart_bt(void);
volatile int res_send;
///=========================================
void hdlc_bt_send_byte_(uint8_t val)
{
uint8_t btmp = val;
res_send = xQueueSend(g_hdlc_bt.ua_snd_dat, &btmp, portMAX_DELAY);
on_int_tx_uart_bt();
}
void hdlc_bt_send_byte(uint8_t val)
{
send_byte_uart0(val);
}
///==========================================

extern uint16_t	pc_get_req_dat(uint8_t num_req,uint16_t offs,uint8_t *buf);
////extern int      pc_set_rec_dat(uint8_t cmd,void *in_buf);
extern int pc_set_rec_dat(uint8_t cmd,void *in_buf);

TaskHandle_t  bt_thr_rx;
TaskHandle_t  bt_thr_tx;
///==========================================
#if 1
#define RX_MSP_BUFFER_SIZE 512
ringbuffer_t rx_msp_buffer;
#endif
///=======================
void init_hdlc_bt(void)
{
#if 1
if(ringbuffer_init(&rx_msp_buffer,RX_MSP_BUFFER_SIZE)==0)
  {
  _printk("\r\n+++ringbuffer_init[rx_msp_buffer] error!!! +++\r\n");
  }
else
	  _printk("\r\n+++ringbuffer_init[rx_msp_buffer] OK !!! +++\r\n");

#endif

g_hdlc_bt.fn_send_byte=hdlc_bt_send_byte;
g_hdlc_bt.fn_set_rec_dat=(int_fn_u8_pvoid)pc_set_rec_dat;
g_hdlc_bt.fn_get_req_dat=pc_get_req_dat;////hdlc_bt_get_req_dat;
///g_hdlc_bt.ua_snd_dat=xQueueCreate(512,sizeof(unsigned char));		//
g_hdlc_bt.ua_snd_dat=xQueueCreate(204,sizeof(unsigned char));		//
g_hdlc_bt.ev_rsv_ack=xQueueCreate(1,1);
g_hdlc_bt.ev_rsv_dat=xQueueCreate(1,1);
g_hdlc_bt.ev_rsv_frame=xQueueCreate(1,1);
g_hdlc_bt.ev_snd_frame=xQueueCreate(16, sizeof(uint32_t));

hdlc1_init(&g_hdlc_bt);

xTaskCreate(hdlc1_obr_frame, (const char*)"HDLC_BT_rx",BT_RX_STACK_SIZE, (void*)&g_hdlc_bt, APP_PRIORITY, &bt_thr_rx);
xTaskCreate(hdlc1_snd_task, (const char*)"HDLC_BT_tx", BT_TX_STACK_SIZE, (void*)&g_hdlc_bt, APP_PRIORITY, &bt_thr_tx);
///	task_queue_init();
}

///=======================

