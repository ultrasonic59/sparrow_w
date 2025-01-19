#ifndef OSCILLOGRAMPLOTTERSETTER2_H
#define OSCILLOGRAMPLOTTERSETTER2_H

#include "OscillogramPlotterSetter.h"
#include "params.h"

class OscillogramPlotterSetter2 : public OscillogramPlotterSetter
{
protected:

	void PlotVerticalSignature(QPainter & painter);

	 inline void PlotRespond(QPainter & painter, const osc_struct_t *osc_par);


public:
	OscillogramPlotterSetter2(QObject *refferece_obj, OscillogramProperties *);
	~OscillogramPlotterSetter2();


	void PlotRespond(const osc_struct_t *osc_par);
	
};

#endif // OSCILLOGRAMPLOTTERSETTER2_H
