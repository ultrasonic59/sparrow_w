#pragma once
#include "qglobal.h"
#include "params.h"

#pragma pack (push, 1)



struct RespondAuto
{
	quint8 type_def;

	quint8 us_on[NUM_CHANS];

	quint8 ampl[NUM_CHANS];
	quint8 ampl2[NUM_CHANS];


	enum types {
		NoControl = 0,
		NoDefect = 1,
		NoContact = 2,
		Defect = 3
	};

};


#pragma pack (pop)

