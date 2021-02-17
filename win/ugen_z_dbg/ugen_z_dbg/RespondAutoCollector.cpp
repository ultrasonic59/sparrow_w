#include "RespondAutoCollector.h"


RespondAutoCollector::RespondAutoCollector(void) : RespondFrame(),
								size_start(20),
								size_multiplicator(2),
								size_max(1000)//(4194304)
{
	SetNewMaxSize(size_start, false);
}

RespondAutoCollector::~RespondAutoCollector(void)
{
}

bool RespondAutoCollector::PushRespondAuto(RespondAuto &resp)
{
	if(Size >= MaxSize)
	{
		quint64 temp_size(MaxSize);
		if(temp_size == 0)
			temp_size = size_start;
		temp_size *= size_multiplicator;
		if(temp_size > size_max)
			ShiftFrameArray(1);
		else
			SetNewMaxSize(temp_size, true);
	}

	AddRespondAuto(resp);
	return true;
}



