#include "SpectrPlotter.h"
#include <QtGui/QPainter>




SpectrPlotter::SpectrPlotter(	OscillogramProperties *newOscProp) : 
				AbstactPlotter(),
				x_sign_step(70),
				y_sign_step(70),
				RedrawAdjustX(1),
				RedrawAdjustY(4),
				frame_width(30),
				OscProp(newOscProp)
{
}


SpectrPlotter::~SpectrPlotter(void)
{
}

QRect & SpectrPlotter::SetPlotRect(const QRect &fullRect)
{
	QRect &PosRect = AbstactPlotter::SetPlotRect(fullRect);

	if(!NeedResize)
		return PosRect;

	width = pixmap_width - 2 * frame_width - 1;
	height = pixmap_height - 2 * frame_width - 1;

	if(width < 0)
		width = 0;

	if(height < 0)
		height = 0;

	x_b =  frame_width + 0.5;
	y_b =  frame_width + 0.5;
	x_step = ((float)width ) / OscProp->column_number;
	y_step = ((float)height) / OscProp->row_number;

	//PaintBackground();

	return PosRect;
}


void SpectrPlotter::ResetPlot()
{
	x_step = ((float)width ) / OscProp->column_number;
	y_step = ((float)height) / OscProp->row_number;

	pixmap_width = 0; 
	pixmap_height = 0;

	UpdatePlot();
}



void SpectrPlotter::ProtSpectr(const quint16 spectr_len, double *spectr_arr, const double max_value)
{
	if(pixmap_width != 0 && pixmap_height != 0)
	{

		QPainter painter(pixmap);
		painter.fillRect(0, 0, pixmap_width, pixmap_height, OscProp->BackgroundColor );
		DrawGrid(painter);
		PlotVerticalSignature(painter, max_value, true);
		PlotHorizontalSignature(painter, spectr_len, true);


		float plot_x_step = (float)width / (spectr_len - 1);
		float plot_y_step = (float)height / max_value;


		QPen pen1( OscProp->OscillogramColor );

		pen1.setWidthF(2);
		painter.setPen(pen1);

		QLine line;
	


		for(int i = 0; i < (spectr_len - 1); i++)
		{
			line.setLine(	x_b + plot_x_step * i, 
							y_b + height - plot_y_step * spectr_arr[i], 
							x_b + plot_x_step * (i + 1), 
							y_b + height - plot_y_step * spectr_arr[i+1]);

			painter.drawLine(line);
		}



		painter.end();

		UpdatePlot();
	}
}


void SpectrPlotter::DrawGrid(QPainter & painter)
{
	int i;

	QPen pen1(OscProp->CellColor);
	pen1.setWidth(1);
	painter.setPen(pen1);

	for(i = 0; i < OscProp->row_number + 1; i++)
	{
		QLine line( x_b, y_b + y_step * i, x_b + width,  y_b + y_step * i);
		painter.drawLine(line);
	}

	for(i = 0; i < OscProp->column_number + 1; i++)
	{
		QLine line( x_b + x_step * i, y_b, x_b + x_step * i, y_b + height);
		painter.drawLine(line);
	}
}



void SpectrPlotter::OutFrameText(QPainter & painter, const QString & string, const int x_rightMid, const int y_rightMid, const int size1, const int legth)
{
	QPen pen1( OscProp->TextColor );
	painter.setPen(pen1);
	QRect rect( x_rightMid - legth, y_rightMid - size1 , legth, size1);
	painter.drawText(rect, Qt::AlignCenter, string);
}




void SpectrPlotter::PlotVerticalSignature(QPainter & painter, const double max_value, const bool FirstEl)
{
	int count_y =  y_sign_step / y_step ;

	if(count_y == 0)
		count_y = 1;

	int num_count = OscProp->row_number / count_y;
		

	float y_val_step = max_value * count_y / OscProp->row_number ;

	float y_val = 0;
	float y_pos = y_b + frame_width/2 + height;

	if(!FirstEl)
	{
		y_val += y_val_step;
		y_pos -= count_y * y_step;
		num_count--;
	}

	for(int i = 0; i <= num_count ; i++)
	{
		OutFrameText(painter, QString::number(y_val, 'f', 2), x_b, y_pos, frame_width, frame_width);

		y_val += y_val_step;
		y_pos -= count_y * y_step;
	}
}

void SpectrPlotter::PlotHorizontalSignature(QPainter & painter, const quint16 spectr_len, const bool FirstEl)
{
	int count_x =  x_sign_step / x_step;

	if(count_x == 0)
		count_x = 1;

	int num_count = OscProp->column_number / count_x;
		

	float x_val = 0; 
	float x_val_step = (float)spectr_len * count_x / OscProp->column_number ;

	float x_pos = x_b + frame_width/2;

	if(!FirstEl)
	{
		x_val += x_val_step;
		x_pos += count_x * x_step;
		num_count--;
	}

	for(int i = 0; i <= num_count ; i++)
	{
		if(x_val >= 0)
			OutFrameText(painter, QString::number(x_val/(spectr_len*2*0.025),'g'), x_pos,  y_b + height + frame_width, frame_width, frame_width);
		else
			OutFrameText(painter, QString(), x_pos,  y_b + height + frame_width, frame_width, frame_width);

		x_val += x_val_step;
		x_pos += count_x * x_step;
	}

}