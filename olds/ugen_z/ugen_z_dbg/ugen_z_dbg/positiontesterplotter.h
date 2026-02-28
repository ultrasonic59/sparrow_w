#ifndef POSITIONTESTERPLOTTER_H
#define POSITIONTESTERPLOTTER_H

#include "AbstactPlotter.h"

class PositionTesterPlotter : public AbstactPlotter
{
public:
	PositionTesterPlotter(void);
	~PositionTesterPlotter(void);

	virtual void TestNewPosition(const QPointF &rel_pos);

	bool IsCursorChanged(void);
	Qt::CursorShape GetCursor();
	void CursorAssepted(void);

	void SetDragging(bool dragging);

protected:

	bool cursor_changed;
	Qt::CursorShape cursor_curr;
	bool is_dragging;

	float curr_x;
	float curr_y;


	float drag_start_x;
	float drag_start_y;


	virtual void DragStarted() = 0;
	virtual void DragEnded() = 0;

};

#endif // POSITIONTESTERPLOTTER_H
