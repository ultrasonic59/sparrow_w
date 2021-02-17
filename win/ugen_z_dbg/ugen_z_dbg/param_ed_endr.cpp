#include "param_ed_endr.h"


param_ed_endr::param_ed_endr(QWidget *parent)
	: QLineEdit(parent),
	i_max(0),
	i_min(0),
	i_inc_step(0),
	p_cdata(nullptr),
	num_dig(0),
	coeff(1),
	num_dights(0),
	Result(0)

{

}

void	param_ed_endr::set_min_max(int imin,int imax)
{
	i_max=imax;
	i_min=imin;
}


void param_ed_endr::set_inc_step(int istep)
{
	i_inc_step=istep;
}
void param_ed_endr::set_num_dig(int inum_dig)
{
	num_dig=inum_dig;
	setMaxLength(inum_dig + 1);
}

void param_ed_endr::set_coeff(int icoeff)
{
	coeff=icoeff;
	if(coeff >= 10)
	{
		num_dights = log10(static_cast<float>(coeff));
		setMaxLength(num_dig+1);
	}
	else
	{
		num_dights = 0;
		setMaxLength(num_dig);
	}
}

void param_ed_endr::set_data(unsigned char *p_data)
{
	p_cdata=p_data;
}


void param_ed_endr::inc_par()
{
QString tstr;
if(p_cdata)
	{
	if((*p_cdata + i_inc_step)<=i_max)
		{
		*p_cdata += i_inc_step;
		}
	else
		{
		*p_cdata=i_max;
		}	

	emit param_changed();
	}
}

void param_ed_endr::dec_par()
{
QString tstr;
if(p_cdata)
	{
	if((*p_cdata - i_inc_step)>=i_max)
		*p_cdata = i_max;
	else if((*p_cdata - i_inc_step)>=i_min)
		*p_cdata -= i_inc_step;
	else
		*p_cdata=i_min;

	emit param_changed();
	}
}

void param_ed_endr::show_par(int val)
{
	QString tstr;

	if(num_dights > 0)
		tstr = QString::number((static_cast<float>(val) / coeff),'f', num_dights);
	else
		tstr = QString::number(val, 10);

	setText(tstr);
}



void param_ed_endr::keyPressEvent(QKeyEvent * pe)
{
switch (pe->key()) 
{
case Qt::Key_Up:
		inc_par();
	break;
case Qt::Key_Down:
		dec_par();
	break;
case Qt::Key_Return:
	emit result_changed();
	break;
case Qt::Key_Comma:
	{
		if(num_dights != 0)
		{
			QString tstr=text();
			if(!tstr.contains('.'))
			{
				tstr+=".";
				setText(tstr);
			}
		}
	}
	break;

case Qt::Key_Period:
	if(num_dights == 0)
		break;
	else
	{
		QString tstr=text();
		if(tstr.contains('.'))
			break;
	}

case Qt::Key_0:
case Qt::Key_1:
case Qt::Key_2:
case Qt::Key_3:
case Qt::Key_4:
case Qt::Key_5:
case Qt::Key_6:
case Qt::Key_7:
case Qt::Key_8:
case Qt::Key_9:
case Qt::Key_Backspace:
case Qt::Key_Delete:
	{
		QLineEdit::keyPressEvent(pe);
		QString tstr=text();

		if(num_dights > 0)
			Result =  tstr.toFloat() * coeff + 0.5f;
		else
			Result = tstr.toInt();	
	}
	break;
case Qt::Key_Left:
case Qt::Key_Right:

	QLineEdit::keyPressEvent(pe);
	break;

}
}


int param_ed_endr::GetResult()
{
	return Result;
}