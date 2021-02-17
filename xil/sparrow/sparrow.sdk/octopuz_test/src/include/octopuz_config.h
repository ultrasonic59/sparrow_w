#ifndef _OCTOPUZ_CONFIG_H_
#define _OCTOPUZ_CONFIG_H_

#define NUM_GENS 			8
#define NUM_POW_GENS 		2
#define NUM_USS 			2
#define NUM_MUX_US 			4

///#define NUM_PUSS 			8
#define MAX_NUM_STRB 		3
////#define MAX_NUM_TAKTS		4
#define MAX_NUM_PVRCH		8
///#define	MAX_V_CHAN			8	///12	//
///#define NUM_STRBS_REZ 	        5
#define NUM_TACTS 			4       ////2       ///1       ///2
#define LEN_OSC 			256
#define DEF_SPD_PSPI		8
#define  MAX_NUM_IZM	 	32

#define TD_DEF				0x0			// дефектоскоп на 3 строба
#define TD_TOL_LAM			0x1			// контроль расслоений + толщины по 1 стробу
#define TD_TOL				0x2			// контроль толщины по разностям из NUM_TIMES
#define TD_TOL_REZ			0x4			// резонансная толщинометрия

#define  MAX_NUM_WORD_DAT_TACT 1024
///====================
#define DEF_CB_REZ_LEN		1024
////#define	NUM_CHANS			8	///12	//

////#define NUM_CHAN_REZ		8
#define NUM_USS_REZ		2
#define NUM_TACT_REZ	4
#define NUM_STRB_REZ	5
#define MAX_AMPL_BUFF_LEN   10

////#define MAX_NUM_STP_R_A		128
///#define DEF_NUM_STP_R_A		126
///#define DEF_BRAZV_A			5
////#define DEF_LEN_STP_R_A		4
#define MAX_LEN_FFT				1024
#define MAX_LEN_STROBS_DAT    	16
////#define MAX_LEN_REZ (MAX_NUM_STP_R_A*NUM_CHAN_REZ)

////#define PERIOD_SAVE_PARAM       300
#endif	///_OCTOPUZ_CONFIG_H_
