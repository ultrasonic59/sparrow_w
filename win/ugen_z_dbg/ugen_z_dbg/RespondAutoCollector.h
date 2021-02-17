#pragma once

#include "RespondFrame.h"


class RespondAutoCollector : public RespondFrame
{
protected:
	const quint16 size_start;
	const quint8 size_multiplicator;
	const quint64 size_max;


public:
	RespondAutoCollector(void);

	bool PushRespondAuto(RespondAuto &resp);

	virtual ~RespondAutoCollector(void);
};

