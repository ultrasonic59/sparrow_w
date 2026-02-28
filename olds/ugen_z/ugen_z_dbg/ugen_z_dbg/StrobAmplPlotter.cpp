#include "StrobAmplPlotter.h"
#include <QtGui/QPainter>


StrobAmplPlotter::StrobAmplPlotter(RespondTrans *trans, OscillogramProperties *newOscProp) :
								OscillogramPlotter(trans, newOscProp, NULL),
								stropAmpl_len(0)
{

}


StrobAmplPlotter::~StrobAmplPlotter(void)
{
}


void StrobAmplPlotter::PlotArray(QPainter & painter, const quint16 curr_stropAmpl, const quint8 *strbAmpl, const bool clean)
{
	QPen pen1( (clean ? OscProp->BackgroundColor : OscProp->OscillogramColor) );

	pen1.setWidth(2);
	painter.setPen(pen1);
	
	float plot_x_step = (float)(width - 1) / (STROB_AMPL_LENGTH - 1);
	float plot_y_step = (float)height / Translator->MaxUInt8;

	x_b++;

	for(int i = 0; i < (curr_stropAmpl - 1); i++)
	{
		QLine line( x_b + plot_x_step * i, y_b + height - plot_y_step * strbAmpl[i], x_b + plot_x_step * (i + 1), y_b + height - plot_y_step * strbAmpl[i+1]);
		painter.drawLine(line);
	}

	x_b--;
}

void StrobAmplPlotter::PlotArray(const quint16 curr_stropAmpl, const quint8 *strbAmpl)
{
	if(!AnyReceived)
		AnyReceived = true;

	if(pixmap_width != 0 && pixmap_height != 0)
	{
		QPainter painter(pixmap);

		PlotArray(painter, stropAmpl_len, stropAmplTemp, true);
		DrawGrid(painter);
	
		memcpy(stropAmplTemp, strbAmpl, sizeof(quint8) * STROB_AMPL_LENGTH);
		stropAmpl_len = curr_stropAmpl;

		PlotArray(painter, curr_stropAmpl, strbAmpl, false);
		painter.end();

		UpdatePlot();
	}
}


void StrobAmplPlotter::PlotTempContent(QPainter & painter)
{
	PlotArray(painter, stropAmpl_len, stropAmplTemp, false);
}
