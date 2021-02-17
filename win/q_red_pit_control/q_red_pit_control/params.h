#ifndef _PARAMS_H_
#define _PARAMS_H_

#include <QGlobal.h>

#ifndef OFF
#define	OFF	0
#endif

#ifndef ON
#define	ON	1
#endif





#define NUM_DIG_QUINT16		5

#define NUM_DIG_AIMP		4
#define NUM_DIG_DEV_FREQ	10



#define	DEF_LENGTH			1000


#define DEF_TIMP_LEN		200
#define DEF_TIMP_OFFSET		200

#define DEF_TCYCLE			500
#define DEF_TDEF			10

#define DEF_AIMPL			4000

#define DEF_DEVICE_FREQ		0.25f	

#define DEF_NUM_PER			1



#define MAX_QUINT8		255
#define	MAX_QUINT16		65534

#define MIN_LENGTH		2
#define MAX_LENGTH		MAX_QUINT16


#define MIN_TIMP_LEN		10
#define MAX_TIMP_LEN		2000

#define MIN_TIMP_OFFSET		0
#define MAX_TIMP_OFFSET		MAX_QUINT16


#define MIN_TCYCLE			10
#define MAX_TCYCLE			MAX_QUINT16

#define MIN_TDEF			0
#define MAX_TDEF			MAX_QUINT16


#define MIN_NUM_PER			1
#define MAX_NUM_PER			MAX_QUINT16


#define MIN_AIMP			0
#define MAX_AIMP			8000


#define MIN_DEV_FREQ		1
#define MAX_DEV_FREQ		10000000



#define COEF_DEV_FREQ		10000


#define COEF_PERIOD_TRANSF	125


struct par_sent_t
{
	quint16 Timp_len;		// полная длина импульа
	quint16 Timp_offset;	// сдвиг импульса отностиельно сигнала синхронизации

	quint16 Tcycle;			// полное время цикла
	quint16 Tdef;			// время, приходящееся на дефект (не может быть больше Tcycle)
	quint16 pre;			// время, приходящееся на дефект (не может быть больше Tcycle)

};




struct par_contr_t
{
	par_sent_t sent_par;


	quint16 num_periods;



	quint16 Aimp;			// амплитуда импульса
	float dev_frequency;	// частота колебаний в импульсе

	bool gaus_enable;

};




///++++++++++++++++++++++++++++++++++

#define CHNG_TIMP_LEN			(0x1<<1)
#define CHNG_TIMP_OFFSET		(0x1<<2)

#define CHNG_TCYCLE				(0x1<<3)
#define CHNG_TDEF				(0x1<<4)

#define CHNG_IMP_POINTS			(0x1<<5)

#define CHNG_PRESC				(0x1<<6)




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