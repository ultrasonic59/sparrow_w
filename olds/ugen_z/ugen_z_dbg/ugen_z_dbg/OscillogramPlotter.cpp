#include "OscillogramPlotter.h"
#include <QtGui/QPainter>

extern bool en_set_device_params;


OscillogramPlotter::OscillogramPlotter(	RespondTrans *trans, 
										OscillogramProperties *newOscProp, 
										Respond *oldResp) : 
				PositionTesterPlotter(),
				AnyReceived(false),
				x_sign_step(70),
				y_sign_step(70),
				RedrawAdjustX(1),
				RedrawAdjustY(4),
				frame_width(30),
				ArrowDim(10),
				ArrowIndent(2),
				ArrowInterval(6),
				StrobWidth(4),
				StrobSetHeight(8),
				VRChPart(0.165f),

				Translator(trans),
				OscProp(newOscProp),
				pOldRespond(oldResp),

				curr_area(OscillogramPlotter::Outside),
				curr_strob(0)

{
}


OscillogramPlotter::~OscillogramPlotter(void)
{
}

QRect & OscillogramPlotter::SetPlotRect(const QRect &fullRect)
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

	PaintBackground();

	return PosRect;
}


void OscillogramPlotter::ResetPlot()
{
	x_step = ((float)width ) / OscProp->column_number;
	y_step = ((float)height) / OscProp->row_number;

	pixmap_width = 0; 
	pixmap_height = 0;

	UpdatePlot();
}



void OscillogramPlotter::PaintBackground(void)
{
	if(pixmap_width != 0 && pixmap_height != 0)
	{

		QPainter painter(pixmap);
		painter.fillRect(0, 0, pixmap_width, pixmap_height, OscProp->BackgroundColor );
		DrawGrid(painter);
		PlotVerticalSignature(painter, true);

		if(AnyReceived)
			PlotTempContent(painter);

		painter.end();
	}
}


void OscillogramPlotter::DrawGrid(QPainter & painter)
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



void OscillogramPlotter::PlotRespond(QPainter & painter, const Respond & respond, bool clean)
{
	float koeff  = 0;
	float OscBegin = respond.begr/TEN_F;

	float plot_x_step = (float)width / LEN_OSC;
	float plot_y_step = (float)height / MAX_OSC;





	if(respond.video_mode)
	{
		koeff  = (float)width /((respond.step_r + 1) * LEN_OSC * STEP_KOEF);


		for(int ii=0;ii<2/*MAX_NUM_STRBS*/;ii++)
		{
			//if(respond.strb[ii].on_strb || clean)
			{
			float s_begin= respond.strb[ii].beg/TEN_F - OscBegin;;
			float s_end= s_begin+ respond.strb[ii].len/TEN_F;
			float s_porog = y_b + (height * (100 - (int)(respond.strb[ii].por * 100 /MAX_OSC)) / 100);
			PlotStrob(painter, koeff, s_begin, s_end, s_porog, OscProp->StrobsColor[ii],(respond.num_sel_strb==ii), clean);
			}
		}
	}
	else
	{
		koeff  = (float)width /((respond.step_r + 1) * (LEN_OSC - 1) * STEP_KOEF);
		float koeff2  = (float)width/(LEN_OSC - 1);
		plot_x_step = (float)width / (LEN_OSC - 1);
	
		if(respond.spectr_mode)
		{
			PlotStrob(painter, koeff2, respond.spectr_first_el, respond.spectr_first_el + respond.spectr_length - 1, y_b + height/2, OscProp->StrobsColor[1],true, clean);

		}

	}


	QPen pen1( (clean ? OscProp->BackgroundColor : OscProp->OscillogramColor) );

	pen1.setWidthF(2);
	painter.setPen(pen1);

	QLine line;
	
	if(respond.video_mode)
	{
		line.setLine( x_b, y_b + height, x_b + plot_x_step, y_b + height - plot_y_step * respond.osc[0]);
		painter.drawLine(line);

		for(int i = 0; i < (LEN_OSC - 1); i++)
		{
			line.setLine( x_b + plot_x_step * (i + 1), y_b + height - plot_y_step * respond.osc[i]
			, x_b + plot_x_step * (i + 2), y_b + height - plot_y_step * respond.osc[i+1]);
			painter.drawLine(line);
		}


	}
	else
	{

		for(int i = 0; i < (LEN_OSC - 1); i++)
		{
			line.setLine( x_b + plot_x_step * i, y_b + height - plot_y_step * respond.osc[i]
			, x_b + plot_x_step * (i + 1), y_b + height - plot_y_step * respond.osc[i+1]);
			painter.drawLine(line);
		}
	}

}



