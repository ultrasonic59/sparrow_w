#include "PlotterWidget.h"
#include "qpainter.h"
#include <QPaintEvent>


PlotterWidget::PlotterWidget(QWidget *parent, Qt::WindowFlags f) :
								QWidget(parent, f),
								pPlotter(NULL)
{

}

PlotterWidget::PlotterWidget(QWidget *parent, AbstactPlotter *plotter, Qt::WindowFlags f) :
								QWidget(parent, f),
								pPlotter(plotter)
{
	pPlotter->ConnectToWidget(this);
}

PlotterWidget::~PlotterWidget(void)
{
	pPlotter = NULL;
}


void PlotterWidget::paintEvent(QPaintEvent *event)
{
	if(pPlotter != NULL)
	{
		QPainter painter(this);
		QRect cr = contentsRect();
		QRect fr = pPlotter->SetPlotRect(cr);
		QPixmap *pm = pPlotter->GetPixmap();

		painter.drawPixmap( fr, *pm);
	}
}


void PlotterWidget::SetOscPlotter(AbstactPlotter *p_Plotter)
{
	pPlotter = p_Plotter;
}
