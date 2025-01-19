#pragma once


#include "OscillogramProperties.h"
#include "AbstactPlotter.h"



class OscillogramPlotterSetter : public AbstactPlotter
{

Q_OBJECT

protected:


	int width;
	int height;

	float x_b;
	float y_b;

	float x_step;
	float y_step;



	int x_sign_step;
	int y_sign_step;

	int frame_width;

	int font_size;


	OscillogramProperties *OscProp;


	inline void DrawGrid(QPainter &);
    inline void PlotRespond(QPainter & painter, qint16 *point_arr, quint16 num_points);


    inline void OutFrameText(QPainter & painter, const QString & string, const int x_rightMid, const int y_rightMid, const int size, const int legth);
    inline void PlotVerticalSignature(QPainter & painter);
//	inline void PlotHorizontalSignature(QPainter & painter, quint16 num_points);


public:

    OscillogramPlotterSetter(QObject *refferece_obj, OscillogramProperties *);
	virtual ~OscillogramPlotterSetter(void);


	virtual QRect & SetPlotRect(const QRect &);
	void ResetPlot();

	void PaintBackground(void);
    void PlotRespond(qint16 *point_arr, quint16 num_points);

};
