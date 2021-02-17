#pragma once
#include "qglobal.h"
#include "params.h"

#pragma pack (push, 1)
#define LEN_OSC 256

struct Respond
{

	quint16		begr;
	quint16		num_sel_strb;
	par_strb_t strb[MAX_NUM_STRBS];


	quint8		step_r;
	quint8		kUs;
	quint8		vrchON;   //.0 vrch,.1 vrch ust .2 time on/off .3 - mm/mks .4 =0 HD,=1 HT .5 -sov/raz .6 mm/mks .7 extended/restricted mode

	quint8		vrchKUS[8];					// NUM_PVRCH
	quint16		vrchT[8];

	quint16		timeD;                   //time def
	quint8		nPoVrch;


	quint16    Tpr;
	quint16    FRQ;
	quint16    cc;
	quint8     alpha;
	quint16    nPep;
	quint8	   Temper;

	quint16   timeD_3;                   //time def

	quint8 osc[LEN_OSC];

	quint8 ampl;
	qint8 ampl_aru;
	quint8 max_ak;


	bool video_mode;


	bool spectr_mode;
	quint8 spectr_first_el;
	quint16 spectr_length;

};

#pragma pack (pop)

