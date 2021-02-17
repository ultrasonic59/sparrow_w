#include "dial_ed_par.h"

DialEdPar::DialEdPar(QWidget *parent, QWidget *reseiver, 
								const int Zoom, 
								const int Focus, 
								const bool FocusSt, 
								const int xx360, 
								const int xx180, 
								const int val_c, 
								const int val_t, 
								const bool led1, 
								const bool led2)
	: QDialog(parent),
	ui()
{
	ui.setupUi(this);


	SetZoomValue(Zoom);
	SetFocusValue(Focus);
	Setxx360Value(xx360);
	Setxx180Value(xx180);
	SetAngelsValue(val_c, val_t);


	ui.ScrollBar_led1->setEnabled(led1);
	ui.checkBox_led1->setChecked(led1);
	ui.ScrollBar_led2->setEnabled(led2);
	ui.checkBox_led2->setChecked(led2);

	ui.ScrollBar_focus->setEnabled(FocusSt);
	ui.checkBox_focus->setChecked(FocusSt);
	


	//sliderMoved
	//stateChanged
	QObject::connect(this, SIGNAL(CloseMe()), reseiver, SLOT(CloseDialogPar()));
	

	QObject::connect(ui.ScrollBar_zoom, SIGNAL(valueChanged(int)), this, SLOT(OnZoomSliderChange(int)));
	QObject::connect(ui.ScrollBar_focus, SIGNAL(valueChanged(int)), this, SLOT(OnFocusSliderChange(int)));
	QObject::connect(ui.ScrollBar_led1, SIGNAL(valueChanged(int)), this, SLOT(OnLED1SliderChange(int)));
	QObject::connect(ui.ScrollBar_led2, SIGNAL(valueChanged(int)), this, SLOT(OnLED2SliderChange(int)));
	QObject::connect(ui.ScrollBar_xx360, SIGNAL(valueChanged(int)), this, SLOT(Onxx360SliderChange(int)));
	QObject::connect(ui.ScrollBar_xx180, SIGNAL(valueChanged(int)), this, SLOT(Onxx180SliderChange(int)));

	QObject::connect(ui.ScrollBar_zoom, SIGNAL(sliderReleased()), this, SLOT(OnZoomSliderRelease()));
	QObject::connect(ui.ScrollBar_focus, SIGNAL(sliderReleased()), this, SLOT(OnFocusSliderRelease()));
	QObject::connect(ui.ScrollBar_led1, SIGNAL(sliderReleased()), this, SLOT(OnLED1SliderRelease()));
	QObject::connect(ui.ScrollBar_led2, SIGNAL(sliderReleased()), this, SLOT(OnLED2SliderRelease()));
	QObject::connect(ui.ScrollBar_xx360, SIGNAL(sliderReleased()), this, SLOT(Onxx360SliderRelease()));
	QObject::connect(ui.ScrollBar_xx180, SIGNAL(sliderReleased()), this, SLOT(Onxx180SliderRelease()));


	QObject::connect(this, SIGNAL(ZoomSliderReleased(int)), reseiver, SLOT(ZoomChange(int)));
	QObject::connect(this, SIGNAL(FocusSliderReleased(int)), reseiver, SLOT(FocusChange(int)));
	QObject::connect(this, SIGNAL(LED1SliderReleased(int)), reseiver, SLOT(LED1Change(int)));
	QObject::connect(this, SIGNAL(LED2SliderReleased(int)), reseiver, SLOT(LED2Change(int)));
	QObject::connect(this, SIGNAL(xx360SliderReleased(int)), reseiver, SLOT(xx360Change(int)));
	QObject::connect(this, SIGNAL(xx180SliderReleased(int)), reseiver, SLOT(xx180Change(int)));


	QObject::connect(ui.checkBox_led1, SIGNAL(stateChanged(int)), this, SLOT(OnLED1Check(int)));
	QObject::connect(ui.checkBox_led2, SIGNAL(stateChanged(int)), this, SLOT(OnLED2Check(int)));
	QObject::connect(ui.checkBox_focus, SIGNAL(stateChanged(int)), this, SLOT(OnFocusCheck(int)));

	QObject::connect(this, SIGNAL(LED1Checked(bool)), reseiver, SLOT(LED1Checked(bool)));
	QObject::connect(this, SIGNAL(LED2Checked(bool)), reseiver, SLOT(LED2Checked(bool)));
	QObject::connect(this, SIGNAL(FocusChecked(bool)), reseiver, SLOT(FocusChecked(bool)));


	QObject::connect(reseiver, SIGNAL(SettedZoom(int)), this, SLOT(SetZoomValue(int)));
	QObject::connect(reseiver, SIGNAL(SettedFocus(int)), this, SLOT(SetFocusValue(int)));
	QObject::connect(reseiver, SIGNAL(Settedxx360(int)), this, SLOT(Setxx360Value(int)));
	QObject::connect(reseiver, SIGNAL(Settedxx180(int)), this, SLOT(Setxx180Value(int)));

	QObject::connect(reseiver, SIGNAL(SettedAngels(int,int)), this, SLOT(SetAngelsValue(int,int)));
 

}



