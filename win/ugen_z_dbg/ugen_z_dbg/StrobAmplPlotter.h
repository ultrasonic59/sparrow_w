#pragma once
#include "oscillogramplotter.h"

#define STROB_AMPL_LENGTH 256


class StrobAmplPlotter : public OscillogramPlotter
{
private:
	quint8 stropAmplTemp[STROB_AMPL_LENGTH];

	quint16 stropAmpl_len;

	inline void PlotArray(QPainter & painter, const quint16 curr_stropAmpl, const quint8 *strbAmpl, const bool clean);

protected:

	virtual void PlotTempContent(QPainter & painter);

public:

	StrobAmplPlotter(RespondTrans *, OscillogramProperties *OscProp);
	virtual ~StrobAmplPlotter(void);

	void PlotArray(const quint16 curr_stropAmpl, const quint8 *strbAmpl);
};

