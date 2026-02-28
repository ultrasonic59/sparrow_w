#include "widgetwithmouseproc.h"
#include "QMouseEvent"

WidgetWithMouseProc::WidgetWithMouseProc(QWidget *parent, Qt::WindowFlags f)
					: PlotterWidget(parent, f),
					pTester(NULL)
{
	setMouseTracking(true);
}


WidgetWithMouseProc::WidgetWithMouseProc(QWidget *parent, PositionTesterPlotter *tester, Qt::WindowFlags f)
					: PlotterWidget(parent, static_cast<AbstactPlotter *>(tester), f),
					pTester(tester)
{
	setMouseTracking(true);
}


WidgetWithMouseProc::~WidgetWithMouseProc()
{
	pTester = NULL;
}


void WidgetWithMouseProc::mouseMoveEvent(QMouseEvent * event)
{
	PlotterWidget::mouseMoveEvent(event);

	if(pTester != NULL)
	{
		pTester->TestNewPosition(event->localPos());

		if(pTester->IsCursorChanged())
		{
			setCursor(pTester->GetCursor());
			pTester->CursorAssepted();
		}
	}
}

void WidgetWithMouseProc::mousePressEvent(QMouseEvent * event)
{
	PlotterWidget::mousePressEvent(event);

	if(pTester != NULL && event->button() == Qt::LeftButton)
		pTester->SetDragging(true);
}

void WidgetWithMouseProc::mouseReleaseEvent(QMouseEvent * event)
{
	PlotterWidget::mouseReleaseEvent(event);

	if(pTester != NULL && event->button() == Qt::LeftButton)
		pTester->SetDragging(false);
}


void WidgetWithMouseProc::SetPosTester(PositionTesterPlotter *p_Tester)
{
	SetOscPlotter(static_cast<AbstactPlotter *>(p_Tester));
	pTester = p_Tester;
}
