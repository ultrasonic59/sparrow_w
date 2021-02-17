#include "OscillogramPlotterSetter.h"
#include <QPainter>


//#define MAX_FLOAT_OSC 255.0f

#define MIN_OSC_VAL	-8000.0f
#define MAX_OSC_VAL	8000.0f


OscillogramPlotterSetter::OscillogramPlotterSetter(QObject *refferece_obj, OscillogramProperties *newOscProp) :

                AbstactPlotter(refferece_obj),

				width(0),
				height(0),
				x_b(0.0f),
				y_b(0.0f),
				x_step(1.0f),
				y_step(1.0f),

				x_sign_step(48),
				y_sign_step(32),

				frame_width(40),

				font_size(16),


                OscProp(newOscProp)
{
}


OscillogramPlotterSetter::~OscillogramPlotterSetter(void)
{
}



QRect & OscillogramPlotterSetter::SetPlotRect(const QRect &fullRect)
{
	AbstactPlotter::SetPlotRect(fullRect);

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
	x_step = static_cast<float>(width) / OscProp->column_number;
	y_step = static_cast<float>(height) / OscProp->row_number;

	PaintBackground();

	return PosRect;
}


void OscillogramPlotterSetter::ResetPlot()
{
	x_step = static_cast<float>(width) / OscProp->column_number;
	y_step = static_cast<float>(height) / OscProp->row_number;

	pixmap_width = 0; 
	pixmap_height = 0;

	UpdatePlot();
}



void OscillogramPlotterSetter::PaintBackground(void)
{
	if(pixmap_width != 0 && pixmap_height != 0)
	{
        QPainter painter(pixmap);

        if(!painter.isActive())
            return;


        painter.fillRect(0, 0, pixmap_width, pixmap_height, OscProp->BackgroundColor );


        DrawGrid(painter);

        PlotVerticalSignature(painter);
        //PlotHorizontalSignature(painter, Old_dop_par, OldUsParam);


        UpdatePlot();
	}
}


void OscillogramPlotterSetter::DrawGrid(QPainter & painter)
{
	int i;

	QPen pen1(OscProp->GridColor);
	pen1.setWidth(1);
	painter.setPen(pen1);

	QLine line;

	for(i = 0; i < OscProp->row_number + 1; i++)
	{
		line.setLine( x_b, y_b + y_step * i, x_b + width,  y_b + y_step * i);
		painter.drawLine(line);
	}

    for(i = 0; i < OscProp->column_number + 1; i++)
	{
		QLine line( x_b + x_step * i, y_b, x_b + x_step * i, y_b + height);
		painter.drawLine(line);
	}

}


void OscillogramPlotterSetter::PlotRespond(QPainter & painter, qint16 *point_arr, quint16 num_points)
{
	QPen pen1( OscProp->OscillogramColor );

	pen1.setWidthF(OscProp->OscillogramWidth);
	painter.setPen(pen1);


    float plot_x_step = static_cast<float>(width) / (num_points - 1);
    float plot_y_step = static_cast<float>(height) / (MAX_OSC_VAL - MIN_OSC_VAL);


    painter.setRenderHints(QPainter::Antialiasing, true);

    QLine line;

    for(int i = 0; i < (num_points - 1); i++)
    {
        line.setLine( x_b + plot_x_step * i, y_b + height - plot_y_step * (point_arr[i] - MIN_OSC_VAL)
            , x_b + plot_x_step * (i + 1), y_b + height - plot_y_step * (point_arr[i+1] - MIN_OSC_VAL));
			painter.drawLine(line);
    }

    painter.setRenderHints(QPainter::Antialiasing, false);
}



void OscillogramPlotterSetter::PlotRespond(qint16 *point_arr, quint16 num_points)
{
	if(pixmap_width != 0 && pixmap_height != 0)
	{
		QPainter painter(pixmap);

		if(!painter.isActive())
			return;


		painter.fillRect(0, 0, pixmap_width, pixmap_height, OscProp->BackgroundColor );

		DrawGrid(painter);						// ??? нужно сделать подстраивающуюс€ ось ’

		PlotVerticalSignature(painter);
//		PlotHorizontalSignature(painter, num_points);
		
        PlotRespond(painter, point_arr, num_points);
		painter.end();


		UpdatePlot();
	}
}


