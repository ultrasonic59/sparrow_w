#include "OscillogramProperties.h"


OscillogramProperties::OscillogramProperties(void) :
				BackgroundColor(Qt::white),
				OscillogramColor(Qt::blue),
				GridColor(Qt::gray),
				TextColor(Qt::black),


				row_number(6),
				column_number(10),

                OscillogramWidth(2.0f)
	
{
}


OscillogramProperties::~OscillogramProperties(void)
{
}