void OscillogramPlotter::PlotRespond(const Respond &New, const Respond &Old)
{
	if(!AnyReceived)
		AnyReceived = true;

	if(pixmap_width != 0 && pixmap_height != 0)
	{
		QPainter painter(pixmap);

		if(New.video_mode != Old.video_mode)
		{
			pOldRespond->video_mode = false;
			ResetPlot();
		}
		else
		{
			PlotRespond(painter, Old, true);
			DrawGrid(painter);
		}


		PlotRespond(painter, New, false);

		if(New.video_mode)
			PlotHorizontalSignature(painter, New, true, true);
		painter.end();

		UpdatePlot();
	}
}


void OscillogramPlotter::OutFrameText(QPainter & painter, const QString & string, const int x_rightMid, const int y_rightMid, const int size1, const int legth, const bool redraw)
{
	QPen pen1( OscProp->TextColor );
	painter.setPen(pen1);
	QRect rect( x_rightMid - legth, y_rightMid - size1 , legth, size1);
	if(redraw)
	{
		rect.adjust( RedrawAdjustX, RedrawAdjustY, -RedrawAdjustX, -RedrawAdjustY);
		painter.fillRect(rect, OscProp->BackgroundColor);
	}
	painter.drawText(rect, Qt::AlignCenter, string);
}

void OscillogramPlotter::PrintScaleLimit(int limit)
{
	QPainter painter(pixmap);
	OutFrameText(painter, QString::number(limit, 10),  x_b + width + frame_width,  y_b + height + frame_width, frame_width, 2*frame_width, true);
	painter.end();
}


void OscillogramPlotter::PlotVerticalSignature(QPainter & painter, const bool FirstEl)
{
	int count_y =  y_sign_step / y_step + 1;

	if(count_y == 0)
		count_y = 1;

	int num_count = OscProp->row_number / count_y;
		

	float y_val_step = (float)(Translator->MaxUInt8) * count_y / OscProp->row_number ;

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
		OutFrameText(painter, QString::number((int)y_val, 10), x_b, y_pos, frame_width, frame_width, false);

		y_val += y_val_step;
		y_pos -= count_y * y_step;
	}
}

void OscillogramPlotter::PlotHorizontalSignature(QPainter & painter, const Respond &respond, const bool clean, const bool FirstEl)
{
	int count_x =  x_sign_step / x_step;

	if(count_x == 0)
		count_x = 1;

	int num_count = OscProp->column_number / count_x;
		

	float FullOscLen=(respond.step_r + 1) * LEN_OSC * STEP_KOEF;
	float x_val = respond.begr/TEN_F; 
	float x_val_step = FullOscLen * count_x / OscProp->column_number ;

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
			OutFrameText(painter, QString::number((int)x_val, 10), x_pos,  y_b + height + frame_width, frame_width, frame_width, clean);
		else
			OutFrameText(painter, QString(), x_pos,  y_b + height + frame_width, frame_width, frame_width, clean);

		x_val += x_val_step;
		x_pos += count_x * x_step;
	}

}