void OscillogramPlotterSetter::OutFrameText(QPainter & painter, const QString & string, const int x_rightMid, const int y_rightMid, const int size1, const int legth)
{
	QRect rect( x_rightMid - legth, y_rightMid - size1 , legth, size1);
	painter.drawText(rect, Qt::AlignCenter, string);
}


void OscillogramPlotterSetter::PlotVerticalSignature(QPainter & painter)
{
	int count_y =  y_sign_step / y_step + 1;

	if(count_y == 0)
		count_y = 1;

	int num_count = OscProp->row_number / count_y;
		

    float y_val_step = (MAX_OSC_VAL - MIN_OSC_VAL) * count_y / OscProp->row_number ;

	float y_val = MIN_OSC_VAL;
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

//	OutFrameText(painter, "%", x_b + frame_width/2, y_b, frame_width, frame_width);

}



/*
=========================================================================================


void OscillogramPlotterSetter::PlotHorizontalSignature(QPainter & painternum_points)
{
		float FullOscLen=(curr_dop_par.curr_step_r + 1) * LEN_OSC * STEP_KOEF;
		float x_val = curr_dop_par.curr_begr/TEN_F; 


		QPen pen1(OscProp->GridColor);
		pen1.setWidth(1);
		painter.setPen(pen1);

		
		QPen pen2( OscProp->TextColor );


		QFont front = painter.font();
		front.setPixelSize(font_size);
		painter.setFont(front);



		
		float x_val_per_pix = FullOscLen / width ;			// приращение, приход€щеес€ на 1 пиксель

		if(!curr_dop_par.rej_osc)
			x_val_per_pix = 0.025f  * LEN_OSC / width ;
		
	

		float alpha = curr_us_par.alpha*3.141592653f/(180.0f*COEF_ALPHA);
		
		switch(curr_dop_par.axis_x_mode)
		{
		case 0:
			x_val = x_val - 0.02f * curr_us_par.t_pr;
			break;
				
		case 1:
			{
				float curr_cos_alpha = cos(alpha);
				if(curr_cos_alpha == 0.0f)
					return;
				
				x_val_per_pix = curr_cos_alpha * 0.5f * x_val_per_pix  * 0.001f * curr_us_par.cc;	
				x_val = curr_cos_alpha * (0.5f * x_val - 0.01f *curr_us_par.t_pr)  * 0.001f * curr_us_par.cc;
			}
			break;
				
		case 2:
			{
				float curr_sin_alpha = sin(alpha);
				if(curr_sin_alpha == 0.0f)
					return;
				
				x_val_per_pix = curr_sin_alpha * 0.5f * x_val_per_pix  * 0.001f * curr_us_par.cc;	
				x_val = curr_sin_alpha * (0.5f * x_val - 0.01f *curr_us_par.t_pr)  * 0.001f * curr_us_par.cc - curr_us_par.x_offs * 0.1f;
			}
			break;
		}
		
		// определ€ем, сколько элементов приходитс€ на 1 клетку
		
		double grid_elem_count = x_val_per_pix * x_step;
		
		double grid_elem_pow = log10(grid_elem_count);
		
		
		QLine line;
		
		
		if(grid_elem_pow > 0)	// т.е. более 1 элемента на клетку
		{
			int dec_elem_pow = (int)grid_elem_pow;
			int value_incrim = 1;
			
			if( (grid_elem_pow - dec_elem_pow) > 0.699 )	// > (log10(5)) т.е. умножаем ещЄ на 5
				value_incrim *= 5;
			else if( (grid_elem_pow - dec_elem_pow) > 0.301 )	// т.е. умножаем ещЄ на 2
				value_incrim *= 2;
			
			for(int i = 0; i < dec_elem_pow; i++)
				value_incrim *= 10;					// итого получаем, на сколько смещаемс€ в элементах
			
			
			float x_val_step = value_incrim / x_val_per_pix;		// пикселей в приращении 
			
			
			int x_curr_val;
			
			if(x_val > 0)
				x_curr_val = (int)(x_val / value_incrim) + 1;
			else
				x_curr_val = (int)(x_val / value_incrim);
			
			x_curr_val *= value_incrim;
			
			

			float x_curr_pos = x_curr_val - x_val;		// от начала до первого значени€ в единицах 
			x_curr_pos /= x_val_per_pix;				// в пиксел€х
			
			
			x_curr_pos += x_b;
			
			
			float temp_x_pos = x_sign_step;
			
			
			
			while(x_curr_pos < (x_b + width))
			{
				if(x_curr_val >= 0)
				{
					if(temp_x_pos >= x_sign_step)
					{
						painter.setPen(pen2);
						OutFrameText(painter, QString::number(x_curr_val, 10), frame_width/2 + (int)x_curr_pos,  pixmap_height, frame_width, frame_width);		
						painter.setPen(pen1);

						line.setLine( x_curr_pos, y_b, x_curr_pos, y_b + height + 5);
						painter.drawLine(line);

						temp_x_pos = 0;
					}
					else
					{
						line.setLine( x_curr_pos, y_b, x_curr_pos, y_b + height);
						painter.drawLine(line);
					}
					
					temp_x_pos += x_val_step;
				}
				else
				{
						line.setLine( x_curr_pos, y_b, x_curr_pos, y_b + height);
						painter.drawLine(line);
				}
				
				x_curr_val += value_incrim;
				x_curr_pos += x_val_step;	
			}		
		}
		else if(grid_elem_pow > -1)		// менее 1 элемента на клетку, но больше 0.1 части, а если ещЄ меньше - не отрисовываем
		{
			// grid_elem_pow от -1 до 0
			
			
			float value_incrim = 0.1f;
			
			if( (grid_elem_pow + 1) > 0.699 )	// > (log10(5)) т.е. умножаем ещЄ на 5
				value_incrim *= 5;
			else if( (grid_elem_pow + 1) > 0.301 )	// т.е. умножаем ещЄ на 2
				value_incrim *= 2;
			
			float x_val_step = value_incrim / x_val_per_pix;		// пикселей в приращении 
			

			float x_curr_val = 0;
			
			if(x_val > 0)
				x_curr_val = (int)(x_val / value_incrim) + 1;
			else
				x_curr_val = (int)(x_val / value_incrim);
			
			x_curr_val *= value_incrim;
			
			

			float x_curr_pos = x_curr_val - x_val;		// от начала до первого значени€ в единицах 
			x_curr_pos /= x_val_per_pix;				// в пиксел€х
			
			
			x_curr_pos += x_b;
			
			
			float temp_x_pos = x_sign_step;	
			
			
			while(x_curr_pos < (x_b + width))
			{
				if(x_curr_val >= 0)
				{
					if(temp_x_pos >= x_sign_step)
					{
						painter.setPen(pen2);
						OutFrameText(painter, QString::number(x_curr_val, 'f', 1), frame_width/2 + (int)x_curr_pos,  pixmap_height, frame_width, frame_width);
						painter.setPen(pen1);

						line.setLine( x_curr_pos, y_b, x_curr_pos, y_b + height + 5);
						painter.drawLine(line);

						temp_x_pos = 0;
					}
					else
					{
						line.setLine( x_curr_pos, y_b, x_curr_pos, y_b + height);
						painter.drawLine(line);
					}
					temp_x_pos += x_val_step;
				}
				else
				{
					line.setLine( x_curr_pos, y_b, x_curr_pos, y_b + height);
					painter.drawLine(line);
				}
				
				x_curr_val += value_incrim;
				x_curr_pos += x_val_step;	
			}				
		}
		
		painter.setPen(pen2);
		
		
		switch(curr_dop_par.axis_x_mode)
		{
		case 0:
			OutFrameText(painter, tr("\320\274\320\272\321\201"), pixmap_width, pixmap_height - frame_width/2, frame_width, frame_width);
			break;
		case 1:
			OutFrameText(painter, tr("\320\274\320\274"), pixmap_width, pixmap_height - frame_width/2, frame_width, frame_width);
			OutFrameText(painter, tr("Y"), pixmap_width, pixmap_height - frame_width, frame_width, frame_width);
			break;
		case 2:
			OutFrameText(painter, tr("\320\274\320\274"), pixmap_width, pixmap_height - frame_width/2, frame_width, frame_width);
			OutFrameText(painter, tr("X"), pixmap_width, pixmap_height - frame_width, frame_width, frame_width);
			break;
		}
		

}
*/

