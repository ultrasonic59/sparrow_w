#ifndef _PARAMS_H_
#define _PARAMS_H_

#include <QGlobal.h>

#ifndef OFF
#define	OFF	0
#endif

#ifndef ON
#define	ON	1
#endif




#define BASE_OSC_LENGTH		512

#define OSC_MAX_VALUE		512



#define NUM_DIG_QUINT16		5

#define NUM_DIG_AIMP		4
#define NUM_DIG_DEV_FREQ	10




#define	DEF_LENGTH			1000



#define DEF_TIMP_LEN		200
#define DEF_TIMP_OFFSET		200

#define DEF_TCYCLE			20000
#define DEF_TDEF			1000

#define DEF_KUS				0
#define DEF_BEG_OSC			0
#define DEF_STEP_OSC		1



#define DEF_AIMPL			4000

#define DEF_DEVICE_FREQ		0.25f	

#define DEF_NUM_PER			1


#define FLAG_ATTENUAT_LEV_0		(0x1<<0)
#define FLAG_ATTENUAT_LEV_1		(0x1<<1)


#define MAX_QUINT8		255
#define	MAX_QUINT16		65534

#define MIN_LENGTH		2
#define MAX_LENGTH		MAX_QUINT16


#define MIN_TIMP_LEN		10
#define MAX_TIMP_LEN		2000

#define MIN_TIMP_OFFSET		0
#define MAX_TIMP_OFFSET		MAX_QUINT16


#define MIN_TCYCLE			1000
#define MAX_TCYCLE			MAX_QUINT16

#define MIN_TDEF			0
#define MAX_TDEF			MAX_QUINT16


#define MIN_NUM_PER			1
#define MAX_NUM_PER			MAX_QUINT16


#define MIN_KUS				0
#define MAX_KUS				255

#define MIN_BEG_OSC			0
#define MAX_BEG_OSC			MAX_QUINT16

#define MIN_STEP_OSC		0
#define MAX_STEP_OSC		MAX_QUINT16



#define MIN_AIMP			0
#define MAX_AIMP			8000




#define MIN_DEV_FREQ		1
#define MAX_DEV_FREQ		10000000








#define COEF_DEV_FREQ		10000


#define COEF_PERIOD_TRANSF	252		////125


struct par_sent_t
{
	quint16 Timp_len;		// полная длина импульа
	quint16 Timp_offset;	// сдвиг импульса отностиельно сигнала синхронизации

	quint16 Tcycle;			// полное время цикла
	quint16 Tdef;			// время, приходящееся на дефект (не может быть больше Tcycle)

	quint16 kus;

	quint16 beg_osc;
	quint16 step_osc;


	quint8 attenuator;
////===========================================
	quint16 rej;
	quint32 freq;		// 

};




struct par_contr_t
{
	par_sent_t sent_par;


	quint16 num_periods;


	quint16 Aimp;			// амплитуда импульса
	float dev_frequency;	// частота колебаний в импульсе

	bool gaus_enable;
quint8 rej_dac;
quint8 rej_sync_dac;
quint8 rej_ext_sync;


};



struct osc_struct_t
{
	quint16 ampl[BASE_OSC_LENGTH];
};






///++++++++++++++++++++++++++++++++++

#define CHNG_TIMP_LEN			(0x1<<1)
#define CHNG_TIMP_OFFSET		(0x1<<2)

#define CHNG_TCYCLE				(0x1<<3)
#define CHNG_TDEF				(0x1<<4)

#define CHNG_IMP_POINTS			(0x1<<5)

#define CHNG_KUS				(0x1<<6)

#define CHNG_BEG_OSC			(0x1<<7)
#define CHNG_STEP_OSC			(0x1<<8)

#define CHNG_ATTENUATOR			(0x1<<9)
////=============================================
#define CHNG_FREQ				(0x1<<10)
#define CHNG_DAC_REJ			(0x1<<11)



//#define CHNG_ALL		(0x1<<31)	// ???




#define OFFS_FLG_CHNG	2
#define OFFS_PARAM_DAT	6
///++++++++++++++++++++++++++++++++++++++++++++++++





class Params
{

public:
	Params();
	~Params();


	static void SetDefaultPar(par_contr_t &par_contr);

};




#endif /// _PARAMS_H_