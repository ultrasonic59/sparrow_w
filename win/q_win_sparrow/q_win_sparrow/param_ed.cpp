#include "param_ed.h"
 
 
param_ed::param_ed(QWidget *parent) :
					QLineEdit(parent),

					i_max(0),
					i_min(0),
					i_inc_step(1),
					p_cdata(nullptr),
					p_sdata(nullptr),
					p_ldata(nullptr),

					any_elem(false),

					num_dig(2),
					coeff(0),
					num_dights(0),
					shift(0),
					ena_enter_on_off(false),
					ena_edit(true)
{
}

int param_ed::GetMinimalWidth() const
{
	QString test_str("_");
	for(int i = 0; i < num_dig; i++)
		test_str += "_";

	return fontMetrics().boundingRect(test_str).width();
}


QSize param_ed::minimumSizeHint() const
{
	if(num_dig == 0)
		return QLineEdit::minimumSizeHint();

	QSize curr_size = QLineEdit::minimumSizeHint();
	curr_size.setWidth( GetMinimalWidth() );
	return curr_size;
}


QSize param_ed::sizeHint() const
{
	if(num_dig == 0)
		return QLineEdit::sizeHint();

	QSize curr_size = QLineEdit::sizeHint();
	curr_size.setWidth( GetMinimalWidth() );
	return curr_size;
}



void param_ed::mouseDoubleClickEvent(QMouseEvent* pe)
{
	if(ena_enter_on_off)
		emit on_off();
}

void param_ed::set_min_max(int imin,int imax)
{
	i_max=imax;
	i_min=imin;
}

void param_ed::set_ena_edit(bool iena_edit)
{
	ena_edit=iena_edit;
}

void param_ed::set_inc_step(int istep)
{
	i_inc_step=istep;
}

void param_ed::set_num_dig(int inum_dig)
{
	num_dig=inum_dig;
	if(num_dights == 0)
		setMaxLength(inum_dig);
	else
		setMaxLength(inum_dig+1);
}

void param_ed::set_coeff(int icoeff)
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


void param_ed::set_shift(int ishift)
{
	shift = ishift;
}


void param_ed::set_data(unsigned char *p_data)
{
	p_cdata=p_data;
	any_elem = p_cdata ? true : false;
	p_sdata=nullptr;
	p_ldata=nullptr;
}

void param_ed::set_data(unsigned short *p_data)
{
	p_sdata=p_data;
	any_elem = p_sdata ? true : false;
	p_cdata=nullptr;
	p_ldata=nullptr;
}

void param_ed::set_data(unsigned long *p_data)
{
	p_ldata=p_data;
	any_elem = p_ldata ? true : false;
	p_cdata=nullptr;
	p_sdata=nullptr;
}

void param_ed::set_colors(const char *col)
{
	if(strcmp("rgb(0, 0, 0)", col) == 0)
		setStyleSheet("");
	else
	{
		QString tstr;
		tstr.sprintf("color:%s",col);
		setStyleSheet(tstr);
	}
}

void param_ed::set_ena_enter_on_off(bool ena)
{
	ena_enter_on_off=ena;
}

void param_ed::inc_par(const int count)
{
	if(!any_elem)
		return;

	if(p_sdata)
		inc_par_val(p_sdata, count);
	else if(p_ldata)
		inc_par_val(p_ldata, count);
	else if(p_cdata)
		inc_par_val(p_cdata, count);

	emit param_changed();
}

void param_ed::dec_par(const int count)
{
	if(!any_elem)
		return;

	if(p_sdata)
		dec_par_val(p_sdata, count);
	else if(p_ldata)
		dec_par_val(p_ldata, count);
	else if(p_cdata)
		dec_par_val(p_cdata, count);

	emit param_changed();
}

void param_ed::show_par()
{
	if(!any_elem)
		return;

	QString tstr;
	
	if(p_sdata)
	{
		if(num_dights > 0)
			tstr = QString::number((static_cast<float>(*p_sdata + shift) / coeff),'f', num_dights);
		else
			tstr = QString::number(*p_sdata + shift, 10);
	}
	else if(p_ldata)
	{
		if(num_dights > 0)
			tstr = QString::number((static_cast<float>(*p_ldata + shift) / coeff),'f', num_dights);
		else
			tstr = QString::number(*p_ldata + shift, 10);
	}
	else if(p_cdata)
	{
		if(num_dights > 0)
			tstr = QString::number((static_cast<float>(*p_cdata + shift) / coeff),'f', num_dights);
		else
			tstr = QString::number(*p_cdata + shift, 10);
	}

	setText(tstr);
}

void param_ed::apply_current()
{
	ApplyNewText();
	show_par();
}

void param_ed::ApplyNewText()
{
	QString tstr=text();

	if(num_dights > 0)
	{
		int itmp = tstr.toFloat() * coeff + 0.5f - shift;

		if(itmp > i_max)
			itmp = i_max;
		else if(itmp < i_min)
			itmp = i_min;

		if(any_elem)
		{
			if(p_sdata)
				*p_sdata=itmp;
			else if(p_ldata)
				*p_ldata=itmp;
			else if(p_cdata)
				*p_cdata=itmp;
		}
	}
	else
	{
		int itmp = tstr.toInt() - shift;

		if(itmp > i_max)
			itmp = i_max;
		else if(itmp < i_min)
			itmp = i_min;

		if(any_elem)
		{
			if(p_sdata)
				*p_sdata=itmp;
			else if(p_ldata)
				*p_ldata=itmp;
			else if(p_cdata)
				*p_cdata=itmp;
		}
	}
}

void param_ed::keyPressEvent(QKeyEvent * pe)
{
	switch (pe->key()) 
	{
	case Qt::Key_Up:
	case 0x42e:
		if(ena_edit)
		{
			inc_par();
			show_par();
		}
		break;
	case Qt::Key_Down:
	case 0x411:
		if(ena_edit)
		{
			dec_par();
			show_par();
		}
		break;
	//case Qt::Key_Enter:
	case Qt::Key_Return:

		show_par();

		if(ena_edit)
			emit param_changed();

		if(ena_enter_on_off)
			emit on_off();
		break;

	case Qt::Key_Comma:
		if(ena_edit)
		{
			QString tstr=text();
			if(num_dights != 0 && !tstr.contains('.'))
			{
				tstr+=".";
				setText(tstr);
			}
			else
			{
				dec_par();
				show_par();
			}
		}
		break;

	case Qt::Key_Period:
		if(ena_edit)
		{
			QString tstr=text();
			if(num_dights == 0 || tstr.contains('.'))
			{
				inc_par();
				show_par();
			}
			else
				QLineEdit::keyPressEvent(pe);
		}
		break;

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

		if(ena_edit)
		{
			QLineEdit::keyPressEvent(pe);
			ApplyNewText();
		}
		break;

	case Qt::Key_Left:
	case Qt::Key_Right:

		QLineEdit::keyPressEvent(pe);
		break;

	}
}
