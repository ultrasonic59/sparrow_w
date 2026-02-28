#ifndef _PARAMS_H_
#define _PARAMS_H_

#ifndef OFF
#define	OFF	0
#endif

#ifndef ON
#define	ON	1
#endif

#define NUM_DIG_NCHAN	1
#define NUM_DIG_NTACT	1
#define NUM_DIG_BEGR	4
#define NUM_DIG_ENDR	4

#define NUM_DIG_NUS		1
#define NUM_DIG_KUS		3	////2
#define NUM_DIG_TT		1
#define NUM_DIG_TV		2
#define NUM_DIG_TV		2
#define NUM_DIG_NSTROB	1
#define NUM_DIG_POR		3
#define NUM_DIG_BSTR	4
#define NUM_DIG_LSTR	4
#define NUM_DIG_NGEN	1
#define NUM_DIG_LEN_IMP	2
#define NUM_DIG_NPOW	1
#define NUM_DIG_VPOW	3
#define NUM_DIG_NPVRCH	1
#define NUM_DIG_TVRCH	4
#define NUM_DIG_KVRCH	2
///++++++++++++++++++++++++++++++++++++++++
#define	LEN_OSC	256
#define NUM_CHANS 		2
#define NUM_GENS 		2
#define NUM_POW_GENS 	1
#define NUM_USS 		2
#define MASK_NUM_USS 	0x1
#define MAX_NUM_STRBS 	4
///#define NUM_TACTS		2		///
///#define MAX_NUM_TACTS	4		///
#define NUM_PEP			2
#define NUM_TACTS		2		///
#define NUM_STRBS 		4

#define MAX_NUM_PVRCH	8

#define MAX_TIME_DEF	65000
#define	MAX_AMPL 255
#define	MIN_AMPL 1
///++++++++++++++++++++++++++++++
#define	DEF_POW_VAL		200
#define	DEF_LEN_IMP		8

#define	DEF_ALPHA		40
#define	DEF_BEGR		2
#define	DEF_CC			2600
#define	DEF_DETPAR		0
#define	DEF_KUS1		150
#define	DEF_KUS2		10
#define	DEF_STP_R		50
#define	DEF_T_PR		100
#define	DEF_TAUT		7
#define	DEF_TAUV		5
#define	DEF_BEG_STRB	10
#define	DEF_LEN_STRB	20
#define	DEF_POR_STRB	50
#define	DEF_TYPE_STRB	0
#define	DEF_NUM_POINTV	2
#define	DEF_KUS_V		0
#define	DEF_TIME_V		10
///++++++++++++++++++++++++++++++
#define	MIN_NUM_CHAN		0
#define	MAX_NUM_CHAN		7
#define	MIN_NUM_TACT		0
#define	MAX_NUM_TACT		1
#define	MIN_BEGR			0
#define	MAX_BEGR			60000
#define	MIN_ENDR			128
#define	MAX_ENDR			60000
#define	MIN_STEPR			1
#define	MAX_STEPR			50


#define	MIN_NUM_US		0
#define	MAX_NUM_US		1
#define	MIN_TT			0
#define	MAX_TT			7
#define	MIN_TV			0
#define	MAX_TV			15
#define	MIN_NSTROB		0
#define	MAX_NSTROB		1
#define	MIN_POR			0
#define	MAX_POR			255

#define	MIN_KUS			0
////#define	MAX_KUS			85
#define	MAX_KUS			255
#define INC_BEGR		5
#define INC_STEPR		1

#define INC_PRE		1
#define INC_RNG		1
#define INC_LEN_DEF		1

#define	MIN_NUM_GEN		0
#define	MAX_NUM_GEN		7
#define	MIN_LEN_IMP		0
#define	MAX_LEN_IMP		15
#define	MIN_NUM_STR		0
#define	MAX_NUM_STR		1
#define	MIN_BSTR		0
#define	MAX_BSTR		60000
#define	MIN_LSTR		1
#define	MAX_LSTR		40000
#define	MIN_NPOW		0
#define	MAX_NPOW		3
#define	MIN_VPOW		0
#define	MAX_VPOW		255
#define	MIN_NPVRCH		0
#define	MAX_NPVRCH		7
#define	MIN_TVRCH		0
#define	MAX_TVRCH		50000
#define	MIN_KVRCH		0
#define	MAX_KVRCH		60

#define COEF_BEGR		10
#define COEF_ENDR		10

