#ifndef _OCTOPUZ_TYPES_H_
#define _OCTOPUZ_TYPES_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "octopuz_config.h"
#ifndef MIN
#define MIN(a,b) (a<b?a:b)
#endif
typedef void (*void_fn_void)(void);

typedef void (*void_fn_u8)(uint8_t);
typedef void (*void_fn_int)(int);
typedef void (*void_fn_pvoid)(void*);
typedef int (*int_fn_u8_pvoid)(uint8_t,void *);
typedef int (*int_fn_pvoid)(void*);
typedef int (*int_fn_void)(void);
typedef uint8_t (*u8_fn_void)(void);
typedef uint8_t (*u8_fn_u8)(uint8_t);
typedef uint16_t (*u16_fn_void)(void);
typedef uint16_t (*u16_fn_pu8)(uint8_t*);
typedef uint16_t (*u16_fn_u8_pu8)(uint8_t,uint8_t*);
typedef uint16_t (*u16_fn_u8_u16_pu8)(uint8_t,uint16_t,uint8_t*);
///typedef void (*void_fn_int)(int);
///typedef void (*void_fn_pvoid)(void*);
typedef uint32_t (*u32_fn_void)(void);
typedef int16_t (*s16_fn_void)(void);
typedef uint32_t (*u32_fn_s16)(int16_t);
typedef void* (*pvoid_fn_void)(void);
typedef int32_t (*s32_fn_void)(void);
typedef void (*void_fn_u8_pvoid)(uint8_t,void *);
typedef uint8_t (*u8_fn_pvoid_pu8_u16)(void*,uint8_t*,uint16_t);


typedef struct __attribute__ ((__packed__)) par_gen_s
{
uint8_t on_gen;
uint8_t len_imp;
}par_gen_t;

typedef struct __attribute__ ((__packed__)) par_strb_s
{
uint8_t on_strb;
uint8_t	por;
uint16_t offs_type;	///
uint16_t beg;
uint16_t len;
}par_strb_t;

typedef struct __attribute__ ((__packed__)) par_vrch_s
{
	uint8_t on_vrch;
	uint8_t	num_p;
	uint16_t	time[MAX_NUM_PVRCH];
uint8_t	kus[MAX_NUM_PVRCH];
}par_vrch_t;
typedef struct __attribute__ ((__packed__)) par_us_s
{
uint8_t on_us;
uint8_t num_pus;
uint8_t rej;
uint8_t kus;
uint8_t detpar;
uint8_t 	taut;
uint8_t 	tauv;
par_strb_t strb[MAX_NUM_STRB];
par_vrch_t vrch;
}par_us_t;

typedef struct __attribute__ ((__packed__)) par_takt_s
{
uint16_t len_hipow;
uint16_t len_tact;
par_gen_t gens[NUM_GENS];
par_us_t uss[NUM_USS];
}par_takt_t;
typedef struct delt_point_vrch_s
{
	uint16_t	dtime;
	int16_t	dkus;
}delt_point_vrch_t;

////#define MAX_STROBS_OBR 2
////#define NUM_TOL_CH			3
typedef struct amp_tim_s
{
	uint8_t amp;
	uint16_t	time;
}amp_tim_t;
#define MAX_ALT_DAT_LEN 8	///64
typedef struct alt_dat_req_s
{
	uint16_t addr;
	uint16_t nbytes;
	uint16_t data[MAX_ALT_DAT_LEN];
}alt_dat_req_t;

///====================================

typedef struct __attribute__ ((__packed__)) udp_rez_dat_strob_s
{
	uint8_t amp;
	uint16_t time;
}udp_rez_dat_strob_t;

typedef struct __attribute__ ((__packed__)) udp_rez_dat_strobs_s
{
udp_rez_dat_strob_t strb_dat[NUM_STRB_REZ];
}udp_rez_dat_strobs_t;
///==================================================
typedef struct __attribute__ ((__packed__)) par_pow_s
{
uint8_t on_pow;
uint8_t val;
}par_pow_t;
typedef struct __attribute__ ((__packed__)) par_izm_s
{
uint8_t num_tacts;		///1...6
uint8_t num_izm;			///1...15
uint8_t num_summ;			///1...15
}par_izm_t;

