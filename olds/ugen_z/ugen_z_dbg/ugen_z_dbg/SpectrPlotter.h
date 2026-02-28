#pragma once

#include "AbstactPlotter.h"
#include "OscillogramProperties.h"




class SpectrPlotter : public AbstactPlotter
{
Q_OBJECT

protected:


	int width;
	int height;

	float x_b;
	float y_b;

	float x_step;
	float y_step;

	const int x_sign_step;
	const int y_sign_step;
	const int RedrawAdjustX;
	const int RedrawAdjustY;

	const int frame_width;



	OscillogramProperties *OscProp;


	inline void DrawGrid(QPainter &);
	inline void OutFrameText(QPainter & painter, const QString & string, const int x_rightMid, const int y_rightMid, const int size, const int legth);
	inline void PlotVerticalSignature(QPainter & painter, const double max_value, const bool FirstEl);
	inline void PlotHorizontalSignature(QPainter & painter, const quint16 spectr_len, const bool FirstEl);


public:


	SpectrPlotter(	OscillogramProperties *);

	virtual ~SpectrPlotter(void);

	virtual QRect & SetPlotRect(const QRect &);

	void ResetPlot();


	void ProtSpectr(const quint16 spectr_len, double *spectr_arr, const double max_value);


};