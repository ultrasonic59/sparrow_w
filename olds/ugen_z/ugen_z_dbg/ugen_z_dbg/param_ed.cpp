#include "param_ed.h"
 
 
param_ed::param_ed(QWidget *parent) :
					QLineEdit(parent),

					i_max(0),
					i_min(0),
					i_inc_step(1),
					p_cdata(NULL),
					p_sdata(NULL),
					p_ldata(NULL),
					num_dig(2),
					coeff(0),
					num_dights(0),
					shift(0),
					ena_enter_on_off(false),
////					data_size(sizeof(char)),
					data_size(10),
					ena_edit(true)

{
///	QSizePolicy temp = this->sizePolicy();
////	temp.setHeightForWidth(true);
///	this->setSizePolicy(temp);

}

#if 0
void myline_ed::resizeEvent(QResizeEvent * event)
{
///QSize size_lab;
////size_lab=ui->label_photo->size();

///qDebug() << "mylabel::resizeEvent";
///QPixmap pixmap = QPixmap::fromImage(m_img1);
///ui->label_photo->setPixmap( pixmap.scaled(ui->label_photo->width(), ui->label_photo->height(), Qt::KeepAspectRatio ) );

}
#endif
#if 1
void param_ed::mouseDoubleClickEvent(QMouseEvent* pe)
{
if(ena_enter_on_off)
	emit on_off();
qDebug() << "myline_ed::mouseDoubleClickEvent";
}
#endif
#if 0
void mylabel::mousePressEvent(QMouseEvent* pe)
{
////load_photos();
////view_n_photo(m_row,m_col,ui->label_pict1);
	qDebug() << "mousePressEvent:"<<pe->button();
}
///++++++++++++++++++++++++++++++++++++++
int myline_ed::heightForWidth(int w) const
{
return (int)((double)w*0.8);
}
///++++++++++++++++++++++++++++++++++++++

#endif
void	param_ed::set_min_max(int imin,int imax)
{
i_max=imax;
i_min=imin;
}
void param_ed::set_ena_edit(bool iena_edit)
{
ena_edit=iena_edit;
}

void param_ed::set_data_size(int idata_size)
{
data_size=idata_size;
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
data_size=sizeof(unsigned char);
p_cdata=p_data;
}
void param_ed::set_data(unsigned short *p_data)
{
data_size=sizeof(unsigned short);
p_sdata=p_data;
}
void param_ed::set_data(unsigned long *p_data)
{
data_size=sizeof(unsigned long);
p_ldata=p_data;
}
void param_ed::set_colors(const char *bg,const char *col)
{
QString tstr;
///sprintf(tstr,"background-color: %d;color:%d",bg,col);
tstr.sprintf("background-color: %s;color:%s",bg,col);
///setStyleSheet("background-color: rgb(200, 0, 0); color: rgb(0, 0, 0)");
setStyleSheet(tstr);
}
void param_ed::set_ena_enter_on_off(bool ena)
{
ena_enter_on_off=ena;
}
void param_ed::inc_par()
{
QString tstr;
if(p_cdata||p_sdata||p_ldata)
	{
	if(data_size==sizeof(short))
		{
		if((*p_sdata + i_inc_step)<=i_max)
			{
			*p_sdata += i_inc_step;
			}
		else
			{
			*p_sdata=i_max;
			}
		}
	else if(data_size==sizeof(long))
		{
		if((*p_ldata + i_inc_step)<=i_max)
			{
			*p_ldata += i_inc_step;
			}
		else
			{
			*p_ldata=i_max;
			}
		}
	else if(data_size==sizeof(char))
		{
		if((*p_cdata + i_inc_step)<=i_max)
			{
			*p_cdata += i_inc_step;
			}
		else
			{
			*p_cdata=i_max;
			}
		}

	emit param_changed();
	}
}
void param_ed::dec_par()
{
QString tstr;
if(p_cdata||p_sdata||p_ldata)
	{
	if(data_size==sizeof(short))
		{
		if((*p_sdata - i_inc_step)>=i_max)
			*p_sdata = i_max;
		else if((*p_sdata - i_inc_step)>=i_min)
			*p_sdata -= i_inc_step;
		else
			*p_sdata=i_min;
		}
	else if(data_size==sizeof(long))
		{
		if((*p_ldata - i_inc_step)>=i_max)
			*p_ldata = i_max;
		else if((*p_ldata - i_inc_step)>=i_min)
			*p_ldata -= i_inc_step;
		else
			*p_ldata=i_min;
		}
	else 
		{
		if((*p_cdata - i_inc_step)>=i_max)
			*p_cdata = i_max;
		else if((*p_cdata - i_inc_step)>=i_min)
			*p_cdata -= i_inc_step;
		else
			*p_cdata=i_min;
		}

	emit param_changed();
	}
}

void param_ed::show_par()
{
QString tstr;
if(p_cdata||p_sdata||p_ldata)
	{
	if(data_size==sizeof(short))
	{
		if(num_dights > 0)
			tstr = QString::number((static_cast<float>(*p_sdata + shift) / coeff),'f', num_dights);
		else
			tstr = QString::number(*p_sdata + shift, 10);
	}
	else if(data_size==sizeof(long))
	{
		if(num_dights > 0)
			tstr = QString::number((static_cast<float>(*p_ldata + shift) / coeff),'f', num_dights);
		else
			tstr = QString::number(*p_ldata + shift, 10);
	}
	else 
	{
		if(num_dights > 0)
			tstr = QString::number((static_cast<float>(*p_cdata + shift) / coeff),'f', num_dights);
		else
			tstr = QString::number(*p_cdata + shift, 10);
	}

	setText(tstr);
	}
}

void param_ed::keyPressEvent(QKeyEvent * pe)
{
////qDebug() << pe->key();
switch (pe->key()) 
{
case Qt::Key_Up:
	if(ena_edit)
		{
		inc_par();
		show_par();
		}
	break;
case Qt::Key_Down:
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
	{
		if(num_dights != 0 && ena_edit)
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

		if(ena_edit)
		{
			QLineEdit::keyPressEvent(pe);
			QString tstr=text();

			if(num_dights > 0)
			{
				int itmp=  tstr.toFloat() * coeff + 0.5f;

				if(itmp > i_max)
					itmp = i_max;
				else if(itmp < i_min)
					itmp = i_min;

				itmp -= shift;

				if(p_cdata||p_sdata||p_ldata)
					{
					if(data_size==sizeof(short))
						*p_sdata=itmp;
					else if(data_size==sizeof(long))
						*p_ldata=itmp;
					else 
						*p_cdata=itmp;
					}
			}
			else
			{
				int itmp=tstr.toInt();

				if(itmp > i_max)
					itmp = i_max;
				else if(itmp < i_min)
					itmp = i_min;

				itmp -= shift;

				if(p_cdata||p_sdata||p_ldata)
					{
					if(data_size==sizeof(short))
						*p_sdata=itmp;
					else if(data_size==sizeof(long))
						*p_ldata=itmp;
					else 
						*p_cdata=itmp;
					}

			}
		}
	break;

case Qt::Key_Left:
case Qt::Key_Right:

	QLineEdit::keyPressEvent(pe);
	break;

}
}
