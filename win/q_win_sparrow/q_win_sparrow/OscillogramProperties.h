#pragma once

#include <QColor>


class OscillogramProperties
{
public:

	QColor BackgroundColor;
	QColor OscillogramColor;
	QColor GridColor;
	QColor TextColor;


	int row_number;
	int column_number;


    float OscillogramWidth;

	

	OscillogramProperties(void);
	virtual ~OscillogramProperties(void);	

};

