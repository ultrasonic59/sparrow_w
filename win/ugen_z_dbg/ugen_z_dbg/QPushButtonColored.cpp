#include "QPushButtonColored.h"
#include <QPaintEvent>
#include <QPainter>

QPushButtonColored::QPushButtonColored(QWidget *parent)
	: QPushButton(parent),
	frame_width(5),
	tempColor(Qt::white)
{
}

QPushButtonColored::~QPushButtonColored()
{

}

void QPushButtonColored::SetColor(const QColor & color)
{
	tempColor = color;
	repaint();
}


void QPushButtonColored::paintEvent(QPaintEvent * event)
{
	QPushButton::paintEvent(event);
	QPainter p(this);
	QRect cRect = event->rect();
	cRect.adjust(frame_width, frame_width, -frame_width /*- 1*/, -frame_width /*- 1*/);
	p.fillRect(cRect,tempColor);
	p.end();
}

const QColor & QPushButtonColored::GetColor(void) const
{
	return tempColor;
}
