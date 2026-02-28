#include "OscillogramProperties.h"


OscillogramProperties::OscillogramProperties(void) :
				BackgroundColor(Qt::black),
				OscillogramColor(Qt::green),
				CellColor(Qt::white),
				TextColor(Qt::white),
				VRChColor(Qt::darkGreen),
				row_number(6),
				column_number(10),
				SepLineColor(Qt::darkGray),
				BaseDefColor(Qt::darkGreen),
				NoContactColor(Qt::black),
				DefectColor(Qt::red)
{
	StrobsColor[0] = Qt::cyan;
	StrobsColor[1] = Qt::red;
}


OscillogramProperties::~OscillogramProperties(void)
{
}