typedef struct __attribute__ ((__packed__)) par_trk_s
{
par_izm_t izm_par;
par_pow_t pow_gen[NUM_POW_GENS];
par_takt_t takts[NUM_TACTS];
}par_trk_t;

typedef struct __attribute__ ((__packed__))  par_osc_s
{
uint8_t	curr_tact;				///
uint8_t  curr_us;
uint8_t  num_inp;
uint16_t	curr_begr;				///
uint16_t	curr_end_r;				///
uint8_t  curr_step_r;
uint8_t  curr_strob;
uint8_t  curr_pvrch;
uint8_t  rej_osc;
}par_osc_t;
typedef struct __attribute__ ((__packed__))  par_spectr_s
{
uint8_t	spectr_on;				///
uint8_t  spectr_shift;
uint16_t	spectr_first_el;				///
uint16_t	spectr_lenght_el;				///
}par_spectr_t;

typedef struct __attribute__ ((__packed__))   par_vdp_s
{
uint8_t	on_off;				///
uint8_t  rez;
uint16_t	speed;				///
}par_vdp_t;

typedef struct __attribute__ ((__packed__)) par_contr_s
{
par_trk_t trk_par;
uint8_t npow;		///2
uint8_t ngen;			///8

par_osc_t osc_par;
par_spectr_t spectr;
par_vdp_t vdp;
}par_contr_t;

extern par_contr_t curr_par_contr;

#define curr_par_trk curr_par_contr.trk_par
#define curr_osc_par curr_par_contr.osc_par

#define _CUR_NUM_IZM 	curr_par_trk.izm_par.num_izm
#define CUR_NUM_TACTS 	curr_par_trk.izm_par.num_tacts

typedef struct dat_tact_s
{
uint8_t amps[NUM_STRB_REZ];
uint16_t tims[NUM_STRB_REZ];
}amp_dat_chan_t;

typedef struct us_dat_tact_one_izm_s
{
uint16_t amps[NUM_STRB_REZ+1];
uint32_t tims[NUM_STRB_REZ];
}us_dat_tact_one_izm_t;
typedef struct us_dat_tact_n_izm_s
{
us_dat_tact_one_izm_t one_izm[MAX_NUM_IZM];
}us_dat_tact_n_izm_t;
typedef struct us_dat_rez_s
{
us_dat_tact_n_izm_t us_dat_tact[NUM_TACT_REZ];
}us_dat_rez_t;
typedef struct dat_rez_s
{
uint32_t cnt_dat;
us_dat_rez_t uss_data[NUM_USS_REZ];
}dat_rez_t;

typedef struct amp_strob_struct_
{
uint16_t time;
uint8_t ampl;
}rez_amp_strob_t;

typedef struct amp_us_struct_
{
rez_amp_strob_t ampl_str[NUM_STRB_REZ];
}rez_amp_us_t;

typedef struct amp_tact_struct_
{
rez_amp_us_t ampl_us[NUM_USS_REZ];
}rez_amp_tact_t;

typedef struct amp_struct_
{
rez_amp_tact_t ampl_tact[NUM_TACT_REZ];
}rez_amp_t;


typedef struct amp_respond_
{
uint32_t coord;			///mm
rez_amp_t ampl;
}amp_respond_t;
typedef struct amp_buff_head_
{
uint8_t staus;			///.4 contr [.3...0] bl
////contr_stat_t stat_contr;
uint8_t count;
}amp_buff_head_t;
typedef struct amp_buff_
{
amp_buff_head_t rez_head;
amp_respond_t elem[MAX_AMPL_BUFF_LEN];
}amp_buff_t;

///====================
#endif	///_OCTOPUZ_TYPES_H_
