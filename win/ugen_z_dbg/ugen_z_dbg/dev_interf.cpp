#include <string.h>
#include <stdlib.h>
///#include "win_tempo.h"
#include "ugen_z_dbg.h"
///#include "dev_interf.h"

char g_mod_ans_str[256];
extern char g_mod_cmd_str[256];

quint8 dev_state=0;
////ggzamer_t curr_zamer;
////zamer_t curr_zamer;
///=====================================
adc_spi_req_t g_adc_spi_req;
adc_spi_req_t g_adc_spi_dat;
alt_dat_req_t g_alt_dat_req;
alt_dat_req_t g_alt_dat;
cpu_dat_req_t g_cpu_dat_req;
////cpu_dat_req_t g_cpu_dat;
///=====================================
///============================
quint8 buffOsc0[LEN_OSC];
quint8 buffOsc1[LEN_OSC];
amp_tim_t m_amp_tims[NUM_CHANS];
////quint8 g_curr_type_def=0;
RespondAuto::types g_curr_type_def=RespondAuto::NoControl;

///amp_tim_t m_amp_tims[NUM_TOL_CH+NUM_DEF_CH];
////u8	m_amps[MAX_TACTS][NUM_US][MAX_STROBS_OBR];
////u32	m_times[MAX_TACTS][NUM_US][MAX_STROBS_OBR];

quint8 *buffOscs[2] = { buffOsc0, buffOsc1 };
///======================================

///=====================================

BOOL pause_control(void )
{
////u8 btmp=0;
///return hdlc1_send_dat(&g_hdlc_pc,NUM_REQ_PAUSE_CONTR,&btmp,1);
return 0;
}
BOOL resume_control(void )
{
quint8 btmp=0;
///return hdlc1_send_dat(&g_hdlc_pc,NUM_REQ_RESUME_CONTR,&btmp,1);
return 0;
}

///=======================================
///#define O_RDONLY	0

///file_hdr_t t_file_hdr;

///======================================================
////extern udp_stat_t g_udp_pc;
extern quint8 *g_ptr_rec_dat;
extern void addfcs16( unsigned char *cp, int len );
///+++++++++++++++++++++++++++++++++++++++++++++++++++++
#if 0
quint8	chan_n;					// число потоков (данных) тракта
cn_info_t	cn_info[NUM_CHANS];		// описание каждого (такто)канала тракта
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
#endif
#if 0
///par_trk_t def_param=
par_contr_t def_param_contr=
{
	{		///izm
	1,		///num izm;
	1,		///num tact;(num_tacts-1)
	},
{
2,	///num_tacts
8,	///num_us
8,	///num_gen
4,	///num_pow
{		///cn_list
8,
0,
{
	{	///0
	0,	///ntact
	0	///nus
	},
	{	///1
	1,	///ntact
	1	///nus
	},
	{	///2
	0,	///ntact
	2	///nus
	},
	{	///3
	1,	///ntact
	3	///nus
	},
	{	///4
	0,	///ntact
	4	///nus
	},
	{	///5
	1,	///ntact
	5	///nus
	},
	{	///6
	0,	///ntact
	6	///nus
	},
	{	///7
	1,	///ntact
	7	///nus
	}
}
},
{		///pow
	{
	0x1,0xc0
	},
	{
	0x1,0xc0
	},
	{
	0x1,0xc0
	},
	{
	0x1,0xc0
	}
},
{		///tact
	{	///0
	1000,	///time
	4000,
		{///gen
			{
			0x1,0x7
			},
			{
			0x1,0x7
			},
			{
			0x1,0x7
			},
			{
			0x1,0x7
			},
			{
			0x1,0x7
			},
			{
			0x1,0x7
			},
			{
			0x1,0x7
			},
			{
			0x1,0x7
			}
		},
////	0,			///mux_us
	},
	{	///1
	1000,	///time
	4000,
		{///gen
			{
			0x1,0x7
			},
			{
			0x1,0x7
			},
			{
			0x1,0x7
			},
			{
			0x1,0x7
			},
			{
			0x1,0x7
			},
			{
			0x1,0x7
			},
			{
			0x1,0x7
			},
			{
			0x1,0x7
			}
		},
	}
}
}
};

#endif
///+++++++++++++++++++++++++++++++++++++++++++++++++++++