void OscillogramPlotter::PlotStrob(QPainter & painter, 
									const float koeff, 
									float s_begin, 
									float s_end, 
									const float porog1, 
									QColor color, 
									const bool selected, 
									bool clean)
{
	bool s_to_plot = true;
	QPen pen1(clean ? OscProp->BackgroundColor : color );

	pen1.setWidthF(2);
	pen1.setCapStyle(Qt::FlatCap);

	painter.setPen(pen1);


	if(s_begin < 0)
	{
		if(s_end * koeff > ArrowIndent)
		{
			s_begin = ArrowIndent / koeff;
			PlotStrobArrow(painter, x_b,  porog1 + 0.5f, true);

			if(selected)
				PlotStrobSetEnd(painter, x_b + koeff * s_end, porog1 + StrobWidth/2);
		}
		else
		{
			s_to_plot = false;

			PlotStrobArrow(painter, x_b, porog1 + 0.5f, true);
			PlotStrobArrow(painter, x_b + ArrowInterval, porog1 + 0.5f, true);
		}
	}

	if(s_end * koeff > width)
	{
		if(s_begin * koeff< (width - ArrowIndent))
		{
			s_end = (width - ArrowIndent) / koeff;
			PlotStrobArrow(painter, x_b + width, porog1 + 0.5f, false);

			if(selected)
				PlotStrobSetEnd(painter, x_b + koeff * s_begin, porog1 + StrobWidth/2);
		}
		else
		{
			s_to_plot = false;

			PlotStrobArrow(painter, x_b + width, porog1 + 0.5f, false);
			PlotStrobArrow(painter, x_b + width - ArrowInterval, porog1 + 0.5f, false);
		}
	}

	if(selected && ((s_begin >= 0) && (s_end * koeff <= width)))
	{
		PlotStrobSetEnd(painter, x_b + koeff * s_end, porog1 + StrobWidth/2);
		PlotStrobSetEnd(painter, x_b + koeff * s_begin, porog1 + StrobWidth/2);
	}


	if(s_to_plot)
	{
		pen1.setWidthF(StrobWidth);
		painter.setPen(pen1);
		QLine line( x_b + koeff * s_begin, porog1 + StrobWidth/2 , x_b + koeff * s_end, porog1 + StrobWidth/2 );
		painter.drawLine(line);
	}
}


void OscillogramPlotter::PlotStrobArrow(QPainter & painter, const float x0, const float y0, const bool left)
{
	if(left)
	{
		QLine line( x0, y0 , x0 + ArrowDim, y0 - ArrowDim );
		painter.drawLine(line);

		line.setLine( x0, y0 , x0 + ArrowDim, y0 + ArrowDim );
		painter.drawLine(line);
	}
	else
	{
		QLine line( x0, y0 , x0 - ArrowDim, y0 - ArrowDim );
		painter.drawLine(line);

		line.setLine( x0, y0 , x0 - ArrowDim, y0 + ArrowDim );
		painter.drawLine(line);
	}
}

void OscillogramPlotter::PlotStrobSetEnd(QPainter & painter, const float x0, const float y0)
{
		QLine line( x0, y0 - StrobSetHeight/2 , x0, y0 + StrobSetHeight/2);
		painter.drawLine(line);
}


void OscillogramPlotter::PlotTempContent(QPainter & painter)
{
	PlotRespond(painter, *pOldRespond, false);

	if(pOldRespond->video_mode)
		PlotHorizontalSignature(painter, *pOldRespond, false, true);
}

float OscillogramPlotter::GetVRChPorog(const Respond &respond, const quint8 i, const quint8 max_val) const
{
	return y_b + VRChPart*height *(max_val - respond.vrchKUS[i]) / max_val;
}

