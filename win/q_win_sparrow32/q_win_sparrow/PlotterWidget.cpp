#include "PlotterWidget.h"
#include <QPainter>
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
    Q_UNUSED(event);
    if(pPlotter != NULL)
	{
		if(pPlotter->IsMultithread())
		{
			if(pPlotter->LockPixmap())
			{
				QPainter painter(this);
				if(painter.isActive())
				{
					QRect cr = contentsRect();
					QRect fr = pPlotter->SetPlotRect(cr);
					QPixmap *pm = pPlotter->GetPixmap();
					painter.drawPixmap( fr, *pm);
				}
				painter.end();
				pPlotter->UnlockPixmap();
			}

			// иначе ничего не рисуем - pixmap не отрисован
		}
		else
		{
			QPainter painter(this);
			if(!painter.isActive())
				return;
			QRect cr = contentsRect();
			QRect fr = pPlotter->SetPlotRect(cr);
			QPixmap *pm = pPlotter->GetPixmap();
			painter.drawPixmap( fr, *pm);
		}
	}
}

void PlotterWidget::SetPlotter(AbstactPlotter *p_Plotter)
{
	pPlotter = p_Plotter;
}