///++++++++++++++++++++++++++++++
#ifndef MIN
	#define MIN(a,b)  ( (a)<(b) ? (a) : (b) )
#endif ///MIN
#ifndef MAX
	#define MAX(a,b)  ( (a)>(b) ? (a) : (b) )
#endif ///MAX



typedef struct par_gen_s
{
quint8 on_gen;
quint8 len_imp;
}par_gen_t;

typedef struct par_pow_s
{
quint8 on_pow;
quint8 val;
}par_pow_t;

typedef struct par_strb_s
{
quint8 on_strb;
quint8	por;
///u8	type;
quint8	dn_up;
quint8	vibro;
///quint16 offs_type;		///15...10-type;9...0-offs
quint16 beg;
quint16 len;
}par_strb_t;

typedef struct point_vrch_s
{
quint16	time;
quint8	kus;
}point_vrch_t;

typedef struct par_vrch_s
{
quint8 on_vrch;
quint8	num_p;
quint16	time[MAX_NUM_PVRCH];
quint8	kus[MAX_NUM_PVRCH];
}par_vrch_t;
typedef struct par_probe_s
{
quint16	alpha;
quint16 t_pr;
quint16 cc;
quint16 strela;
}par_probe_t;

typedef struct par_us_s
{
quint8 on_us;
///u8 mux_us;
quint8 kus1;
quint8 kus2;
quint8 detpar;
quint8 	taut;
quint8 	tauv;
par_probe_t probe;
quint16 beg_r;
quint8 stp_r;
quint8 ots;		///otsechka
par_strb_t strb[NUM_STRBS];
par_vrch_t vrch;
}par_us_t;
typedef struct par_takt_s
{
quint16 len_hipow;
quint16 len_tact;
////quint8 rsv;
par_gen_t gens[NUM_GENS];
par_us_t uss[NUM_USS];
}par_takt_t;
#if 0
typedef struct cn_info_s		// описание канала (канала тракта)
{
quint8		ntact;				// номер такта
quint8		nus;					// номер усилителя
quint8		nstrb;				// номер strob
quint8		def_type;			// тип: TD_TOL, TD_DEF
char		name[13];			// текстовое описание
quint8		rcv1[2];
quint8		rcv2[2];
}cn_info_t;

typedef  struct cn_list_info_s	// описание списка (такто)каналов тракта
{
quint8	chan_n;					// число потоков (данных) тракта
quint8	rsv;	
cn_info_t	cn_info[NUM_CHANS];		// описание каждого (такто)канала тракта
}cn_list_info_t;
#endif
typedef struct par_izm_s
{
quint8 num_tacts;		///1...8
quint8 num_izm;			///1...128
}par_izm_t;

typedef struct par_trk_s
{
par_izm_t izm_par;
///quint8	num_tacts;				// число тактов в схеме
///quint8  num_izm;
quint8  num_us;
quint8  num_gen;
quint8  pow_vol;
//// quint8  ena_rele;
////cn_list_info_t cn_list;
par_pow_t pow_gen[NUM_POW_GENS];
par_takt_t takts[NUM_TACTS];
}par_trk_t;
/*
typedef struct par_rele_s
{
quint8 ena_rele;
quint8 rsv;
quint16 rele_delay;
quint16 rele_len;
}par_rele_t;
*/
/*
typedef struct par_izm_s
{
quint8 num_tacts;		///1...8
quint8 num_izm;			///1...128
}par_izm_t;

typedef struct par_contr_s
{
par_izm_t izm_par;
par_trk_t trk_par;
}par_contr_t;

extern par_contr_t curr_par_contr;


#define curr_param curr_par_contr.trk_par
///extern par_trk_t curr_param;
*/
extern par_trk_t curr_par_trk;
#define curr_param curr_par_trk

typedef struct ed_par_trk_s
{
quint8	curr_chan;				///
quint8	curr_tact;				///
quint16	curr_begr;				///
quint16	curr_endr;				///
quint8	curr_step_r;				///

quint8  curr_us;
quint8  rej_osc;
///quint8  curr_tt;
quint8  curr_pvrch;
quint8  curr_strob;
quint8  curr_gen;
quint8  curr_pow;
quint16	curr_nparam;				///
}ed_par_trk_t;
#define MAX_LEN_BUFF_GEN	16000
typedef struct ugen_par_s
{
quint16	delay_sign;				///
quint16	len_sign;				///
quint16	pre;				///
quint16	rng;				///
quint16	len_def;				///
quint16	gen_dat[MAX_LEN_BUFF_GEN];				///
}ugen_par_t;


