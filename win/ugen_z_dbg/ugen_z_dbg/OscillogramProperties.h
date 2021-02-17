#pragma once

#include <QColor>
#include "params.h"


class OscillogramProperties
{
public:

	QColor BackgroundColor;
	QColor OscillogramColor;
	QColor CellColor;
	QColor TextColor;
	QColor StrobsColor[MAX_NUM_STRBS];
	QColor VRChColor;

	int row_number;
	int column_number;


	QColor SepLineColor;
	QColor BaseDefColor;
	QColor NoContactColor;
	QColor DefectColor;


	OscillogramProperties(void);
	virtual ~OscillogramProperties(void);	

};

