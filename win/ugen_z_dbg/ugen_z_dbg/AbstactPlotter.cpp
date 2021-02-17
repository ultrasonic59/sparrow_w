#include "AbstactPlotter.h"


AbstactPlotter::AbstactPlotter(void) :
				QObject(),
				pixmap_width(0),
				pixmap_height(0),
				WidthUpStep(2),
				HeightUpStep(2),
				NeedResize(false),
				left(0),
				top(0),
				pixmap(NULL),
				PosRect(),
				MainPlotWidget(NULL)
{
}


AbstactPlotter::~AbstactPlotter(void)
{
	if(pixmap != NULL)
		delete pixmap;

	MainPlotWidget = NULL;
}



QRect & AbstactPlotter::SetPlotRect(const QRect &fullRect)
{
	NeedResize = false;

	if( (fullRect.width() - pixmap_width) > WidthUpStep )
	{
		pixmap_width = fullRect.width(); 
		NeedResize = true;
	} 
	else if( fullRect.width() < pixmap_width )
	{
		pixmap_width = fullRect.width() - WidthUpStep; 
		NeedResize = true;
	}

	if( (fullRect.height() - pixmap_height) > HeightUpStep )
	{
		pixmap_height = fullRect.height();
		NeedResize = true;
	}
	else if(fullRect.height() < pixmap_height)
	{
		pixmap_height = fullRect.height() - HeightUpStep;
		NeedResize = true;
	}

	left = (fullRect.width() - pixmap_width)/2;
	top = (fullRect.height() - pixmap_height)/2;

	PosRect.setRect( left, top, pixmap_width, pixmap_height);

	if(!NeedResize)
		return PosRect;

	if(pixmap!= NULL)
	{
		delete pixmap;
		pixmap = NULL;
	}
	pixmap = new QPixmap( pixmap_width,pixmap_height);
	return PosRect;
}

QPixmap * AbstactPlotter::GetPixmap() const
{
	return pixmap;
}

void AbstactPlotter::ConnectToWidget(QWidget *MainPlotWidgetNew)
{
	MainPlotWidget = MainPlotWidgetNew;
}

void AbstactPlotter::UpdatePlot()
{
	if(MainPlotWidget != NULL)
	{
		MainPlotWidget->update();
	}
}
