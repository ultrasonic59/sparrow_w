#include "positiontesterplotter.h"

PositionTesterPlotter::PositionTesterPlotter()
	: AbstactPlotter(),
	cursor_changed(false),
	cursor_curr(Qt::ArrowCursor),
	is_dragging(false),
	curr_x(0.0f),
	curr_y(0.0f),
	drag_start_x(0.0f),
	drag_start_y(0.0f)
{
}

PositionTesterPlotter::~PositionTesterPlotter()
{
}


void PositionTesterPlotter::TestNewPosition(const QPointF &rel_pos)
{
	curr_x = rel_pos.x() - left;
	curr_y = rel_pos.y() - top;
}


bool PositionTesterPlotter::IsCursorChanged(void)
{
	return cursor_changed;
}

Qt::CursorShape PositionTesterPlotter::GetCursor()
{
	return cursor_curr;
}

void PositionTesterPlotter::CursorAssepted(void)
{
	cursor_changed = false;
}

void PositionTesterPlotter::SetDragging(bool dragging)
{
	if( is_dragging != dragging )
	{
		is_dragging = dragging;

		if(is_dragging)
		{
			drag_start_x = curr_x;
			drag_start_y = curr_y;
			DragStarted();
		}
		else
		{
			DragEnded();
		}
	}
}