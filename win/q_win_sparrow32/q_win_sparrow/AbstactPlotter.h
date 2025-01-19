#pragma once

#include <QPixmap>
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

	QColor default_colour;


	QPixmap *pixmap;
	QRect PosRect;


	volatile bool multithread_mode;

	QWidget *MainPlotWidget;

public:



	AbstactPlotter(QObject *parent);
	virtual ~AbstactPlotter(void);


	void ConnectToWidget(QWidget *);
	void UpdatePlot();


	virtual QRect & SetPlotRect(const QRect &);
	QPixmap * GetPixmap();


	const volatile bool &IsMultithread() const;
	virtual bool LockPixmap();
	virtual void UnlockPixmap();


	void SetDefaultColor(const QColor &new_default_colour);

};

