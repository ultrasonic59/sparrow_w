#include "OscillogramPlotterSetter2.h"
#include <QPainter>

OscillogramPlotterSetter2::OscillogramPlotterSetter2(QObject *refferece_obj, OscillogramProperties *newOscProp) : 
		OscillogramPlotterSetter(refferece_obj, newOscProp)
{
}

OscillogramPlotterSetter2::~OscillogramPlotterSetter2()
{
}


void OscillogramPlotterSetter2::PlotVerticalSignature(QPainter & painter)
{

	int count_y =  y_sign_step / y_step + 1;

	if(count_y == 0)
		count_y = 1;

	int num_count = OscProp->row_number / count_y;
		

    float y_val_step = (OSC_MAX_VALUE - 0) * count_y / OscProp->row_number ;

	float y_val = 0;
	float y_pos = y_b + frame_width/2 + height;


	QPen pen1( OscProp->TextColor );
	painter.setPen(pen1);

	QFont front = painter.font();
	front.setPixelSize(font_size);
	painter.setFont(front);


	for(int i = 0; i <= num_count ; i++)
	{
		OutFrameText(painter, QString::number(static_cast<int>(y_val), 10), x_b, y_pos, frame_width, frame_width);

		y_val += y_val_step;
		y_pos -= count_y * y_step;
	}
}


void OscillogramPlotterSetter2::PlotRespond(const osc_struct_t *osc_par)
{
	if(pixmap_width != 0 && pixmap_height != 0)
	{
		QPainter painter(pixmap);

		if(!painter.isActive())
			return;


		painter.fillRect(0, 0, pixmap_width, pixmap_height, OscProp->BackgroundColor );

		DrawGrid(painter);						// ??? нужно сделать подстраивающуюся ось Х

		PlotVerticalSignature(painter);
//		PlotHorizontalSignature(painter, num_points);
		
        PlotRespond(painter, osc_par);
		painter.end();


		UpdatePlot();
	}
}

void OscillogramPlotterSetter2::PlotRespond(QPainter & painter, const osc_struct_t *osc_par)
{
	QPen pen1( OscProp->OscillogramColor );

	pen1.setWidthF(OscProp->OscillogramWidth);
	painter.setPen(pen1);


    float plot_x_step = static_cast<float>(width) / (BASE_OSC_LENGTH - 1);
    float plot_y_step = static_cast<float>(height) / (OSC_MAX_VALUE - 0);


    painter.setRenderHints(QPainter::Antialiasing, true);

    QLine line;

    for(int i = 0; i < (BASE_OSC_LENGTH - 1); i++)
    {
        line.setLine( x_b + plot_x_step * i, y_b + height - plot_y_step * (osc_par->ampl[i] - 0)
            , x_b + plot_x_step * (i + 1), y_b + height - plot_y_step * (osc_par->ampl[i+1] - 0));
			painter.drawLine(line);
    }

    painter.setRenderHints(QPainter::Antialiasing, false);
}