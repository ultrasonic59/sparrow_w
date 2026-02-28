#include "AutoAmplPlotter.h"
#include <QtGui/QPainter>


AutoAmplPlotter::AutoAmplPlotter(RespondFrame *OldFrame, int *New_frame_len, int *Old_frame_len, OscillogramProperties *newOscProp) : AbstactPlotter(),
								AnyReceived(false),
								pOldFrame(OldFrame),
								pNew_frame_len(New_frame_len),
								pOld_frame_len(Old_frame_len),
								pix_per_len(4),
								left_tab(24),
								tact_text_size(11),
								sep_line_W(2),
								defect_height(50),
								defect_base_y( sep_line_W ),
								MaxAmpl(255.0f),
								plot_line_W(1),
								plot_base_y( sep_line_W + defect_height + sep_line_W),
								plot_base1_y(0),
								plot_base_step_y(0),
								plot_step_x(0),
								plot_step_y(0),
								OscProp(newOscProp)
{
	for(quint8 i = 0; i < NUM_CHANS; i++)
		us_on[i] = 1;

	curr_chan_num = NUM_CHANS;
}


AutoAmplPlotter::~AutoAmplPlotter(void)
{
}



bool AutoAmplPlotter::UsChange(void)
{
	bool any_change = false;

	if(*pOld_frame_len == 0)
		return false;


	const RespondAuto last_resp = (pOldFrame->GetFrameArray())[ pOldFrame->GetSize() - 1];

	for(quint8 i = 0; i < NUM_CHANS && !any_change; i++)
	{
		if(us_on[i] != last_resp.us_on[i])
			any_change = true;
	}

	return any_change;
}


void AutoAmplPlotter::ApplyChanChange(void)
{
	if(*pOld_frame_len == 0)
		curr_chan_num = NUM_CHANS;

	const RespondAuto last_resp = (pOldFrame->GetFrameArray())[ pOldFrame->GetSize() - 1];

	curr_chan_num = 0;

	for(quint8 i = 0; i < NUM_CHANS; i++)
	{
		us_on[i] = last_resp.us_on[i];
		if(last_resp.us_on[i])
			curr_chan_num++;
	}


	if(curr_chan_num != 0)
		plot_base_step_y = (float)(pixmap_height - plot_base_y) / curr_chan_num;

	if(plot_base_step_y < 0)
	{
		plot_base_step_y = 0;
		plot_step_y = 0;
	}
	else
	{
		plot_step_y = (plot_base_step_y - sep_line_W - 2*plot_line_W) / MaxAmpl;
		plot_base1_y = plot_base_y + plot_base_step_y - sep_line_W - plot_line_W;
	}
	

	PaintBackground();
}



QRect & AutoAmplPlotter::SetPlotRect(const QRect &fullRect)
{
	QRect &PosRect = AbstactPlotter::SetPlotRect(fullRect);

	if(!NeedResize)
		return PosRect;

	if(curr_chan_num != 0)
		plot_base_step_y = (float)(pixmap_height - plot_base_y) / curr_chan_num;

	if(plot_base_step_y < 0)
	{
		plot_base_step_y = 0;
		plot_step_y = 0;
	}
	else
	{
		plot_step_y = (plot_base_step_y - sep_line_W - 2*plot_line_W) / MaxAmpl;
		plot_base1_y = plot_base_y + plot_base_step_y - sep_line_W - plot_line_W;
	}
	

	plot_step_x = (float)(pixmap_width - left_tab) / *pOld_frame_len;

	PaintBackground();

	*pNew_frame_len = (pixmap_width - left_tab) / pix_per_len;

	return PosRect;
}


void AutoAmplPlotter::PlotBackgroundLines(QPainter & painter)
{
	QPen pen1(OscProp->SepLineColor);
	pen1.setWidth(sep_line_W);
	painter.setPen(pen1);

	float base_y = sep_line_W/2;

	QLine line( left_tab,  base_y, pixmap_width,  base_y);
	painter.drawLine(line);

	base_y += defect_height + sep_line_W;

	line.setLine( left_tab,  base_y, pixmap_width,  base_y);
	painter.drawLine(line);

	
	for(int i = 0; i < NUM_CHANS; i++)
	{
		if(us_on[i])
		{
			base_y += plot_base_step_y;

			line.setLine( left_tab,  base_y, pixmap_width,  base_y);
			painter.drawLine(line);
		}

	}

//	painter.fillRect(left_tab, sep_line_W , pixmap_width - left_tab, defect_height, OscProp->BaseDefColor );

}