void OscillogramPlotter::PlotVRCh(QPainter & painter, const Respond &respond, const bool MKSMode, const float koeff, const float cos_al, const float OscBegin, bool clean)
{
	QPen pen1( (clean ? OscProp->BackgroundColor : OscProp->VRChColor) );
	pen1.setWidthF(2);
	painter.setPen(pen1);

	int i = 0;

	float s_coord = Translator->GetVRChPos(respond, i, MKSMode, cos_al) - OscBegin;
	float s_coord_l = s_coord;


	quint8 max_VRCh_val = (Translator->MaxkUsVal - respond.kUs) * 3;

	if(max_VRCh_val == 0)
		return;


	if(s_coord > 0)
	{
		if(s_coord * koeff > width)
			s_coord = width / koeff;

		QLine line( x_b, GetVRChPorog(respond, i, max_VRCh_val) + 1 , x_b + koeff * s_coord, GetVRChPorog(respond, i, max_VRCh_val) + 1 );
		painter.drawLine(line);
	}

	i++;

	while((i < 8) && (i <= respond.nPoVrch) && (s_coord * koeff < width))
	{
		s_coord_l = s_coord;
		s_coord = Translator->GetVRChPos(respond, i, MKSMode, cos_al) - OscBegin;

		if(s_coord > 0)
		{
			if(s_coord_l < 0)
			{
				float coord_y1 = GetVRChPorog(respond, i - 1, max_VRCh_val);
				float coord_y2 = GetVRChPorog(respond, i, max_VRCh_val);
				float tang = (coord_y2 - coord_y1) / (s_coord - s_coord_l);

				if(s_coord * koeff >= width)
				{
					float b = coord_y1 - s_coord_l*tang;

					QLine line( x_b, b, x_b + width, tang * width / koeff + b );
					painter.drawLine(line);
				}
				else
				{
					QLine line( x_b, coord_y1 + 1 - tang * s_coord_l, x_b + koeff * s_coord, coord_y2 + 1 );
					painter.drawLine(line);
				}
			}
			else if(s_coord * koeff >= width)
			{
				float coord_y1 = GetVRChPorog(respond, i - 1, max_VRCh_val);
				float coord_y2 = GetVRChPorog(respond, i, max_VRCh_val);
				float tang = (coord_y2 - coord_y1) / (s_coord - s_coord_l) ;

				QLine line( x_b + koeff * s_coord_l, coord_y1 + 1, x_b + width, coord_y2 + 1 - (s_coord - width / koeff) * tang );
				painter.drawLine(line);
			}
			else
			{
				QLine line( x_b + koeff * s_coord_l, GetVRChPorog(respond, i - 1, max_VRCh_val) + 1 , x_b + koeff * s_coord, GetVRChPorog(respond, i, max_VRCh_val) + 1 );
				painter.drawLine(line);
			}
		}
		i++;
	}

	i--;

	if(s_coord * koeff < width)
	{
		if(s_coord < 0)
			s_coord = 0;

		QLine line( x_b + koeff * s_coord, GetVRChPorog(respond, i, max_VRCh_val) + 1 , x_b + width, GetVRChPorog(respond, i, max_VRCh_val) + 1 );
		painter.drawLine(line);
	}


	if(clean)
	{
		OutFrameText(painter, QString(), x_b + width + frame_width, y_b + frame_width/2, frame_width, frame_width, true);
		OutFrameText(painter, QString(), x_b + width + frame_width, y_b + frame_width/2 + VRChPart*height , frame_width, frame_width, true);
	}
	else
	{
		OutFrameText(painter, QString::number(max_VRCh_val / 3, 10) , x_b + width + frame_width, y_b + frame_width/2, frame_width, frame_width, false);
		OutFrameText(painter, "0", x_b + width + frame_width, y_b + frame_width/2  + VRChPart*height, frame_width, frame_width, false);
	}


}


bool OscillogramPlotter::IsInsidePlot(const float x, const float y)
{
	return (x >= x_b) && (x <= x_b + width) && (y >= y_b) && (y <= y_b + height);
}


bool OscillogramPlotter::IsInsideBeginOsc(const float x, const float y)
{
	return (y > y_b + height + StrobSetHeight/2 + 2) && (x < (width + 2*left)/5 );
}

bool OscillogramPlotter::IsInsideEndOsc(const float x, const float y)
{
	return (y > y_b + height + StrobSetHeight/2 + 2) && (x > 4*(width + 2*left)/5 );
}

bool OscillogramPlotter::IsInsideStrobVisible(const float koeff, const float s_begin, const float s_end)
{
	return (s_begin >= 0 && ((s_end * koeff) <= width));
}



