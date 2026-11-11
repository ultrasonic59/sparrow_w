#ifndef _ROOK_PARAM_H_
#define _ROOK_PARAM_H_

#include "umka2_types.h"
///++++++++++++++++++++++++++++++++
#define  DEF_MAX_TACT	 0x7
#define  DEF_MAX_IZM	 0x1///0x2
#define DEF_NUM_GEN 	8
#define DEF_NUM_US 		8
#define DEF_NUM_TACT 	1
#define DEF_NUM_IZM 	2
#define DEF_POW_VOL 	0x40
#define DEF_LEN_IMP		8
#define DEF_DET_PAR		0
#define DEF_NUM_PV		2
#define DEF_TIME_VRCH	4
#define DEF_KUS_VRCH	0


#define DEF_KUS 	40
#define DEF_TT		5
#define DEF_TV		7
#define DEF_ALPHA  	0
#define DEF_T_PR  	500
#define DEF_CC  	5900
#define DEF_BEGR  	0
#define DEF_STPR  	10
#define DEF_OTS  	0
#define DEF_POR1	80
#define DEF_BSTR1	10
#define DEF_LSTR1	20
#define DEF_DN_UP	0	///
#define DEF_ON_VIBRO	0	///

#define 	MAX_KUS	85	
#define  DEF_LEN_HIPOW	 2000	///20000////12000	///6000
#define  DEF_LEN_TACT	4000///	16000	///8000



extern 	uint32_t g_changed_param;
///=========================
#if 0
#define N_CHAN		0
#define N_TACT		1
#define N_NUS		2
#define N_ONUS  	3
#define N_KUS		4
#define N_BEGR		5
#define N_ENDR		6
#define N_TT		7
#define N_TV		8
#define N_CURSTRB	9

///#define N_STRB		10
///#define N_NSTRB		11
///#define N_LINK_STRB	12

///#define N_LSTRB		11
///#define N_POR			12

#define N_NUM_PV	13
#define N_VRON		14
#define N_VCP		15
#define N_VAP		16
#define N_CGEN		17

#define N_ONGEN  	18

#define N_LIMP		19
#define N_NPOW		20
#define N_PVAL		21
#define N_PROBE		22

#define N_ROSC		23
#define N_AXIS_XY	24
#define N_ARK	        25
#define N_THICK	        26
#endif
///#define N_CURPV		25
////#define N_CSTRB		26

#define N_ALL			31

#define NUM_PARAM 25

///==================================
#define CHNG_CHAN	(0x1<<0) ///(0x1<<N_CHAN)
#define CHNG_TACT	(0x1<<1)///(0x1<<N_TACT)
#define CHNG_NUS	(0x1<<2)
#define CHNG_ONUS  	(0x1<<3)
#define CHNG_KUS	(0x1<<4)
#define CHNG_BEGR	(0x1<<5)
#define CHNG_ENDR	(0x1<<6)
#define CHNG_TT		(0x1<<7)
#define CHNG_TV		(0x1<<8)
#define CHNG_CURSTRB	(0x1<<9)///(0x1<<N_CURSTRB)
#define CHNG_BSTRB	(0x1<<10)///(0x1<<N_STRB)
#define CHNG_LSTRB		(0x1<<11)
#define CHNG_POR		(0x1<<12)

///#define CHNG_NUMSTRB	(0x1<<N_NSTRB)
///#define CHNG_LINK_STRB	(0x1<<N_LINK_STRB)

#define CHNG_NUMPV	(0x1<<13)
#define CHNG_VRON	(0x1<<14)
///#define CHNG_VCP	(0x1<<N_VCP)		///curent point
///#define CHNG_VAP	(0x1<<N_VAP)		/// all points

#define CHNG_VTIME	(0x1<<15)
#define CHNG_VKUS	(0x1<<16)
#define CHNG_CGEN	(0x1<<17)///(0x1<<N_CGEN)

#define CHNG_ONGEN  (0x1<<18)

#define CHNG_LIMP		(0x1<<19)
#define CHNG_NPOW		(0x1<<20)
#define CHNG_PVAL		(0x1<<21)

///#define CHNG_PROBE	        (0x1<<N_PROBE)

#define CHNG_REJ_OSC	        (0x1<<23)


#define CHNG_IZM		(0x1<<24)
////#define CHNG_VDP		(0x1<<25)
////#define CHNG_LEN_TACT	(0x1<<26)

#define CHNG_ALT		(0x1<<30)
#define CHNG_ALL		(0x1<<31)

///#define MAX_CHANG_PARAMS	20
#define OFFS_NUM_BYTES	0
#define OFFS_FLG_CHNG	2
#define OFFS_PARAM_DAT	6
#define _MAX_NUM_PARAM	27

#if 0
typedef struct sys_par_s
{
uint32_t num_dev;
uint32_t ip_addr;
uint8_t  type_conn;
}sys_par_t;	

typedef struct ans_par_s
{
uint16_t num_par;
uint16_t len_name;
uint8_t  *name;
}ans_par_t;	
#endif

extern uint8_t  *t_par_buff;

////extern sys_par_t sys_par;
/// extern ed_par_trk_t ed_par_trk;
#define cur_num_par sys_par.curr_nparam

extern void set_changed_param(uint8_t*);
extern void _set_changed_param(void);
extern int get_cmd(cmd_t *cmd,char *istr);
extern int load_sys_par(void);
extern int save_sys_par(void);
///extern int load_param(uint16_t num_par);
///extern int save_param(uint16_t num_par);
extern int load_par_num(uint16_t num);
extern int save_par_num(uint16_t num);
extern int load_cur_par(void);
extern int save_cur_par(void);
extern void put_all_par_trk(void);
////extern uint8_t flg_need_save_par;

#define CUR_PAR_NAME "curr.par"
#define PER_WR_PAR      1000
#define TIMER_SAVE_PAR_ID	111

///====================
#endif	///_ROOK_PARAM_H_
