#ifndef __DEFECTOSCOPE_H_
#define __DEFECTOSCOPE_H_
#include "drv_utrk.h"
#include "octopuz_types.h"
#include "_param.h"
#include "ringbuffer.h"

extern circ_buffer_t cb_rezult;

extern uint8_t *tmp_param_buff;

////extern ed_par_trk_t ed_par_trk;
////extern par_contr_t curr_param_contr;
////#define curr_par_trk curr_param_contr.par_trk
////extern par_trk_t curr_par_trk;

extern void defectoscope_init(void);
extern void set_pow_vol(uint8_t num,uint8_t i_vol);
////extern int  udp_send(const void *buf, size_t len);
extern void set_len_per_tact(uint8_t i_tact,uint16_t i_dat);
extern void set_len_hipow_tact(uint8_t i_tact,uint16_t i_dat);
extern uint16_t get_len_hipow_tact(uint8_t i_tact) ;
extern void set_len_imp_tact(uint8_t i_tact,uint8_t i_num,uint8_t i_dat);
extern void set_on_gens_tact(uint8_t i_tact,uint16_t i_dat);

extern uint8_t get_len_imp_tact(uint8_t i_tact,uint8_t i_num);
extern void set_begr_osc_a(uint16_t idat);
extern void set_stp_osc_a(uint8_t idat);

///extern void set_changed_param(uint8_t *ibuff);
///extern void set_changed_param(void);


#endif 

