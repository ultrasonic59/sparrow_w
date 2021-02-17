#include "params.h"


Params::Params() 
{
}


Params::~Params()
{
}


void Params::SetDefaultPar(par_contr_t &par_contr)
{
	par_sent_t &sent_par = par_contr.sent_par;

	sent_par.Timp_len = DEF_TIMP_LEN;
	sent_par.Timp_offset = DEF_TIMP_OFFSET;
	sent_par.Tcycle = DEF_TCYCLE;
	sent_par.Tdef = DEF_TDEF;

	par_contr.num_periods = DEF_NUM_PER;

	par_contr.Aimp = DEF_AIMPL;
	par_contr.dev_frequency = DEF_DEVICE_FREQ;

	par_contr.gaus_enable = false;
}