void DialEdPar::closeEvent( QCloseEvent * event )
{
	QDialog::closeEvent(event);
	CloseMe();
}


DialEdPar::~DialEdPar()
{

}



void DialEdPar::SetZoomValue(int val)
{
	ui.ScrollBar_zoom->setValue(val);
	ui.label_zoom_val->setText(QString::number(val,10));
}

void DialEdPar::SetFocusValue(int val)
{
	ui.ScrollBar_focus->setValue(val);
	ui.label_focus_val->setText(QString::number(val,10));
}

void DialEdPar::Setxx360Value(int val)
{
	ui.ScrollBar_xx360->setValue(val);
	ui.label_xx360_val->setText(QString::number(val,10));
}

void DialEdPar::Setxx180Value(int val)
{
	ui.ScrollBar_xx180->setValue(val);
	ui.label_xx180_val->setText(QString::number(val,10));
}



void DialEdPar::OnZoomSliderChange(int val)
{
	ui.label_zoom_val->setText(QString::number(val,10));
}

void DialEdPar::OnFocusSliderChange(int val)
{
	ui.label_focus_val->setText(QString::number(val,10));
}

void DialEdPar::OnLED1SliderChange(int val)
{
	ui.label_led1_val->setText(QString::number(val,10));
}

void DialEdPar::OnLED2SliderChange(int val)
{
	ui.label_led2_val->setText(QString::number(val,10));
}

void DialEdPar::Onxx360SliderChange(int val)
{
	ui.label_xx360_val->setText(QString::number(val,10));
}

void DialEdPar::Onxx180SliderChange(int val)
{
	ui.label_xx180_val->setText(QString::number(val,10));
}

void DialEdPar::OnLED1Check(int val)
{
	LED1Checked(val>0);
	ui.ScrollBar_led1->setEnabled(val>0);
	ui.label_led1_val->setEnabled(val>0);
}

void DialEdPar::OnLED2Check(int val)
{
	LED2Checked(val>0);
	ui.ScrollBar_led2->setEnabled(val>0);
	ui.label_led2_val->setEnabled(val>0);
}


void DialEdPar::OnFocusCheck(int val)
{
	FocusChecked(val>0);
	ui.ScrollBar_focus->setEnabled(val>0);
	ui.label_focus_val->setEnabled(val>0);
}


void DialEdPar::SetAngelsValue(int val_c, int val_t)
{
	ui.label_course_val->setText(QString::number(val_c,10));
	ui.label_tang_val->setText(QString::number(val_t,10));
}


void DialEdPar::OnZoomSliderRelease()
{
	ZoomSliderReleased(ui.ScrollBar_zoom->value());
}

void DialEdPar::OnFocusSliderRelease()
{
	FocusSliderReleased(ui.ScrollBar_focus->value());
}

void DialEdPar::OnLED1SliderRelease()
{
	LED1SliderReleased(ui.ScrollBar_led1->value());
}

void DialEdPar::OnLED2SliderRelease()
{
	LED2SliderReleased(ui.ScrollBar_led2->value());
}

void DialEdPar::Onxx360SliderRelease()
{
	xx360SliderReleased(ui.ScrollBar_xx360->value());
}

void DialEdPar::Onxx180SliderRelease()
{
	xx180SliderReleased(ui.ScrollBar_xx180->value());
}