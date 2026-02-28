#ifndef DIAL_ED_PAR_H
#define DIAL_ED_PAR_H

#include "ui_dial_ed_par.h"

class DialEdPar : public QDialog
{
Q_OBJECT

private slots:
	void OnZoomSliderChange(int);
	void OnFocusSliderChange(int);
	void OnLED1SliderChange(int);
	void OnLED2SliderChange(int);
	void Onxx360SliderChange(int);
	void Onxx180SliderChange(int);

	void OnZoomSliderRelease();
	void OnFocusSliderRelease();
	void OnLED1SliderRelease();
	void OnLED2SliderRelease();
	void Onxx360SliderRelease();
	void Onxx180SliderRelease();


	void OnLED1Check(int);
	void OnLED2Check(int);
	void OnFocusCheck(int);

	void SetZoomValue(int);
	void SetFocusValue(int);
	void Setxx360Value(int);
	void Setxx180Value(int);


	void SetAngelsValue(int val_c, int val_t);

signals:
	void CloseMe(void);

	void LED1Checked(bool);
	void LED2Checked(bool);
	void FocusChecked(bool);

	void ZoomSliderReleased(int);
	void FocusSliderReleased(int);
	void LED1SliderReleased(int);
	void LED2SliderReleased(int);
	void xx360SliderReleased(int);
	void xx180SliderReleased(int);


protected:
	Ui::DialogEditPar ui;
	virtual void closeEvent( QCloseEvent * event );

public:
	DialEdPar(QWidget *parent, QWidget *reseiver, 
								const int Zoom, 
								const int Focus, 
								const bool FocusSt, 
								const int xx360, 
								const int xx180, 
								const int val_c, 
								const int val_t, 
								const bool led1, 
								const bool led2);
	virtual ~DialEdPar();

	
};

#endif // DIAL_ED_PAR_H