bool OscillogramPlotter::IsInsideStrobLeft(const float koeff, const float s_begin, const float s_end, const float s_porog, const float x, const float y)
{
	float x_min = x_b + koeff * s_begin - 3;
	float x_max = x_b + koeff * s_begin + 1;

	float y_min = s_porog - StrobSetHeight/2;
	float y_max = s_porog + StrobSetHeight/2;


	return (x >= x_min) && (x < x_max) && (y >= y_min) && (y <= y_max);
}

bool OscillogramPlotter::IsInsideStrobMiggle(const float koeff, const float s_begin, const float s_end, const float s_porog, const float x, const float y)
{
	float x_min = x_b + koeff * s_begin + 1;
	float x_max = x_b + koeff * s_end - 1;

	float y_min = s_porog - StrobSetHeight/2;
	float y_max = s_porog + StrobSetHeight/2;


	return (x >= x_min) && (x <= x_max) && (y >= y_min) && (y <= y_max);
}

bool OscillogramPlotter::IsInsideStrobRight(const float koeff, const float s_begin, const float s_end, const float s_porog, const float x, const float y)
{
	float x_min = x_b + koeff * s_end - 1;
	float x_max = x_b + koeff * s_end + 3;

	float y_min = s_porog - StrobSetHeight/2;
	float y_max = s_porog + StrobSetHeight/2;


	return (x > x_min) && (x <= x_max) && (y >= y_min) && (y <= y_max);
}


void OscillogramPlotter::TestNewPosition(const QPointF &rel_pos)
{
	PositionTesterPlotter::TestNewPosition(rel_pos);


	if(is_dragging)
	{
		switch(curr_area)
		{
		case InBeginOsc:
			emit BegOscIntermedChange(CalculateCurrOscBeg());
			break;
		case InEndOsc:
			emit EndOscIntermedChange(CalculateCurrOscEnd());
			break;

		case InStrobLeft:
			{
				float koeff  = ((RefferenceRespond.step_r + 1) * LEN_OSC * STEP_KOEF * TEN_F) / width;

				int length_displ = koeff * (curr_x - drag_start_x);

				if(length_displ > RefferenceRespond.strb[curr_strob].len)
					length_displ = RefferenceRespond.strb[curr_strob].len;

				if(- length_displ > RefferenceRespond.strb[curr_strob].beg)
					length_displ = - RefferenceRespond.strb[curr_strob].beg;

				emit SetStrobIntermedLeft(curr_strob, RefferenceRespond.strb[curr_strob].beg + length_displ , RefferenceRespond.strb[curr_strob].len - length_displ);
			}
			break;

		case InStrobMiggle:
			{
				float koeff  = ((RefferenceRespond.step_r + 1) * LEN_OSC * STEP_KOEF * TEN_F) / width;
				int new_beg = RefferenceRespond.strb[curr_strob].beg + koeff * (curr_x - drag_start_x);
				int new_porog = RefferenceRespond.strb[curr_strob].por - (curr_y - drag_start_y) / height * MAX_OSC;

				if(new_beg < 0)
					new_beg = 0;

				if(new_porog < 0)
					new_porog = 0;
				if(new_porog > MAX_OSC)
					new_porog = MAX_OSC;

				emit SetStrobIntermedPos(curr_strob, new_beg, new_porog);
			}
			break;

		case InStrobRight:
			SetStrobIntermedRight(curr_strob, CalculateCurrStrobLen());
			break;

		default: ;
		}

		return;
	}


	if( IsInsideBeginOsc(curr_x, curr_y) )
	{
		if(cursor_curr != Qt::SizeHorCursor)
		{
			cursor_curr = Qt::SizeHorCursor;
			cursor_changed = true;
			curr_area = InBeginOsc;
		}
	}
	else if( IsInsideEndOsc(curr_x, curr_y) )
	{
		if(cursor_curr != Qt::SizeHorCursor)
		{
			cursor_curr = Qt::SizeHorCursor;
			cursor_changed = true;
			curr_area = InEndOsc;
		}
	}
	else
	{
		bool any_strob = false;

		if(en_set_device_params && pOldRespond->video_mode)
		{
			float koeff  = (float)width /((pOldRespond->step_r + 1) * LEN_OSC * STEP_KOEF);
			float OscBegin = pOldRespond->begr/TEN_F;

			for(int ii=0;ii<2/*MAX_NUM_STRBS*/;ii++)
			{
				//if(respond.strb[ii].on_strb)
				{
					float s_begin= pOldRespond->strb[ii].beg/TEN_F - OscBegin;;
					float s_end= s_begin+ pOldRespond->strb[ii].len/TEN_F;
					float s_porog = y_b + (height * (1.0f - pOldRespond->strb[ii].por * 1.0f /MAX_OSC) );

					if( IsInsideStrobVisible(koeff, s_begin, s_end) )
					{

						if( IsInsideStrobMiggle(koeff, s_begin, s_end, s_porog, curr_x, curr_y) )
						{
							any_strob = true;
							curr_strob = ii;
							curr_area = InStrobMiggle;

							if(cursor_curr != Qt::SizeAllCursor)
							{
								cursor_curr = Qt::SizeAllCursor;
								cursor_changed = true;
							}

						}
						else if( IsInsideStrobLeft(koeff, s_begin, s_end, s_porog, curr_x, curr_y) )
						{
							any_strob = true;
							curr_strob = ii;
							curr_area = InStrobLeft;

							if(cursor_curr != Qt::SizeHorCursor)
							{
								cursor_curr = Qt::SizeHorCursor;
								cursor_changed = true;
							}
						}
						else if( IsInsideStrobRight(koeff, s_begin, s_end, s_porog, curr_x, curr_y) )
						{
							any_strob = true;
							curr_strob = ii;
							curr_area = InStrobRight;

							if(cursor_curr != Qt::SizeHorCursor)
							{
								cursor_curr = Qt::SizeHorCursor;
								cursor_changed = true;
							}
						}
					}
				}
			}
		}

		
		if(!any_strob && cursor_curr != Qt::ArrowCursor)
		{
			cursor_curr = Qt::ArrowCursor;
			cursor_changed = true;
			curr_area = Outside;
		}
	}

}


