#pragma once
#include <QWidget>
#include "AbstactPlotter.h"

class PlotterWidget : public QWidget
{

public:
	PlotterWidget(QWidget *parent=0, Qt::WindowFlags f=0);
	PlotterWidget(QWidget *parent, AbstactPlotter *plotter, Qt::WindowFlags f=0);

	~PlotterWidget(void);

	void SetOscPlotter(AbstactPlotter *);

protected:
    void paintEvent(QPaintEvent *);

private:
	AbstactPlotter *pPlotter;


};

