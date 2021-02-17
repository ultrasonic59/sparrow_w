#pragma once

#include <QtGui/QPixmap>
#include <QWidget>



class AbstactPlotter  : public QObject
{
protected:

	int pixmap_width;
	int pixmap_height;

	int WidthUpStep;
	int HeightUpStep;

	bool NeedResize;

	int left;
	int top;


	QPixmap *pixmap;
	QRect PosRect;

	QWidget *MainPlotWidget;

public:


	AbstactPlotter(void);
	virtual ~AbstactPlotter(void);


	void ConnectToWidget(QWidget *);
	void UpdatePlot();


	virtual QRect & SetPlotRect(const QRect &);
	QPixmap * GetPixmap() const;

};