void OscillogramPlotter::DragStarted()
{
	RefferenceRespond = *pOldRespond;
}

void OscillogramPlotter::DragEnded()
{
	switch(curr_area)
	{
	case InBeginOsc:
		emit BegOscFinalChange();
		break;
	case InEndOsc:
		emit EndOscFinalChange();
		break;

	case InStrobLeft:
		emit SetStrobFinalLeft();
		break;

	case InStrobMiggle:
		emit SetStrobFinalPos();
		break;

	case InStrobRight:
		emit SetStrobFinalRight();
		break;

	default: ;
	}

}


int OscillogramPlotter::CalculateCurrOscBeg()
{
	float koeff  = ((RefferenceRespond.step_r + 1) * LEN_OSC * STEP_KOEF * TEN_F) / width;
	int NewOscBegin = RefferenceRespond.begr - (curr_x - drag_start_x)*koeff;

	if(NewOscBegin < 0)
		NewOscBegin = 0;

	return NewOscBegin;
}

int OscillogramPlotter::CalculateCurrOscEnd()
{
	float koeff  = ((RefferenceRespond.step_r + 1) * LEN_OSC * STEP_KOEF * TEN_F) / width;

	int NewOscEnd = RefferenceRespond.begr - (curr_x - drag_start_x)*koeff + ((RefferenceRespond.step_r + 1) * LEN_OSC * STEP_KOEF * TEN_F);

	if(NewOscEnd < 0)
		NewOscEnd = 0;

	return NewOscEnd;
}


int OscillogramPlotter::CalculateCurrStrobLen()
{
	float koeff  = ((RefferenceRespond.step_r + 1) * LEN_OSC * STEP_KOEF * TEN_F) / width;

	int len = RefferenceRespond.strb[curr_strob].len + koeff * (curr_x - drag_start_x);

	if(len < 0)
		len = 0;

	return len;
}


