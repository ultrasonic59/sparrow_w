#ifndef QPUSHBUTTONCOLORED_H
#define QPUSHBUTTONCOLORED_H

#include <QPushButton>

class QPushButtonColored : public QPushButton
{

public:
	QPushButtonColored(QWidget *parent = 0);
	~QPushButtonColored();

	void SetColor(const QColor & color);
	const QColor & GetColor(void) const;

protected:

	const int frame_width;
	virtual void paintEvent(QPaintEvent * event);

private:
	QColor tempColor;


};

#endif // QPUSHBUTTONCOLORED_H