void AutoAmplPlotter::OutTactLables(QPainter & painter)
{
	QPen pen1(OscProp->TextColor);
	painter.setPen(pen1);

	QFont Front;
	Front.setPointSize(tact_text_size);
	painter.setFont(Front);


	float base_y = sep_line_W / 2 + defect_height + sep_line_W;
	
	for(int i = 0; i < NUM_CHANS; i++)
	{
		if(us_on[i])
		{
			base_y += plot_base_step_y;
			OutString(painter, left_tab / 2, base_y, left_tab, plot_base_step_y, false, QString::number((i+1), 10));
		}
	}

}

void AutoAmplPlotter::PaintBackground(void)
{
	if(pixmap_width != 0 && pixmap_height != 0)
	{

		QPainter painter(pixmap);
		painter.fillRect(0, 0, pixmap_width, pixmap_height, OscProp->BackgroundColor );
		PlotBackgroundLines(painter);

		OutTactLables(painter);

		painter.end();

		PlotOldRespond(false);
	
	}
}

void AutoAmplPlotter::PlotDefectsLine(QPainter & painter, const float curr_x, const quint16 def_type)
{

	switch(def_type)
	{
	case RespondAuto::NoDefect:
		painter.fillRect(curr_x, defect_base_y , plot_step_x + 1, defect_height, OscProp->BaseDefColor );
		break;
	case RespondAuto::NoContact:
		painter.fillRect(curr_x, defect_base_y , plot_step_x + 1, defect_height, OscProp->NoContactColor );
		break;
	case RespondAuto::Defect:
		painter.fillRect(curr_x, defect_base_y , plot_step_x + 1, defect_height, OscProp->DefectColor );
		break;

	default:;
	}

}

void AutoAmplPlotter::PlotDefects(QPainter & painter)
{
	float curr_x = left_tab;
	const RespondAuto *resp_arr = pOldFrame->GetFrameArray();

	for(int i = 0; i < pOldFrame->GetSize() - 1; i++)
	{
		quint16 def_type = resp_arr[i].type_def;

		PlotDefectsLine(painter, curr_x, def_type);

		curr_x += plot_step_x;
	}

}



void AutoAmplPlotter::OutString(QPainter & painter, const int x_middle, const int y_base, const int x_size, const int y_size, const bool clear, QString &str)
{
	QRect rect( x_middle - x_size / 2, y_base - y_size , x_size, y_size);

	if(clear)
		painter.fillRect(rect, OscProp->BackgroundColor);
	else
		painter.drawText(rect, Qt::AlignCenter, str);
}



void AutoAmplPlotter::PlotOldRespond(const bool clearing)
{
	if(!AnyReceived)
		return;

	QPainter painter(pixmap);
	

	plot_step_x = (float)(pixmap_width - left_tab) / *pOld_frame_len;

	if(!clearing)
	{
		painter.fillRect(left_tab, sep_line_W , pixmap_width - left_tab, defect_height, OscProp->BackgroundColor );
		PlotDefects(painter);
	}

	if(curr_chan_num == 0)
		return;

	float curr_x;
	float base_y = plot_base1_y;

	const RespondAuto *resp_arr = pOldFrame->GetFrameArray();


	QPen pen1(clearing ? OscProp->BackgroundColor : OscProp->StrobsColor[0]);
	pen1.setWidth(plot_line_W);
	pen1.setCapStyle(Qt::FlatCap);

	painter.setPen(pen1);

	for(int i = 0; i < NUM_CHANS; i++)
	{
		if(us_on[i])
		{
			curr_x = left_tab;
			for(int j = 0; j < pOldFrame->GetSize() - 1; j++)
			{
				QLineF line( curr_x,  (base_y - resp_arr[j].ampl[i] * plot_step_y), curr_x + plot_step_x,  (base_y - resp_arr[j+1].ampl[i] * plot_step_y));
				painter.drawLine(line);
				curr_x += plot_step_x;
			}
			base_y += plot_base_step_y;
		}
	}

	if(!clearing)
	{
		pen1.setColor(OscProp->StrobsColor[1]);
		painter.setPen(pen1);
	}

	base_y = plot_base1_y;

	for(int i = 0; i < NUM_CHANS; i++)
	{
		if(us_on[i])
		{
			curr_x = left_tab;
			for(int j = 0; j < pOldFrame->GetSize() - 1; j++)
			{
				QLineF line( curr_x,  (base_y - resp_arr[j].ampl2[i] * plot_step_y), curr_x + plot_step_x,  (base_y - resp_arr[j+1].ampl2[i] * plot_step_y));
				painter.drawLine(line);
				curr_x += plot_step_x;
			}
			base_y += plot_base_step_y;
		}
	}


}
void AutoAmplPlotter::PlotNewRespond(void)
{
	if(!AnyReceived)
		AnyReceived = true;

	if(pixmap_width != 0 && pixmap_height != 0)
	{
		if(UsChange())
		{
			ApplyChanChange();
		}

		PlotOldRespond(false);
		UpdatePlot();
	}
}