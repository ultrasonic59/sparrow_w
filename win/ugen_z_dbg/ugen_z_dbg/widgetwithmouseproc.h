#ifndef WIDGETWITHMOUSEPROC_H
#define WIDGETWITHMOUSEPROC_H

#include "PlotterWidget.h"
#include "positiontesterplotter.h"

class WidgetWithMouseProc : public PlotterWidget
{
public:
	WidgetWithMouseProc(QWidget *parent=0, Qt::WindowFlags f=0);
	WidgetWithMouseProc(QWidget *parent, PositionTesterPlotter *tester, Qt::WindowFlags f=0);

	~WidgetWithMouseProc();

	void SetPosTester(PositionTesterPlotter *);

protected:

	PositionTesterPlotter *pTester;

	virtual void mouseMoveEvent(QMouseEvent * event);
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseReleaseEvent(QMouseEvent * event);
	
};

#endif // WIDGETWITHMOUSEPROC_H
