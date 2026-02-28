#ifndef _DIAL_LOAD_SAVE_PAR_H
#define _DIAL_LOAD_SAVE_PAR_H

#include <QCloseEvent>
#include <QtWidgets/QtWidgets>
#include <QPluginLoader>

#include "ui_dial_load_save_par.h"

namespace Ui {
    class Dialog_load_save_par;
}

class DialLoadSavePar : public QDialog
{
Q_OBJECT
#if 0
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

#endif
protected:
	virtual void closeEvent( QCloseEvent * event );
	QString DirName;
	Ui::Dialog_load_save_par *ui;
private slots:
	void on_butt_dir(); 

public:

////	    explicit DialPhoto(QWidget *parent = 0);

	DialLoadSavePar(QWidget *parent= 0);
	virtual ~DialLoadSavePar();
	void  get_path(QString  *o_path);
	void  set_path(QString path);
	void  get_num_par(int  *num_par);
	void  set_num_par(int  num_par);
	void  set_butt_text(QString text);
///private slots:
////	void on_send_butt();
};

#endif // _DIAL_LOAD_SAVE_PAR_H