extern ed_par_trk_t curr_ed_par_trk;

#define DEF_NUM_CHAN	0			///
#define DEF_NUM_TACT	0			///
////#define DEF_BEGR		0			///
#define DEF_ENDR		20				///
#define DEF_NUM_US		0
#define DEF_TT			5
#define DEF_TV			7
#define DEF_NUM_STRB	0
#define DEF_NUM_GEN		0
#define DEF_NUM_POW		0
#define DEF_NUM_PARAM	1				///
#define DEF_PVRCH		0

///=================================
#define DEF_DELAY_SYGN	40
#define DEF_LEN_SYGN	40
#define DEF_PRESCALE	80
#define DEF_RNG			1000
#define DEF_LEN_DEF		10

///=================================
#define MAX_NUM_AMP	128		///512

///#define NUM_TOL_CH 3
///#define NUM_DEF_CH 6
#if 0
typedef struct zamer_tol_s
{
quint16 time;
quint8 amp[2];
}zamer_tol_t;

typedef struct zamer_def_s
{
quint8 amp_ak;
quint8 amp[MAX_NUM_AMP];
}zamer_def_t;

typedef struct zamer_s
{
qint32 Coord;
zamer_tol_t z_tol[NUM_TOL_CH];
quint8 rsv[2];
zamer_def_t z_def[NUM_DEF_CH];
}zamer_t;
#endif
///++++++++++++++++++++++++++++++++++
///extern par_trk_t def_param;
///extern par_contr_t def_param_contr;
extern par_trk_t def_param_contr;

extern quint32 g_changed_param;

#define CHNG_CHAN	(0x1<<0)
////#define CHNG_TACT	(0x1<<1)
#define CHNG_NUS	(0x1<<2)
#define CHNG_ONUS  (0x1<<3)
#define CHNG_KUS	(0x1<<4)
////#define CHNG_BEGR	(0x1<<5)
////#define CHNG_ENDR	(0x1<<6)
///#define CHNG_TT		(0x1<<7)
///#define CHNG_TV		(0x1<<8)
#define CHNG_NSTRB	(0x1<<9)
#define CHNG_BSTRB	(0x1<<10)
#define CHNG_LSTRB	(0x1<<11)
#define CHNG_POR	(0x1<<12)
///#define CHNG_NUMPV	(0x1<<13)
///#define CHNG_VRON	(0x1<<14)
///#define CHNG_VTIME	(0x1<<15)
///#define CHNG_VKUS	(0x1<<16)
///#define CHNG_NGEN	(0x1<<17)

///#define CHNG_ONGEN  (0x1<<18)

///#define CHNG_LIMP		(0x1<<19)


///#define CHNG_NPOW		(0x1<<20)
///#define CHNG_PVAL		(0x1<<21)
////#define CHNG_CHANLIST	(0x1<<22)
///#define CHNG_REJ_OSC	(0x1<<23)
/////#define CHNG_RELE		(0x1<<24)
///#define CHNG_IZM		(0x1<<24)
#define CHNG_PRE		(0x1<<20)
#define CHNG_RNG		(0x1<<21)
#define CHNG_LEN_DEF		(0x1<<22)
#define CHNG_FORM		(0x1<<23)

#define CHNG_ALT		(0x1<<30)
#define CHNG_ALL		(0x1<<31)
///#define MAX_CHANG_PARAMS	20
#define OFFS_NUM_BYTES	0
#define OFFS_FLG_CHNG	2
#define OFFS_PARAM_DAT	6
///++++++++++++++++++++++++++++++++++++++++++++++++
#define ERROR_TUNE	0
#define NO_AK		1
#define NO_DEF		2
#define DEFECT		3
///++++++++++++++++++++++++++++++++++++++++++++++++
#define _CurrUs curr_param.takts[curr_ed_par_trk.curr_tact].uss[curr_ed_par_trk.curr_us]
#define _CurrNgen curr_param.takts[curr_ed_par_trk.curr_tact].gens[curr_ed_par_trk.curr_gen]
#define _CurrPow curr_param.pow_gen[curr_ed_par_trk.curr_pow]

///++++++++++++++++++++++++++++
#endif /// _PARAMS_H_